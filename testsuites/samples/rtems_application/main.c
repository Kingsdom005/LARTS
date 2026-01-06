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
#define _GNU_SOURCE
#include <sched.h>

RTEMS_APPLICATION_USTACKLEN__SET0(0xA00000)
RTEMS_APPLICATION_UHEAPLEN__SET1(0x3200000)
RTEMS_APPLICATION_USTACKSZ__SET2(0x200000)
RTEMS_APPLICATION_UCACHESZ__SET3(0x100000)
RTEMS_APPLICATION_PRIORITY__SET4(150)


void current_root_path_content () {
    printf("========= current system root path content =========\n");
    DIR *dir = opendir("/");
    assert(dir);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("[%p] [%d] %-20s | %3db | %3db\n",
               entry, entry->d_type, entry->d_name,
               entry->d_namlen, entry->d_reclen);
    }
    printf("========= current system root path content =========\n");
}

void test_fork ( void ) {
    int child_pid = fork();
    if (child_pid == -1) {
        perror("fork failed");
        return;
    }

    if (child_pid == 0) {
        // Child domain
        printf("In child domain: Hello from the child! DID: %d\n", getpid());
        return;
    }

    // Parent domain
    printf("Parent domain[%d]: Child domain created with DID %d\n", getpid(), child_pid);
}

int fn(void* arg) {
    // 打印子进程信息
    printf("In child domain: arg: %p\n", arg);
    printf("Child domain[%d]: Hello from the child!\n", getpid());
    // 写入管道
    char buff[256] = {0};
    int* pipedes = (int*) arg;
    printf("pipedes[%p]: %d %d\n", pipedes, pipedes[0], pipedes[1]);
    snprintf(buff, sizeof(buff), "Hello pipe from child domain with DID %d.", getpid());
    write(pipedes[1], buff, strlen(buff));
    // 获取消息队列
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;
    mq = mq_open("/test_mq", O_CREAT | O_RDWR, 0666, &attr);
    printf("domain[%d]: mq: 0x%x\n", getpid(), mq);
    // 发送消息到消息队列
    char mq_buff[256] = {0};
    snprintf(mq_buff, sizeof(mq_buff), "Hello mq from child domain with DID %d.", getpid());
    int mq_result = mq_send(mq, mq_buff, strlen(mq_buff), getpid());
    printf("domain[%d]: mq_send result: 0x%x\n", getpid(), mq_result);

    // 关闭管道
    close(pipedes[0]);
    close(pipedes[1]);
    // 关闭消息队列
    mq_close(mq);
    return 0;
}


void test_clone_pipe_mq ( void ) {
    // 管道
    int pipedes[2];
    int pipe_result = pipe(pipedes);
    printf("domain[%d]: pipedes[%p]: %d %d\n", getpid(), pipedes, pipedes[0], pipedes[1]);
    // 消息队列
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;
    mq = mq_open("/test_mq", O_CREAT | O_RDWR, 0666, &attr);
    printf("domain[%d]: mq: 0x%x\n", getpid(), mq);

    // 子进程栈
    char stack1[0x10000];
    char stack2[0x10000];
    printf("stack[%p]: %d bytes, stack: %p\n", stack1, sizeof(stack1), stack1 + sizeof(stack1));
    printf("stack[%p]: %d bytes, stack: %p\n", stack2, sizeof(stack2), stack2 + sizeof(stack2));

    // 克隆子进程
    int child_pid1 = clone(fn, stack1 + sizeof(stack1), CLONE_VM | CLONE_VFORK | SIGCHLD, &pipedes);
    int child_pid2 = clone(fn, stack2 + sizeof(stack2), CLONE_VM | CLONE_VFORK | SIGCHLD, &pipedes);

    if (child_pid1 == -1 || child_pid2 == -1) {
        perror("clone failed");
        return;
    }

    // 读取子进程写的管道
    char child_msg[256] = {0};
    read(pipedes[0], child_msg, 256);
    // 打印父子进程信息与管道数据
    printf("Parent domain[%d]: Child domain created with DID %d|%d, msg: %s\n", getpid(), child_pid1, child_pid2, child_msg);
    // 读取子进程消息队列消息
    for (int i = 0; i < 2; ++i) {
        char mq_buf[256];
        unsigned int prio;
        ssize_t bytes = mq_receive(mq, mq_buf, 256, &prio);
        printf("domain[%d]: mq_receive result: %zd, msg: %s, prio: %u\n", getpid(), bytes, mq_buf, prio);
    }

    // 关闭管道
    close(pipedes[0]);
    close(pipedes[1]);
    // 关闭消息队列
    mq_close(mq);
    // 删除消息队列
    mq_unlink("/test_mq");
    // Wait for the child domain to finish
    // int status;
    // waitpid(child_pid, &status, 0);

}


int main (int argc, char *argv[])
{
    printf("\033[31mrtems image rtems_application TEST BEGIN did[%d] "
           "argv[0] %p : %s\033[0m\n",
           getpid(), argv[0], argv[0]);

    // printf("!!!!!! rtems_application !!!!!!\n");
    // current_root_path_content();

    // test_fork();

    test_clone_pipe_mq();

    printf("\033[31mrtems image rtems_application TEST END\033[0m\n");
    return 0;
}