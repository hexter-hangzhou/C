this assignment you are going to create an interactive operating system simulation 
which supports a few basic O/S functions. The simulation will be keyboard driven, and 
will make reports to the screen. 
First of all, what do I mean by a simulation? Well, there will be no real processes doing 
any real work. There will, however, be data structures which represent processes and the 
state of the system. For example, when the simulation user issues the command to create 
a process, a PCB for the new process will be created, initialized and placed in the 
appropriate O/S queue. When the user issues the command to signal the expiry of a time 
quantum, the internal state of the simulation will react accordingly and the effects of the 
expiry will be reported to the screen. The commands issued by the simulation user will be 
on behalf of the system or the running process.

Commands 
The following commands should be implemented by your simulation:
Command	Character	Parameters	Action	Reports

Create	
C	int priority (0 = high, 1 = norm, 2
= low)	create a process and put it on the appropriate ready Q.	success or failure, the pid of the created process on success.



Fork	


F	


None	Copy the currently running process and put it on the ready Q corresponding to the original process' priority.
Attempting to Fork the "init" process (see below) should
fail.	

success or failure, the pid of the resulting (new) process on success.

Kill	
K	int pid (pid of process to be killed)	kill the named process and remove it from the system.	action taken as well as success or failure.


Exit	

E	

None	
kill the currently running process.	process scheduling information (eg. which process now gets control of the CPU)

Quantum	
Q	
None	time quantum of running process expires.	action taken (eg. process scheduling information)



Send	


S	pid (pid of process to send message to), char
*msg (null- terminated message string,
40 char max)	


send a message to another process - block until reply	
success or failure, scheduling information, and reply source and text (once reply arrives)


Receive	

R	

None	

receive a message - block until one arrives	scheduling information and (once msg is received) the message text and source of message



Reply	


Y	int pid (pid of process to make the reply to), char *msg (null- terminated reply string, 40 char max)	


unblocks sender and delivers reply	


success or failure



New Semaphore	


N	

int semaphore (semphore ID), initial value (0 or higher)	Initialize the named semaphore with the value given. ID's can take a value from 0 to 4. This can only be done once for a semaphore - subsequent attempts result in
error.	


action taken as well as success or failure.


Sempahore P	

P	

int semaphore (semphore ID)	execute the semaphore P operation on behalf of the running process. You can assume sempahores IDs numbered 0 through 4.	
action taken (blocked or not) as well as success or failure.


Sempahore V	

V	

int semaphore (semphore ID)	execute the semaphore V operation on behalf of the running process. You can assume sempahores IDs numbered 0 through 4.	action taken (whether/ which process was readied) as well as success or failure.


Procinfo	

I	int pid (pid of process information is to
be returned for)	dump complete state information of process to screen (this includes process
status and anything else you
can think of)	

see Action
Totalinfo	T	None	display all process queues and their contents	see Action
