#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <stdio.h>

#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % \n"
#define EXIT_MSG "Bye bye...\n" // We define the exit message.

void print_WELCOME_MSG() {
    ssize_t ret = write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));
    if (ret == -1) {
        perror("Error writing welcome message");
    }
}

void print_PROMPT() {
    ssize_t ret = write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
    if (ret == -1) {
        perror("Error writing prompt");
    }
}

int main() {
    char command[1024];
    print_WELCOME_MSG();

    while (1) {
        print_PROMPT();
        ssize_t number_characters = read(STDIN_FILENO, command, sizeof(command) - 1);
        if (number_characters == -1) {
            perror("Error reading command");
            continue; 
        }

        if (number_characters == 0) {
            ssize_t ret = write(STDOUT_FILENO, EXIT_MSG, strlen(EXIT_MSG)); // We print the exit message when the user exits with Ctrl+D.
            if (ret == -1) {
                perror("Error writing output message");
            }
            break; // We break out of the loop to end the shell.
        }

        command[number_characters - 1] = '\0';

        if (strcmp(command, "exit") == 0) {
            ssize_t ret = write(STDOUT_FILENO, EXIT_MSG, strlen(EXIT_MSG)); // We print the exit message when the user exits with "exit"
            if (ret == -1) {
                perror("Error writing output message");
            }
            break; // We break out of the loop to end the shell.
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Error during fork");
            continue; 
        }

        if (pid == 0) {
            // In the child process, we run the command using "execlp"
            if (execlp(command, command, (char *)NULL) == -1) {
                perror("Error executing command");
                exit(-1); // If execlp fails, we exit the child process with error code -1
            }
        } else {
            // In the parent process, we wait for the child to finish
            if (wait(NULL) == -1) {
                perror("Error waiting for child process");
            }
        }
    }
    return 0;
}