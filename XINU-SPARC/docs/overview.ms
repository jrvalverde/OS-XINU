.TL
Virtual Memory Xinu Project
.AU
Douglas Comer
James Griffioen
.sp .3i
Department of Computer Science
Purdue University
West Lafayette, IN
47906
.space .3i
\f1August 17, 1990\f1
.AB
The Virtual Memory Xinu Project is investigating  a new model of virtual
memory in which  
dedicated, large-memory machines serve as a backing store (page server) for
virtual memory systems operating on a set of (heterogeneous) clients.  The
dedicated page server allows sharing of the large physical memory resource
and provides fast access to data.
.PP
This paper outlines the new virtual memory model of
computing used in the VM Xinu project and 
presents an overview of the VM Xinu operating system, its capabilities, and
the functionality it provides.
.AE
.NH 1
Background
.PP
Xinu
[1][2]
is a small, elegant operating system.
Xinu arranges the components
of the system into a hiearchy of layers, clarifying the interaction between
the various components of the system
and making the system easier to understand and modify.
Despite its small size, Xinu
uses powerful primitives to provide the same functionality
many conventional operating systems supply.
.PP
Xinu originally ran on Digital Equipment Corporation LSI 11's with only 64K
bytes of memory.  Version 6 Xinu, as it was called, supplied primitives to
handle memory management, process management, process
coordination/synchronization, interprocess communication, real-time clock
management, 
device drivers, and intermachine communication (a ring network).
Version 7 Xinu replaced the point-to-point networking capabilities of
Version 6 with 
support for the Ethernet and TCP/IP
Internet protocol software.  Version 7 also included
a shell and a remote file system that allowed Xinu to access remote files
via a remote file server executing on a UNIX system.
.PP
Over the years Xinu versions 6 and 7 have been ported to many
architectures including the
IBM PC, Sun 3, Transputer, MacIntosh, and much of the Vax family.
.PP
Both Version 6 and Version 7 Xinu ran all processes in a single, global,
address space and did not require any hardware virtual memory support.
While designing the VM Xinu operating system we looked at several different
virtual memory hardware architectures.  
Although the LSI 11/23 memory management unit provides virtual memory
support, we decided to base the design on more conventional virtual memory
architectures such as the Vax architecture or the Sun architecture.
.NH 1
Virtual Memory Xinu
.PP
VM Xinu is a virtual memory operating system providing the functionality
required to support large user processes 
executing in large virtual address spaces.  VM Xinu builds on
and enhances the functionality found in Xinu Version 7.
.PP
The Virtual Memory Xinu
project explores a new model of virtual memory in which 
dedicated, large-memory machines serve as a backing store (page server) for
virtual memory systems operating on a set of (heterogeneous) clients.  The
dedicated page server allows sharing of the large physical memory resource
and provides fast access to data.
The project also investigates how virtual memory can be integrated into a
hierarchically designed operating system.
.PP
The VM Xinu operating system executes on the set of client machines
providing large virtual address spaces for jobs requiring large amounts of
memory.  The VM Xinu kernel provides
a multi-threaded kernel,
multi-threaded user processes, shared memory, and remote memory
backing storage.
.NH 2
Model
.PP
The VM Xinu operating system is based on a new model of virtual memory called
the \f2remote memory model\f1 [3].  The remote memory model
consists of several client machines, various servers, one or more
dedicated machines called remote memory servers (or page servers), and a
communication 
channel interconnecting all the machines.  Figure 1 illustrates an example
remote memory model archictecture.
.if t \{
.so model.pic
\}
.if n \{
.so figure.doc
\}
.PP
The model uses dedicated, large-memory machines to provide backing storage
for virtual memory systems executing on heterogeneous client machines.  The
VM Xinu operating system executes on the client machines and
provides the mechanisms needed to efficiently access the large-memory
backing storage over a high speed, high bandwidth, communication channel.
The large-memory page server machines provide backing storage for multiple
heterogeneous client machines at speeds competitive with
local disk speeds.
.PP
Client machines share the physical memory resource located
at the page server.  Each client machine has a private local memory
large enough to support the normal processing demands.
For jobs requiring large amounts of memory, clients page across the
network, storing pages on the large-memory page server machine and
retrieving them as needed. The page server allocates memory on demand 
to clients that require additional memory.  Unlike conventional
distributed virtual memory systems which page to a remote disk with
fixed partitions, the remote memory backing storage model provides a
dynamically allocated shared backing storage resource to heterogeneous
client architectures.
.NH 2
VM Xinu Operating System 
.PP
The VM Xinu operating system is a hierarchically designed virtual
memory operating system which features a multi-threaded kernel,
multi-threaded user processes, shared memory, and remote memory
backing storage.
.PP
VM Xinu uses a hierarchical design to incorporate virtual memory
support into
the operating system, simplifying the kernel and clarifying the
relationship between the various components of the system.  The design
also identifies and separates out the architecture dependent
components of the system.  An architecture interface layer resides at
the lowest level of the hierarchy, hiding the underlying hardware
virtual memory support from the higher layers of the operating system
and 
reducing the effort needed to port the
system to new architectures.
.PP
In Version 7 Xinu, all processes are part of the kernel and execute in a
shared global
address space.
Because processes share the memory space,
a process cannot protect 
private data from other processes.  The virtual memory hardware
support provided by most conventional architectures allows VM Xinu to
create multiple address spaces, each specifying an independent virtual to
physical memory mapping.  VM Xinu uses this ability to provide both
shared and protected/private data.
Each user process in VM Xinu executes in its own address space, only
accessing data in its address space.
Each address space is defined by a mapping from virtual memory to physical
memory that
protects the data in the address space from all other user processes.
.PP
VM Xinu supports multi-threaded user processes allowing concurrent
manipulation of shared data within a process.  A thread is defined as a
point of execution within a process.  All threads within a process execute
instructions from the same text region, each at a different point
in the code, and share the process' data region with all other threads in
the process.  Semaphores and efficient interprocess communication
provide synchronization between threads, both when the threads are in the
same process and when they are in different processes.
.PP
VM Xinu's kernel provides the ability to execute several kernel tasks
concurrently.  
VM Xinu's multi-threaded kernel in many ways resembles Version 7 Xinu's
global address space with multiple concurrent processes.  Many operations
performed by the kernel such as page reclamation, network management,
background paging, etc, can be coded very simply and elegantly when 
thought of as executing concurrently within the kernel.
.PP
In Version 7 Xinu all user processes are compiled into the kernel,
but, in VM Xinu, the kernel dynamically loads user processes from the remote
file system.  VM Xinu provides the ability to read an executable file into
memory from the file system and start up a thread in the newly loaded
process.  All dynamically loaded user processes execute in user-mode and
trap into the kernel via system calls to invoke Xinu kernel routines.
Because the kernel is protected from user processes, user processes only
access kernel data structures via systems calls. 
Unlike user processes in Version 7 Xinu, threads execute on a dynamically
allocated stack rather than a fixed size stack.  The kernel allocates
additional memory to a thread's stack as needed.  Similarly, each process'
heap region grows dynamically.
.PP
The VM Xinu operating system is unique in that it uses remote (physical)
memory, 
accessed via a high speed, high bandwidth network, as shared secondary
storage. 
The kernel provides the support needed to page across the network to a
remote memory server.  The kernel uses a highly efficient,
reliable, data streaming, architecture independent protocol to 
send and receive pages of memory to and from the memory server.
The memory server employs special purpose algorithms to minimize the time
spent locating the desired data.  The result is a high-speed, shared,
secondary storage mechanism operating at speeds competitive with a local
disk.
.PP
A small, but important, change from Version 7 Xinu is the enhanced shell
which provides a way to utilize VM Xinu's new capabilities.  VM Xinu's 
shell now provides commands to examine many of the virtual memory data
structures and also provides methods for invoking dynamically loaded user
processes.
.PP
We designed and implemented a prototype distributed system based on the
remote memory model.  The system consists of heterogeneous client machines
(Sun Microsystems SUN 3/50's, Digital Equipment Corporation Microvax I's
and II's), a memory server, and a remote file server, all connected by a
10 Mb/sec Ethernet.
Sun and Microvax client machines
simultaneously access the remote memory server for backing storage,
demonstrating support for heterogeneous clients.
.NH 1
Prototype Availability
.PP
A prototype implementation of the VM Xinu system, including source code,
for the Sun 3 architecture is
now available.
The kernel executes on a diskless Sun 3 machine and contains the support
needed to access remote files via xinu's remote file system [2]. 
.PP
Backing storage for the VM Xinu kernel is provided by a 
\f2page server\f1.
The page server runs as a UNIX user
level process, allowing us to run the server on a wide variety of
platforms.  We have used a SUN 3/50, Vax 11/780, Microvax II and III,
Vaxserver 3100,
an 8 processor
Sequent Symmetry, running a wide variety of operating systems (SunOS,
4.3BSD, Dynix, Ultrix) as the page server.  
The page server provides backing storage for multiple client machines
simultaneously.  The high speed protocol used to transfer pages between the
client and the page server is built on top of UDP, allowing clients to
access a page server on other network across one or more gateways.
.PP
The VM Xinu kernel supports dynamically loaded user level programs.
Users compile their programs on the UNIX development machine and then
install the executable image so that it is accessible via the remote file
system. 
Clients dynamically load the executable image from the remote file system
and start a thread executing the program.
The Xinu shell provides an easy method
for invoking user level dynamically loaded programs.
.PP
A library of system calls allows user level programs to invoke xinu kernel
routines such as semaphore operations, interprocess communication, thread
creation, remote file system access, network access, etc.  To reduce the
effort required to port UNIX programs to Xinu, the system includes a user
level UNIX simulation library that uses the Xinu system calls to simulate
many of the most common UNIX system calls. 
.NH 1
Distribution Contents
.PP
VM Xinu (version 8.0.0) is available for the SUN 3 and can be ordered as
indicated on the order form in Appendix A.  The VM Xinu version 8.0.0 tape
contains:
.IP \(bu
\f2VM Xinu Kernel\f1 source code.
.IP \(bu
\f2Remote File Server\f1 and \f2Xinu Page Server\f1 source code
.IP \(bu
\f2Cross Development Software\f1 with source code that facilitates
kernel development in a UNIX environment and provides methods for
downloading and testing 
the VM Xinu kernel on a second SUN 3.
.IP \(bu
\f2Xinu System Call Library\f1 and source code for use with user level
programs 
.IP \(bu
\f2UNIX Simulation Library\f1 and source code for use with user level
programs.  The library simulates 
many of the UNIX system calls reducing the effort required to port UNIX
applications to VM Xinu.
.IP \(bu
\f2C Library (subset)\f1 and source code.  A small public domain C library for use with user
programs.
.IP \(bu
\f2Xinu User Programs\f1 and source code.  Several dynamically loaded
Xinu user programs (cat, cp, ls, mv, ...)
.IP \(bu
\f2Manual Pages\f1 for the VM Xinu kernel and the Xinu System Calls
.IP \(bu
\f2Installation Instruction\f1 to get VM Xinu up and running.
.sp .2i
.NH 1
References
.IP [1]
Comer, Douglas E., \f2Operating System Design: The Xinu
Approach\f1, Prentice-Hall, 1984.
.IP [2]
Comer, Douglas E., \f2Operating System Design Vol 2:
Internetworking with Xinu\f1, Prentice-Hall, 1987.
.IP [3]
Comer, Douglas and Griffioen, James, \f2A New Design for Distributed
Systems: The Remote Memory Model\f1, Proceedings of the Summer Usenix
Conference, June 1990
.br
.bp
.ce
Appendix A
.nf
.if t \{
.so orderinfo.ms
\}
.if n \{
.nf
.so orderinfo.fig
.fi
\}


