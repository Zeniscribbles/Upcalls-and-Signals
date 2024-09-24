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
