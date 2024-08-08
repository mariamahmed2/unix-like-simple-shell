#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <signal.h>
#include <fcntl.h>
#include <pwd.h>
#include <time.h>
#include <dirent.h>

#define MAX_CHAR 50
#define MAX_ARGS 10
#define MAX_HISTORY 10


#define clear() printf("\033[999;999H") // Clear the terminal and move the cursor

int flag = 0;                          // Flag of background process
char *log_file = "logFile.txt";
FILE * fptr;
char *args[MAX_ARGS] = {NULL};


void init();
void argsFormater(char line[]);
void scanner(char line[]);
int process(char* args[],char line[]);
int parser(char* args[], char line[]);
void signalHandler(int signal);
void execute(char* args[]);


/************************main************************/
int main() {

    //Remove old logs
    remove("logFile.txt");

    // Set up a signal handler for child processes *******
    signal(SIGCHLD,signalHandler);
    // char * args[MAX_ARGS];      //maximum length of command arguments
    char line[MAX_CHAR];           //maximum length of the command line
    init();

    // Read and excute commands from the user
    while(parser(args,line)) {
        execute(args);
    }
    return 0;
}
/*
/**********************init***********************/

void init() {
	// Clear the terminal screen.
    clear();
    printf("\n\n******************"
        "************************");
    printf("\n\t    HELLO     ");
    printf("\n\n*******************"
        "***********************");

    // Display username and current directory
    char* username = getenv("USER");
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\n\n\nUSER is: @%s", username);
        printf("\nCurrent directory: %s", cwd);
    } else {
        printf("\n\n\nUSER is: @%s", username);
    }

    // Display date and time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("\nDate and time: %s", asctime(tm));

    sleep(2);
    clear();
}


/********************argsFormater*********************/
// modify format of input
void argsFormater(char line[]) {

    int i = 0;
    int j = 0;
    // Trim leading white space
    while (isspace(line[i])) {
        i++;
    }
    // Trim trailing white space
    int len = strlen(line);
    while (isspace(line[len - 1])) {
        line[--len] = '\0';
    }
    // Check for presence of &
    while (line[j] != '\0') {
        if (line[j] == '&') {
            flag = 1;
            line[j] = '\0';
        }
        j++;
    }
    // Split input into individual arguments
    char *token;
    int argIndex = 0;
    token = strtok(line, " \t");
    while (token != NULL && argIndex < MAX_ARGS - 1) {
      args[argIndex] = token;
        token = strtok(NULL, " \t");
        argIndex++;
    }
    args[argIndex] = NULL;
}


void scanner(char line[]) {
    // reads from console
    fgets(line, MAX_CHAR, stdin);
    argsFormater(line);
}

/***********************process***********************/
int process(char* args[],char line[]){
    int i = 0;
    args[i]=strtok(line," ");

    if(args[i]==NULL) {
        printf("Please Enter a COMMAND!\n");
        return 1;
    }

    // Flag variable to indicate whether we are inside a quoted argument
    int inside_quotes = 0;

    while(strcmp(line,"exit")==0 ){exit(0);}

    // Check for CD command
    if (strcmp(args[0], "cd") == 0) {
        char *path;
        if (args[1] == NULL || strcmp(args[1], "~") == 0) {
            // Go to home directory
            path = getenv("HOME");
        } else if (strcmp(args[1], "..") == 0) {
            // Go up one level in directory
            path = "..";
        } else if (args[1][0] == '/') {
            // Absolute path
            path = args[1];
        } else {
            // Relative path
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            strcat(cwd, "/");
            strcat(cwd, args[1]);
            path = cwd;
        }
        if (chdir(path) != 0) {
            printf("Error: %s not found.\n", path);
        }
        return 0;
    }

    // Loop through each character in the input string
    for (int j = 0; j < strlen(line); j++) {
        // If we encounter a quotation mark, toggle the flag
        if (line[j] == '\"') {
            inside_quotes = !inside_quotes;
        }
        // If we encounter a space and we're not inside a quoted argument,
        // start a new argument
        else if (isspace(line[j]) && !inside_quotes) {
            args[++i] = strtok(NULL," ");
        }
    }

    return 1;
}
/***********************parser***********************/
int parser(char* args[], char line[]){
    //scan input
    char* username = getenv("USER");
    printf("%s>> ", username);
    scanner(line);
    //scan array of chars
    process(args,line);
    return 1;
}
/********************signalHandler********************/

void signalHandler(int signal){
    if (signal==SIGCHLD) {
        fptr = fopen(log_file, "a");
        fputs("Child Process was Terminated\n", fptr);
        printf("child terminated \n");
        fclose(fptr);
    }
}

/***********************execute***********************/

void execute(char* args[]){
    int status;
    pid_t pid = fork();
    if (flag == 1 && pid != 0){
            //nothing
    } else {
        if (pid == 0) {
                //child
            execvp(args[0], args);
            printf("Invalid command \n");
        } else {
                //parent
            waitpid(pid,&status,WUNTRACED);
        }
    }
    flag = 0;
}
