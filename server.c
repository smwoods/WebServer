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
#include <sys/wait.h>

#define DIR_LIST    1
#define HTML_FILE   2
#define CGI_SCRIPT  3
#define IMAGE_JPEG  4
#define IMAGE_GIF   5

int webcache, multithreading;
char cachesize[10];

//the thread function
int connection_handler(int);

int main(int argc, char *argv[]) {
<<<<<<< HEAD
<<<<<<< HEAD
=======

    int webcache, multithreading;
>>>>>>> 21c1988... merge
=======

    int webcache, multithreading;

>>>>>>> 3ee3f1d... merge
    webcache = 0;
    multithreading = 0;

    if (argc < 2) {
        fprintf(stderr,"Error, no port provided.");
        exit(1);
    }
    if (argc > 2){
        if(strcmp(argv[2], "multi") == 0){
            multithreading = 1;
        } else if(strcmp(argv[2], "cache") == 0){
            webcache = 1;
        }
        if(argc > 3){
            if(strcmp(argv[3], "cache") == 0){
                webcache = 1;
            }
            else if(strcmp(argv[3], "multi") == 0){
                multithreading = 1;
            }
        }
    }
    if(webcache == 1){
        printf("Web cache initiated!!\n");
        printf("How large should the cache be set to : ");
        gets(cachesize);
    }
    if(multithreading == 1){
        printf("multithreading initiated!\n");
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

int file_exist (char *filename)
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

int request_type(char *token) {
    printf("%s:%s\n", "Token", token);
    // Find pointer to last dot in token
    const char *dot = strrchr(token, '.');

    if (strstr(token, "my-histogram") != NULL){
        return CGI_SCRIPT;
    }
    // If no dot, assume request is for directory
    if (!dot){
        return DIR_LIST;
    }

    if (strcmp(dot, ".html") == 0)
        return HTML_FILE;
    else if (strcmp(dot, ".jpg") == 0)
        return IMAGE_JPEG;
    else if (strcmp(dot, ".jpeg") == 0)
        return IMAGE_JPEG;
    else if (strcmp(dot, ".gif") == 0)
        return IMAGE_GIF;
    else if (strcmp(dot, ".cgi") == 0){
        return CGI_SCRIPT;
    }
    else if (strcmp(dot, ".ico") == 0)
        return -1;
    else{
        return DIR_LIST;
    }

    return -1;

}

void return_404(int newsock_fd){
    char *not_found = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n \
        <html><head><title>404</title></head><body><h1>404 NOT FOUND</h1></body></html>";
    int n;
    n = write(newsock_fd, not_found, strlen(not_found));
    if (n < 0) {
        perror("Error writing to client");
    }
}

int directory_listing(int newsock_fd, char *request) {
    DIR *dp;
    struct dirent *ep;
    char out_buf[2048];
    int n;

    memset(out_buf, 0, 2048);
    dp = opendir(request);

    if (!dp) {
        return_404(newsock_fd);
        return -1;
    }

    strcat(out_buf, "HTTP/1.1 200 OK\nContent-type: text/plain\n\n");
    while ((ep = readdir(dp))) {
        strcat(out_buf, ep->d_name);
        strcat(out_buf, "\n");
    }
    (void) closedir(dp);
    n = write(newsock_fd, out_buf, strlen(out_buf));

    if (n < 0) {
        perror("Error writing to client");
    }

    return 0;
}


void response(void *message, int msglen, int newsock_fd)
{
    char *msg = (char*) message;

    while (msglen > 0)
    {
         int len = write(newsock_fd, msg, msglen);
         if (len <= 0) return;
         msg += len;
         msglen -= len;
    }
}

int image_file(int newsock_fd, char *file_path, int type){ //REWRITE THIS CODE 
    FILE *fp;
    char *buf, header[1024], *file_type;
    int fsize, hsize, nbytes;
    
    file_type = calloc(4, 1);

    fp = fopen(file_path, "r");
    if (!fp) {
        return_404(newsock_fd);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    rewind(fp);

    //Get correct file type for the header
    if(type == IMAGE_JPEG){
        file_type = "jpeg";
    }else{
        file_type = "gif";
    }

    hsize = sprintf(header, "HTTP/1.1 200 OK\r\n"
                            "Content-Length: %d\r\n"
                            "Content-Type: image/%s\r\n\n", fsize, file_type);

    write(newsock_fd, header, hsize);

    buf = (char*)malloc(4096);
    while((nbytes = fread(buf, sizeof(char), 4096, fp)) > 0){
        write(newsock_fd, buf, nbytes);
    }
    free(buf);

    return 0;
}

int html_file(int newsock_fd, char *request) {
    char *file_buf;
    FILE *f;
    int n;

    f = fopen(request, "r");
    if (!f) {
        return_404(newsock_fd);
        return -1;
    }
    
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
    return 0;
}

int cgi_script(int newsock_fd, char *request) {

    pid_t pid;
    int status;
<<<<<<< HEAD
    char *image_name, *data_name;

    //check if that image exists. if so, delete it to ensure it's dynamically created again!
    image_name = calloc(255, 1);
    strcat(image_name, "./img_dir/");
    strcat(image_name, request);
    strcat(image_name, "-output.gif");

    data_name = calloc(255, 1);
    strcat(data_name, "./data_dir/");
    strcat(data_name, request);
    strcat(data_name, "-data.dat");

    if(webcache == 0){
        if (file_exist(image_name)){
            if(remove(image_name) != 0){
                printf("Issue removing %s.. will now abort.\n", image_name);
                return -1;
            }
        }
        if (file_exist(data_name)){
            if(remove(data_name) != 0){
                printf("Issue removing %s.. will now abort.\n", data_name);
                return -1;
            }
        }
    }
=======
>>>>>>> 21c1988... merge

    if ((pid = fork()) == 0) {
        dup2(newsock_fd, STDOUT_FILENO);
        close(newsock_fd);

        if (strstr(request, "my-histogram")) {
            char params[10][70];
            int counter = 1;
            strcpy(params[0], "my-histogram");
            char *argvals = strstr(request, "?");
            while (argvals[0] != '\0') {
                char *e;
                int index;

                strcpy(params[counter], argvals+1);
                if (strchr(params[counter], '&') != NULL) {
                    e = strchr(params[counter], '&');
                    index = (int)(e - params[counter]);
                    params[counter][index] = '\0';
                }
                if (strstr(argvals+1, "&") != NULL) {
                    argvals = strstr(argvals+1, "&");
                }
                else {
                    argvals[0] = '\0';
                }
                counter++;
            }
            // printf("nogger.\n");
            while (counter < 8) {
                // printf("PARAMS %d is %s\n", counter, params[counter]);
                strcpy(params[counter], "");
                counter++;
            }
            // printf("COUNTER IS \"%d\"\n", counter);
            char *const arguments[8] = {params[0], params[1], params[2], params[3], params[4], params[5], params[6], NULL};

            execv("./my-histogram", arguments); //params
            exit(0);
        }
        else{
            execl(request, (char*) 0);
            exit(0);
        }
<<<<<<< HEAD
=======
        execl(request, (char*) 0);
        exit(-1);
>>>>>>> 21c1988... merge
=======
>>>>>>> be3ba6b... push that shit
    }

    if (pid > 0) {
      // printf("%s\n", "In parent");
      close(newsock_fd);
      /* the parent process calls waitpid() on the child */
      if (waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
           // the program terminated normally and executed successfully 
            // printf("%s\n", "success!");
        } 
        else if (WIFEXITED(status) && WEXITSTATUS(status)) {
          if (WEXITSTATUS(status) == -1) {
            printf("%s\n", "Exec failure");
            /* execl() failed */
          }
          else {
            printf("%s\n", "Child process returning nonzero");
          }
        } 
        else {
            printf("%s\n", "Error in child process");
<<<<<<< HEAD
>>>>>>> 21c1988... merge
=======
>>>>>>> be3ba6b... push that shit
        }
      }
      else {
        printf("%s\n", "Waitpid failed");
      }
    }
    else {
      printf("%s\n", "Fork failed");
    }

    return 0;
}


int connection_handler(int newsock_fd) {
    int n;
    char buf[2048];
    char *token, *pathname;
    
    // Fill buffer with zeros
    memset(buf, 0, 2048);
    
    // Read data from client socket into buffer
    n = read(newsock_fd, buf, 2048);
    if (n < 0) {
        perror("Error reading socket, exiting...");
        exit(1);
    }
    
    // Tokenize request packet, get request token, and add a period
    token = strtok(buf, " ");
    token = strtok(NULL, " ");
    pathname = calloc(255, 1);
    strcat(pathname, ".");
    strcat(pathname, token);
    
    // Ignore request for favicon
    int file_type;
    switch (file_type = request_type(pathname)) {
        case DIR_LIST:
            printf("%s\n", "Directory listing");
            directory_listing(newsock_fd, pathname);
            break;
        case HTML_FILE:
            printf("%s\n", "html file");
            html_file(newsock_fd, pathname);
            break;
        case IMAGE_GIF:
        case IMAGE_JPEG:
            printf("%s\n", "static image");
            image_file(newsock_fd, pathname, file_type);
            break;
        case CGI_SCRIPT:
            printf("%s\n", "cgi script");
            cgi_script(newsock_fd, pathname);
            break;
        default:
            printf("--------------------------------\n");
            return_404(newsock_fd);
            break;
    }
    close(newsock_fd);
    
    return 0;
}