# mpi-parallel-message-passing
These are parallel C programs written spring 2019 for a parallel programming class at Rhodes College. The code is written using MPI so that the message passing is done in parallel.

pingpong.c sends messages back and forth between processes in blocking, non-blocking, and both blocking and non-blocking ways.

ring.c sends a message around a ring of processes in a non-blocking fashion.

barrier.c sends multiple messages in a non-blocking fashion around a ring, timing the entirety of the message passing.
