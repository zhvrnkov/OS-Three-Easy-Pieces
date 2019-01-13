# Intro to OS.
##### Von Neuman model of computing: 
CPU fetches instruction (from memory), decodes it, and executes.

##### Operations System:
"body of software", responsible for making it easy to run programms, allowing programs to share the memory, enabling programs to interact with devices etc.

# The Crux of the Problem: How to virtualize resources? How OS doing this?

The primery way the OS does this is through a general technique that we call **virtualization** - the OS takes a **physical** resource (CPU, or memory, or disk) and transforms it into more general, powerful, and easy-to-use **virtual** for of itself.

# 2.1 Virtualizing the CPU:
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }
    char *str = argv[1];
    while (1) {
	Spin(1);
        printf("%s\n", str);
    }
    return 0; 
}
```
+ `Spin(1)` - repeadetly chekcs the time and return once in has run for a second.
+ after `Spin(1)` returns, the program print some Command Line input.
+ because of while(1) this is repeats forever.
+ Also notice that in this example we have CPU with single core.

```c
$ ./program A & ; ./program B & ; ./program C & ; ./program D &
// output is whole mess of A,B,C and D's.

##### Q: so the question is how is it possible with single core in CPU? How CPU can handle it?
##### A: virtualizing the CPU.

> Of course, to run programs, and stop them, and otherwise tell the OS which programs to run, there need to be some interfaces (APIs) that you can use to communicate your desires to the OS.

##### Q: also, if we come back to our example when we simultaneously run some number of programs and ask yourself what program should run first?
##### A: this choice is responsibility of **OS policy**.

# 2.2 Virtualizing Memory
+ Memory is just an array of bytes.
+ to read it, one must specify an **address**
+ to write it, one must also specify the data to be written to the given address.

> Don’t forget that each instruc- tion of the program is in memory too; thus memory is accessed on each instruction fetch.

**code in CodeSnippets**:
1. with multiple running instances of a programm we get the same output for each.
2. The prints one address of memory, prints same value for it
3. Then the printed address cannot be physical address of memory - this is virtual address form **virtual address space**. Each proccess access its own private v.s.a. which the OS somehow maps onto physical memory of the macine.

# 2.3 Concurrency

##### Note: look at previous example: we got 2 processes, for each process OS provide virtaul address space - each process have it. Consider now that thread is some kind of process inside "parent" process - the confuse is that thread don't have v.a.s., they use parent's v.a.s. This is what makes **concurrent** operations complicated.

> We use this concep- tual term to refer to a host of problems that arise, and must be addressed, when working on many things at once (i.e., concurrently) in the same program.

# The Crux: HOW TO BUILD CORRECT CONCURRENT PROGRAMS.

**code in CodeSnippets**: this happens because `counter++` operation occupies 3 instruction:
1. fetch data from RAM into register
2. increment it
3. load data back

# 2.4 Persistence
+ DRAM - stores memory in a volatile manner (power off -> memory lost)
+ need persistent storage: some hardware which will conform this.
+ such hardware comes in the form of I/O device: **hard drive**, although **solid-state drive**.
+ software that manage disk is called **file system**.

> Unlike the abstractions provided by the OS for the CPU and memory, the OS does not create a private, virtualized disk for each application.

# The Crux: HOW TO STORE DATA PERSISTENTLY.

**code in CodeSnippets**:
+ `open()`, `close()` and `write()` - is **system calls** (are routed to the part of the OS called **file system** which then handles the requests and returns some kind of error code to the user)
