# MingDebugger
MingDebugger is a simple prototype to illustrate how debugging work in practice. It has a few components:

## MingCompiler
It is a compiler that take a source file to a 'binary' and 'symbol' file. In this prototype, we will support a very simple programming language take can compute the Fibonacci numbers.

    function fib(a) {
      if (a == 0) {
        return 1;
      } else {
        if (a == 1) {
          return 1;
        } else {
          return fib(a - 1) + fib(a - 2);
        }
      }
    }
    function main() {
      print(fib(10));
    }

The 'binary' are in quotes because it really isn't a binary file. It is simply a text file of instruction and data. (This is just an example, incomplete ...)

    push 10
    call 4
    print
    ...

The symbol file is also a text file that contains the debug information, it would probably look like

    lines:
    main 1
    fib 4
    
    locals:
    a R1

## MingComputer

A ming computer is a mixture of the computer (that interpret the 'binary'), the operating system (that provide a bunch of services such as load and launch a process, and various system calls required by the debugger)

    Launch fib.binary
    
## MingDebugger

A big part of MingComputer is to support debugging the launched process. It will make use of the symbols and do various debugging tasks such as breakpoint, stepping, inspection, and so on.