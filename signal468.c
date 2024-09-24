//////////////////////////////////////////////////////////////////////////////////////
///         University of Hawaii - College of Engineering
///                       Amanda Eckardt
///                     ECE 468 Fall 2024
///
///                     Upcalls and Signals
///             
///                ...Y0u h4v3 4 b4d c0nN3ct10n
///
///@see https://www.gnu.org/software/libc/manual/html_node/Kill-Example.html
///@see https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
///@see https://web.stanford.edu/class/cs110/summer-2021/lecture-notes/lecture-07/#:~:
///     text=fork()%20returns%200%20to,positive%20number)%20to%20the%20parent.
/////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>     // For EXIT_SUCCESS and EXIT_FAILURE
#include <stdio.h>      // For printf()
#include <sys/types.h>  // For data types like pid_t
#include <sys/wait.h>   // For wait()
#include <unistd.h>     // For fork(), getpid(), and sleep()
#include <signal.h>     // For signal handling (SIGUSR1)

// Global variable proc_state, shared between parent and child processes
int proc_state = 0;

// MODIFIED: Signal handler function that increments proc_state when SIGUSR1 is received
void sig_handler(int signo)
{
    if (signo == SIGUSR1) {  // Check if the signal is SIGUSR1
        proc_state++;        // Increment proc_state
    }
}

int main() {

    pid_t pid = fork();     // Creating a child process using fork()

    // If fork() == 0, we are in the child process
    if (pid == 0) {   
        printf("Child begins: address of proc_state = %p\n", (void*)&proc_state); 

     
        pid_t child_id = getpid();    // Get the child process ID

        
        signal(SIGUSR1, sig_handler); // Register sig_handler to handle [RECEIVE] the SIGUSR1 signal in the child process

        // Loop for the child process to print proc_state and wait for signals
        for (int i = 0; i < 6; i++) {
            printf("Child: proc_state = %d, address = %p\n", proc_state, (void*)&proc_state); 
            sleep(1);  
        }

        
        printf("Child: bye bye\n");
        exit(EXIT_SUCCESS);  // Exit the child process

    } else if (pid > 0) { // if pid > 0, this is the parent process 

        printf("Parent begins: address of proc_state = %p\n", (void*)&proc_state); 

        //Parent process sends signals to the child every second
        for (int i = 0; i < 3; i++) {
            sleep(1);  // Sleep for 1 second before sending the signal
            printf("Parent: proc_state = %d, address = %p\n", proc_state, (void*)&proc_state);             
            // Modified: Send SIGUSR1 signal to the child process
            kill(pid, SIGUSR1);  // `pid` is the child process ID
        }

    
        sleep(2);    // Wait for the child process to finish
        wait(NULL);  // Wait for child process to terminate

        // Final Parent Process: state and goodbye message 
        printf("Parent: proc_state = %d, address = %p\n", proc_state, (void*)&proc_state);         printf("Parent: bye bye\n");

    } else {
        
         // Modified: Error Handling
        perror("fork");      // Error occurred during fork()    
        exit(EXIT_FAILURE);  // Exit the program with failure status
    }

    return EXIT_SUCCESS;  // Program exits successfully
}
