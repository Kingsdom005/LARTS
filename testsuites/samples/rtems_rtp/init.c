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

#include <rtems/score/cpu.h>
#include <rtems/score/thread.h>

#include <rtems/score/wkspace.h>
#include <rtems/score/wkspacedata.h>
#include <rtems/score/thread.h>
#include <rtems/score/tls.h>
#include <rtems/score/heapimpl.h>

#include <rtems/rtp/space/rtems_space.h>
#include <rtems/score/threadimpl.h>

#include <rtems/rtp/rt_process.h>
#include <rtl/rtl.h>
#include <rtl/rtl-obj.h>
#include <rtems/rtp/loader/elf_load.h>



static inline void cat_section_memory() {
    printf("---------------------------------------------------------------\n");
//    printf("bsp_user_length                                         %u MB, %u KB, %u B\n", (uint32_t) bsp_user_length / 1024 / 1024,     (uint32_t) bsp_user_length / 1024,     (uint32_t) bsp_user_length);
    printf("bsp_kernel_length                                       %u MB, %u KB, %u B\n", (uint32_t) bsp_kernel_length / 1024 / 1024, (uint32_t) bsp_kernel_length / 1024,     (uint32_t) bsp_kernel_length);
//    printf("bsp_user_ram_length                                     %u MB, %u KB, %u B\n", (uint32_t) bsp_user_ram_length / 1024 / 1024,       (uint32_t) bsp_user_ram_length / 1024,  (uint32_t) bsp_user_ram_length);
    printf("bsp_kernel_ram_length                                   %u MB, %u KB, %u B\n", (uint32_t) bsp_ram_length / 1024 / 1024,       (uint32_t) bsp_ram_length / 1024,       (uint32_t) bsp_ram_length);
    printf("---------------------------------------------------------------\n");
    printf("bsp_ram_address_begin                              0x%lx\n", bsp_ram_address_begin);
    printf("bsp_ram_address_end                                0x%lx\n", bsp_ram_address_end);
//    printf("bsp_user_ram_address_begin                         0x%lx\n", bsp_user_ram_address_begin);
//    printf("bsp_user_ram_address_end                           0x%lx\n", bsp_user_ram_address_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_stack_fiq_size                              %u\n", bsp_stack_fiq_size);
    printf("bsp_stack_abt_size                              %u\n", bsp_stack_abt_size);
    printf("bsp_stack_und_size                              %u\n", bsp_stack_und_size);
    printf("bsp_stack_hyp_size                              %u\n", bsp_stack_hyp_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_start_begin                         0x%lx\n", bsp_section_start_begin);
    printf("bsp_section_start_end                           0x%lx\n", bsp_section_start_end);
    printf("bsp_section_start_size                          %d\n", bsp_section_start_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_vector_begin                        0x%lx\n", bsp_section_vector_begin);
    printf("bsp_section_vector_end                          0x%lx\n", bsp_section_vector_end);
    printf("bsp_section_vector_size                         %d\n", bsp_section_vector_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_text_begin                          0x%lx\n", (uint32_t) bsp_section_text_begin);
    printf("bsp_section_text_end                            0x%lx\n", (uint32_t) bsp_section_text_end);
    printf("bsp_section_text_size                           %zu\n", (size_t) bsp_section_text_size);
    printf("bsp_section_text_load_begin                     0x%lx\n", (uint32_t) bsp_section_text_load_begin);
    printf("bsp_section_text_load_end                       0x%lx\n", (uint32_t) bsp_section_text_load_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_rodata_begin                        0x%lx\n", bsp_section_rodata_begin);
    printf("bsp_section_rodata_end                          0x%lx\n", bsp_section_rodata_end);
    printf("bsp_section_rodata_size                         %u\n", bsp_section_rodata_size);
    printf("bsp_section_rodata_load_begin                   0x%lx\n", bsp_section_rodata_load_begin);
    printf("bsp_section_rodata_load_end                     0x%lx\n", bsp_section_rodata_load_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_data_begin                          0x%lx\n",  bsp_section_data_begin);
    printf("bsp_section_data_end                            0x%lx\n",  bsp_section_data_end);
    printf("bsp_section_data_size                           %zu\n",  (size_t) bsp_section_data_size);
    printf("bsp_section_data_load_begin                     0x%lx\n",  bsp_section_data_load_begin);
    printf("bsp_section_data_load_end                       0x%lx\n",  bsp_section_data_load_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_fast_text_begin                     0x%lx\n", bsp_section_fast_text_begin);
    printf("bsp_section_fast_text_end                       0x%lx\n", bsp_section_fast_text_end);
    printf("bsp_section_fast_text_size                      %u\n", bsp_section_fast_text_size);
    printf("bsp_section_fast_text_load_begin                0x%lx\n", bsp_section_fast_text_load_begin);
    printf("bsp_section_fast_text_load_end                         0x%lx\n", bsp_section_fast_text_load_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_fast_data_begin                            0x%lx\n", bsp_section_fast_data_begin);
    printf("bsp_section_fast_data_end                              0x%lx\n", bsp_section_fast_data_end);
    printf("bsp_section_fast_data_size                             %d\n", bsp_section_fast_data_size);
    printf("bsp_section_fast_data_load_begin                       0x%lx\n", bsp_section_fast_data_load_begin);
    printf("bsp_section_fast_data_load_end                         0x%lx\n", bsp_section_fast_data_load_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_bss_begin                           0x%lx\n", bsp_section_bss_begin);
    printf("bsp_section_bss_end                             0x%lx\n", bsp_section_bss_end);
    printf("bsp_section_bss_size                            %u\n", bsp_section_bss_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_work_begin                          0x%lx\n", bsp_section_work_begin);
    printf("bsp_section_work_end                            0x%lx\n", bsp_section_work_end);
    printf("bsp_section_work_size                           %u\n", bsp_section_work_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_stack_begin                                0x%lx\n", bsp_section_stack_begin);
    printf("bsp_section_stack_end                                  0x%lx\n", bsp_section_stack_end);
    printf("bsp_section_stack_size                                 %u\n", bsp_section_stack_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_nocache_begin                              0x%lx\n", bsp_section_nocache_begin);
    printf("bsp_section_nocache_end                                0x%lx\n", bsp_section_nocache_end);
    printf("bsp_section_nocache_size                               %u\n", bsp_section_nocache_size);
    printf("bsp_section_nocache_load_begin                         0x%lx\n", bsp_section_nocache_load_begin);
    printf("bsp_section_nocache_load_end                           0x%lx\n", bsp_section_nocache_load_end);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_nocachenoload_begin                        0x%lx\n", bsp_section_nocachenoload_begin);
    printf("bsp_section_nocachenoload_end                          0x%lx\n", bsp_section_nocachenoload_end);
    printf("bsp_section_nocachenoload_size                         %u\n", bsp_section_nocachenoload_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_section_nocacheheap_begin                          0x%lx\n", bsp_section_nocacheheap_begin);
    printf("bsp_section_nocacheheap_end                            0x%lx\n", bsp_section_nocacheheap_end);
    printf("bsp_section_nocacheheap_size                           %u\n", bsp_section_nocacheheap_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_vector_table_begin                                 0x%lx\n", bsp_vector_table_begin);
    printf("bsp_vector_table_end                                   0x%lx\n", bsp_vector_table_end);
    printf("bsp_vector_table_size                                  %u\n", bsp_vector_table_size);
    printf("---------------------------------------------------------------\n");
    printf("bsp_start_vector_table_begin                           0x%lx\n", bsp_start_vector_table_begin);
    printf("bsp_start_vector_table_end                             0x%lx\n", bsp_start_vector_table_end);
    printf("bsp_start_vector_table_size                            %u\n", bsp_start_vector_table_size );
    printf("---------------------------------------------------------------\n");
    printf("bsp_translation_table_base                            0x%lx\n", bsp_translation_table_base );
    printf("bsp_translation_table_end                             0x%lx\n", bsp_translation_table_end );
    printf("bsp_user_translation_table_base                       0x%lx\n", bsp_user_translation_table_base );
    printf("bsp_user_translation_table_end                        0x%lx\n", bsp_user_translation_table_end );
    printf("bsp_user_translation_table2_base                      0x%lx\n", bsp_user_translation_table2_base );
    printf("bsp_user_translation_table2_end                       0x%lx\n", bsp_user_translation_table2_end );

    printf("---------------------------------------------------------------\n");

    void* bsp_translation_table_base_address = &bsp_translation_table_base;
    void* bsp_section_start_begin_address    = &bsp_section_start_begin;
    void* bsp_section_vector_begin_address   = &bsp_section_vector_begin;
    printf("bsp_translation_table_base_address                 0x%lx\n", bsp_translation_table_base_address);
    printf("bsp_section_start_begin_address                    0x%lx\n", bsp_section_start_begin_address);
    printf("bsp_section_vector_begin_address                   0x%lx\n", bsp_section_vector_begin_address);

    printf("---------------------------------------------------------------\n");

    printf("rtems_configuration_get_unified_work_area() %d\n", rtems_configuration_get_unified_work_area());
    printf("rtems_configuration_get_maximum_processors() %d\n", rtems_configuration_get_maximum_processors());

    printf("---------------------------------------------------------------\n");
}

static inline void cat_new_malloc() {
    printf("---------------------------------------------------------------\n");
    void* big_malloc_address = malloc(1024 * 1024 * 1024);
    printf("big[1GB]        malloc_address                  0x%lx\n", big_malloc_address);
    void* small_malloc_address = malloc(1024 * 1024 * 260);
    printf("small[260MB]    malloc_address                  0x%lx\n", small_malloc_address);
    void* tiny_malloc_address = malloc(1024 * 1024 * 1);
    printf("tiny[1MB]       malloc_address                  0x%lx\n", tiny_malloc_address);
    printf("---------------------------------------------------------------\n");
}

static inline rtems_id cat_address_is_mapping(const uint32_t address) {
    printf("---------------------------------------------------------------\n");
    rtems_id id;
    rtems_status_code status = rtems_partition_create(
            rtems_build_name('1','1','1','1'),
            address,
            500,
            100,
            RTEMS_LOCAL,
            &id
    );
    printf("rtems_partition_create[begin:0x%lx, end:0x%lx] status: %s\n",
           address,
           address + 500,
           rtems_status_text(status));
    printf("---------------------------------------------------------------\n");
    return id;
}

static inline void asid_heap_malloc_mem() {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 系统堆
    Heap_Control sys_heap[4];
    void* heap_block_p[4];

    // 申请独立地址空间(1)与相应的用户堆
    __Apply_User_Space(1, 1024 * 1024);
    _Heap_Initialize(
            &sys_heap[1],
            1024 * 1024 * 256,
            1024 * 1024,
            8
    );
    heap_block_p[1] = _Heap_Allocate(&sys_heap[1], 1024);
    printf("_Heap_Allocate: %p\n", heap_block_p[1]);
    memset(heap_block_p[1], '1', 3);
    printf("asid %d, sys_heap[1] %p, heap_block_p[1] %p, p1 %s\n", 1, &sys_heap[1], heap_block_p[1], heap_block_p[1]);

    // 申请独立地址空间(2)与相应的用户堆
    __Apply_User_Space(2, 1024 * 1024);
    _Heap_Initialize(
            &sys_heap[2],
            1024 * 1024 * 256,
            1024 * 1024,
            8
    );
    heap_block_p[2] = _Heap_Allocate(&sys_heap[2], 1024);
    printf("_Heap_Allocate: %p\n", heap_block_p[2]);
    memset(heap_block_p[1], '2', 3);
    printf("asid %d, sys_heap[2] %p, heap_block_p[2] %p, p1 %s\n", 2, &sys_heap[2], heap_block_p[2], heap_block_p[2]);

    __Apply_User_Space(1, 1024 * 1024);
    printf("asid %d, sys_heap[1] %p, heap_block_p[1] %p, p1 %s\n", 1, &sys_heap[1], heap_block_p[1], heap_block_p[1]);
    __Apply_User_Space(2, 1024 * 1024);
    printf("asid %d, sys_heap[2] %p, heap_block_p[2] %p, p1 %s\n", 2, &sys_heap[2], heap_block_p[2], heap_block_p[2]);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

static inline void cat_malloc_heap() {
    Memory_Information* memory_block =  _Memory_Get();
    printf("memory_block->count: %d\n", memory_block->count);
    printf("memory_block->areas: 0x%lx\n", memory_block->areas);
    Memory_Area* block_begin = memory_block->areas;
    printf("block_begin->begin: 0x%lx\n", block_begin->begin);
    printf("block_begin->free: 0x%lx\n", block_begin->free);
    printf("block_begin->end: 0x%lx\n", block_begin->end);
}

static inline void cat__Stack_Allocate() {
    void* stack_begin =  _Stack_Allocate(1024);
    printf("stack_begin: %p\n", stack_begin);
}


static rtems_task task_start_test(rtems_task_argument ignored) {
    printf("task_start_test successful by %s!\n", (char *) ignored);
    rtems_task_exit();
}

static rtems_task rtp_start_test(int argc, char* argv[]) {
    uint32_t number = 100;
    printf("task_start_test successful by %s!\n", (char *) argv[0]);
    printf("task_start_test test number == %d!\n", 100);
    rtems_rtp_exit();
}

void Context_SP_Pos_Test (Thread_Control* control) {
//    uint32_t k_sp_pos = _CPU_Context_Get_SP(&control->Registers);
//    uint32_t u_sp_pos = _CPU_Context_Get_SP(&control->User_Registers);
//    printf("k_sp_pos: 0x%lx, u_sp_pos: 0x%lx\n", k_sp_pos, u_sp_pos);
//    sleep(10);
}

/*
 * RTEMS Startup Task
 */
static rtems_task Init(rtems_task_argument ignored) {
    rtems_print_printer_fprintf_putc(&rtems_test_printer);
    cat_section_memory();       // 查看段映射
//    cat_malloc_heap();          // 查看堆映射范围
//    cat_new_malloc();           // 查看申请堆限制
//    cat__Stack_Allocate();      // 查看生成一个栈

    Thread_Control* control = _Thread_Get_executing();

    printf("control->Start.Initial_stack.area: %p, control->Start.allocated_stack: %p\n", control->Start.Initial_stack.area, control->Start.allocated_stack);

    _CPU_PL model = __Get_Privilege_Mode();
    printf("model: %s\n", _CPU_PL_TEXT(model));
    // 划分用户物理地址空间
    __Enable_User_Memory_Space(0x10000000, 0x20000000);
    printf("__Enable_User_Memory_Space end\n");

//    uint32_t k_sp_pos = _CPU_Context_Get_SP(&control->Registers);
//    uint32_t u_sp_pos = _CPU_Context_Get_SP(&control->User_Registers);
//    printf("k_sp_pos: 0x%lx, u_sp_pos: 0x%lx\n", k_sp_pos, u_sp_pos);

    // 3 个进程
    rtems_vmspace_t* vmspace[3] = {NULL};
    for (int i = 1; i < 4; ++i) {
        vmspace[i-1] = __Apply_User_Space(i, 1024 * 1024);
    }

    uint32_t* ptr = 0x10000000;
    __Switch_Context(1);
    memset(ptr, '111', 3);
    __Switch_Context(2);
    memset(ptr, '222', 3);
    __Switch_Context(3);
    memset(ptr, '333', 3);

    __Switch_Context(1);
    printf("asid 1, %s\n", ptr);
    __Switch_Context(2);
    printf("asid 2, %s\n", ptr);
    __Switch_Context(3);
    printf("asid 3, %s\n", ptr);

//    k_sp_pos = _CPU_Context_Get_SP(&control->Registers);
//    u_sp_pos = _CPU_Context_Get_SP(&control->User_Registers);
//    printf("k_sp_pos: 0x%lx, u_sp_pos: 0x%lx\n", k_sp_pos, u_sp_pos);

#if  ARM_MULTILIB_ARCH_V7M
    printf("xxxxxxxxxxxx");
#endif

    char* rtp_args[1][14] = {{"rtems_rtp_create"}};
//    rtems_rtp_load("/test_elf_load.exe", 1, rtp_args);

    sleep(10);

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


