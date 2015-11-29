/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <pthread.h> //for threading , link with lpthread

//the thread function
void *connection_handler(int);
void *request_handler(int);
// void *connection_handler(void *);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  int port;
  port = atoi(argv[1]);

  int sock_fd, newsock_fd;
  
  socklen_t clilen;
  struct sockaddr_in server_addr; 
  struct sockaddr_in client_addr;
 
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


  puts("Waiting for incoming connections...");
  clilen = sizeof(client_addr);
  pthread_t thread_id;
  
  while( (newsock_fd = accept(sock_fd, (struct sockaddr *)&client_addr, (socklen_t*)&clilen)) )
  {
      puts("Connection accepted");
       
      connection_handler(newsock_fd);
      // if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &newsock_fd) < 0)
      // {
      //     perror("could not create thread");
      //     return 1;
      // }
       
      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( thread_id , NULL);
      puts("Handler assigned");
  }
   
  if (newsock_fd < 0)
  {
      perror("accept failed");
      return 1;
  }
  return 0; 
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(int newsock_fd)
// void *connection_handler(void *socket_desc)
{
    int n;
    char buf[2048];
    memset(buf, 0, 2048);

    n = read(newsock_fd, buf, 2048);
    if (n < 0) {
      perror("Error reading socket, exiting...");
      exit(1);
    }

    int index = 0;

    const char s[2] = " ";
      char *token;
       
       /* get the first token */
      token = strtok(buf, s);
      token = strtok(NULL, s);
      // token = strtok(buf, s);
      // printf( "%s\n", token );
 
       // /* walk through other tokens */
       // while( token != NULL ) 
       // {
       //    printf( "%s\n", token );
        
       //    token = strtok(NULL, s);
       // }


    printf("Here is the message: \n%s\n", token);
    n = write(newsock_fd,"I got your message",18);
    if (n < 0) {
      perror("Error reading socket, exiting...");
    }
    close(newsock_fd);

    return 0;
}

void *request_handler(int newsock_fd){

}
