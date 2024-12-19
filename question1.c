#include <string.h>
#include <unistd.h> // We include this to use the write function
#include <stdio.h>  // We include this for perror

#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n" // We define the message that will welcome the user
#define PROMPT "enseash % \n" // We define what the shell prompt 

// Function to display the welcome message
void print_WELCOME_MSG() {
    ssize_t ret = write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG)); // We use write function to send the welcome message to the terminal
    if (ret == -1) {
        perror("Error writing welcome message"); // If there is an error, print the error message
    }
}

// Function to display the prompt
void print_prompt(){
    ssize_t ret = write(STDOUT_FILENO, PROMPT, strlen(PROMPT)); // We use write function to send the prompt to the terminal
    if (ret == -1) {
        perror("Error writing prompt");
    }
}

// The program starts in the main function
int main(){
    print_WELCOME_MSG(); // We call the function to print the welcome message
    print_prompt(); // Then we call the function to print the prompt
    return 0;
}