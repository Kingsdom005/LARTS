//
// Created by dongl on 2025/07/05.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/user.h>

RTEMS_APPLICATION_USTACKLEN__SET0(0xA00000)
RTEMS_APPLICATION_UHEAPLEN__SET1(0x3200000)
RTEMS_APPLICATION_USTACKSZ__SET2(0x200000)
RTEMS_APPLICATION_UCACHESZ__SET3(0x100000)
RTEMS_APPLICATION_PRIORITY__SET4(150)

// Thread execution code
// Thread function, verifying parameter and return value passing
void *pthread_worker(void *arg) {
    int *data = (int *)arg;
    printf("pthread_worker: received argument = %d\n", *data);
    // Returns a value that the main thread can retrieve using pthread_join.
    int *result = malloc(sizeof(int));
    *result = (*data) * 2;
    printf("pthread_worker: return result = %d\n", *result);
    return (void *) result;
}

int t_pthread () {
    pthread_t thread;
    int arg = 21; // Parameters passed to the thread
    int ret = 0;  // Return value of creating a thread

    // thread           Thread ID
    // NULL             Thread  Default Properties
    // pthread_worker   Thread execution body
    // (void *)&arg     Parameters passed to the thread execution body
    ret = pthread_create(&thread, NULL, pthread_worker, (void *)&arg);
    // Check the return value[citation:3]
    if (ret != 0) {
        printf("pthread_create failed: %d | %s\n", ret, strerror(ret));
        return -1;
    }
    printf("pthread_create succeeded\n");

    // Wait for the thread to complete and retrieve the return value
    void *thread_result;
    printf("pthread_join executing:\n");
    ret = pthread_join(thread, &thread_result); // Wait for the thread to finish[citation:1][citation:3]
    if (ret != 0) {
        printf("pthread_join failed: %s\n", strerror(ret));
        return -1;
    }
    printf("pthread_join completed\n");

    if (thread_result != NULL) {
        int *result_value = (int *)thread_result;
        printf("main thread got via pthread_join: %d\n", *result_value);
    }

    return 0;
}


int main (int argc, char *argv[]) {
    printf("\033[31mrtems ini_image TEST BEGIN did[%d] "
           "argv[0] %p : %s\033[0m\n",
           getpid(), argv[0], argv[0]);

    printf("t_pthread begin\n");
    if (t_pthread() == 0) {
        printf("pthread_create test passed\n");
    } else {
        printf("pthread_create test failed\n\n");
    }
    printf("t_pthread end\n");

    return 0;
}
