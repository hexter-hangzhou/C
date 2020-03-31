this assignment you are going to create a simple "chat"-like facility that enables
someone at one terminal (or Xterm) to communicate with someone at another terminal.
The interface will not be pretty, but it will be functional. This facility will be called "stalk"
for simple-talk.
To initiate an s-talk session two users will first agree on two things:
o the machine that each will be running on
o the port number (explained later) each will use
Say that Fred and Barney want to talk. Fred is on machine "csil-cpu1" and will use port
number 6060. Barney is on machine "csil-cpu3" and will use port number 6001.
To initiate s-talk, Fred must type:
s-talk 6060 csil-cpu3 6001
And Barney must type:
s-talk 6001 csil-cpu1 6060.
So, (in case you haven't figured it out) the general format is:
s-talk [my port number] [remote machine name] [remote port number]
The result will be that every line typed at each terminal will appear on BOTH terminals
as they are typed. If you want to learn about "curses" and "cbreak" on your own, you can
alter this slightly so that every character typed appears on both screens as it is typed
rather than having to wait for each [return]. If you are interested look in the man pages
under "curses" (this is NOT a requirement of the assignment).
An s-talk session is terminated when some special character is typed by either user ('!' is a
good choice).
