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
#include "uthash.h"

#define DIR_LIST    1
#define HTML_FILE   2
#define CGI_SCRIPT  3
#define IMAGE_JPEG  4
#define IMAGE_GIF   5


typedef struct Entry {
    char* command;
    struct Entry* next;
    struct Entry* prev;
    char* response_data;
    UT_hash_handle hh;
} Entry;

typedef struct Cache {
    int current_size;
    Entry* first;
    Entry* last;
} Cache;

Cache my_cache = { .current_size = 0, .first = NULL , .last = NULL };
Entry* cache_hash = NULL;
int cache_sz = 100;


int insert_cache_entry(Entry* new_entry) {

    if (my_cache.current_size == 0) {
        my_cache.first = new_entry;
        my_cache.last = new_entry;
        my_cache.current_size = strlen(new_entry->response_data);
    }
    else {
        my_cache.first->next = new_entry;
        new_entry->prev = my_cache.first;
        my_cache.first = new_entry;
        my_cache.current_size += strlen(new_entry->response_data);
    }
    HASH_ADD_STR(cache_hash, command, new_entry);

    while (my_cache.current_size > cache_sz) {
        my_cache.current_size -= strlen(my_cache.last->response_data);
        my_cache.last = my_cache.last->next;
        my_cache.last->prev = NULL;
    }

    return 0;
}

Entry* cache_lookup(char* command) {
    Entry* entry;
    HASH_FIND_STR(cache_hash, command, entry);
    if (entry) {
        if (entry == my_cache.last) {
            my_cache.last = my_cache.last->next;
            my_cache.last->prev = NULL;
            my_cache.first->next = entry;
            entry->prev = my_cache.first;
            my_cache.first = entry;
        }
        else if (entry != my_cache.first){
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
            my_cache.first->next = entry;
            entry->prev = my_cache.first;
            my_cache.first = entry;
        }
    }
    return entry;
}

void print_cache() {
    if (my_cache.first) {
        printf("%s:%s\n", "first", my_cache.first->response_data);
    }
    if (my_cache.last) {
        printf("%s:%s\n", "last", my_cache.last->response_data);
    }
    if (my_cache.current_size) {
        printf("%s:%d\n", "size", my_cache.current_size);
    }
}


<<<<<<< HEAD
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
=======
int file_exist(char *filename) {
  struct stat buffer;   
>>>>>>> fddd76a... Cleaned up code
  return (stat (filename, &buffer) == 0);
}

int request_type(char *token) {
    printf("%s:%s\n", "Token", token);

    // Find pointer to last dot in token
    const char *dot = strrchr(token, '.');

    if (strstr(token, "my-histogram") != NULL){
        return CGI_SCRIPT;
    }

    if (access(token, F_OK) == -1) {
        return -1;
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

int return_404(int new_sock){
    char *not_found = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n \
        <html><head><title>404</title></head><body><h1>404 NOT FOUND</h1></body></html>";
    int n;
    n = write(new_sock, not_found, strlen(not_found));
    if (n < 0) {
        perror("Error writing to client");
        return 1;
    }
    return 0;
}

int cgi_script(int new_sock, char *request) {

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

<<<<<<< HEAD
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
=======
    // if (cache == 0){
    //     if (file_exist(image_name)){
    //         if(remove(image_name) != 0){
    //             printf("Issue removing %s.. will now abort.\n", image_name);
    //             return -1;
    //         }
    //     }
    //     if (file_exist(data_name)){
    //         if(remove(data_name) != 0){
    //             printf("Issue removing %s.. will now abort.\n", data_name);
    //             return -1;
    //         }
    //     }
    // }

>>>>>>> fb566f8... Fixed error with arg parser

    if ((pid = fork()) == 0) {
        dup2(new_sock, STDOUT_FILENO);
        close(new_sock);

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
            // This will only be reached if exec fails
            exit(-1);
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
      close(new_sock);
      if (waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
           return 0;
        } 
        else if (WIFEXITED(status) && WEXITSTATUS(status)) {
          if (WEXITSTATUS(status) == -1) {
            printf("%s\n", "Error running exec in child");
            return -1;
          }
        } 
<<<<<<< HEAD
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
=======
>>>>>>> fddd76a... Cleaned up code
      }
    }
    else {
      printf("%s\n", "Fork failed");
      return -1;
    }

    return 0;
}

int directory_listing(int new_sock, char *request) {
    pid_t pid;
    int status;

    char *const arguments[2] = {"directorylisting.cgi", request};

    if ((pid = fork()) == 0) {
        dup2(new_sock, STDOUT_FILENO);
        close(new_sock);
        execv("cgi_scripts/directorylisting.cgi", arguments);
        exit(1);
    }

    if (pid > 0) {

      if (waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
           return 0;
        } 
        else if (WIFEXITED(status) && WEXITSTATUS(status)) {
          if (WEXITSTATUS(status) == 1) {
            perror("Error starting child process");
            return 1;
          }
        } 
      }
    }

    else {
      perror("Error forking child process");
      return 1;
    }

    return 1;
}

int image_file(int new_sock, char *file_path, int type){ //REWRITE THIS CODE 
    
    FILE *fp;
    char *buf, header[1024], *file_type;
    int fsize, hsize, nbytes;
    
    file_type = calloc(4, 1);

    fp = fopen(file_path, "r");
    if (!fp) {
        return_404(new_sock);
        return -1;
    }

    // Get size of image size
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    rewind(fp);

    //Get correct file type for the header
    if (type == IMAGE_JPEG) {
        file_type = "jpeg";
    }
    else {
        file_type = "gif";
    }

    hsize = sprintf(header, "HTTP/1.1 200 OK\r\n"
                            "Content-Length: %d\r\n"
                            "Content-Type: image/%s\r\n\n", fsize, file_type);

    write(new_sock, header, hsize);

    buf = (char*) malloc(4096);
    while ((nbytes = fread(buf, sizeof(char), 4096, fp)) > 0){
        write(new_sock, buf, nbytes);
    }
    free(buf);

    return 0;
}

// STILL NEED TO ADD HEADERS FOR HTML)
int html_file(int new_sock, char *request) {
    char *file_buf;
    FILE *f;
    int n;

    f = fopen(request, "r");
    if (!f) {
        return_404(new_sock);
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
    n = write(new_sock, file_buf, fsize);
    free(file_buf);
    return 0;
}

int connection_handler(int new_sock) {
    int n;
    char buf[2048];
    char *token, *pathname;
    
    // Fill buffer with zeros
    memset(buf, 0, 2048);
    printf("%s\n", "TEST1");
    // Read data from client socket into buffer
    n = read(new_sock, buf, 2048);
    if (n < 0) {
        perror("Error reading socket");
        exit(1);
    }

    
    // Tokenize request packet, get request token, and add a period
    printf("%s:\n%s\n", "BUF-----------", buf);
    token = strtok(buf, " ");
    token = strtok(NULL, " ");
    pathname = calloc(strlen(token)+1, 1);
    strcat(pathname, ".");
    strcat(pathname, token);
    
    // Ignore request for favicon
    int file_type;
    int ret = 0;
    switch (file_type = request_type(pathname)) {
        case DIR_LIST:
            fprintf(stdout, "Directory listing\n");
            ret = directory_listing(new_sock, pathname);
            if (ret != 0) {

            }
            break;
        case HTML_FILE:
            fprintf(stdout, "HTML file\n");
            ret = html_file(new_sock, pathname);
            break;
        case IMAGE_GIF:
        case IMAGE_JPEG:
            fprintf(stdout, "Static image\n");
            ret = image_file(new_sock, pathname, file_type);
            break;
        case CGI_SCRIPT:
            fprintf(stdout, "CGI script\n");
            ret = cgi_script(new_sock, pathname);
            break;
        default:
            fprintf(stdout, "File not found\n");
            ret = return_404(new_sock);
            break;
    }
    close(new_sock);
    
    return ret;
}

void cache_test() {
    Entry new_entry1 = {
        .command = "entry1", 
        .next = NULL, 
        .prev = NULL, 
        .response_data = "11111111111"
    };
    Entry new_entry2 = {
        .command = "entry2", 
        .next = NULL, 
        .prev = NULL, 
        .response_data = "2222222222"
    };
    Entry new_entry3 = {
        .command = "entry3", 
        .next = NULL, 
        .prev = NULL, 
        .response_data = "33333333"
    };

    Entry* test_ent;
    insert_cache_entry(&new_entry1);
    print_cache();
    insert_cache_entry(&new_entry2);
    print_cache();
    insert_cache_entry(&new_entry3);
    print_cache();
    cache_lookup("entry2");
    print_cache();
}

int main(int argc, char *argv[]) {
    

    // cache_test();

    //Parse command line arguments


    int c;
    int thread = 0;
    int port;

    static int const CACHE_MIN = 4;
    static int const CACHE_MAX = 2000;
    static int const PORT_MIN = 5000;
    static int const PORT_MAX = 65536;

    while ((c = getopt (argc, argv, "p::c:t")) != -1)
    switch (c){
      case 'p':
        if (atoi(optarg) >= PORT_MIN && atoi(optarg) <= PORT_MAX) {
            port = atoi(optarg);
        }
        else {
            fprintf(stderr,"Error, invalid port.\n");
            exit(1);
        }
        break;
      case 'c':
        printf("%s\n", optarg);
        if (atoi(optarg) >= CACHE_MIN && atoi(optarg) <= CACHE_MAX) {
            cache_sz = atoi(optarg) * 1000;
            printf("%s\n", "TEST");
        }
        else {
            fprintf(stderr,"Error, cache size invalid.\n");
            exit(1);
        }
        break;
      case 't':
        thread = 1;
        break;
      default:
        fprintf(stderr, "Error, invalid flag.\n");
        exit(1);
    }

    if (!port) {
        fprintf(stderr, "Error, no port provided.\n");
        exit(1);
    }
    
    // Setup server socket

    int server_sock;
    int new_sock;
    socklen_t client_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    client_size = sizeof(client_addr);
    
    // Allocate memory for the server socket address structure
    memset((char *) &server_addr, 0, sizeof(server_addr));
    
    // Set attributes for server socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Try to open an internet socket, save is file descriptor
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error opening socket");
        exit(1);
    }

    
    // Bind server IP and port to the socket
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }
    
    // Allow the socket to accept connections
    listen(server_sock, 5);
        
    
    // Extract the first connection in queue, create a new socket for
    // it with the same type and address as the original, return a new
    // socket file descriptor in new_sock
    int ret;
    fprintf(stdout, "Socket connection successful, waiting for incoming connections...\n");
    while ((new_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t*)&client_size))) {
        fprintf(stdout, "Incoming request from client, passing to handler.\n");
        ret = connection_handler(new_sock);
        if (ret != 0) {
            fprintf(stderr, "Server error while handling request.\n");
        }
    }
    
    if (new_sock < 0) {
        perror("Error accepting connction to socket");
        exit(1);
    }

    return 0;
}