# APUE

《UNIX 环境高级编程》第三版学习笔记及源码，文件编号保留原有章节顺序。

## 构建入门示例

Linux / macOS 需要 C11 编译器和 CMake 3.20+；运行测试还需要 Python 3.10+。Windows 可使用 WSL。

```sh
git clone https://github.com/weeweetan/APUE.git
cd APUE
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

目前纳入构建和回归检查的是下列 10 个基础示例；其他章节尚未纳入，后续逐步整理。`apue.h` 的历史录入错误与其他章节缺失的辅助函数也尚未修复，不能据此认为全部章节可直接构建。

| 目标 | 原文件 | 内容 |
| --- | --- | --- |
| example-1-1 | [1-1.c](1-1.c) | 列出目录 |
| example-1-2 | [1-2.c](1-2.c) | read / write 复制标准输入 |
| example-1-3 | [1-3.c](1-3.c) | 标准 I/O 复制 |
| example-1-4 | [1-4.c](1-4.c) | 进程 ID |
| example-1-6 | [1-6.c](1-6.c) | errno 与错误文本 |
| example-1-7 | [1-7.c](1-7.c) | 用户和组 ID |
| example-3-1 | [3-1.c](3-1.c) | 标准输入能否 seek |
| example-3-2 | [3-2.c](3-2.c) | 创建带空洞的文件 |
| example-3-3 | [3-3.c](3-3.c) | 文件描述符复制 |
| example-3-4 | [3-4.c](3-4.c) | 查询文件描述符标志 |

```sh
./build/bin/example-1-1 .
printf 'hello\n' | ./build/bin/example-1-2
./build/bin/example-3-1 < README.md
```

`example-3-2` 在当前目录创建并覆盖 `file.hole`，建议在临时目录运行。自动测试只在临时目录生成文件，不运行交互 shell、守护进程或需要特权的实验。

## 公共错误输出

[ourhdr.h](ourhdr.h) 仅保留声明，`err_msg`、`err_ret`、`err_quit`、`err_sys`、`err_dump` 的实现位于 [lib/error.c](lib/error.c)。直接编译章节文件时需要链接它：

```sh
cc -std=c11 -D_POSIX_C_SOURCE=200809L -I. 1-1.c lib/error.c -o example-1-1
```

错误输出直接使用格式化流，避免固定 4096 字节缓冲区的溢出；非致命错误输出保留调用时的 errno。头文件可以重复包含，也可以由多个翻译单元共同使用。

回归检查覆盖 32768 字节错误消息、errno 保留、错误退出状态、多翻译单元链接、空输入 / 二进制复制、目录失败与文件空洞内容。修改公共函数后可启用内存与未定义行为检查：

```sh
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build-asan --parallel
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
  ctest --test-dir build-asan --output-on-failure
```

Linux CI 额外开启泄漏检查；macOS 使用 ASan / UBSan。新增可运行示例时，在 CMake 中登记并补充与行为相关的测试，保留章节文件名方便对照阅读。
