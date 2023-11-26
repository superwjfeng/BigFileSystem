#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_
#include <assert.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

namespace wjfeng {
namespace largefile {
const int32_t LFS_SUCCESS = 0;
const int32_t LFS_ERROR = -1;
const int32_t EXIT_DISK_OPER_INCOMPLETE =
    -8000;  // read or write length is less than required
const int32_t EXIT_INDEX_ALREADY_LOADED_ERROR =
    -8001;  // index is loaded when create or load
const int32_t EXIT_META_UNEXPECT_FOUND_ERROR =
    -8002;  // meta found in index when insert
const int32_t EXIT_INDEX_CORRUPT_ERROR = -8003;
const int32_t EXIT_BLOCKID_CONFLICT_ERROR = -8004;
const int32_t EXIT_BUCKET_CONFIGURE_ERROR = -8005;
const int32_t EXIT_META_NOT_FOUND_ERROR = -8006;
const int32_t EXIT_BLOCKID_ZERO_ERROR = -8007;

// const 和 static const？

static const std::string MAINBLOCK_DIR_PREFIX = "./mainblock/";
static const std::string INDEX_DIR_PREFIX = "./index/";
static const mode_t DIR_MODE = 0755;

enum OperatorType { C_OPER_INSERT = 1, C_OPER_DELETE = 2 };

struct MMapOption {
  int32_t max_mmap_size_;    // 最大映射空间尺寸，例：3M
  int32_t first_mmap_size_;  // 首次映射尺寸，例：4K
  int32_t per_mmap_size_;    // 之后可以继续递增尺寸，例：2K
};

struct BlockInfo {
  uint32_t block_id_;
  int32_t version_;
  int32_t file_count_;
  int32_t size_;
  int32_t del_file_count_;
  int32_t del_size_;
  uint32_t seq_no_;

  BlockInfo() {  // struct 可以用初始化列表吗？
    memset(this, 0, sizeof(BlockInfo));
  }

  inline bool operator==(const BlockInfo &rhs) const {
    return block_id_ == rhs.block_id_ && version_ == rhs.version_ &&
           file_count_ == rhs.file_count_ && size_ == rhs.size_ &&
           del_file_count_ == rhs.del_file_count_ &&
           del_size_ == rhs.del_size_ && seq_no_ == rhs.seq_no_;
  }
};

struct MetaInfo {
 public:
  MetaInfo() { init(); }

  MetaInfo(const uint64_t file_id, const int32_t in_offset,
           const int32_t file_size, const int32_t next_meta_offset) {
    fileid_ = file_id;
    location_.inner_offset_ = in_offset;
    location_.size_ = file_size;
    next_meta_offset_ = next_meta_offset;
  }

  MetaInfo(const MetaInfo &meta_info) {
    memcpy(this, &meta_info, sizeof(MetaInfo));
  }

  MetaInfo &operator=(const MetaInfo &meta_info) {
    if (&meta_info == this) return *this;
    fileid_ = meta_info.fileid_;
    location_.inner_offset_ = meta_info.location_.inner_offset_;
    location_.size_ = meta_info.location_.size_;
    next_meta_offset_ = meta_info.next_meta_offset_;
  }

  MetaInfo &clone(const MetaInfo &meta_info) {
    assert(&meta_info != this);
    fileid_ = meta_info.fileid_;
    location_.inner_offset_ = meta_info.location_.inner_offset_;
    location_.size_ = meta_info.location_.size_;
    next_meta_offset_ = meta_info.next_meta_offset_;
    return *this;
  }

  bool operator==(const MetaInfo &rhs) const {
    return fileid_ == rhs.fileid_ &&
           location_.inner_offset_ == rhs.location_.inner_offset_ &&
           location_.size_ == rhs.location_.size_ &&
           next_meta_offset_ == rhs.next_meta_offset_;
  }

  uint64_t get_key() const { return fileid_; }
  void set_key(const uint64_t key) { fileid_ = key; }
  uint64_t get_file_id() const { return fileid_; }
  void set_file_id(const uint64_t file_id) { fileid_ = file_id; }
  int32_t get_offset() const { return location_.inner_offset_; }
  void set_offset(const int32_t offset) { location_.inner_offset_ = offset; }
  int32_t get_size() const { return location_.size_; };
  void set_size(const int32_t file_size) { location_.size_ = file_size; }
  int32_t get_next_meta_offset() const { return next_meta_offset_; }
  void set_next_meta_offset(const int32_t offset) {
    next_meta_offset_ = offset;
  }

 private:
  uint64_t fileid_;

  struct {
    int32_t inner_offset_;
    int32_t size_;
  } location_;

  int32_t next_meta_offset_;

 private:
  void init() {
    fileid_ = 0;
    location_.inner_offset_ = 0;
    location_.size_ = 0;
    next_meta_offset_ = 0;
  }
};
}  // namespace largefile
}  // namespace wjfeng
#endif  // _COMMON_H_INCLUDED_