# mpi-parallel-des-decryption
These are parallel C programs written spring 2019 for a parallel programming class at Rhodes College. The code is written using MPI so that the message passing is done in parallel.

password.c runs a parallel loop to decrypt a password originally encrypted using DES. It cracks the password using brute force.

paragraph.c decrypts a paragraph originally encrypted using DES. It utilizes the password decrypted from password.c.
