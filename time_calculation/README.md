# LARTS: Language Abstractions for Real-Time and Secure Systems

## 🛠️ Framework

The LARTS programming model is outlined as shown in the figure below.

<img src="https://github.com/Kingsdom005/LARTS/blob/main/overview.png" alt="overview" style="zoom:90%;" />

---

## 📊 Testcase Description

This use case provides test cases for context switching and system call latency. The remaining content follows the same testing methodology. Since testing requires modifications to currently confidential kernel source code, we will present an implementation example of the kernel to aid reader comprehension.

The implementation of the system call dispatch is shown in the following code.

```
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

// This assembly code needs to be encapsulated as a macro in the BSP file.
void rtems_syscall( void )
{
    struct _Thread_Control* executing = _Thread_Get_executing();
    rtems_enhance_task_ptr enhance_task = executing->enhance;

    if(
            enhance_task->thread_uregs.core_registers->number > 0
            && enhance_task->thread_uregs.core_registers->number < NELEM(rtems_syscalls)
            && rtems_syscalls[enhance_task->thread_uregs.core_registers->number])
    {
        // printk("Current syscallNumber: %d\n", enhance_task->thread_uregs.core_registers->number);
        //if(enhance_task->thread_uregs.core_registers->number == 20) {
        //    PERF_TIME_START(syscall_time_perf);
        //}
        // Trigger system call
        enhance_task->thread_uregs.core_registers->result = rtems_syscalls[enhance_task->thread_uregs.core_registers->number]( &enhance_task->thread_uregs );
        // if (enhance_task->thread_uregs.core_registers->number == 20) {
        //     PERF_END(syscall_perf);
        //     PERF_PRINT(syscall_perf, "Syscall cost");
        // }
    } else {
        printk("Invalid system call number[%lu]: The corresponding system call was not found.\n", enhance_task->thread_uregs.core_registers->number);
    }
}
```

We will test the execution time of different categories of system calls based on actual user usage scenarios. These can be distinguished by the system call number stored in `enhance_task->thread_uregs.core_registers->number`. Within the LARTS system, we have developed a timer module to measure the duration of various system behaviors. Here, `PERF_TIME_START` and `PERF_END` are used to record time, while `PERF_PRINT` is employed for output (e.g., printing a message after every nth system call invocation).
