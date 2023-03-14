#include <dragonos/syscall.h>
#include <stddef.h>
#include <bits/ensure.h>
#include <abi-bits/pid_t.h>
#include <mlibc/debug.hpp>
#include <mlibc/all-sysdeps.hpp>
#include <mlibc/thread-entry.hpp>
#include <errno.h>
#include <sys/resource.h>

//#include <mlibc/allocator.hpp>

/*int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window) {
	__ensure(flags & MAP_ANONYMOUS);

	return syscall(SYS_MMAP, (uintptr_t)window, (size + 0xFFF) & ~static_cast<size_t>(0xFFF), (uintptr_t)hint, flags);
}*/

/*int sys_vm_unmap(void* address, size_t size) {
	__ensure(!(size & 0xFFF));

	long ret = syscall(SYS_MUNMAP, (uintptr_t)address, (size + 0xFFF) & ~static_cast<size_t>(0xFFF));

	return ret;
}*/

void sys_libc_panic(){
	sys_libc_log("libc panic!");
	__builtin_trap();
	for(;;);
}

void sys_libc_log(const char* msg){
	syscall(0, (uintptr_t)msg);
}

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
	syscall(SYS_CHDIR, path);
	return 0;
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