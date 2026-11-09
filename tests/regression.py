"""Bounded local regressions; no network, signals to other processes, or root."""
import errno
import os
from pathlib import Path
import subprocess
import sys
import tempfile

BINARY = Path(sys.argv[1]).resolve()


def run(name, *arguments, input=b'', code=0, cwd=None):
    result = subprocess.run([str(BINARY / name), *map(str, arguments)], input=input,
                            capture_output=True, timeout=10, cwd=cwd)
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


{'errors': errors, 'examples': examples}[sys.argv[2]]()
print(sys.argv[2] + ': regression checks passed')
