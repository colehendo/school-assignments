#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>

// constants for pipe FDs
#define WFD 1
#define RFD 0
#define MAX_CLIENTS 10

/**
 * nonblock - a function that makes a file descriptor non-blocking
 * @param fd file descriptor
 */
void nonblock(int fd) {
  int flags;

  if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
    perror("fcntl (get):");
    exit(1);
  }
  if (fcntl(fd, F_SETFL, flags | FNDELAY) == -1) {
    perror("fcntl (set):");
    exit(1);
  }

}

/*
 * monitor - provides a local chat window
 * @param srfd - server read file descriptor
 * @param swfd - server write file descriptor
 */
void monitor(int srfd, int swfd) {
  char buf[1024];
  int rbytes;
  struct pollfd poll_fds[2];
  
  // file descriptor spot for keyboard
  int KFD = 0;
  // fd spot for server pipe
  int SFD = 1;

  // make server and keyboard fds nonblocking
  nonblock(srfd);
  nonblock(swfd);
  nonblock(STDOUT_FILENO);
  nonblock(STDIN_FILENO);

  // loop reads in from server, writes that to screen
  // then reads in from screen and writes that back to server
  do {
    poll_fds[KFD].fd = STDIN_FILENO;
    poll_fds[KFD].events = POLLIN;

    poll_fds[SFD].fd = srfd;
    poll_fds[SFD].events = POLLIN;

    // poll keyboard and server read fds
    if (poll(poll_fds, 2, -1) == -1) {
      perror("poll");
      break;
    }

    // deal with server sending a message to monitor
    if (poll_fds[SFD].revents & POLLIN) {
      if ((rbytes = read(srfd, buf, (sizeof(buf) - 1))) < 0) {
        perror("monitor read from server");
        break;
      }
      else if (rbytes == 0)
        break;
      else
        buf[rbytes] = 0;

      // write that message to the screen
      if (write(STDOUT_FILENO, buf, rbytes) < 0) {
        perror("monitor write to screen");
        break;
      }
    }

    // deal with keyboard input
    if (poll_fds[KFD].revents & POLLIN) {
      if ((rbytes = read(STDIN_FILENO, buf, (sizeof(buf) - 1))) < 0) {
        perror("monitor read from screen\n");
        break;
      }
      else if (rbytes == 0)
        break;
      else
        buf[rbytes] = 0;

      // write that message to the server
      if (write(swfd, buf, rbytes) < 0) {
        perror("monitor write to server");
        break;
      }
    }
  } while (1);
  printf("\n");
}



/*
 * server - relays chat messages
 * @param mrfd - monitor read file descriptor
 * @param mwfd - monitor write file descriptor
 * @param portno - TCP port number to use for client connections
 */

// used: https://www.linuxtoday.com/blog/multiplexed-i0-with-poll.html for help with poll()

void server(int mrfd, int mwfd, int portno) {
  char buf[1024], buf2[1024];
  int socket_fd, accept_fd, rbytes;
  struct sockaddr_in server_addr;
  struct sockaddr_storage client_addr;
  struct pollfd poll_fds[MAX_CLIENTS];
  socklen_t client_len;

  int SFD = 0;
  int MFD = 1;
  int lastFD = 1;

  // allocate memory for bind info
  memset(&server_addr, 0, sizeof server_addr);

  // set bind info
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = portno;

  // create socket
  // used this: https://beej.us/guide/bgnet/html/multi/syscalls.html
  // to help with connection
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return;
  }

  // set current fds to nonblocking
  nonblock(socket_fd);
  nonblock(mrfd);
  nonblock(mwfd);

  int val = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));

  // bind that socket to info above
  if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("bind");
    return;
  }

  // listen for connection on other end of that socket (client)
  if (listen(socket_fd, 5) < 0) {
    perror("listen");
    return;
  }

  client_len = sizeof(client_addr);

  // preset the two current values in poll()
  poll_fds[SFD].fd = socket_fd;
  poll_fds[SFD].events = POLLIN;

  poll_fds[MFD].fd = mrfd;
  poll_fds[MFD].events = POLLIN;

  // loop reads from client, writes that to monitor
  // then reads from monitor and writes that back to client
  do {
    // poll every fd looking for an event
    if (poll(poll_fds, MAX_CLIENTS, 100) == -1) {
      perror("poll");
      break;
    }

    // if an fd wants to connect, accept
    if (poll_fds[SFD].revents & POLLIN) {
      if ((accept_fd = (accept(socket_fd, (struct sockaddr *) &client_addr, &client_len))) < 0)
        perror("accept");
      else {
        // make new fd nonblocking
        nonblock(accept_fd);

        // adjust poll fd array
        lastFD += 1;
        poll_fds[lastFD].fd = accept_fd;
        poll_fds[lastFD].events = POLLIN;
      }
    }

    // look at remaining fds
    for (int i = MFD; i <= lastFD; i++) {
       // if someone has something to say, read it
      if (poll_fds[i].revents & POLLIN) {
        if ((rbytes = read(poll_fds[i].fd, buf, (sizeof(buf) - 1))) < 0) {
          perror("server read");
          break;
        }

        // disconnect an fd
        else if (rbytes == 0) {
          if (close(poll_fds[i].fd) == -1) {
            perror("client close");
            break;
          }

          // shift the poll fd array to remove disconnected fd
          for (int j = i; j < lastFD; j++) {
            poll_fds[j].fd = poll_fds[j + 1].fd;
            poll_fds[j].events = poll_fds[j + 1].events;
          }
          
          // nullify fd at end of array
          poll_fds[lastFD].events = POLLHUP;
          poll_fds[lastFD].revents = POLLHUP;
          lastFD -= 1;
        }
        else {
          buf[rbytes] = 0;
          
          // write client message to monitor
          if (i != MFD) {
            if (write(mwfd, buf, rbytes) < 0) {
              perror("server write to monitor");
              break;
            }
          }  

          // write message to all clients
          for (int j = 2; j <= lastFD; j++) {
            if (j != i) {
              if (write(poll_fds[j].fd, buf, rbytes) < 0) {
                perror("server write to client");
                break;
              }
            }
          }
        }
      }
    }
  } while (1);

  // close remaining clients
  for (int i = 0; i <= lastFD; i++) {
    if (close(poll_fds[i].fd) == -1) {
      perror("client close");
      break;
    }
  }
}



int main(int argc, char **argv) {
  int servToMonFD[2], monToServFD[2], ret, ret2, argCheck;

  // Initialize port to 8080
  int portValue = 8080;

  // Check user arguments
  while ((argCheck = getopt(argc, argv, "p:")) != -1) {
    switch (argCheck) {
    // set port value
    case 'p':
      portValue = atoi(optarg);
      if (portValue < 1) {
        printf("Invalid port number.\n");
        return -1;
      }
      printf("port given\n");
      continue;
    // give help message
    default:
      printf("usage: ./server [-h] [-p port #]\n");
      printf("\t-h - this help message\n\t-p # - the port to use when connecting to the server\n");
      exit(1);
      break;
    }
  }

  // create two pipes to go opposite directions
  // between server and monitor
  ret = pipe(servToMonFD);
  ret2 = pipe(monToServFD);

  if (ret < 0 || ret2 < 0) {
    perror("pipe");
    return -1;
  }

  // fork the program, creating the monitor and server
  pid_t p = fork();

  //sprintf(pidstr, "%d: ", getpid());

  if (p < 0) {
    perror("fork");
    exit(1);
  }
  
  else if (p == 0) {
    // child - monitor
    close(monToServFD[RFD]);
    close(servToMonFD[WFD]);

    monitor(servToMonFD[RFD], monToServFD[WFD]);
    
    close(servToMonFD[RFD]);
    close(monToServFD[WFD]);

    exit(1); // exit child to ensure no zombies

  }
  
  else {
    // parent - writer/talker

    close(servToMonFD[RFD]);
    close(monToServFD[WFD]);

    server(monToServFD[RFD], servToMonFD[WFD], htons(portValue));
    
    close(monToServFD[RFD]);
    close(servToMonFD[WFD]);

    wait(NULL); // wait for child process to terminate
  }
}

