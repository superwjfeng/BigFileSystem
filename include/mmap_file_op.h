#ifndef _LARGEFILE_MMAPFILE_OP_H
#define _LARGEFILE_MMAPFILE_OP_H

#include "common.h"
#include "file_op.h"
#include "mmap_file.h"

namespace wjfeng {
namespace largefile {
class MMapFileOperation : public FileOperation {
 public:
  MMapFileOperation(const std::string &file_name,
                    const int open_flags = O_CREAT | O_RDWR | O_LARGEFILE)
      : FileOperation(file_name, open_flags),
        map_file_(nullptr),
        is_mapped_(false) {}
  ~MMapFileOperation() {
    if (map_file_) {
      delete (map_file_);
      map_file_ = nullptr;
    }
  }

  int mmap_file(const MMapOption &mmap_option); // 映射文件
  int munmap_file();

  int pread_file(char *buf, const int32_t size, const int64_t offset);
  int pwrite_file(const char *buf, const int32_t size, const int64_t offset);

  void *get_map_data() const;
  int flush_file();

 private:
  MMapFile *map_file_;
  bool is_mapped_; // whether the file is already mapped
};
}  // namespace largefile
}  // namespace wjfeng

#endif  // _LARGEFILE_MMAPFILE_OP_H