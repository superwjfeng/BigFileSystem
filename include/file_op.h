#ifndef LARGEFILE_FILE_OP_H_
#define LARGEFILE_FILE_OP_H_

#include "common.h"

namespace wjfeng {
namespace largefile {
class FileOperation {
 public:
  FileOperation(const std::string &file_name,
                const int open_flags = O_RDWR | O_LARGEFILE);
  ~FileOperation();

  int open_file();
  void close_file();
  int flush_file();  // write immediately
  int unlink_file(); // delete file
  int pread_file(char *buf, const int32_t &nbytes, const int64_t &offset);
  int pwrite_file(const char *buf, const int32_t &nbytes,
                  const int64_t &offset);
  int write_file(const char *buf, const int32_t &nbytes);
  int64_t get_file_size();
  int ftruncate_file(const int64_t &len);
  int seek_file(const int64_t &offset);
  int get_fd() const { return fd_; }

 protected:
  int check_file();
 protected:
  int fd_;
  int open_flags_;
  char *file_name_;
 protected:
  static const mode_t OPEN_MODE = 0644;
  static const int MAX_DISK_TIMES = 5; // 最大磁盘读取次数
};
}  // namespace largefile
}  // namespace wjfeng
#endif  // LARGEFILE_FILE_OP_H_