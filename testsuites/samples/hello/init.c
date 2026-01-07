/*
 *  COPYRIGHT (c) 1989-2023.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>

#include <bsp.h> /* for device driver prototypes */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <errno.h>

#include <rtems/io.h>
#include <rtems/libio.h>
#include <rtems/dosfs.h>
#include <rtems/libcsupport.h>

#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <rtems/libio.h>
#include <rtems/blkdev.h>
#include <rtems/dosfs.h>

#define rtems_test_assert assert

#include "nvme-bsp.h"
#include "fs-test.h"

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "RTEMS NVMe TEST";

rtems_task Init(
    rtems_task_argument ignored)
{
  printf("RTEMS NVMe Hello World Now\n");

  // unit test using bare metal NVMe SSD driver
  // nvme_self_test();

  // test NVMe SSD
  fs_test("/dev/nvme0", "/mnt", "/mnt/test-file.txt");
  
  for (;;);

  exit( 0 );
}

#include "nvmedisk.h"
rtems_nvmedisk_config rtems_nvmedisk_configuration[] = {
    { .block_size = 512, .block_num = 1024 * 1024 }};

size_t rtems_nvmedisk_configuration_size = 1;
#define CONFIGURE_APPLICATION_EXTRA_DRIVERS NVMEDISK_DRIVER_TABLE_ENTRY

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 100

#define CONFIGURE_FILESYSTEM_DOSFS

#define CONFIGURE_MAXIMUM_TASKS 100

#define CONFIGURE_EXTRA_TASK_STACKS (8 * 1024)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
