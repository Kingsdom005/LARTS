//
// Created by dongl on 24-9-30.
//



#include <rtems.h>
#include <assert.h>
#include <tmacros.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <rtems/libcsupport.h>
#include <rtems/fsmount.h>
#include <rtems/blkdev.h>

#include <rtems/dosfs.h>
#include <rtems/libio.h>
#include <rtems/ramdisk.h>
#include <bsp.h>
#include <rtems/io.h>
#include <rtems/libio.h>
#include <rtems/libcsupport.h>
#include <rtems/userenv.h>

#include "image.h"
#include "image_bin_le_singlebyte.h"
#include "image_bin_le_multibyte.h"
#include "files.h"





#define BLOCK_NUM 47
#define BLOCK_SIZE 512

rtems_ramdisk_config rtems_ramdisk_configuration [] = {
        { .block_size = BLOCK_SIZE, .block_num = BLOCK_NUM },
        { .block_size = BLOCK_SIZE, .block_num = BLOCK_NUM, .location = &IMAGE_BIN_LE_SINGLEBYTE[0] },
        { .block_size = BLOCK_SIZE, .block_num = BLOCK_NUM, .location = &IMAGE_BIN_LE_MULTIBYTE[0] },
        { .block_size = BLOCK_SIZE, .block_num = sizeof( image_bin ) / BLOCK_SIZE, .location = image_bin }
};

size_t rtems_ramdisk_configuration_size = RTEMS_ARRAY_SIZE(rtems_ramdisk_configuration);


fstab_t fs_table[] = {
        {
                "/dev/sd0","/mnt/sd0", "dosfs",
                RTEMS_FILESYSTEM_READ_WRITE,
                FSMOUNT_MNT_OK | FSMOUNT_MNTPNT_CRTERR | FSMOUNT_MNT_FAILED,
                0
        }
};

#define RAMDISK_PATH "/dev/console"

#define MOUNT_PATH "/mnt"

/*
 * RTEMS Startup Task
 */
static rtems_task Init(rtems_task_argument ignored) {
    rtems_print_printer_fprintf_putc(&rtems_test_printer);

    rtems_status_code status;


    DIR *dir = opendir("/dev");
    assert(dir);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

//    printf("pci_bus_count() :%d\n", pci_bus_count());


    status = msdos_format(RAMDISK_PATH, NULL);
    printf("status: %s\n", rtems_status_text(status));
    rtems_test_assert(status == 0);

    status = mount_and_make_target_path(
            RAMDISK_PATH,
            MOUNT_PATH,
            RTEMS_FILESYSTEM_TYPE_DOSFS,
            RTEMS_FILESYSTEM_READ_WRITE,
            NULL
    );
    printf("status: %s\n", rtems_status_text(status));
    rtems_test_assert(status == 0);


    // 挂载
    printf("--- unused dynamic memory: %lu MB ---\n",
           (unsigned long) malloc_free_space() / 1024 / 1024);

    status = rtems_fsmount(fs_table, sizeof(fs_table)/sizeof(fs_table[0]), NULL);
    printf("result = %s\n", rtems_status_text(status));

    printf("--- unused dynamic memory: %lu MB ---\n",
           (unsigned long) malloc_free_space() / 1024 / 1024);

#if 0
    FILE* fp = fopen("/test.txt", "w+");
    assert(fp);
    // 写入字符串
    fprintf(fp, "Hello, World!\n");
    fclose(fp);
    //
    fp = fopen("/test.txt", "r+");
    assert(fp == NULL);
    char buff[100];
    memset(buff, 0, 100);
    fgets(buff, 100, fp);
    printf("I read: %s\n", buff);
#else
    FILE* fp = fopen("/test.txt", "r");
    printf("fp %p\n", fp);
    assert(fp);
    char buff[100];
    memset(buff, 0, 100);
    fgets(buff, 100, fp);
    printf("I read: %s\n", buff);
#endif

    fclose(fp);
    rtems_test_exit( 0 );
}




#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY    // 干净的内存?

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_APPLICATION_EXTRA_DRIVERS RAMDISK_DRIVER_TABLE_ENTRY
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS  100
#define CONFIGURE_BDBUF_MAX_WRITE_BLOCKS       1000
#define CONFIGURE_SWAPOUT_TASK_PRIORITY        15
#define CONFIGURE_FILESYSTEM_DOSFS


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


