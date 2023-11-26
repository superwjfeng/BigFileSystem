#include "index_handle.h"

static int debug = 1;

namespace wjfeng {
namespace largefile {
IndexHandle::IndexHandle(const std::string &base_path,
                         const uint32_t main_block_id) {
  // create file_op_handle object
  std::stringstream tmp_stream;
  tmp_stream << base_path << INDEX_DIR_PREFIX << main_block_id;
  std::string index_path;
  tmp_stream >> index_path;

  file_op_ = new MMapFileOperation(index_path, O_CREAT | O_RDWR | O_LARGEFILE);
  is_load_ = false;
}
IndexHandle::~IndexHandle() {
  if (file_op_) {
    delete file_op_;
    file_op_ = nullptr;
  }
}

int IndexHandle::create(const uint32_t logic_block_id,
                        const int32_t bucket_size,
                        const MMapOption &map_option) {
  if (debug) {
    printf(
        "create index, block id: %u, bucket size: %d, max_mmap_size: %d, first "
        "mmap size: %d, per mmap size: %d\n",
        logic_block_id, bucket_size, map_option.max_mmap_size_,
        map_option.first_mmap_size_, map_option.per_mmap_size_);
  }

  int ret = LFS_SUCCESS;
  if (is_load_) return EXIT_INDEX_ALREADY_LOADED_ERROR;

  int64_t file_size = file_op_->get_file_size();
  if (file_size < 0)
    return LFS_ERROR;
  else if (file_size == 0) {
    IndexHeader i_header;
    i_header.block_info_.block_id_ = logic_block_id;
    i_header.block_info_.seq_no_ = 1;
    i_header.bucket_size_ = bucket_size;
    // point to start of MetaInfo by default;
    i_header.index_file_size_ =
        sizeof(IndexHeader) + bucket_size * sizeof(int32_t);

    // prepare space for MetaInfo: index header + total buckets
    char *init_data = new char[i_header.index_file_size_];
    memcpy(init_data, &i_header, sizeof(IndexHeader));
    memset(init_data + sizeof(IndexHeader), 0,
           i_header.index_file_size_ - sizeof(IndexHeader));

    // write index header and buckets into index file
    ret = file_op_->pwrite_file(init_data, i_header.index_file_size_, 0);

    delete[] init_data;
    init_data = nullptr;

    if (ret != LFS_SUCCESS) return ret;

    ret = file_op_->flush_file();
    if (ret != LFS_SUCCESS) return ret;
  } else {  // file size > 0, index already exist
    return EXIT_META_UNEXPECT_FOUND_ERROR;
  }

  ret = file_op_->mmap_file(map_option);
  if (ret != LFS_SUCCESS) return ret;
  is_load_ = true;
  if (debug) {
    printf(
        "init blockid %d index successful, data file size: %d, index file "
        "size: %d, bucket_size: %d, free head offset: %d, seqno: %d, size:%d, "
        "filecount: %d, del_size: %d, del_file_count:%d, version: %d\n",
        logic_block_id, get_index_header()->data_file_offset_,
        get_index_header()->index_file_size_, get_index_header()->bucket_size_,
        get_index_header()->free_head_offset_, get_block_info()->seq_no_,
        get_block_info()->size_, get_block_info()->file_count_,
        get_block_info()->del_size_, get_block_info()->del_file_count_,
        get_block_info()->version_);
  }
  return LFS_SUCCESS;
}

int IndexHandle::load(const uint32_t logic_block_id, const int32_t bucket_size,
                      const MMapOption &map_option) {
  int ret = LFS_SUCCESS;
  if (is_load_) return EXIT_INDEX_ALREADY_LOADED_ERROR;

  int64_t file_size = file_op_->get_file_size();
  if (file_size < 0)
    return file_size;
  else if (file_size == 0) {  // empty file
    return EXIT_INDEX_CORRUPT_ERROR;
  }

  MMapOption tmp_map_option = map_option;
  if (file_size > tmp_map_option.first_mmap_size_ &&
      file_size <= tmp_map_option.max_mmap_size_) {
    tmp_map_option.first_mmap_size_ = file_size;
  }

  ret = file_op_->mmap_file(tmp_map_option);
  if (ret != LFS_SUCCESS) return ret;

  // check validity of data
  if (get_bucket_size() == 0 || get_block_info()->block_id_ == 0) {
    fprintf(stderr, "Index corrupt error, blockid: %u, bucket size: %d\n",
            get_block_info()->block_id_, get_bucket_size());
    return EXIT_INDEX_CORRUPT_ERROR;
  }

  // check file size
  int32_t index_file_size =
      sizeof(IndexHeader) + get_bucket_size() * sizeof(int32_t);

  if (file_size < index_file_size) {
    fprintf(
        stderr,
        "Index corrupt error, blockid: %u, bucket size: %d, file size: %ld, "
        "index file size: %d\n",
        get_block_info()->block_id_, get_bucket_size(), file_size,
        index_file_size);
    return EXIT_INDEX_CORRUPT_ERROR;
  }

  // check block id
  if (logic_block_id != get_block_info()->block_id_) {
    fprintf(stderr, "block id conflict, blockid: %u, index blockid: %u\n",
            logic_block_id, get_block_info()->block_id_);
    return EXIT_BLOCKID_CONFLICT_ERROR;
  }

  // check bucket size
  if (bucket_size != get_bucket_size()) {
    fprintf(stderr,
            "index configure error, old bucket size: %u, new bucket size: %u\n",
            get_bucket_size(), bucket_size);
    return EXIT_BUCKET_CONFIGURE_ERROR;
  }

  is_load_ = true;
  if (debug) {
    printf(
        "load blockid %d index successful, data file size: %d, index file "
        "size: %d, bucket_size: %d, free head offset: %d, seqno: %d, size:%d, "
        "filecount: %d, del_size: %d, del_file_count:%d, version: %d\n",
        logic_block_id, get_index_header()->data_file_offset_,
        get_index_header()->index_file_size_, get_index_header()->bucket_size_,
        get_index_header()->free_head_offset_, get_block_info()->seq_no_,
        get_block_info()->size_, get_block_info()->file_count_,
        get_block_info()->del_size_, get_block_info()->del_file_count_,
        get_block_info()->version_);
  }
  return LFS_SUCCESS;
}

int IndexHandle::remove(const uint32_t logic_block_id) {
  if (is_load_) {
    if (logic_block_id != get_block_info()->block_id_) {
      fprintf(stderr, "block id conflict, blockid: %d, index blockid: %d\n",
              logic_block_id, get_block_info()->block_id_);
      return EXIT_BLOCKID_CONFLICT_ERROR;
    }

    int ret = file_op_->munmap_file();
    if (LFS_SUCCESS != ret) return ret;

    ret = file_op_->unlink_file();
    return ret;
  }
}

int IndexHandle::flush() {
  int ret = file_op_->flush_file();
  if (ret != LFS_SUCCESS) {
    fprintf(stderr, "index flush fail, ret: %d error desc: %s\n", ret,
            strerror(errno));
  }
  return ret;
}

int IndexHandle::write_segment_meta(const uint64_t key, MetaInfo &meta) {
  int32_t current_offset = 0;
  int32_t previous_offset = 0;

  // 从文件哈希表中查找key是否存在
  int ret = hash_find(key, current_offset, previous_offset);
  if (LFS_SUCCESS == ret) {
    return EXIT_META_UNEXPECT_FOUND_ERROR;
  } else if (ret != EXIT_META_NOT_FOUND_ERROR) {
    return ret;
  }
  // key 若不存在就写入meta到文件哈希表中
  ret = hash_insert(key, previous_offset, meta);
  return ret;
}

int IndexHandle::hash_find(const uint64_t key, int32_t &current_offset,
                           int32_t &previous_offset) {
  int ret = LFS_SUCCESS;
  MetaInfo meta_info;
  current_offset = 0;
  previous_offset = 0;
  // 1. 确定key存放的桶（slot）的位置
  int32_t slot = static_cast<int32_t>(key % get_bucket_size());
  /*
    2. 读取桶首节点存储的第一个节点的偏移量
    若偏移量为0，则返回EXIT_META_NOT_FOUND_ERROR
  */
  // 3. 根据偏移量读取存储的metainfo
  /*
    4. 与key进行比较，相等则设置current_offset和previous_offset
    并返回TFS_SUCCESS否则继续执行5
  */
  /*
    5. 从metainfo中取得下一个节点在文件中的偏移量，
    若偏移量为0，直接返回EXIT_META_NOT_FOUND_ERROR
  */
  int32_t pos = bucket_slot()[slot];
  for (; pos != 0;) {
    ret = file_op_->pread_file(reinterpret_cast<char *>(&meta_info),
                               sizeof(MetaInfo), pos);
    if (ret != LFS_SUCCESS) return ret;

    if (hash_compare(key, meta_info.get_key())) {
      current_offset = pos;
      return LFS_SUCCESS;
    }

    previous_offset = pos;
    pos = meta_info.get_next_meta_offset();
  }

  return EXIT_META_NOT_FOUND_ERROR;
}

int32_t IndexHandle::hash_insert(const uint64_t key, int32_t previous_offset,
                                 MetaInfo &meta) {
  int ret = LFS_SUCCESS;
  MetaInfo tmp_meta_info;
  // 1. 确定key存在的桶（slot）的位置
  int32_t slot = static_cast<int32_t>(key) % get_bucket_size();

  // 2. 确定meta节点存储在文件中的偏移量
  int32_t current_offset = get_index_header()->index_file_size_;
  get_index_header()->index_file_size_ += sizeof(MetaInfo);

  // 3. 将meta节点写入索引节点中
  meta.set_next_meta_offset(0);
  ret = file_op_->pwrite_file(reinterpret_cast<const char *>(&meta),
                              sizeof(MetaInfo), current_offset);
  if (ret != LFS_SUCCESS) {
    get_index_header()->index_file_size_ = sizeof(MetaInfo);
    return ret;
  }

  // 4. 将meta节点插入到哈希链表中

  // 当前一个节点已经存在
  if (previous_offset != 0) {
    ret = file_op_->pread_file(reinterpret_cast<char *>(&tmp_meta_info),
                               sizeof(MetaInfo), previous_offset);
    if (ret != LFS_SUCCESS) {
      get_index_header()->index_file_size_ -= sizeof(MetaInfo);
      return ret;
    }

    tmp_meta_info.set_next_meta_offset(current_offset);
    ret = file_op_->pwrite_file(reinterpret_cast<char *>(&tmp_meta_info),
                                sizeof(MetaInfo), previous_offset);
    if (ret != LFS_SUCCESS) {
      get_index_header()->index_file_size_ -= sizeof(MetaInfo);
      return ret;
    }
  } else {  // 不存在前一个节点
    bucket_slot()[slot] = current_offset;
  }

  return LFS_SUCCESS;
}

int IndexHandle::update_block_info(const OperatorType oper_type,
                                   const uint32_t modify_size) {
  if (get_block_info()->block_id_ == 0) return EXIT_BLOCKID_ZERO_ERROR;

  if (oper_type == C_OPER_INSERT) {
    get_block_info()->version_++;
    get_block_info()->file_count_++;
    get_block_info()->seq_no_++;
    get_block_info()->size_ += modify_size;
  }

  if (debug) {
    printf(
        "load blockid %d index successful, data file size: %d, index file "
        "size: %d, bucket_size: %d, free head offset: %d, seqno: %d, size:%d, "
        "filecount: %d, del_size: %d, del_file_count:%d, version: %d\n",
        get_block_info()->block_id_, get_index_header()->data_file_offset_,
        get_index_header()->index_file_size_, get_index_header()->bucket_size_,
        get_index_header()->free_head_offset_, get_block_info()->seq_no_,
        get_block_info()->size_, get_block_info()->file_count_,
        get_block_info()->del_size_, get_block_info()->del_file_count_,
        get_block_info()->version_);
  }
  return LFS_SUCCESS;
}

}  // namespace largefile
}  // namespace wjfeng
