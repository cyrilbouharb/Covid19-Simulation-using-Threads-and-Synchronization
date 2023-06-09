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
static sem_t  LINES;            //counting semaphore for Lines
static int NUM_OPERATORS = 5;   //static variable for the number of operators
static int NUM_LINES = 8;       //static variable for the number of Lines
static int connected = 0;       //static variable for the number of connected Lines
int Next_id=1;                  //Next id variable for assigning the caller id


/*
    Phone call function which first validates the connection
    it thens make sure the Number of active lines are available
    it then assigns a call to the opeerator and wait for the operator to free
    Sleep is of 3s

*/
void* phonecall(void * vagrp){  
  
    //local variable the caller id
    int caller_id;

    label:
    sem_wait(&connected_lock);
      //check if the num_lines==connect
        if(NUM_LINES==connected){
            printf("Try Again.\n");
            sleep(3);
            //prints try again and resets semaphores
            sem_post(&connected_lock);
            //pthread exit funciton
            goto label;
        }
        else //if the if the num_lines<connect
        {
            //assign the incremented id
            caller_id=Next_id++;
            connected+=1;
            printf("Thread %d is attempting to connect.\n",caller_id);
        }
    sem_post(&connected_lock);

    //semaphores for the number of the lines
    sem_wait(&LINES);
        printf("Thread %d connects to an available Line, Call Ringing \n",caller_id);
    sem_post(&LINES);

    //semaphore for the number of the operators
    sem_wait(&operators);
        printf("Thread %d is speaking to an Operator in the Local health center \n",caller_id);
        //sleeping for the execution of the call
        sleep(3);
        printf("Thread %d has made an appointment for the test! The operator has left \n",caller_id);
        printf("Thread %d  has hung up! \n",caller_id);
        //decrmenting the connected variable so that other calls can join the local health helpline
        connected-=1;
    sem_post(&operators);
    //exiting the thread
    pthread_exit(NULL);
    
}
int main(int argc, char *argv[]){
    //handling wrong inputs
    if(argv[1]==NULL || atoi(argv[1])<=0)
    {
        printf("Please pass the valid number of phone calls in the argument!\n");
        return 0;
    }
    int num_phone_calls=atoi(argv[1]);
    pthread_t tid[atoi(argv[1])];
    sem_init(&LINES, 0,NUM_LINES);
    sem_init(&connected_lock, 0,1);
    sem_init(&operators, 0,NUM_OPERATORS);
    for(int i=0;i<num_phone_calls;i++)
    {
        pthread_create(&tid[i],NULL,phonecall,NULL);
    }
    pthread_join(tid[num_phone_calls-1],NULL);
    sem_destroy(&LINES);
    sem_destroy(&connected_lock);
    sem_destroy(&operators);
}