# CEC16: High Performance Computing

## List of Assignments 

### 1. **Assignment 1**

Use vi editor/gedit, gcc compiler, makefile (optional) and gdb debugger in developing these programs.

    (a) Create and handle Processes in Linux operating system. Make use of fork, sleep, wait, exit, getpid, getppid system calls and ps, kill Linux commands. 
   
    (b) Design and implement a simple, interactive shell program that prompts the user for a command, parses the command, and then executes it with a child process by looking at PATH environment variable.  Implement running of command in background also. Properly handle Ctrl-c, Ctrl-d signals.

**Some Background Material:**

Unix shell runs all the commands in a new forked child process. It means, the shell in which the command is invoked becomes the parent that creates a child process ( another shell process). The child in turn exec's the command by overlaying itself with the command's image. 
Why would the new process be created for executing the command? Can't the shell run the command in the same process. The answer is: If the shell overlays itself with the command's process image, it would have nowhere to return when the command finishes executing. This would close the parent shell. Running "exec command_name" effectively does this i.e. runs the command that eventually closes the shell when the command execution is over. To avoid closing the invoking shell itself, all commands are run in a new child process by the shell.

### 2. **Assignment 2**

Learn to use

    A) system calls for implementing Shared Memory: ftok, shmget, shmat, shmdt, shmctl.

    B) commands ipcs, ipcrm

    C) semaphore related system calls: semget, semctl, semop

D) Now, you will implement a game called **WAR** by using the semaphore system calls and C library functions: In War, initially process 1 (an executable of process1.c) will fork process2. Each process will sleep for a random amount of time before competing on accessing the semaphore. The one that goes through first will kill the other process, increment it's counter, fork a new process, and signal the semaphore.
The winning process (winner of the current battle) will then sleep for a random amount of time before competing again against the newly created process, and so on. The counter of each new process is reset before it enters the battlefield. The one process that wins 3 consecutive battles (i.e. when it's counter becomes 3) will win the War and terminate the game.
Programming: Write one C program (process1.c) that will perform the specifications as explained above. The following statistics will be reported by the winning process:
1. WAR launching time.
2. Time of creation of the processes
3. Time of termination of the process

### **Producer Consumer Problem**
 Write program to provide solution for problem having the following detailed specification: Suppose one process, a producer, is generating information that a second process, a consumer, is using. They communicate by a single shared integer variable. The producer generates a random number, write into the shared variable. The consumer reads the data from shared variable and prints it. We want producer and consumer to cooperate in such a manner that data written to shared variable is neither lost (producer overwrites a number which consumer has not read) nor duplicated (consumer reads the same number again).
 
**Multiple Processes Based Solution:** 
Write three programs: Controller Process, Producer Process, and Consumer Process. They’ll communicate using System V IPC: Shared Memory and Semaphore.

_Controller Process_: It will create a shared memory, create required number of semaphores, initialize them properly and will sleep. While it is sleeping, we will run our producer and consumer processes in two separate terminal windows to observe their operation. When it comes out of sleep, it removes the created shared memory and semaphores.

_Producer Process_: It uses the shared memory, and semaphores created by controller process. It does its job as specified above.

_Consumer Process_: It uses the shared memory, and semaphores created by controller process. It does its job as specified above.

### **Multiple Thread Based Solution:**
Write a program that provides the solution using two **POSIX threads**: **producer** and **consumer**. Main thread (function) will create and initialize POSIX mutex and semaphore. Then it will create two posix thread: producer and consumer and waits for the two threads to terminate. Before terminating, Main thread will remove the mutex and semaphore created by it in the beginning. The program will have two functions: producer function implementing the functionality defined for producer and consumer function implementing the functionality defined for consumer. Producer thread executes producer function and consumer thread executed consumer function.

**Bounded Buffer** - We have a producer process and a consumer process that are cooperating through a shared buffer. Buffer has limited space of length N. The buffer temporarily stores the values generated by producer until removed by the consumer. Solution to this problem should satisfy the following conditions:

    a)no “items” are read from an empty buffer;
    b)data items are read only once;
    c)data items are not overwritten before they are read;
    d)items are consumed in the order they are produced; and
    e)all items produced are eventually consumed.
Verify that your solution satisfy the properties of Mutual Exclusion, Absence of Deadlock, Absence of Unnecessary Delay and Eventual Entry. Use binary semaphores as synchronisation primitives.
Devise a solution to the above bounded buffer problem using multiple Producer–multiple Consumer. Use general semaphores as synchronisation primitives.

### **Assignment 3**

Develop a C/C++/Java/Python/R program to find **Minimal Average Latency** for a given reservation table. Reservation Table is available in a text file. The name of text file is to be read as a command line argument. It can be of variable size. Number of stages of the pipelined system is decided by the number of rows in the reservation table and number of clock cycles needed for one instance of task execution is decided by number of columns of the reservation table. A row will have an entry as x if some stage is being used in some clock cycle and - if that stage is not being used. The different entries of x and - in a row are separated by , 
Your program will display following on different lines as output:
1) Forbidden Latencies:
2) Permissible Latencies:
3) Collision Vector:
4) Textual representation of collision state diagram: (Hint: Say starting node is N1. It has 3 immediate child nodes N2, N3, N4.Further, N2 has 2 nodes namely N5, N6. You will show it as: 
 N1->N2, N3, N4
 N2->N5, N6 ..........................and so on.
5) List of Simple Cycles with node numbers and latencies:
6) List of Greedy Cycles with node numbers and latencies:
7) Minimal Average Latency: 

### **Assignment 5**
Parallel Algorithm Implementation Using Threads
Use pthreads with C/C++  or Threads in Java or Threads in Python

* Given an array of integers, write a parallel program to find out the sum of the array. Assume the entire array is stored initially in one location.

* Write a program to perform a Parallel Matrix Multiplication. The goal is to multiply an MxN matrix called A by an NxP matrix called B and store the result into the MxP matrix called C. 

### Assignment 6****

Implement Odd Even Transposition Sort 
Using Multiple Processes
Using Multiple Thread 


### **Assignment 7**
* Parallel Programming through Message Passing Interface (MPI) using C

Installing MPICH2 on a Single Machine : http://mpitutorial.com/tutorials/installing-mpich2/ 
 
An Introduction to the MPI using C : http://condor.cc.ku.edu/~grobe/docs/intro-MPI-C.shtml

* Given an array of integers, write a parallel program using MPI to find out the sum of the array. Assume the entire array is stored initially in one location.


### OpenMP

* Write programs making use of OpenMP directives (to have parallel processing) that

    a) compute the sum of all the elements in an array A (in C/C++)

    b) finds the largest number in an array A. 



### **Batchers Sort**

