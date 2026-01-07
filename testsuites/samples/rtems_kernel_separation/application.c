//
// Created by dongl on 24-9-30.
//

/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */


//#include <stdio.h>
#include <rtems.h>
#include <tmacros.h>
#include <rtems/shell.h>
#include <rtems/rtems/tasks.h>

#include <stdio.h>
#include <rtems/bspIo.h>

#include <rtems/score/context.h>
#include <rtems/score/cpu.h>
#include <rtems/score/thread.h>
#include <rtems/score/protectedheap.h>

#include <rtems/score/wkspacedata.h>
#include <rtems/score/threadimpl.h>
#include <rtems/rtp/rt_process.h>
#include <rtems/rtp/score/percpu.h>
#include <rtems/rtp/rtems/rtps.h>
#include <rtems/rtp/kernel/rtems_context.h>



void test1() {}
void test2() {}
void test3() {
    rtems_rtp_syscall(0x400, 0);
}
//
void* rtems_rtp_test (int argc, void *args[]) {
    test1();
    test2();
    test3();

    Thread_Control *executing = _USER_Thread_Get_executing();

    user_printk("+----------    rtems_rtp_test begin[%d]    ----------+\n", executing->Object.id);

    char* print_hello = _Protected_heap_Allocate(&executing->rtems_rtp->user_heap, 100);
    sprintf(print_hello, "* thread[%d], hello world for %p address",
            executing->Object.id, print_hello);

    for (int i = 0; i < 10; i++) {
        user_printk("%s\n", print_hello);
        rtems_rtp_wake_after(1000);
    }

    user_printk("+----------    rtems_rtp_test end[%d]    ----------+\n", executing->Object.id);

    rtems_rtp_exit();
}

void* rtems_rtp_test1 (int argc, void *args[]) {
    test1();
    test2();
    test3();

    Thread_Control *executing = _USER_Thread_Get_executing();

    user_printk("+----------    rtems_rtp_test1 begin[%d]    ----------+\n", executing->Object.id);

    char* print_hello = _Protected_heap_Allocate(&executing->rtems_rtp->user_heap, 100);
    sprintf(print_hello, "** thread[%d], hello world for %p address",
            executing->Object.id, print_hello);

    for (int i = 0; i < 10; i++) {
        user_printk("%s\n", print_hello);
        rtems_rtp_wake_after(1000);
    }

    user_printk("+----------    rtems_rtp_test1 end[%d]    ----------+\n", executing->Object.id);

    rtems_rtp_exit();
}

void* rtems_rtp_test2 (int argc, void *args[]) {
    Thread_Control *executing = _USER_Thread_Get_executing();
    user_printk("+----------    rtems_rtp_test2 begin[%d]    ----------+\n", executing->Object.id);

    // bsp_section_text_begin                          0x508c80
    user_printk("bsp_section_text_begin: %p\n", bsp_section_text_begin);
    user_printk("bsp_section_text_begin: 0x%lx\n", bsp_section_text_begin);
    char t_buf[10];
    read(bsp_section_text_begin, t_buf,10);
    user_printk("read ok\n");
    memset(t_buf, 0, 10);
    write(bsp_section_text_begin, t_buf, 10);
    user_printk("write ok\n");
    memcpy(bsp_section_text_begin, t_buf, 10);
    user_printk("memcpy ok\n");
    memset(bsp_section_text_begin, 0, 10);
    user_printk("memset ok\n");

    read(bsp_section_nocache_begin, t_buf,10);
    user_printk("read bsp_section_nocache_begin ok\n");
    write(bsp_section_nocache_begin, t_buf,10);
    user_printk("write bsp_section_nocache_begin ok\n");

    printk("user model >> printk ok\n");
    printf("user model >> printf ok\n");

    uint32_t* d = 0x10000000-100;
    d[0] = 1;

    user_printk("+----------    rtems_rtp_test2 end[%d]    ----------+\n", executing->Object.id);
    rtems_rtp_exit();
}

static rtems_task Init(rtems_task_argument ignored) {

    rtems_print_printer_fprintf_putc(&rtems_test_printer);

    // 上下文属性在内存地址中的偏移量计算的初始化
    global_context_information_initialization();

    printf("Offset of Kernel_Registers: %u\n", kernel_context_offset);
    printf("Offset of sp: %u\n", kernel_context_sp_offset);

    Thread_Control     *executing;
    executing = _Thread_Get_executing();
    printk("Init task id %d\n", executing->Object.id);

    _CPU_PL model = __Get_Privilege_Mode();
    printk("model: %s\n", _CPU_PL_TEXT(model));

    // 划分用户物理地址空间
    __Enable_User_Memory_Space(0x10000000, 0x20000000);

    rtems_rtp_entry* entry[1] = {{ (rtems_rtp_entry *) rtems_rtp_test}};
    void** void_entry = (void**) entry;
    rtems_rtp_entry* entry1[1] = {{ (rtems_rtp_entry *) rtems_rtp_test1}};
    void** void_entry1 = (void**) entry1;
    rtems_rtp_entry* entry2[1] = {{ (rtems_rtp_entry *) rtems_rtp_test2}};
    void** void_entry2 = (void**) entry2;
    rtems_rtp_load("rtp1", 1, void_entry);
    rtems_rtp_load("rtp2", 1, void_entry);
    rtems_rtp_load("rtp3", 1, void_entry1);
    rtems_rtp_load("rtp4", 1, void_entry1);
    rtems_rtp_load("rtp5", 1, void_entry2);

    rtems_task_wake_after(20000);

    rtems_test_exit( 0 );
}


#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS  100
#define CONFIGURE_BDBUF_MAX_WRITE_BLOCKS       1000
#define CONFIGURE_SWAPOUT_TASK_PRIORITY        15
#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_DOSFS

#define CONFIGURE_FILESYSTEM_IMFS


/* 堆栈大小设置 */
#define CONFIGURE_EXECUTIVE_RAM_SIZE            (1024 * 1024 * 10)
#define CONFIGURE_MAXIMUM_SEMAPHORES            200     // 最大信号量数目
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES        200     // 队列
#define CONFIGURE_MAXIMUM_TASKS                 200     // 并行任务最大数23+

#define CONFIGURE_MP_MAXIMUM_GLOBAL_OBJECTS     200     // 全局变量数目


#define CONFIGURE_INIT_TASK_STACK_SIZE      (1024 * 500)
#define CONFIGURE_INIT_TASK_PRIORITY        50
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | \
                                           RTEMS_NO_TIMESLICE | \
                                           RTEMS_NO_ASR | \
                                           RTEMS_INTERRUPT_LEVEL(0))

/*
 * XXX: these values are higher than needed...
 */
#define CONFIGURE_MICROSECONDS_PER_TICK    1000 // 10000
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 100  // 100


//#define CONFIGURE_MALLOC_STATISTICS

#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS


#define RTEMS_NETWORKING 1
#define CONFIGURE_SHELL_COMMANDS_ALL_NETWORKING

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS
#define CONFIGURE_SHELL_MOUNT_RFS
#define CONFIGURE_SHELL_DEBUGRFS


#include <rtems/shellconfig.h>


#define CONFIGURE_INIT
#include <rtems/confdefs.h>