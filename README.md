# Introduction

This is a preemptive multi-tasking operating system called SBUnix
developed as a part of operating systems course in Stony Brook
University.
I have implemented paging, free list, kmalloc, interrupts, scheduling,
context switching, terminal and syscalls. SBUnix also contains binaries
like echo, sleep, cat, ls, kill and ps.

# Functionalities Completed

  * All functions from include/*.h work
  * Virtual memory, user processes
  * tarfs: open, read, close, opendir, readdir, closedir
  * read(stdin), write(stdout), write(stderr)
  * Binaries: echo, sleep, cat, ls, kill -9, ps
  * Able to run my own sbush

    I wasn't able to switch to ring 3 to load binary. The code exists
    but has some bugs in it. Therefore the SBUnix currently runs fine
    in ring 0.

# Running The OS

## To boot the system in QEMU, run:
```
qemu-system-x86_64 -curses -drive id=boot,format=raw,file=$USER.img,if=none -drive id=data,format=raw,file=$USER-data.img,if=none -device ahci,id=ahci -device ide-drive,drive=boot,bus=ahci.0 -device ide-drive,drive=data,bus=ahci.1 -gdb tcp::9999
```
  * Explanation of parameters:
  
    -curses         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; use a text console (omit this to use default SDL/VNC console)<br/>
    -drive ...      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; connect a CD-ROM or hard drive with corresponding image<br/>
    -device ...     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; configure an AHCI controller for the boot and data disks<br/>
    -gdb tcp::9999  &nbsp;&nbsp; listen for "remote" debugging connections on port NNNN<br/>
    -S              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; wait for GDB to connect at startup<br/>
    -no-reboot      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; prevent reboot when OS crashes

  * When using the -curses mode, switch to the qemu> console with ESC-2.

  * To connect a remote debugger to the running qemu VM, from a different window:
    ```
    gdb ./kernel
    ```

    At the (gdb) prompt, connect with:
    ```
    target remote localhost:9999
    ```

# Commands supported

  * Print the present working directory
    ```
    pwd
    ```
  * Print the contents of the current directory
    ```
    ls
    ```
  * Change the current working directory
    ```
    cd <Path>
    e.g.
    cd /rootfs/etc/
    ```
  * Echo
    ```
    echo <Word/Variable>
    e.g.
    echo hello
    echo $HOME
    ```
  * Print the information of the running processes
    ```
    ps
    ```
  * Print the contents of the file on the terminal
    ```
    cat <Path/To/The/File>
    e.g.
    cat /rootfs/etc/test.txt
    ```
  * Sleep for 's' seconds
    ```
    sleep <Time in seconds>
    e.g.
    sleep 5
    ```
  * Add/change value of an environment variable
    ```
    export Name=Value
    e.g.
    export hello=world
    ```
    You can later check the value of the variable using the echo command as follows:
    ```
    echo $hello
    ```
  * Kill a process
    ```
    kill <pid>
    e.g.
    kill 1
    ```
    If you kill a process with pid > 1, the process will be killed.<br/>
    But if you try to kill process with pid 1, SBUnix will ask you if
    you want to shut down the system as this is the special sbush
    process which runs as soon as you start the SBUnix.
  * Shutdown SBUnix
      ```
      shutdown
      ```

# Testing some additional functionalities

## Testing preemptive task switching

  * Uncomment the two lines in the ```switch_to()``` function present
  in the ```/sys/process.c:304``` and ```/sys/process.c:305```.
  * You will see something like this every 1 second:
    ```
    Value in rsp p1 is 0xffffffff8026b590
    value in rsp p2 is 0xffffffff80396200
    Value in rsp p1 is 0xffffffff80396200
    value in rsp p2 is 0xffffffff8026b590
    ```
    <b>Explaination:</b>
  The p1 address is the rsp of the current running process and p2
  address is the rsp of the new process that the scheduler has picked
  to run. <br/>
  Initially, the sbush process is running and its rsp is
  0xffffffff8026b590, lets call it process A. After 1 second, the SBUnix
  received a timer interrupt which called the scheduler and the current
  process got switched to process B with the rsp 0xffffffff80396200.
  The other process B is the original process that initialized the
  SBUnix and created a process - sbush and it does not have anything
  significant to work, hence it yeilds in a continuous ```while()```
  loop. As the B yeilds, the scheduler kicks in and swithcs to A back
  as there are no other processes. So, you see line 3 and 4 where the
  switch happens from process B to process A. This change happens
  immediately, hence you will see the 4 lines printed continuously in a
  interval of 1 second. <br/>
  Note that the addresses printed may vary.<br/>
  I have currently set the preemptive switching frequency to 1 switch
  per second for the ease of testing. You can set it to the frequency
  of the timer interrupt of SBUnix i.e. 100 times per second.