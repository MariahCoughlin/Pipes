#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>  
#include <sys/types.h>
#include <limits.h>
#include "data.h"

//prototypes
car cars[MAX_CARS];
int getMin(char id[]);
int getMax(char id[]);
float getSum(char id[]);
float getAvg(car cars[], char id[]);

int main (int argc, char *argv[])
{
    char readBuffer[SIZE];   //user input
    char writeBuffer[SIZE];  //output for user
    int fdIn, fdOut, err, car1, car2;
    int  m , n, i, j;
    char id[9];
    float avg;

    //-----------------------------
    // Convert from Ascii to int
    //-----------------------------
    fdIn = atoi(argv[1]);
    fdOut = atoi(argv[2]);

    //--------------------
    // Opening Data file
    //--------------------
    FILE *finput;
    finput = fopen("gasData.txt","r");

    
    //----------------------------------
    // Read input from file
    //-----------------------------------
    for (i = 0; i < MAX_CARS; i++){
        fscanf(finput, "%s %d %f", cars[i].id, &cars[i].odometer, &cars[i].gallons);
    }//endfor
   
    //----------------------------------
    //Display the file input
    //----------------------------------
    for (j = 0; j < MAX_CARS; j++){
        printf("element = %d: id = %s, odometer = %d, gallons = %.6f\n", j, cars[j].id,cars[j].odometer, cars[j].gallons);
    }//endfor
    
    
    //-------------------------
    // Check for server error
    //-------------------------
    if (argc < 2)  {
        printf("Error occured in Server\n");
        exit(1);
    }//endif

 
    
    while(1) {
        err = read(fdIn, readBuffer, SIZE);
        
        //--------------------------
        // Check if read worked
        //--------------------------
        if(err == -1 ) {
            printf ("Error occured in Read: %d\n", errno);
            exit(8);
        }//-------------------------

        readBuffer[err] = 0;
        writeBuffer[0] = '\0';

        
        //-----------------
        // EXIT command
        //-----------------
        if(strncmp(readBuffer, "exit", 4) == 0){
             strcpy(writeBuffer, "Server complete. \n");
        }

        //----------------
        // MPG command
        //-----------------
        else if(strncmp(readBuffer, "mpg,",4) == 0){
            strcpy(id, &readBuffer[4]);
            avg = getAvg(cars, id); //displays average
            if(avg == -1)
                sprintf(writeBuffer, "ID not found");
            else
                sprintf(writeBuffer, "Average MPG = %f", avg);
        }
        //-----------------
        // LIST command
        //------------------
        else if(strncmp(readBuffer, "list,",5) == 0){
            strcpy(id, &readBuffer[5]);

            int temp=0;
            float temp1 =0.0;
            char data[MAX_CARS];
            int x=0;   //incrememnt if strings match
            for(m = 0; m < MAX_CARS; m++) {
                if(strcmp(id, cars[m].id) == 0) {
                    data[x] = m;
                    x++;
                }//end if
            }//end for
            
            //---------------------
            //saves element number
            //-----------------------
            for(n = x; n<MAX_CARS; n++){
                data[n] =-1;
            }//end for
            
            //------------------------------------------
            // Sort Odometer reading in increasing order
            //------------------------------------------
            for(car1 = 0; car1 < x-1;car1++) {
                for(car2 = car1+1; car2 < x; car2++) {
                    if (cars[data[car1]].odometer > cars[data[car2]].odometer) {
                            temp = cars[data[car1]].odometer; //swap
                            cars[data[car1]].odometer = cars[data[car2]].odometer;
                            cars[data[car2]].odometer = temp;
                            temp1 = cars[data[car1]].gallons;
                            cars[data[car1]].gallons = cars[data[car2]].gallons;
                            cars[data[car2]].gallons = temp1;
                    }//end if
                }//end nested for
            } // end for
            
            //------------------------
            // Display Results
            //------------------------
            for (car1 = 0; car1 < x; car1++) {
                sprintf(writeBuffer + strlen(writeBuffer),"element = %d: id = %s, odometer = %d, gallons = %.6f\n", data[car1], id,cars[data[car1]].odometer, cars[data[car1]].gallons);
            }//end for
        }//end else-if
        
        //--------------------
        // Invalid Command
        //--------------------
        else{
            strcpy(writeBuffer, "Invalid command");
        }//end else
        
        
        err = write(fdOut, writeBuffer, strlen(writeBuffer));
        
        
        //--------------------------
        // Check if write worked
        //--------------------------
        if (err == -1){
            printf( "Error occurred in Write : %d\n", errno);
            exit(9);
        }//endif

        //----------------
        // EXIT Command
        //------------------
        if (strncmp(readBuffer, "exit", 4) == 0){
            exit(0);
        }//end if
    
    }//end while
}//end Main
//---------------------------------------------------------------

//------------getMax---------------
// Calculates Maximum Odometer Value
//----------------------------------
int getMax(char id[]){
    int maxOdometer = 0;
    int i;
    for (i = 0; i < MAX_CARS; i++){
        if(strcmp(id, cars[i].id) == 0){
            if (maxOdometer < cars[i].odometer)
                maxOdometer = cars[i].odometer;
        }//endif
    }//endfor
    return maxOdometer;
}//end getMax

//------------getMin---------------
// Calculates Minimum Odometer Value
//----------------------------------
int getMin(char id[]){
    int flag=0;
    int minOdometer = 999999;
    int i;
    for (i = 0; i < MAX_CARS; i++){
        if(strcmp(id, cars[i].id) == 0){
            flag = 1;
            if (minOdometer > cars[i].odometer)
                minOdometer = cars[i].odometer;
        }//end if
    }//endfor

    if (!flag){
        printf("Input id not found\n");
        exit(11);
    }//endif
    return minOdometer;
}//end getMin

//------------getSum---------------
// Sum of gallons
//----------------------------------
float getSum(char id[]){
    float sum = 0;
    int i;
    for (i = 0; i < MAX_CARS; i++){
        if(strcmp(id, cars[i].id) == 0)
            sum += cars[i].gallons;
    }//endfor
    return sum;
}//end getSum

//------------getAvg---------------
//Calculates and returns average
//----------------------------------
float getAvg(car cars[], char id[]){
    int minimum = 0, maximum = 0;
    float sum = 0, avg = 0;

    minimum = getMin(id);
    maximum = getMax(id);
    sum = getSum(id);

    avg= ((float)maximum-(float)minimum) / sum;

    return avg;
}
