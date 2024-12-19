# ENSEASH
As part of our synthesis practical work, our aim is to create a micro shell, displaying the output codes and execution times of the programs we run.

# Organisation
    This project was carried out by Mohamed HAMTINE and Wilal HASSANI (2G2/TD1/TP1) and covers two 8-hour sessions. We have structured the project so that each file corresponds to a different question. 
    We developed our code on Github Codespaces.

# How it works
    Before executing the code, it's important to ensure that the path is correctly defined, to check that we're in the right working directory to execute our code. To do this, use the “pwd” command. If you're not in the right directory, use the following command: cd /workspaces/codespaces-blank/enseash.

    To check that you're in the right directory, you can use the “ls” command, which lists the contents of the current directory, including files and sub-directories.

    Then, to execute the code for each file (each question), simply write :
    - make (to compile everything)
    - gcc -o enseash question1.c (example for file question1.c)
    - ./enseash

# For question 2
    In our tests, we used commands such as "date", "cal" or "clear". Please note that the "fortune" command does not work on this IDE.

# For question 5
    To test the time count, we used the "sleep 5" command, which causes the program to pause for 5s. The execution time displayed is indeed 5s "[exit:0|5002ms]".

# For question 6
    To test the time measurement functionality, we followed these steps:
        1. Executing a command and observing the child's PID
            In the main terminal, after running a command, the PID (Process ID) of the child process is displayed.
        2. Testing with the "sleep" command
            We executed the following command in the main terminal to make the child process pause for 100 seconds: "sleep 100"
        3. Killing the child process from another terminal
            In a second terminal, we used the "kill" command to terminate the child process: "kill -9 [child's PID]"
        4. Observing the result in the main terminal
            Back in the main terminal, we observed that the child process had been successfully killed. The prompt displayed: "[sign:9|64545ms]"

    This output confirmed that the process was terminated by signal 9 (SIGKILL) and indicated the elapsed time since the command started.

# For question 7
    To test this program, we took the following steps:
        1. Creating a file with output redirection
            We used the following command to redirect the output of the ls command to a file named filelist.txt: "ls > filelist.txt"
        This lists the files in the current directory and saves the result in "filelist.txt".
        2. Count lines in file with input redirection
            Next, we counted the number of lines in the filelist.txt file using the following command: "wc -l < filelist.txt"
        This command correctly displays the number of lines in the file.

    These tests confirm that the program's handling of input (<) and output (>) redirections works as expected.