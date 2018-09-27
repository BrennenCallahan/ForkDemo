#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define WRITE 1
#define READ 0

char * convertCase(char*);
int main(int argc, char *argv[])
{
  int pid;
  char *origin;
  char read_msg_c[1024];
  char read_msg_p[1024];
  int pfd1[2], pfd2[2];

  if (argc !=2){
    fprintf(stderr, "Usage: %s ''string here'' \n", argv[0]); //error checking the input
    exit(-1);
  }
  origin = argv[1];                                     //user input

  if(pipe(pfd1) < 0 || pipe(pfd2) < 0){
    printf("COULD NOT OPEN PIPE \n");                     //for if creating a pipe fails
    exit(-1);
  }
  if((pid = fork()) < 0){ 
     printf("COULD NOT FORK \n");
     exit(-1);                                   //FORKING PROCESS
  }
  else if(pid > 0){ // code for the parent process
    close(pfd1[READ]);
    close(pfd2[WRITE]);
    write(pfd1[WRITE], origin, strlen(origin)+1);
    close(pfd1[WRITE]);

    int nbytes = read(pfd2[READ], read_msg_p, sizeof(read_msg_p));
    if (nbytes <= 0)
        printf("COULDNT READ: PARENT\n"); //parent reports reading fail
    else
        printf("");
        
    close(pfd2[READ]);
  }
  else if(pid == 0){ 
    close(pfd1[WRITE]);   //closing the read and write ends of the pipe
    close(pfd2[READ]);
    int nbytes = read(pfd1[READ], read_msg_c, sizeof(read_msg_c)); 
    if (nbytes <= 0)
        printf("COULDNT READ: CHILD\n"); //child reports reading fail
    else{  
        char * changedInput=convertCase(read_msg_c);
        printf("%s\n",changedInput);
        write(pfd2[WRITE], read_msg_c, nbytes);
			}         
		}
	}

char * convertCase(char s[1024]){ //function to convert the cases of a character array
   int i = 0;
   char cha;
  while (s[i] != '\0') {
      cha = s[i];
      if (cha >= 'A' && cha <= 'Z')
         s[i] = s[i] + 32;
      else if (cha >= 'a' && cha <= 'z')
         s[i] = s[i] - 32;  
      i++;  
   }
  return s;
}
