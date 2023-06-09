#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//global variables
static sem_t connected_lock;    //binary semaphore for connection
static sem_t operators;         //counting semaphore for operators
static int NUM_OPERATORS = 4;   //static variable for the number of operators
static int NUM_LINES = 7;       //static variable for the number of Lines
static int connected = 0;       //static variable for the number of connected Lines
int Next_id= 0 ;                //Next id variable for assigning the caller id


/*
    Phone call function which first attempt to connect, it validates the connection
    by making sure that their are available free lines, if so the caller is connected
    it then assigns the caller to the operator and makes the appointment, freeing the operator after.
    If there are no available lines it exits the critical section, prints a message and tries again.
    Sleep is of 3s

*/
void* phonecall(void * vagrp){  
   //binary semaphore
    sem_wait(&connected_lock);
    Next_id++;
    int caller_id = Next_id;  //local variable the caller id initialized to Next_id -> 1
    printf("Thread [%d] is attempting to connect...\n",caller_id); //printing that an attempt has been made
    sem_post(&connected_lock);
    //Access to connected is  controlled by a critical section 
    //and is done by using the connected_lock binary semaphore
    sem_wait(&connected_lock);
    while ( NUM_LINES == connected) {
     sem_post(&connected_lock);
        printf("Line is busy. Thread [%d] please try again! \n",caller_id); //uncomment this while
                                                                          // running for it to be clearer
        sleep(3); //using sleep here to reduce the number of messages
     sem_wait(&connected_lock);
    }
    connected++; //incrementing the number of connected lines
    sem_post(&connected_lock);
    printf("Thread [%d] connects to an available Line, Call Ringing...\n",caller_id); 

    //Counting semaphore for the number of operators used once a call connects
    sem_wait(&operators);
        printf("Thread [%d] is speaking to an Operator in the Local health center. \n",caller_id);
        //sleeping for the execution of the call
        sleep(3);
        printf("Thread [%d] has made an appointment for the test! The operator has left... \n",caller_id);
    sem_post(&operators);
    sem_wait(&connected_lock);
        connected--; //decrement the number of connected lines using binary semaphore
        printf("Thread [%d]  has hung up! \n",caller_id);  //call is Over
    sem_post(&connected_lock);

    
   return NULL;
    
}
int main(int argc, char *argv[]){
    //handling wrong inputs and  no input.
    if(argv[1]==NULL || atoi(argv[1])<=0)
    {
        printf("Please pass the valid number of phone calls in the argument!\n");
        return 0;
    }
    int num_phone_calls=atoi(argv[1]);
    pthread_t tid[atoi(argv[1])];   //creating an array of threads id’s that will represent each phone call
    sem_init(&connected_lock, 0,1); //Binary semaphore. Initialize the semaphore at the address &connected_lock,
                                    //initial value is 1 which indicates one thread may be allowed to enter
                                    //the critical section.
                                    //The value 0 indicates that the semaphore is shared between the threads
                                    //of a process and should be located at some address that is visible
                                    // to all threads

    sem_init(&operators, 0,NUM_OPERATORS); //Counting semaphore. Initialize the semaphore at the address
                                           //&connected_lock, initial value is NUM_OPERATORS which is 4,
                                          // so 4 threads may be allowed to enter the
                                          //critical section.  

    for(int i=0;i<num_phone_calls;i++)
    {
        pthread_create(&tid[i],NULL,phonecall,NULL); //create new threads depending on the argument passed
    }
    for(int i=0; i<num_phone_calls;i++) {
        pthread_join(tid[i],NULL); //make calling thread wait for termination of the thread.
    }
    sem_destroy(&connected_lock); //destroys the semaphore at the address &connected_lock
    sem_destroy(&operators);      //destroys the semaphore at the address &operators  
}
