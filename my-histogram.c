#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

typedef struct keyword {
    int argv_index;
    int occurences;
} keyword;


char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int file_exist (char *filename)
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

int num_occurences(char *string, char *substring){
    int total=0;
    while ( (string=strstr(string,substring)) != NULL ){
        total++;
        string++;
    }
    return total;
}

int find_max_padding(keyword *array, int argc){
    int a, max;
    max = 0;
    for( a = 2; a < argc; a = a + 1 ){
        if(array[a-2].occurences > max){
            max = array[a-2].occurences;
        }
    }
    return max + (max/2);
}

int cgi_script(char *request, char *parameters) {
    pid_t pid;
    int status;

    char *envp[] = { NULL };
    char *argv[] = { request, parameters };

    if ((pid = fork()) == 0) {
        execve(request, argv, envp);
        // execl(request, (char*) 0);
        exit(0);
    }

    if (pid > 0) {
      printf("%s\n", "In parent");
      // close(newsock_fd);
      /* the parent process calls waitpid() on the child */
      if (waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
           // the program terminated normally and executed successfully 
            printf("%s\n", "success");
        } 
        else if (WIFEXITED(status) && WEXITSTATUS(status)) {
          if (WEXITSTATUS(status) == 127) {
            printf("%s\n", "failure ");
            /* execl() failed */
          }
          else {
            /* the program terminated normally, but returned a non-zero status */
            printf("%s\n", "fuck");
          }
        } 
        else {
          /* the program didn't terminate normally */
            printf("%s\n", "double fuck");
        }
      }
      else {
        /* waitpid() failed */
      }
    }
    else {
      /* failed to fork() */
    }

    return 0;
}


int main(int argc, char *argv[]) {
    int max_padding, webcache, multithreading;
    FILE *subject_file, *dat_file;
    char *gnu_name, *image_name, *data_name;

    webcache = 0;
    multithreading = 0;

    if(argc < 2){
        printf("please include a file paramater.\n");
        exit(0);
    }else if(argc < 3){
        printf("please include a word to be searched for.\n");
        exit(0);
    }else if(argc > 22){
        printf("Apologies but we can only handle up to 20 keywords.\n");
        exit(0);
    }
  
    subject_file = fopen(argv[1],"r"); // read mode
 
    if( subject_file == NULL )
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
 
    char line[256], buffer[10];

    int a;
    keyword karray[20];
    //Initialize the array to have counters set to zero
    for( a = 2; a < argc; a = a + 1 ){
        karray[a-2].argv_index = a;
        karray[a-2].occurences = 0;
    }

    while (fgets(line, sizeof(line), subject_file)) {
            for( a = 2; a < argc; a = a + 1 ){
                karray[a-2].occurences += num_occurences(line, argv[a]);
            }
    }

    fclose(subject_file);
    max_padding = find_max_padding(karray, argc);
    sprintf(buffer, "%d", max_padding);

    // make sure to remove data.dat first
    // dat_file=fopen("data.dat","a");
    image_name = calloc(255, 1);
    strcat(image_name, "./img_dir/");
    strcat(image_name, argv[1]);
    strcat(image_name, "-output.gif");
    data_name = calloc(255, 1);
    strcat(data_name, "./data_dir/");
    strcat(data_name, argv[1]);
    strcat(data_name, "-output.dat");
    gnu_name = calloc(255, 1);
    strcat(gnu_name, "./gnuplot.cgi ");
    strcat(gnu_name, argv[1]);
    strcat(gnu_name, " ");
    strcat(gnu_name, buffer);


    dat_file=fopen(data_name,"w");
    for( a = 2; a < argc; a = a + 1 ){
        fprintf(dat_file, "%d %s %d\n", a-2, argv[a], karray[a-2].occurences);
    }
    
    max_padding = find_max_padding(karray, argc);
    printf("<html><head><style>body {font-size: 16px;}h1 {color: red;text-align: center;} \
        IMG.center{display: block; margin-left: auto; margin-right: auto;}</style> \
        <title>CS410 Webserver</title></head><body><h1> CS410 Webserver <br></h1><img class=\"center\" \
         src=\"%s\"></body></html>", image_name);
 
    fclose(dat_file);

    // cgi_script("./gnuplot", gnu_name);
    system(gnu_name); // <---- FIX!!!!

    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */


    return 0;
}