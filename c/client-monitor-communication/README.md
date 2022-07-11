# client-monitor-communication

This is a program that allows a single monitor to communicate to up to 10 clients through a server.

The monitor connects to the server using pipes and file descriptors.
The server connects clients with sockets and polling on a queue of file descriptors.

This was a program written for an Operating Systems class in 2019, the fall of my senior year in college.
