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
        fprintf(stderr,"Error, no port provided.");
        exit(1);
    }
    
    int port;
    int sock_fd;
    int newsock_fd;
    socklen_t client_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    // Get port argument, convert to integer
    port = atoi(argv[1]);
    
    // Allocate memory for the server socket address structure
    memset((char *) &server_addr, 0, sizeof(server_addr));
    
    // Set attributes for server socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Try to open an internet socket, save is file descriptor
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error opening socket, exiting...");
        exit(1);
    }
    
    // Bind server IP and port to the socket
    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket, exiting...");
        exit(1);
    }
    
    // Allow the socket to accept connections
    listen(sock_fd, 5);
    
    
    puts("Waiting for incoming connections...");
    client_len = sizeof(client_addr);
    // pthread_t thread_id;
    
    // Extract the first connection in queue, create a new socket for
    // it with the same type and address as the original, return a new
    // socket file descriptor in newsock_fd
    while ((newsock_fd = accept(sock_fd, (struct sockaddr *)&client_addr, (socklen_t*)&client_len))) {
        puts("Connection accepted");
        
        connection_handler(newsock_fd);
        // if( pthread_create( &thread_id , NULL , connection_handler , (void*) &newsock_fd) < 0)
        // {
            // perror("could not create thread");
            // return 1;
        // }
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        
    }
    
    if (newsock_fd < 0) {
        perror("Error, connection was not accepted");
        return -1;
    }
    return 0;
}

#define DIR_LIST 1
#define HTML_FILE 2
#define STATIC_IMG 3
#define CGI_SCRIPT 4

int requestType(char *token) {
    printf("%s:%s\n", "Token", token);
    // Find pointer to last dot in token
    const char *dot = strrchr(token, '.');

    // If no dot, assume request is for directory
    if (!dot)
        return DIR_LIST;

    if (strcmp(dot, ".html") == 0)
        return HTML_FILE;
    else if (strcmp(dot, ".jpg") == 0)
        return STATIC_IMG;
    else if (strcmp(dot, ".jpeg") == 0)
        return STATIC_IMG;
    else if (strcmp(dot, ".gif") == 0)
        return STATIC_IMG;
    else if (strcmp(dot, ".cgi") == 0)
        return CGI_SCRIPT;
    else if (strcmp(dot, ".ico") == 0)
        return -1;
    else
        return DIR_LIST;
    
    return -1;

}


void *directory_listing(int newsock_fd, char *request) {
    DIR *dp;
    struct dirent *ep;
    char *input;
    char out_buf[2048];
    int n;

    input = calloc(255, 1);
    memset(out_buf, 0, 2048);
    
    strcat(input, ".");
    strcat(input, request);
    dp = opendir(input);
    
    
    if (dp != NULL) {
        while ((ep = readdir(dp))) {
            strcat(out_buf, ep->d_name);
            strcat(out_buf, "\n");
        }
        (void) closedir(dp);
        n = write(newsock_fd, out_buf, strlen(out_buf));

        if (n < 0) {
            perror("Error writing to client");
        }

    }

    else {
        perror("Error, couldn't open the directory");
        return 0;
    }

    return 0;
}

int html_file(int newsock_fd, char *request) {
    char *pathname;
    char *file_buf;
    FILE *f;
    int n;

    pathname = calloc(255, 1);

    // Get the relative path and open the file
    strcat(pathname, ".");
    strcat(pathname, request);

    f = fopen(pathname, "r");
    if (!f) return -1;

    // Find the file size in bytes, create a buffer to fit it
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    file_buf = malloc(fsize + 1);

    // Fill buffer from file, null terminate, and close file
    fread(file_buf, fsize, 1, f);
    file_buf[fsize] = 0;
    fclose(f);

    // Write buffer to socket
    n = write(newsock_fd, file_buf, fsize);
    
}


void *connection_handler(int newsock_fd) {

    int n;
    char buf[2048];
    char *token;
    
    // Fill buffer with zeros
    memset(buf, 0, 2048);
    
    // Read data from client socket into buffer
    n = read(newsock_fd, buf, 2048);
    if (n < 0) {
        perror("Error reading socket, exiting...");
        exit(1);
    }
<<<<<<< HEAD
    
    // Tokenize request packet, get request token
    token = strtok(buf, " ");
    token = strtok(NULL, " ");
    
    // Ignore request for favicon

    switch (requestType(token)) {
        case DIR_LIST:
            printf("%s\n", "Directory listing");
            directory_listing(newsock_fd, token);
            break;
        case HTML_FILE:
            printf("%s\n", "html file");
            html_file(newsock_fd, token);
            break;
        case STATIC_IMG:
            printf("%s\n", "static image");
            break;
        case CGI_SCRIPT:
            printf("%s\n", "cgi script");
            break;
        default:
            break;
    }
    
=======

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
>>>>>>> 98da198da33e535aad085ac70a3bcc620f2a7024
    close(newsock_fd);
    
    return 0;
}

<<<<<<< HEAD
=======
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
>>>>>>> 98da198da33e535aad085ac70a3bcc620f2a7024
