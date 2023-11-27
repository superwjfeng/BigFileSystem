#ifndef LARGEFILE_FILE_INDEX_HANDLE_H_
#define LARGEFILE_FILE_INDEX_HANDLE_H_

#include "common.h"
#include "mmap_file_op.h"

namespace wjfeng {
namespace largefile {
struct IndexHeader {
 public:
  IndexHeader() { memset(this, 0, sizeof(IndexHeader)); }

 public:
  BlockInfo block_info_;  // meta block info
  int32_t bucket_size_;   // hash backet size
  // TODO: rename to block_data_offset?
  int32_t data_file_offset_;  // offset to write next data in block
  // TODO: rename to last_meta_offset?
  // size of all IndexHandel: index_header + all buckets;
  int32_t index_file_size_;
  int32_t free_head_offset_;  // free meta node list for reuse
};

class IndexHandle {
 public:
  IndexHandle(const std::string &base_path, const uint32_t main_block_id);
  ~IndexHandle();

 public:
  // logic block includes both main block and extend block
  int create(const uint32_t logic_block_id, const int32_t bucket_size,
             const MMapOption &map_option);

  int load(const uint32_t logic_block_id, const int32_t bucket_size,
           const MMapOption &map_option);

  int remove(const uint32_t logic_block_id);  // remove index: unmmap and unlink

  int flush();

  IndexHeader *get_index_header() {
    return static_cast<IndexHeader *>(file_op_->get_map_data());
  }
  // BlockInfo *block_info() { return &(index_header()->block_info_); }
  BlockInfo *get_block_info() {
    return static_cast<BlockInfo *>(file_op_->get_map_data());
  }
  int32_t get_bucket_size() const {
    return static_cast<IndexHeader *>(file_op_->get_map_data())->bucket_size_;
  }

  int32_t get_block_data_offset() const {
    return static_cast<IndexHeader *>(file_op_->get_map_data())
        ->data_file_offset_;
  }

  // return an array
  int32_t *bucket_slot() {
    return reinterpret_cast<int32_t *>(
        static_cast<char *>(file_op_->get_map_data()) + sizeof(IndexHeader));
  }

  // write MetaInfo into IndexHandle
  int32_t write_segment_meta(const uint64_t key, MetaInfo &meta);

  int32_t hash_find(const uint64_t key, int32_t &current_offset,
                    int32_t &previous_offset);

  int32_t hash_insert(const uint64_t key, int32_t previous_offset,
                      MetaInfo &meta);

  void commit_block_data_offset(const int file_size) {
    static_cast<IndexHeader *>(file_op_->get_map_data())->data_file_offset_ +=
        file_size;
  }

  int update_block_info(const OperatorType &oper_type,
                        const uint32_t modify_size);

 private:
  bool hash_compare(const uint64_t left_key, const uint64_t right_key) {
    return left_key == right_key;
  }

 private:
  MMapFileOperation *file_op_;
  bool is_load_;
};

}  // namespace largefile
}  // namespace wjfeng

#endif  // LARGEFILE_FILE_INDEX_HANDLE_H_