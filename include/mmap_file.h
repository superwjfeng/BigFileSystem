#ifndef LARGEFILE_MMAPFILE_H_
#define LARGEFILE_MMAPFILE_H_

#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <cstring>
#include <iostream>

namespace wjfeng {
namespace largefile {
struct MMapOption {
  int32_t max_mmap_size_; // 最大映射空间尺寸，例：3M
  int32_t first_mmap_size_; // 首次映射尺寸，例：4K
  int32_t per_mmap_size_; // 之后可以继续递增尺寸，例：2K
};
class MMapFile {
 public:
  MMapFile();
  explicit MMapFile(const int &fd);
  MMapFile(const MMapOption &mmap_option, const int &fd);
  ~MMapFile();

  // file synchronization
  bool sync_file(); 
  // map file to memory, set access control flag
  bool map_file(const bool &write = false);
  void *get_data() const;
  int32_t get_size() const;

  bool munmap_file();
  bool remap_file();
 private:
  // 调整文件大小，从而支持同步
  bool ensure_file_size(const int32_t &size) const;
 private:
  int32_t size_;
  int fd_;
  void *data_;
  MMapOption mmap_file_option_;
};
}  // namespace largefile
}  // namespace wjfeng

#endif // LARGEFILE_MMAPFILE_H_