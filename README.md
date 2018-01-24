# ShellProgrammingProject_P2_inC

In this assignment, I have to write a Linux terminal shell using C programming language. The shell will act similar to bash, which is the default shell when you log into a unix or linux terminal. At a high level, the shell should accomplish the following task:
- Print a prompt when waiting for input from the user.
- Accept commands fromt the user and execute them until the user exits the shell. If the command doesnt exists in the system, an error message should be displayed.
- Accept the change directory command, and appropriately change the current working directory. If the directory the users enters does not exist, an error message should be displayed.
- List the contents of directory on dir command.
- The shell must keep a history of the last 5 commands executed. The user should be able to cycle through the history using the up and down arrow keys.
- When user presses left and right arrow, cursor should move left and right respectively. It should work like in Linux shell.
- The shell must accept the pipe '|' operator. This will execute two commands, where the first process will output to the input of the second process.
- Pause operation of the shell until 'Enter' is pressed when pasue command is entered.
- Accept 'quit' command that will close the shell properly. Before executing the quit command, it must print a prompt to comfirm whether the user wants to exit or not.


