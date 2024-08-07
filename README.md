# Custom Shell Program

## Introduction
This report presents a custom shell program written in C. The program provides a command-line interface where users can enter commands to be executed by the shell. The shell program supports basic commands, such as changing directories, list, and echo.

## Code Description
Video Link: [https://youtu.be/8vgIPugdS0](https://youtu.be/8vgIPugdS0)

The custom shell program is composed of several functions that are responsible for different aspects of the program.

- `init()`: Clears the terminal screen and displays the current user, directory, and time.
- `argsFormater(char line[])`: Formats the input string by removing leading and trailing white space and checking for the presence of an ampersand (&) to indicate that a process should be run in the background. The function also splits the input string into individual arguments and stores them in an array.
- `scanner(char line[])`: Reads input from the console using the `fgets()` function and passes the input to the `argsFormater(char line[])` function.
- `process(char* args[], char line[])`: Processes the input by checking for the `cd` command and splitting the input string into individual arguments.
- `parser(char* args[], char line[])`: Scans input from the user and calls the `scanner(char line[])` and `process(char* args[], char line[])` functions to process the input.
- `execute(char* args[])`: Executes the command entered by the user. If the process is run in the background, the function sets the flag variable to 1 and executes the process using the `execvp()` function. If the process is run in the foreground, the function waits for the process to complete using the `waitpid()` function.
- `signalHandler(int signal)`: Called when a child process terminates and sets the flag variable to 0.

## The Algorithm
The program is separated into small functions:

### main()
The `main()` function starts by removing any old logs by calling the remove function with the name of the log file. It then sets up a signal handler for child processes using the `signal` function with the `SIGCHLD` signal and the `signalHandler` function.

The function declares an array of char called `line` with a maximum size of `MAXCHAR`. This array will be used to store the user’s command line input.

The program then calls the `init` function, which likely initializes any necessary variables or data structures. The function enters a loop that will continue to read and execute commands from the user until the `parser` function returns false. The `parser` function is not shown in the code, but it likely parses the user’s input into an array of strings, which are then stored in the `args` array.

For each iteration of the loop, the program calls the `execute` function with the `args` array as its argument. The `execute` function likely takes the array of strings and executes the appropriate command based on the user’s input.

Finally, it returns 0 to indicate successful completion.

### init()
The `init()` function initializes the terminal by displaying some information to the user such as username, current directory, date, and time. Here are the steps it follows:
- Clear the terminal screen.
- Display a greeting message.
- Get the username using `getenv("USER")`.
- Get the current working directory using `getcwd()` and display it.
- Display the current date and time using `time()` and `localtime()`.
- Sleep for 2 seconds.
- Clear the terminal screen again.

Overall, this function provides some basic information to the user and gives the terminal a clean and organized appearance.

### argsFormater(char line[])
The `argsFormater` function takes a character array `line` as input and formats it into individual arguments for command execution. Here is a breakdown of what it does:
1. It trims any leading whitespace characters in the input string `line`.
2. It trims any trailing whitespace characters in the input string `line`.
3. It checks for the presence of an ampersand (&) character at the end of the input string `line`. If found, it sets a global flag variable to 1 and removes the ampersand from the string.
4. It splits the input string `line` into individual arguments using `strtok()` function with delimiter of white space (" "). The individual arguments are stored in the global `args` array, with the last element of `args` set to NULL.

Note that `MAXARGS` is a macro defined at the beginning of the program and specifies the maximum number of arguments that can be parsed. The `args` array is also a global variable defined at the beginning of the program and stores the parsed command arguments.

### scanner(char line[])
The `scanner` function reads a line of input from the console using the `fgets` function and passes the resulting string to the `argsFormater` function. The `fgets` function takes three arguments: a pointer to a character array (`line`), the maximum number of characters to read (`MAXCHAR`), and a pointer to the file from which to read the input (`stdin` in this case, which represents standard input, i.e., the console).

After reading the line of input, the `argsFormater` function is called to format the input and store the resulting arguments in the `args` array.

### process(char* args[], char line[])
The `process` function takes the `args` array and the `line` string as input. The `args` array is expected to have been initialized and cleared before calling this function. The function first checks if the first argument is NULL or empty, and if so, it prints an error message and returns 1. Then it checks if the first argument is `cd`, and if so, it handles the `cd` command by setting the path variable and changing the current directory using `chdir`. Finally, it loops through each character in the `line` string and sets the `args` array to each argument separated by spaces, while ignoring arguments enclosed in quotes.

If the `process` function encounters the `exit` command, it will not exit the program but will continue to execute the loop in the `main` function. This is because the `exit` command is checked within the while loop of the `main` function, and if the command is detected, the loop will exit and the program will terminate.

### parser(char* args[], char line[])
The `parser` function is responsible for getting input from the user, processing it, and returning a flag indicating whether the program should continue running. Here’s how it works:
- It first prints the prompt to the user, consisting of their username followed by ">> ".
- It then calls the `scanner` function, which reads input from the console and formats it into an array of arguments using the `argsFormater` function.
- It then calls the `process` function, which further processes the array of arguments to handle special commands like `cd` and to handle quoted arguments.
- Finally, it returns a flag indicating that the program should continue running.

Overall, the `parser` function serves as the main entry point for getting user input and processing it, allowing the shell to interact with the user and execute commands.

### signalHandler(int signal)
The `signalHandler` function is a callback function that is called when the parent process receives a `SIGCHLD` signal from one of its child processes.

When a child process terminates, the operating system sends a `SIGCHLD` signal to the parent process. The purpose of the signal is to notify the parent process that one of its child processes has terminated.

The `signalHandler` function opens a log file in append mode, writes a message indicating that a child process was terminated, and then closes the file. The function also prints a message to the console indicating that a child process was terminated.

### execute(char* args[])
The `execute()` function uses `fork()` to create a new child process and then executes the command passed in through the `args` array using `execvp()`. If the flag variable is set to 1, it indicates that the command should run in the background, so the parent process does not wait for the child process to complete. Otherwise, the parent process waits for the child process to complete using `waitpid()`.
