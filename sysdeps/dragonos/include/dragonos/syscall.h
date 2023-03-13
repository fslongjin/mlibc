#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

// 系统调用号
#define SYS_NOT_EXISTS 0
#define SYS_PUT_STRING 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define SYS_READ 4
#define SYS_WRITE 5
#define SYS_LSEEK 6
#define SYS_FORK 7
#define SYS_VFORK 8
#define SYS_BRK 9
#define SYS_SBRK 10

#define SYS_REBOOT 11    // 重启
#define SYS_CHDIR 12     // 切换工作目录
#define SYS_GET_DENTS 13 // 获取目录中的数据
#define SYS_EXECVE 14    // 执行新的应用程序
#define SYS_WAIT4 15     // 等待进程退出
#define SYS_EXIT 16      // 进程退出
#define SYS_MKDIR 17     // 创建文件夹
#define SYS_NANOSLEEP 18 // 纳秒级休眠
#define SYS_CLOCK 19     // 获取当前cpu时间
#define SYS_PIPE 20

#define SYS_MSTAT 21        // 获取系统的内存状态信息
#define SYS_UNLINK_AT 22    // 删除文件夹/删除文件链接
#define SYS_KILL 23         // kill一个进程(向这个进程发出信号)
#define SYS_SIGACTION 24    // 设置进程的信号处理动作
#define SYS_RT_SIGRETURN 25 // 从信号处理函数返回
#define SYS_GETPID 26 // 获取当前进程的pid（进程标识符）

#ifdef __cplusplus
extern "C"{
#endif


__attribute__((always_inline))
static inline long syscalln0(uint64_t call) {
    volatile long ret;
    asm volatile("int $0x80" : "=a"(ret) : "a"(call)); 
    return ret;
}

__attribute__((always_inline))
static long syscalln1(uint64_t call, uint64_t arg0) {
    volatile long ret;
    asm volatile("int $0x80" : "=a"(ret) : "a"(call), "r8"(arg0) : "memory"); 
    return ret;
}

__attribute__((always_inline))
static long syscalln2(uint64_t call, uint64_t arg0, uint64_t arg1) {
    volatile long ret;
    asm volatile("int $0x80" : "=a"(ret) : "a"(call), "r8"(arg0), "r9"(arg1) : "memory"); 
    return ret;
}

__attribute__((always_inline))
static long syscalln3(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    volatile long ret;
    asm volatile("int $0x80" : "=a"(ret) : "a"(call), "r8"(arg0), "r9"(arg1), "r10"(arg2) : "memory"); 
    return ret;
}

__attribute__((always_inline))
static long syscalln4(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    volatile long ret;
	register uint64_t arg3r asm("r10") = arg3; // put arg3 in r10
    asm volatile("int $0x80" : "=a"(ret) : "a"(call), "r8"(arg0), "r9"(arg1), "r10"(arg2), "r11"(arg3r) : "memory"); 
    return ret;
}

__attribute__((always_inline))
static long syscalln5(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    volatile long ret;
	register uint64_t arg3r asm("r10") = arg3; // put arg3 in r10
	register uint64_t arg4r asm("r9") = arg4; // put arg4 in r9
    asm volatile("int $0x80" : "=a"(ret) : "a"(call), "r8"(arg0), "r9"(arg1), "r10"(arg2), "r11"(arg3r), "r12"(arg4r) : "memory"); 
    return ret;
}

__attribute__((always_inline))
static long syscalln6(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    volatile long ret;
	register uint64_t arg3r asm("r10") = arg3; // put arg3 in r10
	register uint64_t arg4r asm("r9") = arg4; // put arg4 in r9
	register uint64_t arg5r asm("r8") = arg5; // put arg5 in r8
    asm volatile("int $0x80" : "=a"(ret) : "a"(call), "r8"(arg0), "r9"(arg1), "r10"(arg2), "r11"(arg3r), "r12"(arg4r), "r13"(arg4r) : "memory"); 
    return ret;
}

#ifdef __cplusplus
}
    __attribute__((always_inline)) static inline long _syscall(uint64_t call) { return syscalln0(call); }
    __attribute__((always_inline)) static inline long _syscall(uint64_t call, uint64_t arg0) { return syscalln1(call, arg0); }
    __attribute__((always_inline)) static inline long _syscall(uint64_t call, uint64_t arg0, uint64_t arg1) { return syscalln2(call, arg0, arg1); }
    __attribute__((always_inline)) static inline long _syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2) { return syscalln3(call, arg0, arg1, arg2); }
    __attribute__((always_inline)) static inline long _syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) { return syscalln4(call, arg0, arg1, arg2, arg3); }
    __attribute__((always_inline)) static inline long _syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)  { return syscalln5(call, arg0, arg1, arg2, arg3, arg4); }
    __attribute__((always_inline)) static inline long _syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5)  { return syscalln6(call, arg0, arg1, arg2, arg3, arg4, arg5); }

    template<typename... T>
    __attribute__((always_inline)) static inline long syscall(uint64_t call, T... args){
        return _syscall(call, (uint64_t)(args)...);
    }
#else
    #define GET_SYSCALL(a0, a1, a2, a3, a4, a5, a6, name, ...) name
    #define syscall(...) GET_SYSCALL(__VA_ARGS__, syscalln6, syscalln5, syscalln4, syscalln3, syscalln2, syscalln1, syscalln0)(__VA_ARGS__) 
#endif

#endif