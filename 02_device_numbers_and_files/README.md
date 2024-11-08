# Device numbers and Files

`ls /dev` lists all devices on the system


Results of `ls -al /dev/tty*`:
```
crw--w---- 1 root tty  4,  5 Nov  6 16:31 /dev/tty5
                       ^   ^ Minor device number
                       ^ Major device number
```

Find all major device numbers and what driver they are assigned `cat  /proc/devices`\
Two device files: character device, and block device.\
Character device files you can read and write to and from like a regular device

## Running code to create device number
1. Compile the code
2. Load the module `sudo insmod device_number.ko`
3. See if device number exists ` cat /proc/devices | grep my_device_number

## Making device file
1. `sudo mknod /dev/my_device c 30 0` to create major device 30, minor device 0
2. `sudo chmod 666 /dev/my_device` To allow proper permissions
3. Run test binary
