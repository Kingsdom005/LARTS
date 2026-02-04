#include <sys/user.h>
#include <sys/prctl.h>

RTEMS_APPLICATION_USTACKLEN__SET0(0xA00000)
RTEMS_APPLICATION_UHEAPLEN__SET1(0x3200000)
RTEMS_APPLICATION_USTACKSZ__SET2(0x200000)
RTEMS_APPLICATION_UCACHESZ__SET3(0x100000)
RTEMS_APPLICATION_PRIORITY__SET4(150)

#include "tinyexpr.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    const char *c = "sqrt(5^2+7^2+11^2+(8-2)^2)";
    double r = te_interp(c, 0);
    printf("The expression:\n\t%s\nevaluates to:\n\t%f\n", c, r);
    return 0;
}