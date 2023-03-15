#include <dragonos/syscall.h>
#include <stddef.h>
// #include <bits/ensure.h>
#include <abi-bits/pid_t.h>
#include <mlibc/debug.hpp>
// #include <mlibc/all-sysdeps.hpp>
#include <mlibc/internal-sysdeps.hpp>
// #include <mlibc/thread-entry.hpp>
#include <errno.h>
// #include <sys/resource.h>

//#include <mlibc/allocator.hpp>
namespace mlibc{

int sys_tcb_set(void *pointer){
	return -ENOTSUP;
}
int sys_anon_allocate(size_t size, void **pointer){
	return -ENOTSUP;
}

int sys_anon_free(void *pointer, size_t size){
	return -ENOTSUP;
}

int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window){
	return -ENOTSUP;
}
int sys_vm_unmap(void *pointer, size_t size){
	return -ENOTSUP;
}

int sys_futex_tid(){
	// return syscall(SYS_GETTID);
	return -ENOTSUP;
}

int sys_futex_wait(int *pointer, int expected, const struct timespec *time){
	return -ENOTSUP;
	// return syscall(SYS_FUTEX_WAIT, pointer, expected);
}

int sys_futex_wake(int *pointer) {
	return -ENOTSUP;
	// return syscall(SYS_FUTEX_WAKE, pointer);
}

void sys_libc_panic(){
	sys_libc_log("libc panic!");
	__builtin_trap();
	for(;;);
}

void sys_libc_log(const char* msg){
	syscall(SYS_PUT_STRING, (uintptr_t)msg, 0x00ffffff, 0);
}

#ifndef MLIBC_BUILDING_RTDL
void sys_exit(int status){
	syscall(SYS_EXIT, status);

	__builtin_unreachable();
}

pid_t sys_getpid(){
	uint64_t _pid;
	syscall(SYS_GETPID, (uintptr_t)&_pid);

	pid_t pid = _pid;
	return pid;
}

int sys_clock_get(int clock, time_t *secs, long *nanos) {
	syscall(SYS_CLOCK, nanos);

	*secs = (*nanos) / 1000000000;
	*nanos = (*nanos) - (*secs) * 1000000000;

	return 0;
}

int sys_chdir(const char *path){
	return syscall(SYS_CHDIR, path);
}

int sys_sleep(time_t* sec, long* nanosec){
	syscall(SYS_NANOSLEEP, (*sec) * 1000000000 + (*nanosec));
	return 0;
}

int sys_waitpid(pid_t pid, int *status, int flags, struct rusage *ru, pid_t *ret_pid){
	if(ru) {
		mlibc::infoLogger() << "mlibc: struct rusage in sys_waitpid is unsupported" << frg::endlog;
		return ENOSYS;
	}

	pid_t ret = syscall(SYS_WAIT4, pid, status, flags);

	if(ret < 0){
		return -ret;
	}

	*ret_pid = ret;

	return 0;
}

int sys_fork(pid_t *child){
	long pid = syscall(SYS_FORK, 0);
	if(pid < 0){
		errno = pid;
		return -1;
	}

	*child = pid;
	return 0;
}

int sys_execve(const char *path, char *const argv[], char *const envp[]){
	return -syscall(SYS_EXECVE, path, argv, envp);
}

int sys_stat(fsfd_target fsfdt, int fd, const char *path, int flags,
		struct stat *statbuf){
			return -ENOTSUP;
		}

#endif
}
