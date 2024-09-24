# Upcalls and Signals

This is a small programming assignment about upcalls or signals. The program has a parent and a child process. Both processes have a global integer variable `proc_state`, initially set to 0. Our task is to modify the program so that the parent can change the value of the `proc_state` of the child.

### Main Components of the Program:

- **Global variable `proc_state`:** Notice that both the parent and child processes have their own `proc_state` variable.
  
- **Signal handler (`sig_handler`):** This is the function that will increment the `proc_state` when a signal is received.
  
- **Parent process:** The parent has a for-loop where each pass will cause the `proc_state` of the child to increment by 1. This is done by sending a `SIGUSR1` signal to the signal handler of the child. Each pass is delayed by 1 second, and it displays the value and address of the parent’s `proc_state`.

- **Child process:** The child should register its signal handler with the kernel using the system call `signal()`. The child has a for-loop where each pass is delayed by 1 second and displays the value and address of its `proc_state`. The `proc_state` of the child will increment as it receives `SIGUSR1` signals from the parent.

### Problem:
The current program doesn’t work. It neither sends signals from the parent nor receives them in the child. Modify the program so that it works.

### Expected Output:

When the program is working correctly, the output should look something like this (though it may differ slightly since the processes are running in parallel):

```plaintext
Child begins: address of proc_state = 404068
Child: proc_state = 0, address = 0x404068
Child: proc_state = 1, address = 0x404068
Child: proc_state = 2, address = 0x404068
Child: proc_state = 3, address = 0x404068
Child: proc_state = 3, address = 0x404068
Child: proc_state = 3, address = 0x404068
Child: bye bye
Parent begins: address of proc_state = 0x404068
Parent: proc_state = 0, address = 0x404068
Parent: proc_state = 0, address = 0x404068
Parent: proc_state = 0, address = 0x404068
Parent: proc_state = 0, address = 0x404068
Parent: bye bye
```

# Tips and Helpful Information for Signal Handling in C

When working with signals and processes in C, especially in parent-child relationships, there are several key points and best practices to keep in mind. These tips can help improve the reliability and clarity of your program.

## 1. **Signal Handling Setup**
   - Use the `signal()` function to register a signal handler for a specific signal (e.g., `SIGUSR1`).
   - The signal handler should be simple and efficient because it executes asynchronously. Avoid calling non-reentrant functions like `printf()` inside the handler. Instead, use simple, signal-safe operations like modifying variables.
   - Example for setting up the signal handler:
     
     ```c
     signal(SIGUSR1, sig_handler);  // Register sig_handler for SIGUSR1
     ```

## 2. **Parent-Child Process Communication**
   - **Parent Process**: Use `kill()` to send signals from the parent to the child. The `kill()` function requires the process ID of the child and the signal you want to send.
     
     ```c
     kill(child_pid, SIGUSR1);  // Send SIGUSR1 signal to child
     ```
   - **Child Process**: The child needs to listen for signals and modify its state upon receiving them using the signal handler.

## 3. **Global Variables and Process Address Space**
   - Both the parent and child processes have their own copies of global variables. Even though they share the same addresses (since they start from the same memory layout after `fork()`), the variables are **not** shared between processes. They exist in separate memory spaces. This explains why `proc_state` has the same address in both parent and child but different values.

## 4. **Error Handling**
   - Always check for errors after system calls like `fork()` and `signal()`. If `fork()` fails, it returns `-1`, and you should handle this case properly.
   - Use `perror()` to output error messages for system call failures.
     
     ```c
     if (fork() == -1) {
         perror("fork failed");
         exit(EXIT_FAILURE);  // Exit if fork fails
     }
     ```

## 5. **Process Termination**
   - Ensure that the parent waits for the child to finish by calling `wait(NULL)`. This prevents zombie processes and ensures proper process cleanup.
     
     ```c
     wait(NULL);  // Wait for child to finish
