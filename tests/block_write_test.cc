#include "common.h"
#include "index_handle.h"

static const wjfeng::largefile::MMapOption mmap_option = {
    10485760, 4096, 4096};                                 // 10MB, 4KB, 4KB
const static uint32_t main_block_size = 1024 * 1024 * 64;  // 64MB
const static uint32_t bucket_size = 1000;                  // 1000 个哈希桶
static int32_t block_id = 1;
static int debug = 1;

int main(int argc, char **argv) {
  std::string mainblock_path;
  std::string index_path;
  int32_t ret = wjfeng::largefile::LFS_SUCCESS;

  std::cout << "Type your blockid: " << std::endl;
  std::cin >> block_id;

  if (block_id < 1) {
    std::cerr << "Invalid bockid, exit." << std::endl;
    exit(-1);
  }

  // 1. 加载索引文件
  wjfeng::largefile::IndexHandle *index_handle =
      new wjfeng::largefile::IndexHandle(".", block_id);

  if (debug) printf("Init index ...\n");
  ret = index_handle->load(block_id, bucket_size, mmap_option);
  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "load index %d failed", block_id);
    delete index_handle;
    exit(-2);
  }

  // 2. 写入文件到main block
  std::stringstream tmp_stream;
  tmp_stream << "." << wjfeng::largefile::MAINBLOCK_DIR_PREFIX << block_id;
  tmp_stream >> mainblock_path;

  wjfeng::largefile::FileOperation *mainblock =
      new wjfeng::largefile::FileOperation(mainblock_path,
                                           O_RDWR | O_LARGEFILE | O_CREAT);
  char buffer[4096];
  memset(buffer, '6', sizeof(buffer));

  int32_t data_offset = index_handle->get_block_data_offset();
  uint32_t file_no = index_handle->get_block_info()->seq_no_;
  if ((ret = mainblock->pwrite_file(buffer, sizeof(buffer), data_offset)) !=
      wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "write to main block failed, ret: %d, reason: %s\n", ret,
            strerror(errno));
    mainblock->close_file();

    delete mainblock;
    delete index_handle;
    exit(-3);
  }

  // 3. 索引文件IndexHandle中写入新的Metainfo
  wjfeng::largefile::MetaInfo meta;
  meta.set_file_id(file_no);
  meta.set_offset(data_offset);
  meta.set_size(sizeof(buffer));

  ret = index_handle->write_segment_meta(meta.get_key(), meta);
  if (ret == wjfeng::largefile::LFS_SUCCESS) {
    // 1. 更新索引信息
    index_handle->commit_block_data_offset(sizeof(buffer));
    // 2. 更新块信息，插入还是删除操作？
    index_handle->update_block_info(wjfeng::largefile::C_OPER_INSERT,
                                    sizeof(buffer));
    ret = index_handle->flush();
    if (ret != wjfeng::largefile::LFS_SUCCESS) {
      fprintf(stderr, "flush mainblock %d failed, file no: %u\n", block_id,
              file_no);
    }
  } else {
    fprintf(stderr, "write_segment_meta - mainblock %d failed, file no: %u\n",
            block_id, file_no);
  }

  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "write to mainblock %d failed, file no: %u\n", block_id,
            file_no);
  } else {
    if (debug)
      printf("write successfully, file no: %u, block_id: %d\n", file_no,
             block_id);
  }

  mainblock->close_file();

  delete mainblock;
  delete index_handle;

  return 0;
}
