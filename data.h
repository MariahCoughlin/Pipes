#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED


#define READ 0
#define WRITE 1
#define SIZE 256
#define MAX_CARS 12 //max number of cars
#define get(x) (((x) & (0xFF << 8)) >> 8)


typedef struct
{
    char id[8];
    int odometer;
    float gallons;
}car;



#endif // DATA_H_INCLUDED

