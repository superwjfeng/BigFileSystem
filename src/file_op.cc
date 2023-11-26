#include "file_op.h"

#include "common.h"

namespace wjfeng {
namespace largefile {
FileOperation::FileOperation(const std::string &file_name, const int open_flags)
    : fd_(-1), open_flags_(open_flags) {
  // 拷贝字符串s的一个副本，由函数返回值返回，这个副本有自己的内存空间，和s不相干
  file_name_ = strdup(file_name.c_str());
}

FileOperation::~FileOperation() {
  if (fd_ > 0) {
    close(fd_);
  }
  if (file_name_ != nullptr) {
    free(file_name_);  // strdup 动态开辟内存
    file_name_ = nullptr;
  }
}

int FileOperation::open_file() {
  if (fd_ > 0) {  // 已经打开了
    close(fd_);
    fd_ = -1;
  }
  fd_ = ::open(file_name_, open_flags_, OPEN_MODE);
  if (fd_ < 0) return -errno;

  return fd_;
}

void FileOperation::close_file() {
  if (fd_ < 0) return;
  ::close(fd_);
  fd_ = -1;
}

int FileOperation::check_file() {
  if (fd_ < 0) fd_ = open_file();
  return fd_;
}

int64_t FileOperation::get_file_size() {
  int fd = check_file();  // 确保文件打开了
  if (fd < 0) return -1;
  struct stat statbuf;
  if (fstat(fd, &statbuf) != 0) return -1;
  return statbuf.st_size;
}

int FileOperation::ftruncate_file(const int64_t len) {
  int fd = check_file();
  if (fd < 0) return fd;
  return ftruncate(fd, len);
}

int FileOperation::seek_file(const int64_t offset) {
  int fd = check_file();
  if (fd < 0) return fd;
  return lseek(fd, offset, SEEK_SET);
}

int FileOperation::flush_file() {
  if (open_flags_ & O_SYNC) return 0;  // 如果同步的话一定已经flush过了
  int fd = check_file();
  if (fd < 0) return fd;
  return fsync(fd);
}

int FileOperation::unlink_file() {
  close_file();
  return ::unlink(file_name_);
}

int FileOperation::pread_file(char *buf, const int32_t nbytes,
                              const int64_t offset) {
  int32_t left = nbytes;  // 剩余的数据
  int64_t read_offset = offset;
  int32_t read_len = 0;
  char *p_tmp = buf;

  int i = 0;
  while (left > 0) {
    if (++i >= MAX_DISK_TIMES) break;
    if (check_file() < 0) return -errno;
    read_len = ::pread64(fd_, p_tmp, left, read_offset);
    if (read_len < 0) {
      // 不直接使用errno，因为有可能多线程
      read_len = -errno;
      //  EINTR 临时中断，EAGAIN 继续尝试，EBADF fd损坏 三种情况下继续循环
      if (-read_len == EINTR || -read_len == EAGAIN || -read_len == EBADF) {
        continue;
      } else {
        return read_len;
      }
    }
    left -= read_len;
    p_tmp += read_len;
    read_offset += read_len;
  }
  if (left != 0) return EXIT_DISK_OPER_INCOMPLETE;
  return LFS_SUCCESS;
}

int FileOperation::pwrite_file(const char *buf, const int32_t nbytes,
                               const int64_t offset) {
  int32_t left = nbytes;
  int64_t write_offset = offset;
  int32_t written_len = 0;
  const char *p_tmp = buf;

  int i = 0;
  while (left > 0) {
    if (++i >= MAX_DISK_TIMES) break;
    if (check_file() < 0) return -errno;
    written_len = ::pwrite64(fd_, p_tmp, left, write_offset);
    if (written_len < 0) {
      written_len = -errno;
      if (-written_len == EINTR || -written_len == EAGAIN ||
          -written_len == EBADF) {
        continue;
      } else {
        return written_len;
      }
    }
    left -= written_len;
    p_tmp += written_len;
    write_offset += written_len;
  }
  if (left != 0) return EXIT_DISK_OPER_INCOMPLETE;
  return LFS_SUCCESS;
}

int FileOperation::write_file(const char *buf, const int32_t nbytes) {
  int32_t left = nbytes;
  int32_t written_len = 0;
  const char *p_tmp = buf;

  int i = 0;
  while (left > 0) {
    if (++i >= MAX_DISK_TIMES) break;
    if (check_file() < 0) return -errno;
    written_len = ::write(fd_, p_tmp, left);
    if (written_len < 0) {
      written_len = -errno;
      // 不直接使用errno，因为有可能多线程
      if (-written_len == EINTR || -written_len == EAGAIN ||
          -written_len == EBADF) {
        continue;
      } else {
        return written_len;
      }
    }
    left -= written_len;
    p_tmp += written_len;
  }
  if (left != 0) return EXIT_DISK_OPER_INCOMPLETE;
  return LFS_SUCCESS;
}

}  // namespace largefile
}  // namespace wjfeng