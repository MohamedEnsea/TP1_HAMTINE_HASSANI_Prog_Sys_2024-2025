#include <unistd.h> 
#include <string.h>
#include <stdlib.h> // We include this to use functions like "exit"
#include <sys/wait.h> // We include this to use the "wait" function
#include <stdio.h>  

#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "

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
    char command[1024]; // We use this array to store the user command input
    print_WELCOME_MSG(); // We print the initial welcome message when the shell starts

    // We create a loop to read and run user commands
    while (1) {
        print_PROMPT(); // We print the prompt to show that the shell is ready for input
        ssize_t number_characters = read(STDIN_FILENO, command, sizeof(command) - 1); // We read the user command from input
        if (number_characters == -1) {
            perror("Error reading command");
            continue; // If read fails, we continue the loop
        }

        command[number_characters - 1] = '\0'; // We change the last character from a new line to null

        pid_t pid = fork(); // We create a new process with fork()
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