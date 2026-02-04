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
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    if (argc < 1) {
        printf("Usage: example2 \"expression\"\n");
        return 0;
    }

    const char *expression = argv[0];
    printf("Evaluating:\n\t%s\n", expression);

    /* This shows an example where the variables
        * x and y are bound at eval-time. */
    double x, y;
    te_variable vars[] = {{"x", &x}, {"y", &y}};

    /* This will compile the expression and check for errors. */
    int err;
    te_expr *n = te_compile(expression, vars, 2, &err);

    if (n) {
        /* The variables can be changed here, and eval can be called as many
            * times as you like. This is fairly efficient because the parsing has
            * already been done. */
        x = 3; y = 4;
        const double r = te_eval(n); printf("Result:\n\t%f\n", r);
        te_free(n);
    } else {
        /* Show the user where the error is at. */
        printf("\t%*s^\nError near here", err-1, "");
    }

    return 0;
}