.TL


ConcurrenC: A Concurrent Version
of the C Programming Language
for UNIX

.AU
Douglas Comer
.sp 3
Kenneth Rodemann
.AI
.sp 3
Computer Science Department
Purdue University
West Lafayette, IN 47907
.sp 3
.AB
A programming language supports concurrency
if it permits the programmer to specify a computation
that contains multiple threads of control.
Although specified as a set of concurrent tasks,
such computations are often executed on uniprocessor
architectures where a run-time support system
provides apparent concurrency by switching a single
processor among the subtasks of the computation.
Concurrent language run-time systems are distinguished from
general-purpose multiprogrammed operating systems in that they provide
mechanisms for communication and coordination among the pieces
of the computation.
Often, tasks in a concurrent program share memory.
.PP
ConcurrenC is a concurrent version of the C programming
language which supports multiple threads of control in a
single address space
as well as coordination and communication among these threads.
This paper describes the language as well as the implementation
of its run-time support in the UNIX operating system.
.AE
.sp 2
.NH
Introduction
.PP
Concurrent languages in general provide an easy way to program
problems which naturally break down into concurrent
threads of control.  For example, a solution to the Dining
Philosophers problem,
in which each philosopher can be simulated with a separate task,
is actually executable in a concurrent language.
.PP
The main goal of ConcurrenC
is a language which provides efficient
coordination and communication among tasks while preserving the
syntax and semantics of the C programming
language as much as possible.
Because C is closely linked to the UNIX operating system, we
have tried to preserve the usual semantics of UNIX system calls as well.
.PP
Throughout this paper, we will make a distinction between ConcurrenC
.I tasks
and UNIX
.I processes.
A UNIX process is a complete program unit within the UNIX operating system.
It executes in its own address space, isolated from all other processes.
A ConcurrenC task is one thread of control within a ConcurrenC
program (which executes as a UNIX process).
The UNIX operating system is ignorant of the fact that there are many
ConcurrenC tasks running in parallel within one of its processes;
there is no communication between the UNIX process scheduler and
the ConcurrenC task scheduler.
.PP
In order to concurrently execute many tasks within one
UNIX process, the ConcurrenC runtime system must
switch the processor among the tasks.  Switching the processor from
one task to another is called a
.I context
.I switch,
because the 'context' of the running environment is switched -- saved
for one task and restored for the other.
.PP
The UNIX operating system provides the user with
an asynchronous signal mechanism that can be
thought of as a
.I software
.I interrupt
mechanism.
Signals are pseudo-interrupts sent and received by processes within
the UNIX system.  UNIX allows a user to disable or enable receipt of particular
signals, as well as assign procedures as
.I interrupt
.I handlers
for these signals.
When a process receives a signal which is enabled,
the process's state is saved, the
interrupt handler for the
signal is executed,
and then execution of the process 
returns to the point at which the interrupt occurred.
.NH
The Language ConcurrenC 
.PP
Calling ConcurrenC a
.I language
is a bit presumptuous.
All concurrent operations are handled by a set of library routines.
ConcurrenC programs are compiled with the normal C compiler.  In fact, a
valid C program is also a valid ConcurrenC program.
.PP
The foundation upon which ConcurrenC is built is the
.I XINU
operating system.  ConcurrenC uses the concept of shared-memory tasks
and many system primitives from XINU, including
.I counting
.I semaphores
for mutual exclusion and task synchronization, XINU
.I ports
for general-purpose message
passing,
.I send
and
.I receive
primitives for intra-task message passing to
specific tasks, and
.I buffer
.I pool
routines for partitioned memory allocation.
ConcurrenC also has available most of the UNIX system calls and
library routines, including all
.I read/write
and
.I socket
calls as well as
.I stdio
routines.
.PP
Tasks are assigned priorities, and task
scheduling is based on this priority.
At any given time, the task ready to run with the highest priority
is the task which executes.
If more that one ready task has the highest priority, those tasks
share the CPU in a round-robin fashion.
Primitives are provided to
.I create
and
.I start
tasks,
.I suspend
and
.I resume
tasks,
.I change
.I priority
of tasks, and
.I kill
(terminate) tasks.
.NH
The ConcurrenC Runtime Environment
.PP
All tasks within a ConcurrenC program execute in the same UNIX address space.
There is one text segment. Many tasks may execute the same code
at the same time.
All tasks have access to global memory, which is how they share data
efficiently.  In order to maintain integrity of shared data,
tasks use semaphores for mutual exclusion and synchronization.
.PP
When a new task is created, it is allocated its own (uninitialized) stack
space.  All
local (automatic) data in a routine is allocated on this
stack.  Because local data is separate for each task,
two tasks can run the same routine in parallel without
destroying each other's local data.
The user can
specify the stack size of a task at runtime (using
.I create
).
.PP
UNIX provides each process with a
.I virtual
.I clock,
which runs only
when the process is executing, and a
.I real-time
.I clock,
which alway runs.
These clocks can be set to send a signal after a specified amount of time.
The ConcurrenC system uses these clocks for its timing purposes.  It uses the
real-time clock for timing tasks that request real-time delays (by calling
.I sleep
), and it uses the
virtual clock for task preemption.  When a context switch is performed, a
task is chosen to execute
and the virtual clock is set to give a signal in
0.2 seconds virtual time.  Because a task may be interrupted while
inside a ConcurrenC system call which changes global system data,
each system routine disables software interrupts,
and restores the old interrupt mask upon exit.  This keeps integrity
of global system data.
.NH
Implementation details
.PP
Before executing the user's main routine, the ConcurrenC system
initializes itself.  The system gets control before the user does
by syntactically changing the name of the user's main routine to
"_ccc__userproc" and naming the system
initialization routine "main".  After the system is initialized,
the user's main routine (now _ccc__userproc) is created and
resumed, while the task which did the initialization becomes
the
.I null
.I task.
.PP
One of the more interesting problems confronted in designing ConcurrenC
was how to deal with I/O.  Under normal circumstances, calling the system
.I read
or
.I write
routines will block a UNIX process if the operation cannot be
completed immediately.
Consider the following scenario:  many tasks are running when
one of them reads from a terminal, causing it to block.
Because UNIX does not know
there are many tasks running in this process, it blocks the
process, as normal.  But this now blocks the whole ConcurrenC system,
so the tasks not doing I/O cannot continue, as one would like them
to.
.PP
ConcurrenC handles this problem by
.I forking
a shadow process to run in parallel with the ConcurrenC process.  This shadow
process, called the
.I io
.I server,
handles all the tasks' I/O asynchronously.
When a task executes a UNIX system call which references the I/O table,
the call is intercepted by ConcurrenC.  An encoding of the call,
its arguments, and the
task id of the caller are sent to the io server via a pipe
communication link.  The io server uses non-blocking I/O to
handle the request.  If the call would block, the request is saved
and re-tried later when it won't block.
It then sends the results back to the ConcurrenC system, which
resumes the task and passes the results to it.  The io server is able
to handle many of these requests at one time without blocking, using the
.I select
system call.
.PP
The creation of the io server is not done at system initialization time,
but deferred until a second task is created.  If only one user task is
running, it does not matter if the call blocks, because there
are no other tasks that could be running.  This makes I/O 
more efficient for uni-task programs,
and there is no overhead for forking a UNIX process.
.PP
Each UNIX process has a global variable called
.I errno,
where system calls place an error code, should an error occur.
In keeping with this convention, ConcurrenC gives each task its
own errno.  This is accomplished by reserving a spot in each task table
entry to save the task's errno.  When a context switch is performed,
the old (current) value of errno is saved in the entry for the old task,
and errno for the new task is restored.  Thus, a task
can report its own errors without worrying about race conditions for errno.
.NH
Conclusions
.PP
ConcurrenC achieves most of what is was designed to do.  There are a few UNIX
system calls which are not available, such as
.I fork,
.I execve,
and
.I setitimer,
but most are available.  The syntax of the C programming language is
not changed at all.  Because of the necessity of having to deal very
closely with the system calls of UNIX, ConcurrenC will only run
on a Digital Equipment Corp. VAX computer running the Berkeley 4.2 UNIX
operating system.
.PP
ConcurrenC is low level in nature, in keeping with the C programming language
philosophy, but it is a good base to build upon.  Higher level operators
could be built upon the low level primatives, such as implementing
monitors on top of semaphores.
.PP
It is hoped that ConcurrenC will become a useful tool in the systems
programming community.
.LP
