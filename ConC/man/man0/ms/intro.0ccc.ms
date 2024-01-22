.TL
.ND




ConcurrenC Programmer's Manual


Version 1.0






May 1985



.AB no
This manual contains a description of the ConcurrenC software.
It has been divided into five sections.
The introduction in section 0 gives this file.
Section 1 gives a manual page for
.I ccc
and
.I tccc,
the ConcurrenC compiling shell scripts.
Section 2 describes ConcurrenC system procedures that programs call
to invoke ConcurrenC operating system services and UNIX system procedures which
are either changed or not available in ConcurenC.
Section 3 describes ConcurrenC procedures available from the standard
libraries and UNIX library procedures either changed, not available, or
those with which care must be taken in order to use them.
Section DEBUG describes ConcurrenC routines for runtime debugging.
From the programmer's point of view,
there is little distinction between library routines and
system calls.
.PP
As in the UNIX manual, each page describes a command, system
call or library routine;
section numbers appear in the page header
as "(<digit>)" for UNIX routines,
as "(<digit>CCC)" for ConcurrenC routines,
and as "(DEBUG)" for ConcurrenC debugging routines, following the name
of the program;
References have the same form as headers (e.g., "swait(2CCC)"
refers to the page for "swait" in section 2ccc).
.AE
.LP
.ds LH INTRO(0)
.ds RH INTRO(0)
.ds CH "ConcurrenC Programmer\'s Manual
.ds RF %
.ds CF "\n(mo/\n(dy/\n(yr
.ds LF "Version 1.0
.bp
.B
.ce
ConcurrenC Implementation Notes (taken from XINU)
.R
.sp
.SH
Some quick ideas:
.IP - 3
There are multiple tasks.
.IP - 3
A task is known by its task id.
.IP - 3
The task id is an index into the task table.
.IP - 3
There are counting semaphores.
.IP - 3
A semaphore is known by its index in the semaphore table.
.IP - 3
The system schedules tasks based on priority.
.IP - 3
I/O is done via the UNIX file system (no non-blocking I/O allowed). 
.IP - 3
There is a one-word message passing mechanism.
.IP - 3
Tasks can create tasks, kill tasks, suspend tasks,
restart tasks, and change the priority of tasks.
.LP
.SH
SEE ALSO
.IP
intro(1CCC), intro(2CCC), intro(3CCC), intro(DEBUG)
.SH
Discussion of implementation:
.sp
.IP 0. 3
Files.
.br
The system sources are organized in the following way:
.sp
.nf
   {ConC-dir}/include:
      C_ioctl.h     - UNIX ioctl.h include file with mods
      bufpool.h     - Buffer pool constants and format.
      errno.h       - UNIX errno.h include file with mods
      kernel.h      - General symbolic constants; misc
                      defined macros.
      mem.h         - Memory management declarations.
      netdb.h       - UNIX netdb.h include file with mods
      ports.h       - Definitions for communications ports
                      (queued intertask rendevous points).
      proc.h        - Task table entry structure
                      declaration; state constants.
                      (the name is for historical reasons)
      q.h           - q data structure declaration;
                      defined macros for q predicates.
      sem.h         - Semaphore table entry structure
                      declaration; semaphore constants.
      stdio.h       - UNIX stdio.h include file with mods
      syscall.h     - UNIX syscall.h include fl with mods

   {ConC-dir}/src:
      ConC_sys      - ConcurrenC system routines
      compat-4.1    - UNIX compat-4.1 sources with mods
      gen           - UNIX gen sources with modifications
      inet          - UNIX inet sources with modifications
      net           - UNIX net sources with modifications
      stanford      - UNIX stanford sources with mods
      stdio         - UNIX stdio sources with mods
      sys_interface - ConcurrenC system interface routines
                     (to interface with UNIX system calls)
      vax:
          gen       - UNIX vax/gen sources with mods
          net       - UNIX vax/net sources with mods
          stdio     - UNIX vax/stdio sources with mods;
                      also includes kprintf and
                                    kprintf_serv
          sys       - files of the form "sys_<syscall>.s",
                      which ConcurrenC calls in order to
                      access the actual UNIX system calls.
.fi
.sp
Most of the important ConcurrenC routines are in
.br
{ConC-dir}/src/ConC_sys
and {ConC-dir}/src/sys_interface.
.IP 1. 3
Task states.
Each task has a state given by the pstate field in the task
table entry.
The state values are given by symbolic constants PRxxxx.
PRFREE means that the task entry is unused.
PRREADY means that the task is linked into the ready list
and is eligible for the CPU.
PRWAIT means that the task is waiting on a semaphore (given by
psem).
PRSUSP means that the task is in hibernation; it is not on any list.
PRSLEEP means that the task is in the queue of sleeping
tasks waiting to be awakened.
PRCURR means that the task is (the only one) currently running.
The currently running task is NOT on the ready list.
In addition to the actual state, there is a "mark" field (pmark) which 
indicates pending state changes.
PMDEAD indicates that the task has been killed and should be
removed as soon as it reaches the ready queue.
PMSUSP indicates that the task has been suspended and should
move to the suspended state as soon as it reaches the ready
queue.
PMNONE indicates no pending action.
.IP 2. 3
Semaphores.
Semaphores reside in the array semaph.
Each entry in the array corresponds to a semaphore and has a
count (scount), and state (sstate).
The state is SFREE if the semaphore slot is unassigned,
SUSED if the semaphore is in use.
If the count is -p then the sqhead and sqtail fields point to
a FIFO queue of p tasks waiting for the semaphore.
If the count is nonnegative p then no tasks are waiting.
More about the head and tail pointers below.
.IP 3. 3
Suspension.
Suspended tasks are forbidden from using the CPU.
They may remain on semaphore/sleep queues until they
are to be moved to the ready queue.
A call to ready(p), where p has been marked suspended, will
NOT place it on the ready queue.
It will merely result in p being placed in the suspended state.
Suspending a task that is already on the ready queue will
remove it.
Suspending the current task forces it to give up the CPU.
.IP 4. 3
Sleeping.
When a task calls sleep(n) it will be delayed n seconds.
This is achieved by placing the task on a queue of jobs
ordered by wakeup time and relinquishing the CPU.
The granularity of clock counts is 1/10 of a second.
The interrupt handler maintains a clock and moves tasks
back to the ready queue when their wakeup time has been
reached.
Notice that a task may put itself, but no one else, to
sleep.
.IP 5. 3
Queues and ordered lists.
There is one data structure for all heads, tails, and
elements of queues or lists of tasks: q[].
The first NPROC entries in q (0 to NPROC-1) correspond to the
NPROC tasks.
If one wants to link task i onto a queue or list, then one
uses q[i].qnext and q[i].qprev as the forward and backward
pointers.
.sp
The remaining entries in q are used for the heads and tails of
lists.
The integer nextqueue always points to the next available
entry in q to assign.
When initialize builds the heads and tails of various lists,
it assigns entries in q sequentially.
Thus, the sqhead and sqtail fields of a semaphore are really
the indices of the head and tail of the list in q.
The advantage of keeping all heads and tails in the same
data structure is that enqueueing, dequeuing, testing for
empty/nonempty, and removing from the middle (eg., when a task
is killed) are all handled by a small set of simple
procedures (files queue.c and q.h).
An empty queue has the head and tail pointing to each other.
Since all real items have index less than NPROC, testing whether
a list is empty becomes trivial.
In addition to FIFO queues, q also contains ordered lists based
on an integer kept in the qkey field.
For example, tasks are inserted in the ready list (head at
position q[rdylist]) based on their priority.
They are inserted in the sleep list based on wakeup time.
Ordered lists are always in ascending order with the inserted
item stuck in BEFORE those with an equal key.
Thus, tasks are removed from the ready list from the tail
to get the highest priority task.
Also, tasks of equal priority are scheduled round robin.
Since the sleep queues are serviced from the smallest to
largest keys, items are removed from the head of the queue
(equal keys do not matter for sleeps).
.IP 6. 3
Task 0.
Task 0 is a null task that is always available to run
or is running.
Care must be taken so that task 0 never executes code that
could cause its suspension (e.g. it cannot wait for a semaphore).
Since task 0 may be running during interrupts, this means that
interrupt code may never wait for a semaphore.
Task 0 initializes the system, creates the first user task,
starts it executing the main program, and
goes into a sigpause waiting for an interrupt.
Because its priority is lower than that of any other task, the
null task loop executes only when no other task is ready.
It uses a sigpause instruction while waiting to avoid taking up cpu time.
.LP
