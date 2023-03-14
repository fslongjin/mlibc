#include <sys/types.h>

#include <mlibc/ansi-sysdeps.hpp>

#include <dragonos/syscall.h>

namespace mlibc
{

    // TODO：支持sys_sigprocmask
    // int sys_sigprocmask(int how, const sigset_t *__restrict set,
    // 	sigset_t *__restrict retrieve){
    // int ret = syscall(SYS_SIGPROCMASK, how, set, retrieve);
    //     inr ret = syscall_invoke()
    //     if(ret < 0){
    //         return -ret;
    //     }

    //     return 0;
    // }

    int sys_sigaction(int signal, const struct sigaction *__restrict action,
                      struct sigaction *__restrict oldAction)
    {
        long ret = syscall(SYS_SIGACTION, signal, action, oldAction);
        if (ret < 0)
        {
            return -ret;
        }
        return 0;
    }

    int sys_kill(int pid, int signal)
    {
        long ret = syscall(SYS_KILL, pid, signal);
        if (ret < 0)
        {
            return -ret;
        }
        return 0;
    }

}
