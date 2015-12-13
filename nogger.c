#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>

int main ()
{
  // char * source = "___ abc123def ___ ghi456 ___";
  char * source = " nigger nagger nagger nagger nagger nagger nagger nagger nagger nogger ";
  char * regexString = "nagger";//"[a-z]*([0-9]+)([a-z]*)";
  size_t maxMatches = 2;
  size_t maxGroups = 3;
  
  regex_t regexCompiled;
  regmatch_t groupArray[maxGroups];
  unsigned int m;
  char * cursor;

  if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
    {
      printf("Could not compile regular expression.\n");
      return 1;
    };

  m = 0;
  cursor = source;
  for (m = 0; m < maxMatches; m ++)
    {
      if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
        break;  // No more matches

      unsigned int g = 0;
      unsigned int offset = 0;
      for (g = 0; g < maxGroups; g++)
        {
          if (groupArray[g].rm_so == (size_t)-1)
            break;  // No more groups

          if (g == 0)
            offset = groupArray[g].rm_eo;

          char cursorCopy[strlen(cursor) + 1];
          strcpy(cursorCopy, cursor);
          cursorCopy[groupArray[g].rm_eo] = 0;
          printf("Match %u, Group %u: [%2u-%2u]: %s\n",
                 m, g, groupArray[g].rm_so, groupArray[g].rm_eo,
                 cursorCopy + groupArray[g].rm_so);
        }
      cursor += offset;
    }

  regfree(&regexCompiled);

  return 0;
}

// int match(const char *string, char *pattern){
//     int    status;
//     regex_t    re;

//     if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
//         return(0);      /* Report error. */
//     }
//     status = regexec(&re, string, (size_t) 0, NULL, 0);
//     regfree(&re);
//     if (status != 0) {
//         return(0);      /* Report error. */
//     }
//     return(1);
// }


// int main(int argc, char *argv[]) {
//     regex_t regex;
//     int reti;
//     char msgbuf[50];
//     strcpy(msgbuf, " nogger one two three");

//     regmatch_t matches[2];
//     int start;
//     int end;  

// /* Compile regular expression */
//     reti = regcomp(&regex, "^a[[:alnum:]]", 0);
//     if( reti ){ fprintf(stderr, "Could not compile regex\n"); exit(1); }

//     printf("~~~~~~~~~~~~~~~~~~~~~\n");
// /* Execute regular expression */
//     reti = regexec(&regex, "nogger", 0, matches, 0);
//     printf("~~~~~~~~~~~~~~~~~~~~~\n");

//     if( !reti ){
//         printf("I AM NOGGER!\n");
//         start = matches[0].rm_so;
//         /* rm_eo is the end index of the match */
//         end = matches[0].rm_eo;
//         printf("%.*s, \n", (end - start), (msgbuf + start));
//         puts("Match");
//     }
//     else if( reti == REG_NOMATCH ){
//         puts("No match");
//     }
//     else{
//         regerror(reti, &regex, msgbuf, sizeof(msgbuf));
//         fprintf(stderr, "Regex match failed: %s\n", msgbuf);
//         exit(1);
//     }

// /* Free compiled regular expression if you want to use the regex_t again */
//     regfree(&regex);

//     return 0;
// }

// char *buffer;
//     buffer = (char *) malloc(20);
//     strcpy(buffer, " hello 0 1 4 5");

//     regmatch_t matches[2];
//     int start;
//     int end;  
//     int count = 0;
//     regex_t re;
//     int reti = regcomp(&re, " hello ", REG_EXTENDED); //k?k //[^0-9]*([0-9]+)
//     // int reti = regcomp(&re, "[^0-9]*([0-9]+)", REG_EXTENDED); //k?k //[^0-9]*([0-9]+)

//     while (1) {
//         reti = regexec(&re, buffer, 2, matches, 0);

//         /* rm_so is the start index of the match */
//         start = matches[1].rm_so;
//         /* rm_eo is the end index of the match */
//         end = matches[1].rm_eo;
//         /* break if we didn't find a match */
//         if (reti) break;

//         /* print the substring that contains the match */
//         // printf("start is: %d")
//         printf("%.*s, \n", (end - start), (buffer + start));
//         printf("start is \"%d\" and end is \"%d\"\n", start, end);
//         /* increment the count of matches */
//         count = count + 1;

//         /* This is really important!
//          Move the start of the string forward so we don't keep matching the same number! */
//         buffer = buffer + end;
//     } 

//     /* print the count */
//     printf("count = %d\n", count);