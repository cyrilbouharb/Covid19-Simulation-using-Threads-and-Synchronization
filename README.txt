Overview of Implementation:
---------------------------
- This assignment is a simulation of making the appointment for the COVID Test by multiple people at the same time.
- We assume that there are seven call lines, but only four operators.
- When a phone call arrives, we check if there is a free line.
- If so, the phone rings and the caller is considered connected; 
- Otherwise it is busy.
- Once the phone rings, we wait until there is a free operator.
- When we get one, we simulate the process of making the appointment.
- Each phone call is represented by a thread, and we may have more threads running than phone lines.
- Sleep is of 3s


Code Satisfaction with Requirements: (the comments in Covid_test.c are further helpful in explaining where it meets each requirement)
------------------------------------

- Two semophores have been created: operators(counting semaphore), connected(binary semaphore).
- Binary semaphores are used to protect critical regions of code and not to protect non-critical regions of code.
- The counting semaphore operators is used to restrict the use of resources.
- All semaphores are initialized and destroyed. (sem_init/sem_destroy).
- Threads are created and joined properly: the program writes a loop calling pthread_create given
  the phonecall thread function and each corresponding entry of the array of thread id’s.
  After we create each phonecall thread, we block the main thread until all the other phonecall threads
  complete using another loop that uses pthread_join on each thread.
- A global variable next_id exists and is properly updated in the thread function and used to set the caller’s id.
- The phonecall thread properly updates the shared state for the number of connected callers in a critical section.
- The program prints properly formatted outputs with the caller’s id.
- The static modifier is used properly for both thread local variables as well as any global variables. 
- Manage user input correctly, including handling irregular user input or no input. (check main function)
- The code being executed satisfies the output requirements.
- The program runs to completion with a different number of phone calls.
- The program does not cause segmental fault in any cases.
- Threads that are in wait, are exited and print the required message and try again. logic used is same as provided: if (connected==NUM_LINES).
- Threads are then forwarded to next section if line is avaiilable and wait for operator.
- When line is available, new threads allowed to connect.
- Sleep is set to 3 seconds as mentioned.
- The code is compilable by using Makefile.
