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
```

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

# 4.4 Process State

+ Running: a process is running on a processor. Executing instructions of that process
+ Ready: process is ready to run, but OS has chosen not to run it at this given point
+ Blocked: in the blocked state, a process has performed some kind of operation that makes it not ready to run until some other event takes place

> A common example: when a process initiates an I/O request to a disk, it becomes blocked and thus some other process can use the processor.

Running <== Scheduled/Descheduled ==> Ready
Running (I/O: initiate) ==> Blocked <== (I/O: done) Ready

Being moved from **running** to **ready** means that the process has been **scheduled**. Reverse is **descheduled**.

Once a process come **blocked**, OS will keep it as such until some events occur (e.g. I/O completion).

# 4.5 Data Structures 
OS is programm and also have some key data structures that track various relevant peices of information (e.g. **process list** for all processes that are ready as well as some additional inforamtion about running processes; also OS needs to track blocked processes in some way because OS will do something with blocked process **when some events occur** (I/O event complete)).

**TODO**: make code snippet for this
**code for xv6 Proc Structure**

> Also, a process could be placed in a final state where it has exited but has not yet been cleaned up (in UNIX-based systems, this is called the zombie state1). This final state can be useful as it allows other processes (usually the parent that created the process) to examine the return code of the process and see if the just-finished process executed successfully (usually, programs return zero in UNIX-based systems when they have accomplished a task successfully, and non-zero otherwise).

##### Q: after this quote i wonder about returns. How they actually works?
+ Running c program in shell. `return 0;` from `main`. After completion we got something "Program ended with exit code: 0". At current moment i suppose that parant process of any C program which was called in terminal is shell. Here is the question: how shell get that return code? Profram runs, return 0 and shell somehow handle it like callback? Or program run, return 0 and being setted to final (zombie) state, after some amount of time (small amount) the parent process somehow examinate that return code of the child process and manipulate with that (in case of shell it will just print it). Based on how i understand that text snippet from book the last way is the right way.

##### Q: also, can parent process get acces to address space of child process? If no, then how parent process can manage the return code?

##### Q: how preciesly parent process know that the child process is finished. By look at final process memory? Or somehow handle the return? Or what?

##### Q: i think understanding the `wait` system call will clean that part.

### Homework:
-I RUN_IO_IMMEDIATE with -S SWITCH_ON_IO is some sort of asynchronous IO when we call IO, while waiting execute other instructions, when IO complete immediate processed it (run another io) and so forth. Thus i think that running IO immediate is really good idea in most cases. For better performance i'll also recommend to run IO immediate but without interrupting other processes. When we interrupt process, we need to stash its instruction in RAM, clear registers, load IO instruction - this is loses in performance too.  The better way is allow last process to complete its instructions and then processed IO calls (in that case we reduce IO performance by idle it while waiting - not perfect too).

Interesing task about process tracing between states.

# Interlude: Process API

# The Crux: How to create and Control Process

# 5.1 The `fork()` System Call
- is used to create a new process.

**code snippet p1.c**
`fork()`:
1. create new, child process
2. the body (or code if you like) starts after call `fork()` in "parent-code" (if that will be before `fork()` call, then we will got recursivly creation of new processes).
3. Also, by this reason, `rc == 0` in children process because `int rc = fork()` didn't get called.

> play with it code. Remove `wait(NULL` but notice, that without `wait` call that code isn't derministic (parent is gone, but child is going).

# 5.2 The `wait()` System Call
- system call `wait` will not return until all child process has run and exited

# 5.3 The `exec()` System Call
- is useful when you want to run a program that is different from the calling (parent, current) program.

**code snippet p3.c**
- print which will not has been called.
  1. This is because `execvp()` load the exact code (and all neccessary static data) to the place, where it's been called.
  2. The heap and stack and other parts of the memory space of the program re-initialized (because we exactly add CODE dynamically!!! compiler know nothing about it. We add code at runtime)
  3. Then the OS simply runs that program, passing in any arguments as the argv of that process. Thus, it does not create a new process; rather, it transforms the currently running program (formerly p3) into a different running program (wc).
  4. print never get called because in body of `wc` (code of which were inserted before that print) located `return` statement.

# 5.4 Why? Motivating The API

##### Q: do not clear understand terms and they relationships: shell and system calls, shell and os
##### Q: also play with `fork` and `exec` calls. Why they are separate, what we got from they separation, what we will got if they become integral?

Briefly explain what is **shell** - just program that shows prompt and waits for you tu type something in it. Shell is program -> you type a command -> Shell call `fork` -> some configurations (based of your arguments etc.) -> `execvp`

The separation of `fork` and `exec` allowing many things. For example - output redirections.

`$ wc p3.c > newline.txt`

after `fork` get called, shell redifin standart output to `newline.txt` 

also add this to `p3.c` code snippet:
```c
if (rc == 0) {
  close(STDOUT_FILENO);
  17 open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  ...
```

- when parrent get called and `fork` inside its body didn't get called, the first file desciptor which is available is STDOUT_FILEN (look more about it in chapter). When `fork()` get called and we are inside children process in `if (rc == 0) { ... }` we close that file desciptor (output), and open another. Now all `prints` and etc will be sended to that output.

Unix pipe `|` - in bash: `grep -o foo file | wc -l`. Output of one process become input of another via connecting by **pipe**.

### Homework #2 
##### Unsolved Questions:
1. Commited code: the `printf` statement get called in child body even if it placed before `fork` call. It depends on `\n` spec. symbol. Very interesting situation

1. Firstly, I had confused when I assume that x isn't equal 0 (because I thought that `int x =10` is the same as `int rc = fork()`. `rc` in the child process is equal 0 (because `rc` is child process) and I thought that situation with `x` will be the same. I was wrong, and all variable declaration are works (the best ways of thinking about it is `fork` -which is create this child- will be called inside that child but returns 0). Neverless, the answer of the homework exerciese is: 100, parent and child has their own address spaces.

2. Yes, this is default behavior (file descriptors and other kind of such things are transfered to the child process, but child process can change it in own code only for himself.

##### `dup` system call:
+ The `dup()` system call creates a copy of a file descriptor.

	- It uses the **lowest-numbered unused descriptor** for the new descriptor.
    - If the copy is successfully created, then the original and copy file descriptors may be used interchangeably.
	- They both refer to the same open file description and thus share file offset and file status flags.

> Currently i don't know much about file descriptors (but 0 - stdin, 1 - stdout, 2 - stderr). So dup(1) will create and return a copy of stdout. `close(STDOUT_FILENO)` is the same (in brand new program) as `close(1)` (while 1 is stdout - you can change it).
+ The `dup2()` system call is similar to dup() but the basic difference between them is that **instead of using the lowest-numbered unused file descriptor, it uses the descriptor number specified by the user**.
> So you cannot write `dup2(1)`, but you can:
```c
	int saved = dup(1);
	int anotherSaved;
	dup2(saved, anotherSaved);
```

More preciesly:
```c
int dup2(int oldfd, int newfd);
// oldfd: old file descriptor
// newfd new file descriptor which is used by dup2() to create a copy
```

If child and parent will write concurrently: I cannot achive concurrently writing in it as i understand it. They write in order.

3. You can achieve this by `sleep(int)` system call. I thought that there is some way to do it with `while`, but since parent and child has their own address spaces, I think it is impossible.

4. + `execl` and `execlp` are the same, but first argument of l should be path (`/bin/ls`), but in lp first argument should be file (`ls`)
   + `execvp()` only can work with array `execvp(myargs[0], myargs)`. `myargs[size(of: myargs)] == NULL`. Second argument must be `null` terminated array of strings

They all do the same things but with different kind of arguments.

5. `wait` returns `-1`. If child will `wait` and its doesn't have child processes, then nothing happend. If opposite, then default behavior (it just waits for child of himself).

6. `waitpid` is useful when we want to wait some starting process. with just `wait` the process can only wait of its children processes, but with `waitpid`...

7. Child prints to nothing (the file and console will not have a sign of it) - meybe it will write for `dev/null`
##### Q: What preciesly happens?

8. 
```c
	int p[2];
	write(p[1], msg, msgsize);
	read(p[0], msg, msgsize);
```

**Syntax**:
```
int pipe(int fds[2]);

Parameters :
fd[0] will be the fd(file descriptor) for the 
read end of pipe.
fd[1] will be the fd for the write end of pipe.
Returns : 0 on Success.
-1 on error.
```
##### Q: need more information about file descriptors, pipes, read and write sytem calls.

# Mechanism: LIMITED DIRECT EXECUTION

**:** run one process for a little while, then run another one, and so forth - **time sharing**.
By time shaing the **CPU** in this manner, virtualization is achieved.

Few chalenges in it:
1. *Performance*: how can we implement virtualization without adding excessive overhead to the system? (virtualize CPU in manner, that different from **time sharing**?)
2. *Control*: how can we run processes efficiently while retaining control over the CPU?

##### THE CRUX: HOW TO EFFICIENTLY VIRTUALIZE THE CPU WITH CONTROL?

# 6.1 Basic Technique: Limited Direct Execution

### Direct execution:
| OS                            | Program                  |
|-------------------------------|--------------------------|
| Create entry for process list |                          |
| Allocate memory for program   |                          |
| Load program into memory      |                          |
| Set up stack with argc/argv   |                          |
| Clear registers               |                          |
| Execute call main()           |                          |
|                               | Run main()               |
|                               | Execute return from main |
| Free memory of process        |                          |
| Remove from process list      |                          |

The problems if we use such approach and want virtualization is:

1. if we just run a program, how can the OS make sure the program doesn’t do anything that we don’t want it to do, while still running it efficiently?

2. when we are running a process, how does the operating system stop it from running and switch to another process, thus implementing the **time sharing** we require to virtualize the CPU?

# 6.2 Problem #1: Restricted Operations

Direct execution is fast; the program runs natively on the hardware CPU and thus executes as quickly as one would expect.

But running on the CPU introduces a problem: what if the process wishes to perform some kind of restricted operation. 

Firtstly, it seems that this is concern of compiler - to check that written code doesn't do anything restricted. But what if code do not pass compilation? What if we write code in binary? That is the case. Also! Compiler isn't CPU, compiler doesn't run code and you see, that we can dynamically add code by `exec` system call.

##### THE CRUX: HOW TO PERFORM RESTRICTED OPERATIONS

##### Q: why system calls looks like procedure calls?
##### A:
because in C, the `open()`, `close()`, `fork()` and etc are the procedure calls, but inside these procedure calls is the famous **trap** instruction.
Calling `open()` you execute the procedure call into the **C library**. Inside that call (or any other system call provided) the library used an agreed-upon calling (договоренная на базе вызова) convention with the **kernel** to put the arguments (of that `open()` call) in *well-known locations* (stack or specific register), put the system call number into *well-known location* (stack or specific register) and then executed aformentioned trap instruction. The code in the library after the trap unpacks return values and returns control to the program that issued the system call.

As mentioned above, we cannot let any process do whatever it like (issue I/O). For avoiding this, system calls were invented. Process call that system call (above answer is about how C call system call), give all control to OS via entering via exectuing **trap instruction**. 

> 1. This instruction simultaneously jumps into the kernel and raises the privilege level to kernel mode; once in the kernel, the system can now per- form whatever privileged operations are needed (if allowed), and thus do the required work for the calling process
> 2. When finished, the OS calls a special return-from-trap instruction, which, as you might expect, returns into the calling user program while simultaneously reducing the privi- lege level back to user mode.

The hardware in this case needs to be carefull for proper returning from trap. The system call returns something, so hardware and OS needs to take care that current process will handle that results properly. 

> On x86, for example, the processor will push the program counter, flags, and a few other registers onto a per-process kernel stack; the return-from- trap will pop these values off the stack and resume execution of the user- mode program

-----

##### Q: want to know about it more preciesly. What exectly is **kernel stack**? What does stack pop after returning-from-trap mean?
##### A:
got a little mess with stack in general. To summarize my thoughts:
1. I wondered, why reference types are stored on the heap even if we declare such variable at global scope. So I has concluded, that we just need such objects with heap-behavior. The heap is really needy part of any process and process cannot work with dynamic data without heap. We cannot store the dynamic data in stack. So then I decide, that reference types are stored on the heap, because we need in some cases such behavior.

Part of question about **kernel stack**.

Why we need such separation in process stack: kernel and user stack?

Ok, user stack is for process execution in user-mode, kernel stack is for execution system calls of that process.

> While the thread is in user space the kernel stack is empty except for the thread_info structure at the bottom.

##### Note:
We really need such save user stack mechanism when pass control back to OS. In next, OS can decide to switch context to another process, but for continuing prevoius process, OS need to know about it progress - this is achieved by saving proc structure. 

The real things about **kernel stack** is that OS works with it in **kernel mode**, but why it cannot work with just **user stack** in **kernel mode** (while perfomrming system call)? 

When trap handler correctly handle trap instruction and knows what code should be performed for that system call, the instructions will be executed (because system call is also code!) this is process (but in kernel mode and under OS control) and it is also need stack for local and other variables. 

!!!: Back to the beginning: system call -> system call placed to some register, parameters of system call are in user stack  -> trap instruction -> switch to kernel mode -> move parameters for system call to **kernel stack** (save regs to kernel stack) -> jump to trap handler -> perform handle and run bunch of instructions (is this system call is process too?) -> got some output before it -> return from-trap -> put output (which is stored in **kernel stack**) to the user stack -> move to user mode -> jump to PC after trap (to executing last process instruction).

##### Q: is system call is a process (but on behalf of kernel)? is it use kernel stack in the same way, as just process use user stack?

-----

##### Q: Also how does the trap knows what code it should run inside the OS?
##### A:
> the calling process can't specify an address to jump to.
- Because this code will run at the **kernel mode** and if we could specify that code, we could run some code that was written by yourself at the **kernel mode**. If that so, then concept of **system calls** will be broken.

The kernel does so by setting up a **trap table** at boot time. 

> When the machine boots up, it does so in privileged (kernel) mode, and thus is free to configure machine hardware as need be.

1. the first things the OS thus does is to tell the hardware what code to run when certain exceptional events occur.
> For example, what code should run when a hard- disk interrupt takes place, when a keyboard interrupt occurs, or when a program makes a system call?

The OS informs hardware of the locations of these **trap handlers** (is it places, that was mentioned in **system calls in C**, where procedure call put its arguments and system call number in some *well-known locations*? Is that **trap handlers**? One the hardware is informed it knows what to do when system calls and other exceptional events occurs.

- No, **trap handler** isn't stack or some specified register. **trap handler** is what check that locations when process execute **trup** instruction.

##### Really neet explanation.
> To specify the exact system call, a system-call number is usually as- signed to each system call. The user code is thus responsible for placing the desired system-call number in a register or at a specified location on the stack; the OS, when handling the system call inside the trap handler, examines this number, ensures it is valid, and, if it is, executes the corre- sponding code. This level of indirection serves as a form of protection; user code cannot specify an exact address to jump to, but rather must request a particular service via number.

Also, one of the features of **trap table** is that OS determine the kit of system calls. So then user-mode processes cannot call restricted operations.

##### 6.2 Problem #2: Switching Between Processes

Here is the thing: if a process is running on the CPU, this by definition means the OS is *not* running.

##### THE CRUX: HOW TO REGAIN CONTROL OF THE CPU

## A Cooperative Approach: Wait For System Calls

OS *trust* to processes and regain control by system call from that process. Also there is some another system call in such OS's - **yield**, which does nothing except to transfer control to the OS so it can run other processes.

The real problem is, what if process will stack in infinite loop, or/and never makes system calls?

> Because restricted operations were did by system calls, such OS has some **protection** in that case, because system call regain control to OS when get called 

## A Non-Cooperative Approach: The OS Takes Control

