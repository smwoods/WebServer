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
#include <dirent.h>
#include <sys/stat.h>

//the thread function
void *connection_handler(int);
void *request_handler(int, char*);
int file_type(const char*);
// void *connection_handler(void *);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port providedn");
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
        // if( pthread_create( &thread_id , NULL , connection_handler , (void*) &newsock_fd) < 0)
        // {
            // perror("could not create thread");
            // return 1;
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

    char *token;
    
    token = strtok(buf, " ");
    token = strtok(NULL, " ");
    
    if(strcmp(token, "/favicon.ico") == 0){
        close(newsock_fd);
        return 0;
    }

    printf("REQUEST SUBMITTED: %s\n\n", token);
    request_handler(newsock_fd, token);

    printf("Here is the message: \n%s\n", token);
    
    n = write(newsock_fd,"I got your message",18);
    if (n < 0) {
        perror("Error reading socket, exiting...");
    }
    close(newsock_fd);
    
    return 0;
}

void *request_handler(int newsock_fd, char* request){
  int type;
  char *input;
  input = calloc(80, 1);

  strcat(input, ".");
  strcat(input, request);
  type = file_type(input);
  switch(type){
    case 0:
      printf("directory file\n");
      break;

    case 1:
      printf("regular (static) file\n");
      break;

    case 2:
      printf("html file\n");
      break;

    case 3:
      printf("cgi script\n");
      break;

    default:
      printf("return 404\n");
      break;
  }


  if (type != 0){
    printf("do not directory stuff\n");
  }
  else{
    DIR *dp;
    struct dirent *ep;
    dp = opendir (input);

    if (dp != NULL)
    {
      while ((ep = readdir (dp))){
        puts (ep->d_name);
      }

      (void) closedir (dp);
    }
    else {
      perror ("Couldn't open the directory");
    }
  }
  
  return 0;
}

int file_type(const char *path) {
  struct stat path_stat;

  if( stat(path, &path_stat) < 0){ //SCOTT DEAL WITH THIS
    return -1;
  }

  if(strstr(path, ".cgi") != NULL){
    puts ("RUNNING SCRIPT!!!! \n");
  }

  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}