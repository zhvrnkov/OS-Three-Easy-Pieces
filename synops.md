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
    char* str = argv[1];
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
  open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  ...
}
```

- when parent get called and `fork` inside its body didn't get called, the first file desciptor which is available is STDOUT_FILEN (look more about it in chapter). When `fork()` get called and we are inside children process in `if (rc == 0) { ... }` we close that file desciptor (output), and open another. Now all `prints` and etc will be sended to that output.

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
```c
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

Firstly, it seems that this is concern of compiler - to check that written code doesn't do anything restricted. But what if code do not pass compilation? What if we write code in binary? That is the case. Also! Compiler isn't CPU, compiler doesn't run code and you see, that we can dynamically add code by `exec` system call.

##### THE CRUX: HOW TO PERFORM RESTRICTED OPERATIONS

##### Q: why system calls looks like procedure calls?
##### A:
because in C, the `open()`, `close()`, `fork()` and etc are the procedure calls, but inside these procedure calls is the famous **trap** instruction.
Calling `open()` you execute the procedure call into the **C library**. Inside that call (or any other system call provided) the library used an agreed-upon calling (договоренная на базе вызова) convention with the **kernel** to put the arguments (of that `open()` call) in *well-known locations* (stack or specific register), put the system call number into *well-known location* (stack or specific register) and then executed aformentioned trap instruction. The code in the library after the trap unpacks return values and returns control to the program that issued the system call.

As mentioned above, we cannot let any process do whatever it like (issue I/O). For avoiding this, system calls were invented. Process call that system call (above answer is about how C call system call), give all control to OS via entering via exectuing **trap instruction**.

> 1. This instruction simultaneously jumps into the kernel and raises the privilege level to kernel mode; once in the kernel, the system can now perform whatever privileged operations are needed (if allowed), and thus do the required work for the calling process
> 2. When finished, the OS calls a special return-from-trap instruction, which, as you might expect, returns into the calling user program while simultaneously reducing the privilege level back to user mode.

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

> kernel stack isn't about that. OS do not save process progress into its kernel stack

The real things about **kernel stack** is that OS works with it in **kernel mode**, but why it cannot work with just **user stack** in **kernel mode** (while perfomrming system call)?

When trap handler correctly handle trap instruction and knows what code should be performed for that system call, the instructions will be executed (because system call is also code!) this is process (but in kernel mode and under OS control) and it is also need stack for local and other variables.

##### !!!:
Back to the beginning: system call -> system call number placed to some register, parameters of system call are in user stack  -> trap instruction -> switch to kernel mode -> move parameters for system call to **kernel stack** (save regs to kernel stack) -> jump to trap handler -> perform handle and run bunch of instructions (is system call is process too?) -> got some output before it -> return from-trap -> put output (which is stored in **kernel stack**) to the user stack -> move to user mode -> jump to PC after trap (to executing last process instruction).

##### Q: is system call is a process (but on behalf of kernel)? is it use kernel stack in the same way, as just process use user stack?

-----

##### Q: Also how does the trap knows what code it should run inside the OS?
##### A:
> the calling process can't specify an address in which it will jump. (an address of executable)

- Because this code will run at the **kernel mode** and if we could specify that code, we could run some code that was written by yourself at the **kernel mode**. If that so, then concept of **system calls** will be broken.

The kernel does so by setting up a **trap table** at boot time.

> When the machine boots up, it does so in privileged (kernel) mode, and thus is free to configure machine hardware as need be.

1. the first things the OS thus does is to tell the hardware what code to run when certain exceptional events occur.
> For example, what code should run when a hard disk interrupt takes place, when a keyboard interrupt occurs, or when a program makes a system call?

The OS informs hardware of the locations of these **trap handlers** (is it places, that was mentioned in **system calls in C**, where procedure call put its arguments and system call number in some *well-known locations*? Is that **trap handlers**? One the hardware is informed it knows what to do when system calls and other exceptional events occurs.

- No, **trap handler** isn't stack or some specified register. **trap handler** is what check that locations when process execute **trap** instruction.

##### Really neat explanation.
> To specify the exact system call, a system-call number is usually assigned to each system call. The user code is thus responsible for placing the desired system-call number in a register or at a specified location on the stack; the OS, when handling the system call inside the trap handler, examines this number, ensures it is valid, and, if it is, executes the corresponding code. This level of indirection serves as a form of protection; user code cannot specify an exact address to jump to, but rather must request a particular service via number.

Also, one of the features of **trap table** is that OS determine the kit of system calls. So then user-mode processes cannot call restricted operations.

##### 6.2 Problem #2: Switching Between Processes

Here is the thing: if a process is running on the CPU, this by definition means the OS is *not* running.

##### THE CRUX: HOW TO REGAIN CONTROL OF THE CPU

## A Cooperative Approach: Wait For System Calls

OS *trust* to processes and regain control by system call from that process. Also there is some another system call in such OS's - **yield**, which does nothing except to transfer control to the OS so it can run other processes.

The real problem is, what if process will stack in infinite loop, or/and never makes system calls?

> Because restricted operations were did by system calls, such OS has some **protection** in that case, because system call regain control to OS when get called. But such protection don't solve infinite loop problem

## A Non-Cooperative Approach: The OS Takes Control

If OS is doing process management by **cooperative approach** and some process enter infinity loop, then only one way could gain control back to OS - **reboot**.

##### THE CRUX: HOW TO GAIN CONTROL WITHOUT COOPERATION

Or OS could perform **timer interrupt** - raise interrupt every `N` milliseconds; when the interrupt is raised, the currently runnings process is halted, and preconfigured **interrupts handler** in the OS runs.

> At this point, the OS has regained control of the CPU, and thus can do what it pleases: stop the current process, and start a different one

> the OS must inform the hardware of which code to run when the timer interrupt occurs
- thus, at boot time, the OS does exactly that
- also OS must start the timer, which is of course privileged operation
> The timer can also be turned off (also a privileged operation), something we will discuss later when we understand concurrency in more detail.

Also hardware have some responsibility when an interrupt occurs: save enough of the state of the program that was running when interrupt occurred.
> This set of actions is quite similar to the behavior of the hardware during an explicit system-call trap into the kernel, with various registers thus getting saved (e.g., onto a kernel stack) and thus easily restored by the return-from-trap instruction.

### Saving and Restoring Context

##### Context Switch:
1. save a few register values for the current-running process (onto its kernel stack for example) (**Q**: why???)
2. restore a few for the soon-to-be-executing process (from its kernel stack)

**A**: well the answer is clear. In Kernel Mode OS works only with kernel stack. OS make decision to pause the process in Kernel Mode thus it works  with Kernel Stack.

1. Hardware save process state implicitly (in kernel stack) when handle trap instruction.
2. If OS decide to pause that process and run another, then it will save previous process kernel stack into memory (process structure) and restore kernel stack of future process from its proc struct.
3. Is OS decide to continue that process then restore saved into k-stack regs of that process and move to user mode

**Figure 6.4: The xv6 (AT&T assembly) Context Switch Code**

##### 6.4 Worried About Concurrency?

##### Q:
> "Hmm... what happens when, during a system call, a timer interrupt occurs?" or "What happens when you’re handling one interrupt and an- other one happens? Doesn’t that get hard to handle in the kernel?"

-----

##### ASIDE: How Long Does Context Switches Take?
##### :
+ For example, in 1996 running Linux 1.3.37 on a 200-MHz P6 CPU, system calls took roughly **4 microseconds**, and a context switch roughly **6 microseconds** [MS96].

-----

##### TIP: Reboot Is Useful

##### Homework (Measurment) - in microseconds:
1. `open("", O_RDONLY)` - 2.105571
2. `open("/", O_RDONLY)` - 1.1
3. `sched_yield()` - 0.853582
4. `printf("Hello world");` - 6.262538
6.  Nothing - 0.3

> The results for all cases except *Nothing* is recorded without time subtraction that passes between two `rdtsc()` calls.
> The results provides as arithmetic average of **1000000** iterations.





# Scheduling: Introduction

##### THE CRUX: HOW TO DEVELOP SCHEDULING POLICY

> The origins of scheduling, in fact, **predate** computer systems

## 7.1 Workload assumptions
1. Each job runs for the same amount of time.
2. All jobs arrive at the same time.
3. Once started, each job runs to completion.
4. All jobs only use the CPU (i.e., they perform no I/O) 5. The run-time of each job is known.


## 7.2 Scheduling metrics
`T`turnaround = `T`completion - `T`arrival

## 7.3 FIFO
- clear, simple and easy to implement.
Processes are run just in queue

If we relax first assumption, then FIFO is really bad policy for `T`-turnaround in case, where first job is larger the others - **convoy effect** (where a number of relatively-short potential consumers of a resource get queued behind a heavyweight resource consumer)

## 7.4 Shortest Job First (SJF)
This policy is **optimal** for cases, where jobs arrive at the same time and don't have equal length

##### ASIDE: Preemptive Schedulers:
In old days of batch programming, a number of non-**preemptive** schedulers were invented - they just run each process to completion and after it consider to run another job. Virtually all modern schedulers are preemptive and quite willing to stop one process from running in order to run another. Such **preemptive** characteristic is a merit of **context-switch**.

Resolve second assumption - now jobs can arrive not at the same time.

Because we don't relax our third assumption (that jobs runt to completion), SJF cannot perform **context-switch** and this is not optimal policy in cases, when biggest job arrives at 0, and some number of small jobs arrives after it.

## 7.5 Shortest Time-to-Completion First (STCF)

###### Q: think about SJF and relaxed third assumption

1. Some big job arrive - run it because there is no others
2. In big process duration comes two small processes - pause big job (at timer interrupt) and perform context switch to run that two small jobs in queue
3. Timer interrupt - continue with big job

## 7.6 A New Metric: Response Time
`T`response = `T`firstrun - `T`arrival

In case of this metrics, all mentioned above policies aren't optimal.

## 7.7 Round Robin
: instead of running jobs to completion, RR runs a job for a **time slice** (sometimes *scheduling quantum*) and then switches to the next job in the run queue. It repeatedly does so until the jobs are finished. (RR - sometimes *time-slicing*).

##### NOTE: the length of time slice must be N * timer-interrupt period (N is Integer > 0) - think about it (RR cannot perform context-switch yet time-slice pass)

This policy is great for `T`response but even worth for `T`turnaround (`T`firstrun is best, `T`completion is worst).

The length of **time-slice** is critical for RR policy. `time-slice`.length ∝ (proportional) `T`response. Making time slice too short also isn't good. Suddenly cost of context switch will **dominate** overall performance.

> Thus, deciding on the length of the time slice presents a trade-off to a system de- signer, making it long enough to **amortize** the cost of switching without making it so long that the system is no longer responsive

##### TIP: Amortization Can Reduce Costs
Time slice is 10 ms and the context-switch is 1 ms, roughly 10% of time is spent context switching and thus wasted. Setting time slice to 100 ms will reduce cost of context switch to 1%.

Cost os context switch isn' just OS actions of saving and restoring a few registers. The hardware also increase cost here. When context switch the **CPU** must change the states of: cache, TLBs, branch predictions, and other on-chip hardware.

> Switching to another job causes this state to be flushed and new state relevant to the currently-running job to be brought in, which may exact a noticeable performance cost

About worst `T`completion:
> More generally, any policy (such as RR) that is fair, i.e., that evenly divides the CPU among active processes on a small time scale, will perform poorly on metrics such as turnaround time.

## Incorporating I/O
Relax assumption 4 (program can make I/O requests)

While process perform waiting for I/O response, it isn't use **CPU**, so for better performance - run another process, overlap waiting process!

## 7.9 No More Oracle
The last assumption is that OS knows the length of process  - relax it. In real world the OS knows very little about process length. So how then we can build an approach the behaves like SJF/SCTF without such *priori* knowledge. Further, how can we incorporate some of the ideas we have seen with the RR scheduler so that response time is also quite good?

## 7.10 Summary
A scheduler that uses past to predict the future - **multi-level feedback queue**.

## Homework in my note




# Scheduling: The Multi-Level Feedback Queue
**MLFQ**  tries to address two-fold:
1. *turnaround time* optimization
2. minimize *response time* for better user-interaction

> RR is best for r.t. but terrible for t.t.

###### THE CRUX: HOW TO SCHEDULE WITHOUT PERFECT KNOWLEDGE?

###### TIP: LEARN FROM HISTORY
The **MLFQ** - the queue that learns from past to predict the future.

## 8.1 MLFQ: Basic Rules
:
+ has a number of distinct **queues**
+ each queue is assigned to a different **priority level**

MLFQ decide which job should run at a given time:
***a job with higher priority is chosen to run***

Of course, more then one job may be on a given queue, and thus have the *same* priority:
***in this case, we will just with RR scheduling among those jobs***

Thus, the key to MLFQ scheduling is about:
***deciding the priority of a job***

Also MLFQ *varies* the priority of a job based on its *observed behavior*

### The first two basic rules of MLFQ:
1. **Rule 1**: If Priority(A) > Priority(B) then A run (B doesn't)
2. **Rule 2**: If Priority(A) = Priority(B) then A&B run in RR

[Hight Priority] Q8: -> (A) -> (B)
...
                 Q4: -> (C)
...
[Low Priority]   Q1: -> (D)

:
+ A and B runs in RR
+ What about C and D? Seems like that they execution status is in trouble

***So we need to understand how job priority changes over time***

## 8.2 Attempt#1: How To Change Priority

##### First attempt at a priority- adjustment algorithm:
3. **Rule 3**: When a job enters the system, it is placed at the highest priority (the topmost queue)
4. :
    + **Rule 4a**: If a job uses up an entire time slice while running, its priority is *reduced* (i.e., it moves down one queue).
    + **Rule 4b**: If a job gives up the CPU before the time slice is up, it stays at the *same* priority level.

> Gives up the CPU before the time slice and keep staying in queue is system call case

**Note**:
+ Rule 4a isn't fair for jobs, that at some period of time were CPU-costly and in future become *low* CPU-costly. Because of that they must be elevated in priority
+ Rule 4b is good for cheater-processes, that artificially satisfy that Rule by making system call each period of time

### Example 1: A Single Long-Running Job

### Example 2: Along Came A Short Job
(***About how MLFQ approximate SJF policy***)

one of the major goal of mlfq algorithm:
> because it doesn’t **know** whether a job will be a short job or a long-running job, it first **assumes** it might be a short job, thus giving the job high priority
> If it actually is a short job, it will run quickly and complete;
> if it is not a short job, it will slowly move down the queues, and thus soon prove itself to be a long-running more batch-like process.

- this is how MLFQ approximates SJF

### Example 3: What About I/O?

### Problems With Our Current MLFQ
1. What if we got more really big number of quick jobs that runs on the *topmost* priority? The lower priority processes may never run in this case
2. Cheaters, that gives up the CPU specially for satisfying the rule and stay on the topmost queue
3. A program can *change its behavior* over time; what was CPU-bound may transition to a phase of **interactivity**.

## 8.3 Attempt #2: The Priority Boost
We can solve 1 and 3 problem by one shoot: perform the **** of priority for all executing processes - put all on the topmost queue.

5. **Rule 5**: After some time period S, move all the jobs in the system to the topmost queue.

##### Q: what should S be set to?
##### A: John Ousterhout, a well-regarded systems researcher [O11], used to call such values in systems *voo-doo constants*, because they seemed to require some form of black magic to set them correctly

> Unfortunately, S has that flavor. If it is set **too high**, long-running jobs could starve; **too low**, and interactive jobs may not get a proper share of the CPU.

## Attempt #3: Better Accounting
We now have one more problem to solve: how to prevent gaming (**cheaters**) of our scheduler?

The real **culprit** here, as you might have guessed, are **Rules 4a and 4b**

4. **Rule 4**: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).

## 8.5 Tuning MLFQ And Other Issues
***Parameterizing***
##### Q: number of queues, duration of time slice, value of S - all that need to be set

##### A:
There are no easy answer to these questions, and thus only some experience with workloads and subsequent tuning of the scheduler will lead to a satisfactory balance.

For example, most **MLFQ** variants allow varying time-slice length across different queues:
+ The **high-priority queues** are usually given short time slices; they are comprised of interactive jobs, after all, and thus quickly alternating between them makes sense (e.g., 10 or fewer millisec- onds)
+ The low-priority queues, in contrast, contain long-running jobs that are CPU-bound; hence, longer time slices work well (e.g., 100s of ms)

##### TIP: AVOID VOO-DOO CONSTANTS (OUSTERHOUT’S LAW)
If you want to customize your OS constant - do it in some file. But for those users, that don't want to do it - here is the default values. - ***Ousterhout’s Law***

+ **Rule 1**: If Priority(A) > Priority(B), A runs (B doesn’t).
+ **Rule 2**: If Priority(A) = Priority(B), A & B run in RR.
+ **Rule 3**: When a job enters the system, it is placed at the highest priority (the topmost queue).
+ **Rule 4**: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).
+ **Rule 5**: After some time period S, move all the jobs in the system to the topmost queue.



# 9 Scheduling: Proportional Share  

Such scheduling mechanism try to guarantee that each job obtain a certain percentage of CPU time *instead* of optimizing turnaround or response time.

One of the excellent **proportional share** scheduler is **LOTTERY SCHEDULER**.

##### THE CRUX: HOW TO SHARE THE CPU PROPORTIONALLY

## 9.1 Basic Concept: Tickets Represent Your Share

**Tickets** are used to some king of represent the share of resources that a process *should recieve*.

proc. A got 75 tickets, proc. b got 25. We would expect that A will receive 75% of CPU time and B 25%.
- Lottery scheduling achieves this probabilistically (not deterministically) by holding a lottery every so often (every time slice).

To make that scheduler works it need to know the *total amount* of tickets. The scheduler picks random number from that range (0..<amount) and pick the winner (if number in 0..<A-ticket-amount then A win and etc).

# TIP: Use Randomness
1. It is robust and simple
2. lightweight, there is now any proc. tracking. Only need to know total amount of tickets
3. The speed is proportional to speed of random number generating

# TIP: Use Tickets to Represent Share
The **ticket** is used to represent a process's *share of the CPU* in these examples.

## 9.2 Ticket Mechanisms

+ **Ticket currency** - allows a user with a set of ticket to allocate tickets among their own job in whatever currency they would like; the system will automatically convert each currency to one-common (global) currency

+ **Ticket transfer** - a process can temporarily hand off its tickets to another process. Very useful in client/server case: client ask the server to do some work on the client's behalf. To speed up the work, the client transfer all its tickets to the server.

+ **Ticket inflation** - a process can temporarily raise or lower the number of tickets it owns. This make little sense in scenario, where processes don't trust each other, but in another scenario this is vary useful mechanism.

```c
// Consider the total tickets as a number line, the `counter` is current position on that line
int counter = 0;

// Generate random point on that number line - when we pass or have reached that point then we have reached the winner process.
// Each process is something like sector on that number line. The width of that sector is representing by number of tickets of that process
int winner = getrandom(0, totaltickets);

// starts from the head
node_t *current = head;

// in c while will stop when condition will equal 0 (false?)
while(current) {
    counter += current->tickets;

    if (counter > winner)
        break;

    current = current->next
}
```

## 9.3 Implementation

For implementing lottery scheduling you need:
+ random number generator
+ data structure to track the process (e.g. linked list)
+ total number of tickets

> Notice that order of nodes (processes) doesn't affect the correctness of the alrogithm

## 9.4 An Example

We introduce `U` - unfairness metric.
`U = #1JOB-time-completion / #2JOB-time-completion`

A perfectly fair scheduler have `U ~ 1`

## 9.5 How to Assign Tickets?
- a tough problem of that scheduling mechanism
- "ticket-assignment problem"

## 9.6 Why Not Deterministic?

The deterministic variant of **lottery scheduler** is **stride scheduler** (a deterministic fair-share scheduler)

+ each job in the system has a **stride**. The stride for some proc is `= tickets.count / Some large constant`
+ each job in the system has a **pass value** (e.g. all proc in the system starts with 0 pass value)
+ every time a process runs, we will increment its **pass value** by its **stride**.
+ at any given time, pick the process to run that has the lowest pass value so far; when you run a process, increment its pass counter by its stride.

##### PSEUDO CODE:
```c
current = remove_min(queue); // pick client with minimum pass
schedule(current); //use resource for quantum
current->pass += current->stride; // compute next pass using stride
insert(queue, current); // put back into the queue
```

##### Q: Why lottery and not stride?
##### A:
Lottery scheduler has no **global state**
> Imagine a new job enters in the middle of our stride scheduling example above; what should its pass value be? Should it be set to 0?
> If so, it will monopolize the CPU
> In Lottery scheduler we don't have any global state and adding new proc isn't a problem

## 9.7 Summary

# 10. Multiprocessor Scheduling (Advanced)

## 10.1 Background: Multiprocessor Architecture

1. Don't clear understand the concept of caches **locality**. The localities itself is perfeclty clear, but what it is in terms of caches?
   + Caches could have one of these localitites?
   + Or system can fill the cache based on the decision of necessary locality in current case? Then how it make that decision? By analizing "code"?

Prevoiusly we take a big part of **scheduling**, but there is sceduling in case of single-threaded processes. Now we will discuss scheduling multi-threaded processes in multicore system.

+ Previous chapters of scheduling is for single-core systems.

The main topic of this chapter is **caches** in multi-core systems.

### Locality
Caches could be of two *locality* types:
1. temporal - data placed inside cache and stored here because we consider, that it will be accessed again in the near future. (e.g. variables inside loop or some another repeated operation)

2. spatial - data places inside cache and stored ehre because we consider, that close data will be accessed after that. A data item at address `x`, is is likely to access data items near `x` as well. (e.g. iteration through array, instructions executing one after another)

### Cache coherence
##### Q: multi-core system with single shared main memory.
Consider this situation:
1. CPU1 reads value D from address A
2. CPU1 stash value D to its cache
3. CPU1 modify value D -> updating its cache with that new D' value

> writing the data through all the way to main memory is slow, so the system will (usually) do that later

4. OS decide to transfer that process from CPU1 to CPU2
5. re-reads the value at address A because there is no such data in CPU2 cache (D' stored in CPU1 cache and didn't save in shared main memory). Proc get old D value instead the correct value D'.

##### Q:
1. program interrupted.
2. Most recent value D (the result of that program manipulations with that value) stored in cache and don't been saved in memory.
3. why at this moment (context switch) we don't save that value D to proc structure?
4. And then, when OS decide to continue that proc. on different CPU(2) (where cache is empty regarding that process) restore that value D and get exactly this value that we expect

+ Also why here isn't a word about **dirty bit**?

##### A:
I consider that this value D isn't just variable that stored on the stack (and will be saved in proc. struct), and isn't variable from the heap. So when OS saves **regs and stacks** of that proc. it don't save cache of that process. Don't clear understand why, but maybe that really heavy, to save current cache to proc. structure for each process, or decide what part of cache need to be saved also heavy and complex operation.

The basic solution of that problem is **provided by the hardware**:
> by monitoring memory accesses, hardware can ensure that basically the “right thing” hap- pens and that the view of a single shared memory is preserved

One way to do this is **bus snooping**:
+ each cache pays attention to memory updates by observing the bus that connects them to main memory
+ cpu sees the update of data, that stored in its cache -> will notice that -> **invalidate** data in its cache (remove or update it).

## 10.2 Don’t Forget Synchronization

The problem **described above** is about relevance data, that stored in cache of single CPU.

##### Q: but what about access to shared memory by many CPUs?

To guarantee correctness we as a programmers need to use mutual exclusion primitives (such as **locks**).

> other approaches, such as building **lock-free** data structures, are complex and only used on occasion; see the chapter on deadlock in the piece on concurrency for details (private contexts in CoreData and etc)

### Example
+ Single shared queue being accessed on multiple CPUs concurrently
+ Without locks, concurrent manipulations with this queue will not work as expected

1. Single shared linked list
2. Two separate processes which runs parallel on different CPUs
3. Each proc get head of that linked list
4. Each proc frees that memory, in which head were placed
5. Each proc return that value of head

**Summary**:
1. Double free
2. Double return the same value

```c
typedef struct Node_t {
    int value;
    struct Node_t (pointer)next;
} Node_t;

int List_Pop() {
    Node_t (pointer)tmp = head;
    int value = head->value;
    head = head->next;
    free(tmp);
    return value;
}
```

### Solution
make such routines correct via **locking**. In this case allocation simple **mutex** (`pthread_mutex_tm`) and then adding a `lock(&m)` at the beginning of the routine and `unlock(&m)` at the end.

> Unfortunately, as we will see, such an approach is not without **problems**, in particular with regards to **performance**. Specifically, as the number of CPUs grows, access to a synchronized shared data structure becomes quite **slow**

## 10.3 One Final Issue: Cache Affinity (связь, близость, сходство)

A proc when run on a particular CPU, build up a fair bit of state in caches (and TLBs) of the CPU. The next time the proc runs, it is often advantageous to **run it on the same CPU**, as it will run **faster** if some of its state is **already** present in the caches on that CPU.

## 10.4 Single-Queue Scheduling

The most basic approach is to simply **reuse the basic framework** for single processor scheduling, by putting all jobs that need to be scheduled into a single queue. - ***Single queue multiprocessor scheduling*** (SQMS).

1. Put all jobs on a single queue
2. Decide best N jobs (where N is number of cores) based on existing policies
3. Pick that N jobs and run on this CPUs

### Problems
+ **scalability**:
because there is a single queue of processes, locking needs to be performed which is great performance **reducing**
> Locks ensure that when SQMS code accesses the single queue (say, to find the next job to run), the proper outcome arises

---------------------------
##### Q:
At what moment we locking that queue? When CPU get that process (manipulations with queue), or even when CPU runs that process.
##### A:
I think first, if second then there is no "multicoring".
---------------------------

+ **cache affinity**:
queue:
a -> b -> c -> d -> null

CPU0: abcd (repeat)
CPU1: bcda (repeat)
CPU2: cdab (repeat)
...

There is no **cache affinity** because each CPU simply picks the nest job to run from the globally shared queue.

Solution is include in SQMS some kind of affinity mechanism. Make some balance regarding this:

CPU0: adaa (repeat)
CPU1: bbdb (repeat)
CPU2: cccd (repeat)

## 10.5 Multi-Queue Scheduling
Multiple Queues - one per CPU

**multi-queue multiprocessor scheduling** (MQMS)

+ Each queue will likely follow a particular scheduling discipline, such as round robin, though of course **any algorithm** can be used.
+ Job enters the system -> placed on exactly one scheduling queue (randomly, or picking one with fewer jobs that others)
+ Because there is not shared queue, there is no needs to perform locking.

q1:
    a -> c
q2:
    b -> d

CPU0: aaccaaccaacc (repeat)
CPU1: bbddbbddbbdd (repeat)

### Advantages against SQMS:
+ more scalable - no locking (locking queues of execution). Number of CPU grows, but because there is no locking - it isn't central problem
+ intrinsically provides cache affinity - jobs stay on the same CPU.

### Problems
+ **load imbalance**:
c job from q1 get completed:
q1:
    a
q2:
    b -> d

CPU0: aaaaaaaaaaaa (repeat)
CPU1: bbddbbddbbdd (repeat)
Which is load imbalance, a process monopolize the CPU

Also, when a get completed, CPU0 will **just idle**

##### CRUX: HOW TO DEAL WITH LOAD IMBALANCE

The answer is process **migration**.
When CPU0 just idle, scheduler need to migrate b or d processes to that CPU - it is simple

But what about the case, where a monopolize the CPU? - migration here isn't so simple at first sight. But what can we do? **Just keep switching jobs between these CPUs**

The trickiest part of it is how should the system decide to enact(утверждать) such a migration?

One basic approach is **work stealing**:
+ a source queue (that is low on jobs) will occasionally peek at another (target) queue, to see how full it is
+ If the target queue is (notably) more full than the source queue, the source will “steal” one or more jobs from the target to help balance load

## 10.6 Linux Multiprocessor Schedulers
No common solution has approached to build such thing.

Over time, three different schedulers arose:
+ the O(1) scheduler,
+ the Completely Fair Scheduler (CFS),
+ and the BF Scheduler (BFS) (brain-fuck)

O(1) and CFC use multiple queues, whereas BFS uses a single queue, showing that both approaches can be useful.

O(1) is priority based scheduler (similar to MLFQ)
CFS is deterministic proportional-share approach (like Stride scheduler)
BFS, the only single-queue approach among the three, is also proportional-share, but based on a more complicated scheme known as Earliest Eligible Virtual Deadline First (EEVDF)

# The Abstraction: Address Spaces

## 13.1 Early Systems
The OS was **just a library** that sat in memory (e.g. at 0 address) and there would be one running program (a process) that currently sat in physical memory (at 64KB address for example)

## 13.2 Multiprogramming and Time Sharing
The era of **multiprogramming** was born - the sharing of computer become more common.

Multiple processes were ready to run in single system. OS provides that by switching between these processes (e.g. when one decides to perform I/O).

Further more, era of **time-sharing** was born - the notion of interactivity became important

> The difference between time-sharing and multiprogramming is in time-sharing systems, context switch performs every time slice while multiprogramming system decide to switch to another job when current job perform I/O (not using the CPU)

## 13.3 The Address Space
- is the running program's view of memory in the system

The address space of a process **contains all of the memory** state of the running program:
+ code (the instruction)
+ stack (keep track where the program is, local variables, parameters, return values)
+ heap (dynamically-allocated user-managed memory)

1. Code is **static** - placed at the 0 address of address space. It won't need anymore space

Two regions that may grow:
2. the heap (placed at the top - after code)
3. the stack (placed at the bottom of address space)
    - they grow in opposite directions

> However, **this placement** of stack and heap is just a **convention**; you could arrange the address space in a different way if you’d like (as we’ll see later, when multiple threads co-exist in an address space, no nice way to divide the address space like this works anymore, alas).

!!! The program isn't in memory at physical address 0 through 16KB - it is illusion.

##### THE CRUX: HOW TO VIRTUALIZE MEMORY

For example, a.s. of proc A is placed in 320 KB address. When proc try to access the memory of **its** address 0, OS, in tandem with some hardware support, will have to make sure the load doesn't actually go to 0 address (of physical memory) but rather to 320 KB address (where A's a.s. is placed) - this is key to virtualization of memory.

##### TIP: The Principle of Isolation
is a key principle in building reliable systems. If two entities are isolated, then one of them could fail without implies to another. OS strive to isolate processes from each other and in this way prevent one from harming the other. In terms of OS the key principle is **memory isolation** but some OS's go further and provide **microkernels** - such thing may provide greater reliability than typical monolithic kernel design.

## 13.4 Goals
1. transparency
> The OS should implement virtual memory in a way that is invisible to the running program

**program behaves as if it has its own private physical memory**

2. efficiency
> The OS should strive to make the virtualization as efficient as possible, both in terms of time and space

hardware support (features such TLBs) will help in this goal

3. protection
> The OS should make sure to protect processes from one another as well as the OS itself from processes

**each process should be running in its own isolated cocoon**

##### ASIDE: Every Address You See Is Virtual

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    printf("location of code  : %p\n", (void*) main);
    printf("location of heap  : %p\n", (void*) malloc(1));
    int x = 3;
    printf("location of stack : %p\n", (void*) &x);
    return x;
}
```

The output is:
```
location of code  : 0x1095afe50
location of heap  : 0x1096008c0
location of stack : 0x7fff691aea64
```
Notice, that all these three addresses are virtual!

# Interlude: Memory APIs
Best practice is to come back to that interlude when I will really need it (homework). Right now I don't really need to know how to debug in `gdb` or use memory manager (`valgrind`) to proper C programming.

# Mechanism: Address Translation
**Limited direct execution** (or **LDE**): for the most part let the program run directly on the hardware; however at certain key points in time (such as when process issues a system call, or a timer interrupt execution), arrange so that the OS gets involved and makes sure the "right" thing happens.

**Efficiency** is hardware support
**Control** is memory isolation

##### THE CRUX: HOW TO EFFICIENTLY AND FLEXIBLY VIRTUALIZE MEMORY

One of generic techniques that OS use is **LDE**, but we add another one - **hardware-based address translation**.
+ hardware transforms each memory access by changing the **virtual** address provided by the instruction to a **physical** address.
+ or *redirect application memory references to their actual locations in memory*.

But now one hardware will implement memory virtualization. OS will also take part in it, more precisely it will **manage memory**, keeping track of which locations are free and which are in use.

## 15.1 Assumptions
+ space must be places *contiguously*
+ address space is not too big (*less than size of physical memory*)
+ each address space is exactly the *same size*

## 15.2 An Example

```c
void func() {
    int x;
    x = x + 3; // point of interest
}
```
x86_assembly:
```
128: movl 0x0(%ebx), %eax ;load 0+ebx into eax
132: addl $0x03, %eax     ;add 3 to eax register
135: movl %eax, 0x0(%ebx) ; store eax back to memory
```
from the perspective of the process it looks like this:
```
+ Fetch instruction at address 128
+ Execute this instruction (load from address 15 KB) • Fetch instruction at address 132
+ Execute this instruction (no memory reference)
+ Fetch the instruction at address 135
+ Execute this instruction (store to address 15 KB)
```

Notice that from the program's perspective, its address space starts at address 0 and (in our case) grows to maximum 16 KB; all memory references it generates should be within these bounds. However, to virtualize memory, the OS wants to place the process somewhere else in physical memory, not necessarily at address 0.

## 15.3 Dynamic (Hardware-based) Relocation
ideas: base and bounds, dynamic reallocation

OS need to registers: base register and bounds (limit) register

1. OS decide where in physical memory program should be loaded and sets the **base register** to that value
2. Any memory access will be translated by the following:
`physical address = virtual address + base`

##### ASIDE: Software-based relocation

Lets dive in what happens when:
`128: movl 0x0(%ebx), %eax`

PC (program counter) is set to 128
Because `base` is 32 KB, then hardware add 32768 + 128 = 32896 and receive the physical address of that instruction

Notice, that because this relocation take place at runtime, this calls **dynamic relocation**

##### TIP: Hardware-based Dynamic Relocation
+ **bases** fro transforming virtual addresses
+ **limit** for ensuring that such address are within the confines of the address space

Also notice, that first (after translation to p.address) hardware will check that provided address is legal by comparing it (virtual address) with **limit**. If `virtual address > limit` then CPU will raise an exception.

**limit** register can be defined in two ways:
1. just store the size of address space. OS will check that virtual address is less than bounds and everything is fine
2. store the physical address of the end of address space and thus the hardware first add base to virtual address and then compare that received value is less then bounds
- Both methods are logically equivalent

##### ASIDE: Data Structure - The Free List
list of the ranges of the physical memory which are not currently in use

## 15.3 Hardware Support: A Summary
Summarize the support which we need from the hardware
+ OS runs on **kernel mode**
+ apps runs on **user mode**

> A single bit, perhaps stored in some kind of processor status word, indicates which mode the CPU is currently running in

Hardware need to provide **base and bounds registers**. Each CPU have the addition part of registers which are part of **memory management unit** (MMU) of the CPU.

Hardware bust be able to validate the address

Hardware should provide the special instructions to modify the base and bounds registers(**privileged operations**), allowing the OS to change them when different processes run.

CPU must be able to generate **exceptions**

> There is perfect table of hardware requirements on the book

## 15.5 Operating System issues
OS need to perform **memory management**, finding free space and etc.

OS need to do some work when a process is terminated (exit or killed). Reclaiming all of its memory. Put memory of that terminated process back to free list, clean all its data.

OS must *save* and *restore* the base-and-bounds pair when it perform context switch.

In addition, when the process is stopped (i.e. not running), it is possible for the OS to move an address space from one location in memory to another rather easily:
1. deschedule the process
2. copies the address space from the current location to new location
3. updates the saved base registers

OS must provide **exception handler**

**TODO**: Dive in in Figure 15.5

## Summary
+ internal fragmentation
+ segmentation

# Segmentation
You might noticed the big chunk of "free" space right in the middle, between the stack and the heap

Base and bounds mechanism isn't too flexible as we would like because of two things:
1. big chunk of "free" memory in the middle is wasteful
2. because of this it is quite hard to run a program when the entire address space doesn't fit into memory. We got single and **solid** address space which is unwieldy in some sort of things.

##### THE CRUX: HOW TO SUPPORT A LARGE ADDRESS SPACE

## 16.1 Segmentation: Generalized Base/Bounds
To solve this problem the idea was born - **segmentation** (very early 1960's)

The idea is simple: instead of having one pair of base/bound register per address space, why not have pair of registers for each logic **segment** of the address space (code, stack, heap).

Segmentations is **allows OS** to place each one of those segments in different places in physical memory (with this mechanism we will not have solid address space from "physical memory point of view" which will add more flexibility).

> With a base and bounds pair per segment, we can place each segment **independently** in physical memory

##### ASIDE: The Segmentation Fault
or violation arises from a memory access on a segmented machine to an **illegal address**. Humorously, the term persists, *even on machines with no support for segmentation at all*.

##### Note: also some notes in notebook.

## 16.2 Which segment are we referring to?
Hardware uses segment registers during translation.

##### Q: How does it know the offset into a segment, and to which segment an address refers?

VA: 0b01000001101000
- top two bits are the number of segments (01)
- other bits is offset

For example 00 - code segment, 01 - heap, 10 - stack

0b01000001101000 (4200) virtual address
1. 01 - heap
2. 12 last bits `0b000001101000`, or `0x068` or `104` is offset

+ Hardware takes two top bits to determine the segment
+ Get other bits to calculate the offset
+ Check that offset is less then limit register
+ Add offset to base register and get p.address

```c
// get top 2 bits of 14-bit VA
Segment = (VirtualAddress & SEG_MASK) >> SEG_SHIFT
// now get offset
Offset  = VirtualAddress & OFFSET_MASK
if (Offset >= Bounds[Segment])
    RaiseException(PROTECTION_FAULT)
else
    PhysAddr = Base[Segment] + Offset
    Register = AccessMemory(PhysAddr)

// SEG MASK = 0x3000
// SEG_SHIFT = 12
// OFFSET_MASK = 0xFFF
```
> In some system only one bit of VA is used as segment number - they just concat HEAP and CODE segments into one

This techniques is **implicit approach** (if that tells you something).

## 16.3 What about the Stack?
Stack *grows backward*. In physical memory it starts at `28KB` and grows back to `26KB` (VA: `16KB` -> `14KB`) and translation must be proceed differently.

Need little hardware support for this: Instead of just base and bounds values, the hardware also needs to know which way the segment grows - 1 bit of information (0 - negative direction, 1 - positive).

In case of stack we need to subtract offset, but from what, from base value? I think no and this is reasonable, because stack grows in opposite direction, then start of the stack is also in opposite direction = base + limit. ***Also add note in notebook aboit it***.

## 16.4 Support for sharing.
> ... to save memory sometimes it is useful to **share** certain memory segments between address spaces. In particular, **code sharing** is common and still in use in systems today.

To support this we need little support from the hardware - new **protections bits**. Add a few bits per segment indicating whether or not program can read or write a segment, or perhaps execute code that lies within the segment.

##### TODO: Dont understand that well. Find additional info about it.

## 16.5 Fine-grained vs Coarse-grained Segmentation

Coarse-gained segments are our case: heap, code and stack are relatively bit segments regarding *fine-grained*.

Fine-grained segments were in use in early systems, where OS supports thousands of segments per process. For each segment there is a row in *segment table* which were stored in memory.

## 16.6 OS Support
1. What should OS do on a context switch?
the segment registers must be saved and restored

2. Managing free space in physical memory.
notice that segments might be a different size

#### External fragmentation
> physical memory quickly becomes full of little holes of free space, making it difficult to allocate new segments, or to grow existing ones

One solution of this problem is **compact** physical memory be rearranging the existing segments. OS could:
+ stop whichever process are running,
+ copy their data to one contiguous region of memory,
+ change their segment register values regarding new physical memory

Simpler approach is to use **free-list** management algorithm that tries to keep **compact** arrangement from the beginning. There are literally hundreds of such approaches which means that **there is no single solution** and external fragmentation is **still exists**.
