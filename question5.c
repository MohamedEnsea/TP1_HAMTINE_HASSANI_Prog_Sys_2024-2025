#define _POSIX_C_SOURCE 199309L // This macro enable POSIX 1993 features like clock_gettime
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h> // For clock_gettime

#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "
#define EXIT_MSG "Bye bye...\n"

void print_WELCOME_MSG() {
    write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));
}

// Function to print the prompt and exit status or signal
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

int main() {
    char command[1024];
    int status = 0;
    long exec_time_ms = 0; // Execution time in ms

    print_WELCOME_MSG();

    while (1) {
        struct timespec start_time, end_time; // We declare 2 variable to store start and end times
        print_PROMPT(status, exec_time_ms); // We display the prompt with the command's previous status and execution time

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

        // We measure the time before forking
        clock_gettime(CLOCK_REALTIME, &start_time);

        pid_t pid = fork();

        if (pid == 0) { 
            if (execlp(command, command, (char *)NULL) == -1) {
                perror("Command execution error");
                exit(-1); 
            }
        } else if (pid > 0) { // Code is executed by the parent
            waitpid(pid, &status, 0); // We wait for the child to finish

            // We measure the time after the child process has finished
            clock_gettime(CLOCK_REALTIME, &end_time);

            // We calculate execution time in milliseconds
            exec_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                           (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
        } else {
            perror("Fork failed");
        }
    }
    return 0;
}