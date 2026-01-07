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

#include <rtems.h>
#include <tmacros.h>
#include <stdio.h>
#include <rtems/tasks.h>

//#include <bsp/arm-cp15-start.h>
//#include <bsp/linker-symbols.h>
#include <rtems/shell.h>

#include <rtems/score/context.h>
#include <rtems/score/cpu.h>
#include <rtems/score/thread.h>

#include <rtems/score/wkspace.h>
#include <rtems/score/wkspacedata.h>
#include <rtems/score/thread.h>
#include <rtems/score/tls.h>
#include <rtems/score/heapimpl.h>

#include <rtems/rtp/space/rtems_space.h>
#include <rtems/score/threadimpl.h>

#include <rtems/rtp/rt_processess.h>
#include <rtl/rtl.h>
#include <rtl/rtl-obj.h>
#include <rtems/rtp/loader/elf_load.h>

uint32_t g_rtems_rtp_test_number = 0;

void* rtems_rtp_test (int argc, void *args[]) {
    printf("+----------    rtems_rtp_test begin[%d]    ----------+\n", g_rtems_rtp_test_number++);

    Thread_Control *executing = _Thread_Get_executing();
    ISR_lock_Context lock_context;

//    _Thread_State_acquire(executing, &lock_context);

    _Context_Save(&executing->User_Registers);  // &executing->User_Registers 为用户上下文 初始位置最原始状态
    printf("*** _Context_Save ***\n");
    _Context_Switch(&executing->Registers, &executing->User_Registers);

//    _Thread_State_release( executing, &lock_context );

    printf("+----------    rtems_rtp_test end[%d]    ----------+\n", g_rtems_rtp_test_number);
    rtems_rtp_exit();
}


/*
 * RTEMS Startup Task
 */
static rtems_task Init(rtems_task_argument ignored) {
    rtems_print_printer_fprintf_putc(&rtems_test_printer);

    CPU_PL model = __Get_Privilege_Mode();
    printf("model: %s\n", CPU_PL_TEXT(model));
    // 划分用户物理地址空间
    __Enable_User_Memory_Space(0x10000000, 0x20000000);
    printf("__Enable_User_Memory_Space end\n");

    rtems_rtp_entry* entry[1];
    entry[0] = (rtems_rtp_entry *) rtems_rtp_test;
    rtems_rtp_load("null", 1, entry);


    sleep(1000);

    rtems_test_exit( 0 );
}

#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY    // 干净的内存?

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
//#define CONFIGURE_EXTRA_TASK_STACKS         (1024*1024*300)
#define CONFIGURE_EXECUTIVE_RAM_SIZE        (1024 * 1024 * 10)
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
#define CONFIGURE_MICROSECONDS_PER_TICK    1000 //10000
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


