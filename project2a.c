//  project2a.c
//
//  Created by Ryan Capala on 9/11/17.
//  Copyright © 2017 Ryan Capala. All rights reserved.
//  
//  SHELL PROGRAMMING
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/types.h>


/***********function prototype************/
void EchoEnable(int EchoOn);
void allocateMemory(char **arrayPointer, int size, int num);
void moveBack();
void processPipe(char **cmd1, char **cmd2);
void moveCursorLeft();
void processCommand(char **cmd1);
/*****************************************/


int main(){

	// EXIT FLAGS
	int breakOuter = 0;  // to exit main while loop
	char exitString[] = "quit";
    char mergeCMD[] = "merge";
	int breakInner = 0;	 // to exit inner while loop

    	// ECHO VARIABLES
    	int enable = 1;     	// for echo
    	int disable = 0;    	// for echo

    	// FORK VARIABLES
    	int pid1;
    	int status;

    	// COUNTERS
    	int i = 0;
    	int j = 0;
    	int wordCount = 0;
    	int location = 0;		// used to mark the location of pipe
    	int pipeChecker = 0; 	// to check if pipe is used ; 1=true, 0=false

    	char *command1[100];
    	char *command2[100];
    	char *cwdBuffer = (char*) malloc(256);
    	char x = ' ';
    	char y = ' '; // will be used for arrow keys
    	char z = ' '; // will be used for arrow keys
    	char pipeOperator[] = "|";
    	int checker = 0;
    	int size = 256;
    	int num = 15;

    	// STORED COMMANDS
    	char *storedCommands[num];
    	int storedCount = 0;
    	int previousCMDlocation = 0;
        int cursorLocation = 0;

    	// call allocate memory function
    	allocateMemory(storedCommands, size, num);
    	size = 100;
    	allocateMemory(command2, size, size);

    	// call echo enabler function
    	EchoEnable(disable);	// echo disabled


        /*** START OF THE MAIN LOOP ***/
    	while(breakOuter != 1){
    		char *inputBuffer = (char*) malloc(256);
    		/** get and print current directory **/
    		getcwd(cwdBuffer, 256);
    		printf("%s/shell > ", cwdBuffer );

    		// initialize variables
    		x = ' ';
    		wordCount = 0;
    		location = 0;
    		previousCMDlocation = -1;
    		breakInner = 0;
    		i = 0;
    		checker = 0;
            /*** START OF SECONDARY LOOP ***/
    		while(breakInner != 1)
            {
    			x = getchar();
    			//check if user presses enter key
    			if(x == '\n')
                {
    				if(i != 0){
    					inputBuffer[i] = '\0';
    					if(storedCount < 15){
    						for(j = storedCount; j>0; j--){
    							strcpy(storedCommands[j], storedCommands[j-1]);
    						}
    						strcpy(storedCommands[0], inputBuffer);
    						storedCount++;
    					} else {
    						for(j = 14; j > 0; j--){
    							strcpy(storedCommands[j], storedCommands[j-1]);
    						}
    						strcpy(storedCommands[0], inputBuffer);
    					}
    					breakInner = 1;

    				} else {
    					breakInner = 1;
    				}
    			} 
                else if(x == 8)// for backspace key
                { 
    				if(i > 0){
    					moveBack();
    					i--;
    					inputBuffer[i] = '\0';
    				}
    			} 
                else if(x == 127)// for delete key
                { 
    				if(i > 0){
    					moveBack();
    					i--;
    					inputBuffer[i] = '\0';
    				}
    			} 
                else if(x == 27)// for arrow keys
                { 
    				y = getchar();
    				z = getchar();
                    // up arrow key
    				if(y == 91 && z == 65)
                    {
    					// check for history
    					if(storedCount > 0)
                        {
    						if(previousCMDlocation < storedCount - 1)
                            {
	    						for (j = 0; j < i; j++)
                                {
	    							moveBack();
	    						}

	    						if(checker == 1 && previousCMDlocation == -1)
                                {
	    							strcpy(inputBuffer, storedCommands[++previousCMDlocation]);
	    							checker = 0;
	    						} 
                                else 
                                {
	    							strcpy(inputBuffer, storedCommands[++previousCMDlocation]);
	    						}

	    						printf("%s", inputBuffer);
	    						// get length of buffer
	    						i = strlen(inputBuffer);
    						}
    					}
    				} // end of if for 'up=65'
                    // down arrow key
                    else if(y == 91 && z == 66)
                    {
    					// check for history
    					if(storedCount > 0)
                        {
    						if(previousCMDlocation > 0)
                            {
    							for(j = 0; j<i; j++)
                                {
    								moveBack();
                                    cursorLocation = 0;
    							}

    							strcpy(inputBuffer, storedCommands[--previousCMDlocation]);
    							printf("%s", inputBuffer );
    							// get length of buffer
    							i = strlen(inputBuffer);
    						} 
                            else
                            {
    							for(j = 0; j<i; j++)
                                {
    								moveBack();
                                    cursorLocation = 0;
    							}

    							checker = 1;
    							previousCMDlocation = -1;
    							i = 0;
    						}
    					}
    				} 
                    // for right arrow '67=right'
                    else if(y == 91 && z == 67)
                    { 
                        if(storedCount > 0)
                        {
                            int len = strlen(inputBuffer);
                            if(previousCMDlocation <= storedCount - 1)
                            {
                                int index = len - cursorLocation;
                                if(cursorLocation != 0)
                                {
                                    printf("%c", inputBuffer[index] );
                                    cursorLocation--;
                                }
                                
                            }
                        } 
                    }// end else if for right arrow
                    // left arrow key
                    else if(y == 91 && z == 68)
                    { 
                        if(storedCount > 0)
                        {
                            if(previousCMDlocation <= storedCount - 1)
                            {
                                int temp = 0;
                                if(cursorLocation == 0)
                                {
                                    ++temp;
                                    moveCursorLeft();
                                    cursorLocation = temp;
                                }
                                else
                                {
                                    moveCursorLeft();
                                    temp = cursorLocation;
                                    ++temp;
                                    cursorLocation = temp;
                                }
                                
                            }
                        }
                    }// end else if for left arrow    					
    			} // end of for arrow keys 'else if'
                else 
                {
    				printf("%c", x);
    				inputBuffer[i] = x;
    				i++;
    			}
    		}//							>>>> INNER WHILE <<<<<<<<

    		// if null 
    		if(inputBuffer[0] == '\0')
            {
    			printf("\n");
    		} 
            else
            {
    			printf("\n");
    		

        		// TOKENIZING THE INPUT
        		command1[0] = strtok(inputBuffer, " \n"); // store first input
        		i = 0;
        		while(command1[i] != NULL){
        			i++;
        			command1[i] = strtok(NULL, " \n"); // store remaining input
        		}
        		// add null at the end of the array
        		command1[i+1] = NULL;
        		wordCount = i;

        		/***************
        		 *	COMMANDS
        		 **************/
        		// check if user entered quit
        		if(strcmp(command1[0], exitString) == 0)
                {
        			j = 0; 
        			while(j == 0){
        				printf("Exit the shell (y or n)? ");
        				x = getchar();
        				printf("%c", x);
        				inputBuffer[i] = x;
        				i++;

        				if(x == 'y'){
        					j = 1;
        					breakOuter = 1; // exit main while loop flag
        				} else if(x == 'n'){
        					j = 1;
        				}
        				printf("\n");
        			}//end while
        		} 
                ////testing only when user entered merge
                else if(strcmp(command1[0], mergeCMD) == 0)
                {
                    command1[0] = "cat";
                    processCommand(command1);
                }
                /// end of testing only
                else 
                {
        			// check for pipe
        			pipeChecker = 0;
        			for(i = 0; i < wordCount; i++){
        				if(strcmp(command1[i], pipeOperator) == 0){
        					location = i;
        					pipeChecker = 1;
        					break;
        				}
        			}//end for loop

        			// if pipe is used
        			if(pipeChecker){
        				command1[location] = 0;
        				location++;
        				// copy second command
        				for(i = 0; command1[location+i]; i++){
        					strcpy(command2[i], command1[location+i]);
        				}
        				command2[i] = 0;
        				processPipe(command1, command2);
        			} 
                    else if(strcmp(command1[0], "cd") == 0)
                    {
        				if((chdir(command1[1]) != 0)){
        					perror("getcwd() error");
        				}
        			} 
                    else 
                    {
                        processCommand(command1);
        			} 
    		    }//end else
    		    free(inputBuffer);
            }
    	}//								<<<< MAIN WHILE LOOP>>>>


    free(cwdBuffer);
    EchoEnable(enable);
	return 0;
}///@@@ END MAIN @@@///


/***********	function	************/

/************************************************
 *  to turn off echo on the terminal
 *  disable: int disable = 0;
 *  enable:  int enable = 1;
 ***********************************************/
void EchoEnable(int EchoOn)
{
    struct termios TermConf;
    
    tcgetattr(STDIN_FILENO, &TermConf);
    
    if(EchoOn)
        TermConf.c_lflag |= (ICANON | ECHO);
    else
        TermConf.c_lflag &= ~(ICANON | ECHO);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &TermConf);
}

/************************************************
 *	function to allocate memory
 ************************************************/
void allocateMemory(char **arrayPointer, int size, int num){
    int i;
	for (i = 0; i < num; ++i)
	{
		arrayPointer[i] = (char*) malloc(size);
	}
}

/************************************************
 *	to print '\b'
 ************************************************/
 void moveBack(){
 	printf("\b");
 	printf(" ");
 	printf("\b");
 }

 void moveCursorLeft(){
    printf("\b");
 }



/************************************************
 *	pipeUsed	
 ************************************************/
 void processPipe(char **cmd1, char **cmd2){
 	int pid1;
 	int pid2;
 	int fd[2];
 	int status;

 	pid1 = fork();
 	pipe(fd);
 	if (pid1 == 0) {
		pipe(fd);
		pid2 = fork();
		if (pid2 > 1) {
			close(1);
			dup(fd[1]);
			close(fd[0]);
			execvp(cmd1[0], cmd1);
			perror("No such command.");
			exit(1);
                
		} else if (pid2 == 0){
			close(0);
			dup(fd[0]);
			close(fd[1]);
			execvp(cmd2[0], cmd2);
			perror("No such command.");
			exit(1);
		}
                
                
	} else if (pid1 > 1){
		waitpid(pid1, &status, WUNTRACED);
	}
		//free(inputBuffer); // use to clear the buffer

 }

 /*****************************************************
 *  process command
  *****************************************************/
 void processCommand(char **cmd1){

    int status;
    int pid1 = fork();
    if(pid1 > 0){
    waitpid(pid1, &status, WUNTRACED);
    } 
    else if(pid1 == 0){
    //child process
    if((execvp(cmd1[0], cmd1)) == -1){
    perror(NULL);
    exit(0);
    }
    //execvp(command1[0], command1);
    //exit(0);
    } else {
    perror(NULL);
    }

 }


