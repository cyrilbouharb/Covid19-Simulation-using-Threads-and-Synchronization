Overview of Implementation:
---------------------------

- Phone call function which first validates the connection
- It thens make sure the Number of active lines are available
- Assigns a call to the operator and wait for the operator to free
- Sleep is of 3s


Code Satisfaction with Requirements:
------------------------------------

- Three semophores have been created: operators(counting semaphore), lines(counting semaphore), connected(binary semaphore)
- Critical section is adjusted to the number of threads allowed.
- Threads that are in wait, are exited and prints the required message. logic used is same as provided: if (connected==NUM_LINES).
- Threads in critical section are forwarded to next section.
- When semophore is available, new threads allowed to enter critical section.
- Sleep is set to 3 seconds as mentioned.