# Simple LKM Notes:

First, compile with 'make'

hello.ko file is the kernel module file.

To follow kernel log to see callback function calls:
`$ sudo dmesg -W`

To load kernel module:
`$ sudo insmod hello.ko`

To remove kernel module:
`$ sudo rmmod hello.ko`

There should be changes to the kernel log
