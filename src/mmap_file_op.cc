#include "mmap_file_op.h"

#include "common.h"

static int debug = 1;

namespace wjfeng {
namespace largefile {
int MMapFileOperation::mmap_file(const MMapOption &mmap_option) {
  if (mmap_option.max_mmap_size_ < mmap_option.first_mmap_size_)
    return LFS_ERROR;
  if (mmap_option.max_mmap_size_ <= 0) return LFS_ERROR;
  int fd = check_file();
  if (fd < 0) {
    fprintf(stderr, "MMapFileOperation::mmap_file - checking file failed!");
    return LFS_ERROR;
  }
  if (!is_mapped_) {
    if (map_file_) delete map_file_;
    map_file_ = new MMapFile(mmap_option, fd);
    is_mapped_ = map_file_->map_file(true);
  }
  if (is_mapped_)
    return LFS_SUCCESS;
  else
    return LFS_ERROR;
}

int MMapFileOperation::munmap_file() {
  if (is_mapped_ && map_file_ != nullptr) {
    delete map_file_;
    is_mapped_ = false;
  }
  return LFS_SUCCESS;
}

void *MMapFileOperation::get_map_data() const {
  if (is_mapped_) {
    return map_file_->get_data();
  } else {
    return nullptr;
  }
}

int MMapFileOperation::pread_file(char *buf, const int32_t size,
                                  const int64_t offset) {
  // 情况1，内存已经映射，检查一下映射区域的大小
  if (is_mapped_ && (offset + size) > map_file_->get_size()) {
    if (debug) {
      fprintf(stdout,
              "MMapFileOperation::pread_file:, size:%d, ofset: %" __PRI64_PREFIX
              "d, map file size: %d, need remap\n",
              size, offset, map_file_->get_size());
    }
    map_file_->remap_file();
  }

  if (is_mapped_ && (offset + size) <= map_file_->get_size()) {
    memcpy(buf, (char *)map_file_->get_data() + offset, size);
    return LFS_SUCCESS;
  }

  // 情况2，内存没有映射或是要读取的数据映射不全。派生类指针this调用基类的方法
  return FileOperation::pread_file(buf, size, offset);
}

int MMapFileOperation::pwrite_file(const char *buf, const int32_t size,
                                   const int64_t offset) {
  // 情况1，内存已经映射
  if (is_mapped_ && (offset + size) > map_file_->get_size()) {
    if (debug) {
      fprintf(
          stdout,
          "MMapFileOperation::pwrite_file:, size:%d, ofset: %" __PRI64_PREFIX
          "d, map file size: %d, need remap\n",
          size, offset, map_file_->get_size());
    }
    map_file_->remap_file();
  }

  if (is_mapped_ && (offset + size) <= map_file_->get_size()) {
    memcpy((char *)map_file_->get_data() + offset, buf, size);
    return LFS_SUCCESS;
  }

  // 情况2，内存没有映射或是要读取的数据映射不全。派生类指针this调用基类的方法
  return FileOperation::pwrite_file(buf, size, offset);
}

int MMapFileOperation::flush_file() {
  if (is_mapped_) {
    if (map_file_->sync_file())
      return LFS_SUCCESS;
    else
      return LFS_ERROR;
  }

  return FileOperation::flush_file();
}
}  // namespace largefile
}  // namespace wjfeng