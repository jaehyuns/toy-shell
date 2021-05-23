
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <dirent.h>

#define BUF_SIZE 1024
#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	30
#define maxx 100



int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    //char *cwd;
    char wd[BUFSIZ];
    char * cwd = (char *)malloc(sizeof(char) * 1024);
    DIR * dir = NULL;
    struct dirent * entry = NULL;
    getcwd(cwd, 1024);



    while (true) {
        char *s;
        int len;
        char hostname[LEN_HOSTNAME + 1];
        struct dirent * entry = NULL;
        char exit_1[] ="exit";
        char clear_1[]="clear";
        char pwd_1[]="pwd";
	      char ls_1[] = "ls";
        char cp_1[]="cp";
        char cd_1[]="cd";
	      memset(hostname, 0x00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);
        printf("\033[1;33m%s@%s:%s$\033[66m", getpwuid(getuid())->pw_name, hostname,cwd);
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s== NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }


        len = strlen(command);
        printf("\033[33;34m%d\033[63m\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        printf("\033[33;34m[%s]\033[63m\n", command);
        if (!strcmp(command,exit_1)){
            exit(1);
        }
        if (!strcmp(command,clear_1)){
          system("clear");
          printf("지우기 완료\n");
        }
        if (!strcmp(command,pwd_1)){
          //args[0] = "/bin/pwd";
          cwd = getcwd(NULL, BUFSIZ);
          printf("%s\n", cwd);
        }
        if (!strcmp(command,ls_1)){
          if( (dir = opendir(cwd)) == NULL)

          {
                  printf("current directory error\n");

          }
          while( (entry = readdir(dir)) != NULL)

          {
                  printf("%s\n", entry->d_name);
          }

          free(cwd);
          closedir(dir);
        //args[0] = "/bin/ls";
          }
        

        if(!strcmp(command,cp_1)){
          char *c;
          char *d;
          char a[MAX_LEN_LINE],b[MAX_LEN_LINE];
          //mount -o remount,rw;
          printf("복사를 진행할 파일을 입력하세요:");
          c = fgets(a, MAX_LEN_LINE, stdin);
          printf("복사를 받을 파일을 입력하세요:");
          d = fgets(b, MAX_LEN_LINE, stdin);
          FILE *src, *dst;
          int ret;
          char buffer[BUF_SIZE];
          if(a == NULL && b==NULL) {
              printf("ERROR\n");


          }
          src = fopen(c, "rb");
          dst = fopen(d, "wb");
          while(ret = fread(buffer, 1, sizeof(buffer), src)) {
              fwrite(buffer, 1, ret, dst);
          }
          fclose(src);
          fclose(dst);
          }

        if(strncmp(command,s,strlen(command))== 0){
          chdir(s);
          cwd = getcwd(NULL, BUFSIZ);
            //if(isFileOrDir(s)==0){
              //printf("%s directory로 이동.\n",s);
        //}
        }




        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status));
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);

            if (ret < 0) {
                fprintf(stderr, "execve failed\n");

                return 1;
            }
        }
    }
    return 0;
}
