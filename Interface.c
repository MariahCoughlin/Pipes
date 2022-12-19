#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "data.h"

int main()
{
    pid_t pid;
    int err, num, i;
    int interfacePipe[2], serverPipe[2];
    char writeBuffer[SIZE], readBuffer[SIZE];
    char buf1[SIZE], buf2[SIZE] ;

    
    //-------------------------------------
    // Open/check first pipe
    //-------------------------------------
    if(pipe(&interfacePipe[0]) == -1) {
        perror("Error occurred opening interface pipe.");
        exit(1);
    }
    
    //------------------------------------
    // Open/check second pipe
    //------------------------------------
    if(pipe(&serverPipe[0]) == -1) {
        perror("Error occurred opening server pipe");
        exit(2);
    }

    
    for(i = 0; i<SIZE; i++) //clears input
    writeBuffer[i] = '\0';

    pid = fork(); //create fork
    
   
    //****************CHILD PROCESS******************
    if(pid == 0){
        close(interfacePipe[WRITE]); //close write function of interface
        close(serverPipe[READ]);   //close read function of server

        sprintf(buf1, "%d",interfacePipe[READ]);
        sprintf(buf2, "%d",serverPipe[WRITE]);

       
        err = execl("./server", "Server", buf1, buf2, 0); //exec function

        //------------------------------------
        // Check if exec function worked
        //------------------------------------
        if(err == -1) {
            printf("Error while executing. Errno: %d\n",errno);
            exit(3);
        }//----------------------------------
    }
    
    //****************PARENT PROCESS*****************
    else if (pid > 0)
    {
        close(interfacePipe[READ]);    //close read function of interface
        close(serverPipe[WRITE]);   //close write function of server

        while(1)
        {
            printf("Input command: \n");
            scanf("%s", writeBuffer);

            err = write(interfacePipe[WRITE], writeBuffer, strlen(writeBuffer));
            
            //-------------------------------
            // Check if write function worked
            //-------------------------------
            if(err == -1) {
                printf("Error occurred with write. Errno: %d\n",errno);
                exit(4);
            }//-------------------------------
            
            int j;
            for(j = 0; j < SIZE; j++)   //clears out output array
            readBuffer[j] = '\0';

            err = read(serverPipe[READ], readBuffer, SIZE);
            
            //-----------------------------------
            // Check if read function worked
            //-----------------------------------
            if(err == -1) {
                printf("Error occurred with read. Errno: %d\n",errno);
                exit(5);
            }//-----------------------------------
            
            readBuffer[err] = '\0';
            printf("Response: %s\n",readBuffer);

            if(strncmp(writeBuffer, "exit", 4) == 0)//compares 2 c-strings using strncmp
            {
                err = waitpid(pid, &num, 0);
                
                //-----------------------------------
                // Check if waitpid function worked
                //-----------------------------------
                if(err == -1){
                    printf("Error occured with waitpid. Error: %d\n",err);
                    exit(6);
                }
                
                
                printf("Interface: child process (%d) completed.\n",pid);
                printf("Interface: child process exit status = %d.\n",get(num));
               
                printf("Interface: Complete.\n");
                exit(8);
            }
        }
    }
    else
    {
         printf("Error wile forking\n");
                exit(9);
    }
    return 0;
}
