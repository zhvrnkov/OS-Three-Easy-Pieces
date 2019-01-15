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
+ `open()`, `close()` and `write()` - is **system calls** (this s.calls are routed to the part of the OS called **file system** which then handles the requests and returns some kind of error code to the user - this is how system calls works: programm call it, pass control to OS, OS handles the request, return the output and control to program).

# 2.6 Some History

### Early Operating Systems: Just Libraries

> Basically, it was just a set of libraries of commonly-used functions; for example, instead of having each programmer of the system write low-level I/O handling code, the “OS” would provide such APIs, and thus make life easier for the developer.

### Protection: System Calls

+ code which run on behalf of the OS was special; it had control of devices and thus should be treated dif- ferently than normal application code

+ imagine if any code could have such permisions. The notion of privacy goes out the window, as any program could read any file.
+ Thus implementing the **file system**, but as a library it make a little sense.

+ To improve this **system calls** were invented and firstly come with *Atlas computing system*:
	+ Instead of providing OS routines as a library (where you just make **procedure calls** to access them), the idea here was to add a special pair of hardware instructions and hardware state to make the transition into the OS a more formal, controlled process.

##### System call vs. Procedure call:
System call transfers control (i.e. jumps) into the OS while simultaneously raising the **hardware privilege level**. User applications run in **user mode** which means the hardware restricts what ap- plications can do.

> for example, an application running in user mode can’t typically initiate an I/O request to the disk, access any physical memory page, or send a packet on the network

1. System call from user application
2. Hardware transfers control to pre-specified **trap handler** (OS set it up previously)
3. Privilege level set to **kernel mode**
4. OS done servicing this request
5. OS passes control back to the application (via special **return-from-trap** instruction which reverts to **user mode** and passing control back to where the application left off)

# PART I
# VIRTUALIZATION

## The Abstraction: The Process
process is a **running program**

# The Crux: HOW TO PROVIDE THE ILLUSION OF MANY CPUS?

The OS can run hundred of processes with less then 10 CPUs (cores). This is achivied by **CPU virtualization**: running one process, then stopping it and running another, and so forth, the OS can promote the illusion that many virtual CPUs exist when in fact there is only one physical CPU (or a few).
This basic technique is know as **time sharing** of the CPU, allows users to run as many concurrent processes as they would like; the potential cost is performance, as each will run more slowly if the CPU(s) must be shared.

To implement it (v. of CPU) the OS will need bot some low-level machinery as well as some hight-level intelligence:
1. **Machinery** - low-level mechanisms; **Mechanisms** - are low-level methods or protocols that implement a needed piece of functionality.
2. **Polices** - algorithms for making some kind of decision within the OS.

##### Note: the policy that dedicate what process will run first is called **scheduling policy**.

# 4.1 The Abstraction: A Process

The abstraction provided by the OS of a running program is something we will call a **process**.

**Machine state** of a process is what program can read or update when it is running. What parts of a machine are important for program execution.

**Memory** is obvious part of a machine state. Instructions lie in memory; some instructions works with memory (address space).
Thus the memory that the process can address (called its address space) is part of the process.

Also registers (Program counter - tells us which instruction of the process is currently being executed), similarly **stack pointer** and associated **frame pointer** (are used to manage the stack for function parameters, local variables, and return addresses).

# TIP: Policy and Mechanism
Mechanism is about *how*: how does an operating system perform a context switch?

Policy is about *which*: which process should the operating system run right now?

##### Stack Pointer and Frame Pointer
Frame pointer - the value of Stack Pointer before function call. A frame pointer of a given invocation of a function is a copy of the stack pointer as it was before the function was invoked.

# 4.2 Process API
+ Create: some method to create a new process. Typing command to shell, double-click app icon is invoke OS to create a new process for that program.
+ Destroy: destroy process forcefully.
+ Wait: stop running a process.
+ Miscellaneous Control: other that killing or waiting for a process. Some systems allows to suspend process and the resume it and so forth.
+ Status: some status information about a process.

# 4.3 Process Creation: A Little More Detail

##### Q: how does the OS get a program up and running? How does process creation actually work?
##### A:
1. to run program, OS need to load its code and any static data (e.g. initialized variables) into memory (address space of a process).

> Programs initially reside on disk in some kind of executable format; thus, the process of loading a program and static data into memory re- quires the OS to read those bytes from disk and place them in memory somewhere

> In early (or simple) operating systems, the loading process is done ea- gerly, i.e., all at once before running the program; modern OSes perform the process lazily, i.e., by loading pieces of code or data only as they are needed during program execution

2. allocate memory for **run-time** stack

> As you should likely already know, C programs use the stack for local variables, function parameters, and return addresses

> The OS will also likely initial- ize the stack with arguments; specifically, it will fill in the parameters to the main() function, i.e., argc and the argv array

3. allocate some memory for **heap**.

> Heap will be small at first; as the program runs, and requests more memory with `malloc()` library API, the OS may get involved and allocate more memory to the process to help satisfy such calls.

4. some other initialization tasks regarding I/O.

> For example, in UNIX systems, each process by default has three open file descriptors, for standard input, output, and error; these descriptors let programs easily read input from the terminal as well as print output to the screen

5. finally, set the stage of program execution

> In C this is jumping to `main` routine and and transfers control to the **CPU**


### Homework:
-I RUN_IO_IMMEDIATE with -S SWITCH_ON_IO is some sort of asynchronous IO when we call IO, while waiting execute other instructions, when IO complete immediate processed it (run another io) and so forth. Thus i think that running IO immediate is really good idea in most cases. For better performance i'll also recommend to run IO immediate but without interrupting other processes. When we interrupt process, we need to stash its instruction in RAM, clear registers, load IO instruction - this is loses in performance too.  The better way is allow last process to complete its instructions and then processed IO calls (in that case we reduce IO performance by idle it while waiting - not perfect too).
