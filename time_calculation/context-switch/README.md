# Context Switch in LARTS

## 📋 Quick Start

In this context switch test case, we set a 5000-tick delay on the main thread to facilitate observation of the results.

Just run the following commands:

```shell
cd ./image
qemu-system-arm -serial null -serial mon:stdio -M xilinx-zynq-a9 -m 1024M -gdb tcp::7777 -nographic -no-reboot -append "--video=off --console=/dev/com1" -kernel ./LARTS_kernel.exe
```
