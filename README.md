# Command Line Calculator written in C++ with LLVM
callvm is a commad line calculator that generates LLVM IR code.

## Install
```
git clone https://github.com/agatan/callvm.git
mkdir build && cd build
cmake ..
make
```

This application depends on LLVM and Boost. If you haven't installed them, `cmake` will be failed.

## Usage
```
./callvm [output-file-name]
```
After you execute the command, this application output like this.
```
user>
```

And you type some arithmetic expressions, this application will return LLVM IR that include the value of your expressions.

```
user> 1 + 2
(1 + 2)
this is an integer expression
; ModuleID = 'toplevel'

define i32 @func0() {
entry:
  ret i32 3
}
user> 3.0 * (4 - 1)
(3.000000 * (4 - 1))
this is an float expression
; ModuleID = 'toplevel'

define i32 @func0() {
entry:
  ret i32 3
}

define float @func1() {
entry:
  ret float 9.000000e+00
}
user>
```

When you want to quit this, you can type `quit`.
This application will be stopped and LLVM IR that contains all of your expressions will be output to the file.

### Get the value from C
This generates LLVM IR, so you can use those values from C.

main.c:
```c
#include <stdio.h>

float func0();
int main(int argc, char **argv)
{
    printf("%f\n", func0());
}
```

and calc.ll:
```
; ModuleID = 'toplevel'

define float @func0() {
entry:
  ret float 2.500000e+01
}
```

First, generate object file from LLVM IR.
```
llc -filetype=obj -o calc.o calc.ll
```
And then, compile `main.c` with `calc.o`
```
clang main.c calc.o
```
