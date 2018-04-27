# Introduction

This is a preemptive operating system called SBUnix developed as a part of operating systems course in Stony Brook University.
I have implemented paging, free list, kmalloc, interrupts, scheduling, context switching, terminal and syscalls. SBUnix also contains binaries like echo, sleep, cat, ls, kill and ps.

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

## Commands supported

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
    But if you try to kill process with pid 1, SBUnix will ask you if you
    want to shut down the system as this is the special sbush process
    which runs as soon as you start the SBUnix.
  * Shutdown SBUnix
      ```
      shutdown
      ```