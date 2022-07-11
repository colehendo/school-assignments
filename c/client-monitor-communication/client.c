#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define KFD 0
#define SFD 1

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

int main(int argc, char **argv) {
  bool gotInfo = false;
  int socket_fd, argCheck, status, rbytes;
  char buf[1024];
  struct addrinfo hints, *res;
  struct pollfd poll_fds[2];

  // initialize port to 8080
  char *portValue = "8080";

  // allocate memory for socket connect info
  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  // Check user arguments
  while ((argCheck = getopt(argc, argv, "h:p:")) != -1) {
    switch (argCheck) {
    // get address info for server specified by command line parameter
    case 'h':
      if ((status = getaddrinfo(optarg, portValue, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
      }
      gotInfo = true;
      continue;
    // Set port value to specified argument following '-p'
    case 'p':
      if (atoi(optarg) < 1) {
        printf("Invalid port number.\n");
        return -1;
      }
      else
        portValue = optarg;
      continue;
    // print help message
    default:
      printf("usage: ./server [-h] [-p port #]\n");
      printf("\t-h - this help message\n\t-p # - the port to use when connecting to the server\n");
      exit(1);
      break;
    }
  }

  // get info for senna if user didn't specify server
  if (!gotInfo) {
    if ((status = getaddrinfo("senna", portValue, &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      return -1;
    }
  }

  // Set up socket
  if ((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
    perror("socket");
    return -1;
  }

  // Used https://stackoverflow.com/questions/26190337/connect-function-fails-when-passing-sockaddr-in-as-argument to help with this line
  // Connect socket to server
  if (connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("connect");
    return -1;
  }
  
  if (write(STDOUT_FILENO, "connected to server...\n", 23) < 0) {
    perror("write to server");
    return -1;
  }

  // make fds nonblocking
  nonblock(socket_fd);
  nonblock(STDOUT_FILENO);
  nonblock(STDIN_FILENO);

  // loop reads from screen, writes that to server
  // then reads in from server and writes that to screen
  do {

    poll_fds[KFD].fd = STDIN_FILENO;
    poll_fds[KFD].events = POLLIN;

    poll_fds[SFD].fd = socket_fd;
    poll_fds[SFD].events = POLLIN;

    // poll keyboard and server
    if (poll(poll_fds, 2, -1) == -1) {
      perror("poll");
      return -1;
    }

    // deal with keyboard input
    if (poll_fds[KFD].revents & POLLIN) {
      if ((rbytes = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        perror("read from screen");
        return -1;
      }
      else if (rbytes == 0)
        break;
      
      // write keyboard input to server
      if (write(socket_fd, buf, rbytes) < 0) {
        perror("write to server");
        return -1;
      }
    }

    // deal with message from server
    if (poll_fds[SFD].revents & POLLIN) {
      if ((rbytes = read(socket_fd, buf, sizeof(buf))) < 0) {
        perror("read from server");
        return -1;
      }
      else if (rbytes == 0)
        break;
      
      // write that message to the screen
      if (write(STDOUT_FILENO, buf, rbytes) < 0) {
        perror("write to server");
        return -1;
      }
    }
  } while (1);

  printf("hanging up\n");
 
  // close the socket
  close(socket_fd);
  freeaddrinfo(res);
  return 0;
}

