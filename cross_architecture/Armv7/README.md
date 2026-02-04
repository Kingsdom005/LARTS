# LARTS: Language Abstractions for Real-Time and Secure Systems

## 🛠️ Framework 

The framework is built on a layered architecture:

<img src="https://github.com/Kingsdom005/LARTS/blob/main/overview.png" alt="overview" style="zoom:90%;" />

## 📋 ARMv7 Quick Start

In armv7, we set a 5000-tick delay on the main thread to facilitate observation of the results.

Just run the scripts:

```shell
cd [project_root]
# ARMv7 Test
# change [./Armv7/LARTS_kernel_armv7_test0.exe] to test binary
qemu-system-arm -serial null -serial mon:stdio -M xilinx-zynq-a9 -m 1024M -gdb tcp::7777 -nographic -no-reboot -append "--video=off --console=/dev/com1" -kernel ./Armv7/LARTS_kernel_armv7_test0.exe
```

## 📊 Testcase Description

Testcase description:

* **Test0** : `LARTS_kernel_armv7_test0.exe`  <=> `[project_rootdir]\testsuites\samples\rtems_application\image0.c`
* **Test1** : `LARTS_kernel_armv7_test1.exe`  <=> `[project_rootdir]\testsuites\samples\rtems_application\image1.c`
* **Test2** : `LARTS_kernel_armv7_test2.exe`  <=> `[project_rootdir]\testsuites\samples\rtems_application\thread-test\main.c`
