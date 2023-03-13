#include <mlibc/ansi-sysdeps.hpp>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <dragonos/syscall.h>

namespace mlibc{
int sys_write(int fd, const void* buffer, size_t count, ssize_t* written){
	long ret = syscall(SYS_WRITE, fd, (uintptr_t)buffer, count);

	if(ret < 0)
		return -ret;

	*written = ret;
	return 0;
}

int sys_read(int fd, void *buf, size_t count, ssize_t *bytes_read) {
	long ret = syscall(SYS_READ, fd, (uintptr_t)buf, count);

	if(ret < 0){
		*bytes_read = 0;
		return -ret;
	}

	*bytes_read = ret;
	return 0;
}

int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
	long ret = syscall(SYS_LSEEK, fd, offset, whence);

	if(ret < 0){
		return -ret;
	}

	*new_offset = ret;
	return 0;
}

int sys_open(const char* filename, int flags, mode_t mode, int* fd){
	long ret = syscall(SYS_OPEN, (uintptr_t)filename, flags);

	if(ret < 0)
		return -ret;

	*fd = ret;

	return 0;
}

int sys_close(int fd){
	return syscall(SYS_CLOSE, fd);
}

int sys_access(const char* filename, int mode){
	int fd;
	if(int e = sys_open(filename, O_RDONLY, 0, &fd)){
		return e;
	}

	sys_close(fd);
	return 0;
}

int sys_mkdir(const char* path, mode_t){
	long ret = syscall(SYS_MKDIR, path);

	if(ret < 0){
		return -ret;
	}

	return 0;
}

int sys_rmdir(const char* path){
	long ret = syscall(SYS_UNLINK_AT, 0, path, AT_REMOVEDIR);

	if(ret < 0){
		return -ret;
	}

	return 0;
}

int sys_unlinkat(int fd, const char *path, int flags) {
	long ret = syscall(SYS_UNLINK_AT, fd, path, flags);

	if(ret < 0) {
		return -ret;
	}

	return 0;
}

int sys_open_dir(const char* path, int* handle){
	return sys_open(path, O_DIRECTORY, 0, handle);
}

int sys_pipe(int *fds, int flags){
	return -syscall(SYS_PIPE, fds, flags);
}

int sys_read_entries(int handle, void *buffer, size_t max_size, size_t *bytes_read){
	dirent tmp_dir_ent;
	long ret = syscall(SYS_READ, handle, &tmp_dir_ent, sizeof(dirent));

	if(!ret){
		*bytes_read = 0;
		return 0;
	} else if(ret > 0){
		dirent* dir = (dirent*)buffer;
		strcpy(dir->d_name, tmp_dir_ent.d_name);
		dir->d_ino = tmp_dir_ent.d_ino;
		dir->d_off = 0;
		dir->d_reclen = sizeof(dirent);
		dir->d_type = tmp_dir_ent.d_type;

		*bytes_read = sizeof(dirent);
		return 0;
	} else {
		return -ret;
	}
}

}