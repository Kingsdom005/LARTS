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

// 线程执行代码
// 线程函数，验证参数和返回值传递
void *pthread_worker(void *arg) {
    int *data = (int *)arg;
    printf("pthread_worker: received argument = %d\n", *data);
    // 返回一个值，主线程可通过pthread_join获取
    int *result = malloc(sizeof(int));
    *result = (*data) * 2;
    printf("pthread_worker: return result = %d\n", *result);
    return (void *) result;
}

int t_pthread () {
    pthread_t thread;
    int arg = 21; // 传递给线程的参数
    int ret = 0;  // 创建线程的返回值

    // thread           线程ID
    // NULL             线程属性 默认属性
    // pthread_worker   线程执行体
    // (void *)&arg     传入线程执行体的参数
    ret = pthread_create(&thread, NULL, pthread_worker, (void *)&arg);
    // 检查返回值[citation:3]
    if (ret != 0) {
        printf("pthread_create failed: %d | %s\n", ret, strerror(ret));
        return -1;
    }
    printf("pthread_create succeeded\n");

    // 等待线程结束并获取返回值
    void *thread_result;
    printf("pthread_join executing:\n");
    ret = pthread_join(thread, &thread_result); // 等待线程结束[citation:1][citation:3]
    if (ret != 0) {
        printf("pthread_join failed: %s\n", strerror(ret));
        return -1;
    }
    printf("pthread_join completed\n");

    if (thread_result != NULL) {
        int *result_value = (int *)thread_result;
        printf("main thread got via pthread_join: %d\n", *result_value);
        free(thread_result); // 释放线程中分配的内存
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