//
// Created by dongl on 2025/01/23.
//

#include <sys/user.h>
#include <stdio.h>
#include <assert.h>

RTEMS_APPLICATION_USTACKLEN__SET0(0xA00000)
RTEMS_APPLICATION_UHEAPLEN__SET1(0x3200000)
RTEMS_APPLICATION_USTACKSZ__SET2(0x200000)
RTEMS_APPLICATION_UCACHESZ__SET3(0x100000)
RTEMS_APPLICATION_PRIORITY__SET4(150)

int main (int argc, char *argv[])
{
    printf("\033[31mrtems image1 TEST BEGIN pid[%d] "
           "argv[0] %p : %s\033[0m\n",
           getpid(), argv[0], argv[0]);

    assert(NULL);

    printf("\033[31mrtems image1 TEST END\033[0m\n");
    return 0;
}