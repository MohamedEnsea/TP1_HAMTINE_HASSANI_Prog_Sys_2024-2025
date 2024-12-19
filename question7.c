#define _POSIX_C_SOURCE 199309L // This macro enable POSIX 1993 features like clock_gettime
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>  // For open()

#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "
#define EXIT_MSG "Bye bye...\n"

void print_WELCOME_MSG() {
    write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG)); 
}

void print_PROMPT(int status, long exec_time_ms) {

    if (WIFEXITED(status)) { 
        char exit_msg[50];
        int exit_code = WEXITSTATUS(status);
        snprintf(exit_msg, sizeof(exit_msg), "[exit:%d|%ldms]", exit_code, exec_time_ms);
        write(STDOUT_FILENO, exit_msg, strlen(exit_msg)); 
    } else if (WIFSIGNALED(status)) { 
        char signal_msg[50];
        int signal_code = WTERMSIG(status); 
        snprintf(signal_msg, sizeof(signal_msg), "[sign:%d|%ldms]", signal_code, exec_time_ms);
        write(STDOUT_FILENO, signal_msg, strlen(signal_msg)); 
    }
    
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

// Function to split a command into arguments
void split_command_into_args(char *input, char **argv) {
    char *token = strtok(input, " ");
    int index = 0;

    while (token != NULL) {
        argv[index++] = token; 
        token = strtok(NULL, " ");
    }
    argv[index] = NULL; 
}

int main() {
    char command[1024]; 
    char *argv[100];    
    int status = 0;     
    long exec_time_ms = 0; 

    print_WELCOME_MSG(); 

    while (1) { 
        struct timespec start_time, end_time; 

        print_PROMPT(status, exec_time_ms);

        ssize_t number_characters = read(STDIN_FILENO, command, sizeof(command) - 1);

        if (number_characters == 0) {
            write(STDOUT_FILENO, EXIT_MSG, strlen(EXIT_MSG)); 
            break;
        }

        command[number_characters - 1] = '\0';

        if (strcmp(command, "exit") == 0) { 
            write(STDOUT_FILENO, EXIT_MSG, strlen(EXIT_MSG)); 
            break;
        }

        clock_gettime(CLOCK_REALTIME, &start_time);

        split_command_into_args(command, argv);

        pid_t pid = fork(); 

        if (pid == 0) { 
            int i;
            int fd_in = -1, fd_out = -1;

            // We check if there is an input redirection (<)
            for (i = 0; argv[i] != NULL; i++) {
                if (strcmp(argv[i], "<") == 0) { 
                    fd_in = open(argv[i + 1], O_RDONLY); // We open the file to read from it
                    if (fd_in == -1) {
                        perror("Input file open failed"); // We display an error if the file cannot be opened
                        exit(EXIT_FAILURE); // We stop the process because we cannot continue
                    }
                    argv[i] = NULL; // We remove "<" and the file name from the command
                    break;
                }
            }

            // We check if there is an output redirection (>)
            for (i = 0; argv[i] != NULL; i++) {
                if (strcmp(argv[i], ">") == 0) { 
                    fd_out = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // We open the file to write into it
                    if (fd_out == -1) {
                        perror("Output file open failed"); // We display an error if the file cannot be created or opened
                        exit(EXIT_FAILURE); // We stop the process because we cannot continue
                    }
                    argv[i] = NULL; // We remove ">" and the file name from the command
                    break;
                }
            }

            // We apply the redirections
            if (fd_in != -1) {
                dup2(fd_in, STDIN_FILENO); // We redirect the standard input to the file
                close(fd_in); // We close the file descriptor since it's no longer needed
            }
            if (fd_out != -1) {
                dup2(fd_out, STDOUT_FILENO); // We redirect the standard output to the file
                close(fd_out); // We close the file descriptor since it's no longer needed
            }

            // We execute the command
            if (execvp(argv[0], argv) == -1) {
                perror("Command execution failed"); // We display an error if the command cannot run
                exit(-1); // We stop the child process
            }
        } else if (pid > 0) { 
            waitpid(pid, &status, 0); // We wait for the child process to finish

            // We calculate the execution time
            clock_gettime(CLOCK_REALTIME, &end_time);

            exec_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                           (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
        }
    }
    return 0; 
}
