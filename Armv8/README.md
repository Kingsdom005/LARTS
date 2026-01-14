# LARTS: Language Abstractions for Real-Time and Secure Systems

## 🛠️ Framework 

The framework is built on a layered architecture:

<img src="https://github.com/Kingsdom005/LARTS/blob/main/overview.png" alt="overview" style="zoom:90%;" />

## 📋 ARMv8 Quick Start

In armv8, we have the main thread enter a permanent sleep state after creating threads for other domains, facilitating observation of the results.

Just run the scripts:

```shell
cd [project_root]
# ARMv8 Test
# change [./Armv8/LARTS_kernel_armv8_test0.exe] to test binary
qemu-system-aarch64 -gdb tcp::7777 -no-reboot -nographic -serial mon:stdio -machine xlnx-versal-virt -m 4096 -kernel ./Armv8/LARTS_kernel_armv8_test0.exe
```

## 📊 Testcase Description

Testcase description:

* **Test0** : `LARTS_kernel_armv8_test0.exe`  <=> `[project_rootdir]\testsuites\samples\rtems_application\image0.c`
* **Test1** : `LARTS_kernel_armv8_test1.exe`  <=> `[project_rootdir]\testsuites\samples\rtems_application\image1.c`
* **Test2** : `LARTS_kernel_armv8_test2.exe`  <=> `[project_rootdir]\testsuites\samples\rtems_application\thread-test\main.c`
