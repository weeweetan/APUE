"""Bounded local regressions; no network, signals to other processes, or root."""
import errno
import os
from pathlib import Path
import subprocess
import sys
import tempfile

BINARY = Path(sys.argv[1]).resolve()


def run(name, *arguments, input=b'', code=0, cwd=None, env=None):
    result = subprocess.run([str(BINARY / name), *map(str, arguments)], input=input,
                            capture_output=True, timeout=10, cwd=cwd, env=env)
    assert result.returncode == code, (name, result.returncode, result.stderr)
    for marker in [b'AddressSanitizer', b'UndefinedBehaviorSanitizer', b'runtime error:', b'LeakSanitizer']:
        assert marker not in result.stderr, result.stderr
    return result


def errors():
    assert run('test-error', 'long').stderr == b'begin:' + b'x' * 32768 + b':end\n'
    assert run('test-error', 'return').stderr == ('cannot open input: ' + os.strerror(errno.EACCES) + '\n').encode()
    assert run('test-error', 'sys', code=1).stderr == ('missing input: ' + os.strerror(errno.ENOENT) + '\n').encode()
    assert run('test-error', 'quit', code=1).stderr == b'bad argument: 42\n'


def examples():
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        (root / 'sample').write_bytes(b'example')
        listing = run('example-1-1', root).stdout.splitlines()
        assert b'sample' in listing and b'.' in listing and b'..' in listing
        assert b"can't open" in run('example-1-1', root / 'absent', code=1).stderr
        assert b'a single argument' in run('example-1-1', code=1).stderr
        payload = bytes(range(256)) * 256 + b'tail without newline'
        for example in ['1-2', '1-3', '3-3']:
            for data in [b'', payload]:
                result = run('example-' + example, input=data)
                assert result.stdout == data and result.stderr == b''
        assert run('example-1-4').stdout.startswith(b'hello world from process ID ')
        assert run('example-1-7').stdout == f'uid = {os.getuid()}, gid = {os.getgid()}\n'.encode()
        assert os.strerror(errno.EACCES).encode() in run('example-1-6').stderr
        assert run('example-3-1').stdout == b'cannot seek\n'  # A subprocess stdin pipe.
        run('example-3-2', cwd=root)
        assert (root / 'file.hole').read_bytes() == b'abcdefghij' + b'\0' * 30 + b'ABCDEFGHIJ'
        assert run('example-3-4', '0').stdout.startswith(b'read only')
        assert b'usage:' in run('example-3-4', code=1).stderr
        assert b'fcntl error' in run('example-3-4', '-1', code=1).stderr

        readable = run('example-4-2', root / 'sample')
        assert readable.stdout.splitlines() == [b'read access OK', b'open for reading OK']
        assert readable.stderr == b''
        unreadable = root / 'unreadable'
        unreadable.write_bytes(b'example')
        unreadable.chmod(0)
        try:
            # A privileged test runner can read mode-000 files; only assert the
            # permission distinction when the runner is actually denied read access.
            if os.access(unreadable, os.F_OK) and not os.access(unreadable, os.R_OK):
                denied = run('example-4-2', unreadable)
                assert b'read access OK' not in denied.stdout
                assert b'access error for' in denied.stderr
                assert b'open error for' in denied.stderr
        finally:
            unreadable.chmod(0o600)


def copies():
    payload = bytes(range(256)) * 256 + b'tail without newline'
    for name in ['test-copy-1-2', 'test-copy-3-3']:
        for mode in ['normal', 'short-read', 'short-write', 'read-eintr', 'write-eintr', 'mixed']:
            env = dict(os.environ, APUE_COPY_MODE=mode)
            for data in [b'', payload]:
                result = run(name, input=data, env=env)
                assert result.stdout == data and result.stderr == b'', (name, mode)
        for mode, expected, message in [
            ('read-error', b'', os.strerror(errno.EIO).encode()),
            ('read-error-after-data', payload[:11], os.strerror(errno.EIO).encode()),
            ('write-error', b'', os.strerror(errno.ENOSPC).encode()),
            ('write-error-after-data', payload[:7], os.strerror(errno.ENOSPC).encode()),
            ('write-zero', b'', b'zero bytes written'),
            ('write-zero-after-data', payload[:7], b'zero bytes written'),
        ]:
            result = run(name, input=payload, code=1, env=dict(os.environ, APUE_COPY_MODE=mode))
            assert result.stdout == expected, (name, mode)
            assert message in result.stderr, (name, mode, result.stderr)


{'errors': errors, 'examples': examples, 'copies': copies}[sys.argv[2]]()
print(sys.argv[2] + ': regression checks passed')
