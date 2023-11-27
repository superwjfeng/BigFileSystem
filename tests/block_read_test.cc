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

  if (debug) printf("load index ...\n");
  ret = index_handle->load(block_id, bucket_size, mmap_option);
  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "load index %d failed", block_id);
    delete index_handle;
    exit(-2);
  }

  // 2. 读取文件的MetaInfo
  uint64_t file_id = 0;
  std::cout << "Type your file_id" << std::endl;
  std::cin >> block_id;

  if (block_id < 1) {
    std::cerr << "Invalid blockid, exit." << std::endl;
    exit(-2);
  }

  wjfeng::largefile::MetaInfo meta;

  ret = index_handle->read_segment_meta(file_id, meta);
  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "read_segment_meta error, file_id: %ld, ret: %d\n", file_id,
            ret);
    exit(-3);
  }

  // 3. 根据meta读取文件
  std::stringstream tmp_stream;
  tmp_stream << "." << wjfeng::largefile::MAINBLOCK_DIR_PREFIX << block_id;
  tmp_stream >> mainblock_path;

  wjfeng::largefile::FileOperation *mainblock =
      new wjfeng::largefile::FileOperation(mainblock_path, O_RDWR);

  char *buffer = new char[meta.get_size() + 1];
  ret = mainblock->pread_file(buffer, meta.get_size(), meta.get_offset());
  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "read from main block failed, ret: %d, reason: %s\n", ret,
            strerror(errno));
    mainblock->close_file();

    delete mainblock;
    delete index_handle;
    exit(-3);
  }

  buffer[meta.get_size()] = '\0';
  printf("read: %s\n", buffer);

  mainblock->close_file();
  delete mainblock;
  delete index_handle;

  return 0;
}
