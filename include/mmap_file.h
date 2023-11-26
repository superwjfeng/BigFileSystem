#ifndef LARGEFILE_MMAPFILE_H_
#define LARGEFILE_MMAPFILE_H_

#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"

namespace wjfeng {
namespace largefile {

class MMapFile {
 public:
  MMapFile();
  explicit MMapFile(const int fd);
  MMapFile(const MMapOption &mmap_option, const int fd);
  ~MMapFile();

  // file synchronization
  bool sync_file();
  // map file to memory, set access control flag
  bool map_file(const bool write = false);
  void *get_data() const;
  int32_t get_size() const;

  bool munmap_file();
  bool remap_file();  // 重新映射，追加内存

 private:
  // 调整文件大小，从而支持映射
  bool ensure_file_size(const int32_t size) const;

 private:
  int32_t size_;
  int fd_;
  void *data_;
  MMapOption mmap_file_option_;
};
}  // namespace largefile
}  // namespace wjfeng

#endif  // LARGEFILE_MMAPFILE_H_