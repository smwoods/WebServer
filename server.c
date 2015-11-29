/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h> 
#include <netinet/in.h>


int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  int port;
  port = atoi(argv[1]);

  int sock_fd, newsock_fd;
  
  socklen_t clilen;
  char buf[256];
  struct sockaddr_in server_addr; 
  struct sockaddr_in client_addr;
  int n;
 
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error opening socket, exiting...");
    exit(1);
  }

  memset((char *) &server_addr, 0, sizeof(server_addr));
 
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("Error binding socket, exiting...");
    exit(1);
  }

  listen(sock_fd, 5);

  clilen = sizeof(client_addr);
  newsock_fd = accept(sock_fd, (struct sockaddr *) &client_addr, &clilen);
  if (newsock_fd < 0) {
    perror("Error accepting connection to socket, exiting...");
    exit(1);
  }

  memset(buf, 0, 256);

  n = read(newsock_fd, buf, 255);
  if (n < 0) {
    perror("Error reading socket, exiting...");
    exit(1);
  }

  printf("Here is the message: \n%s\n", buf);
  n = write(newsock_fd,"I got your message",18);
  if (n < 0) {
    perror("Error reading socket, exiting...");
  }

  close(newsock_fd);
  close(sock_fd);
  shutdown(newsock_fd, 2);
  shutdown(sock_fd, 2);
  return 0; 
}