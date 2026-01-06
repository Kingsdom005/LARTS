//
// Created by dongl on 25-2-27.
//


#include <sys/user.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <mqueue.h>
//#define _GNU_SOURCE
//#include <sched.h>

RTEMS_APPLICATION_USTACKLEN__SET0(0xA00000)
RTEMS_APPLICATION_UHEAPLEN__SET1(0x3200000)
RTEMS_APPLICATION_USTACKSZ__SET2(0x200000)
RTEMS_APPLICATION_UCACHESZ__SET3(0x100000)
RTEMS_APPLICATION_PRIORITY__SET4(150)

int main (int argc, char *argv[])
{
    printf("\033[31mrtems image0 TEST BEGIN did[%d] "
           "argv[0] %p : %s\033[0m\n",
           getpid(), argv[0], argv[0]);

    printf("!!!!!! image0-0 !!!!!!\n");
    //sleep(1);
    printf("!!!!!! image0-1 !!!!!!\n");
    int fd = open("/newfile.txt",
                  O_WRONLY | O_CREAT | O_EXCL,
                  0644);
    if (fd == -1) { perror("open failed\n");}
    printf("!!!!!! image0-2 !!!!!!\n");
    FILE *fp = fdopen(fd, "r");
    if (!fp) { perror("fdopen failed"); }
    printf("!!!!!! image0-3 !!!!!!\n");
//    close(fd);
    printf("!!!!!! image0-4 !!!!!!\n");
    fclose(fp);
    printf("!!!!!! image0-5 !!!!!!\n");

    printf("\033[31mrtems image0 TEST END\033[0m\n");
    return 0;
}