# Introduction

This is a preemptive multi-tasking operating system called SBUnix
developed as a part of operating systems course in Stony Brook
University.
I have implemented paging, free list, kmalloc, interrupts, scheduling,
task switching, terminal, syscalls, kernel-threads, VFS and tarfs file
access. SBUnix also contains binaries like echo, sleep, cat, ls, kill
and ps. 


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
    target remote localhost:9961
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
    cd /rootfs/
    ```
    Note that you have to enter absolute path right from ```/rootfs/```.
    Also, note that every directory ends with a '/'. So if you enter
    ```cd /rootfs``` instead of ```cd /rootfs/```, it won't work.
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
  * Kill a process
      ```
      kill -9 <pid>
      e.g.
      kill -9 1
      ```
      If you kill a process with pid > 1, the process will be killed.<br/>
      But if you try to kill process with pid 1, SBUnix will ask you if
      you want to shut down the system as this is the special sbush
      process which runs as soon as you start the SBUnix.
  * Print the contents of the file on the terminal
    ```
    cat <Path/To/The/File>
    e.g.
    cat /rootfs/etc/test.txt
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
  * Sleep for 's' seconds
    ```
    sleep <Time in seconds>
    e.g.
    sleep 5
    ```
  * Shutdown SBUnix
      ```
      shutdown
      ```

# References

  * http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
  * https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
  * https://wiki.osdev.org/USTAR
  * http://www.osdever.net/bkerndev/Docs/intro.htm
  * http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
  * https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
  * https://notes.shichao.io/lkd/ch15/
