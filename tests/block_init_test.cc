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

  // 1. 创建索引文件
  wjfeng::largefile::IndexHandle *index_handle =
      new wjfeng::largefile::IndexHandle(".", block_id);

  if (debug) printf("Init index ...\n");
  ret = index_handle->create(block_id, bucket_size, mmap_option);
  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "create index %d failed", block_id);
    delete index_handle;
    exit(-2);
  }

  // 2. 生成main block文件
  std::stringstream tmp_stream;
  tmp_stream << "." << wjfeng::largefile::MAINBLOCK_DIR_PREFIX << block_id;
  tmp_stream >> mainblock_path;

  wjfeng::largefile::FileOperation *mainblock =
      new wjfeng::largefile::FileOperation(mainblock_path,
                                           O_RDWR | O_LARGEFILE | O_CREAT);
  ret = mainblock->ftruncate_file(main_block_size);

  if (ret != 0) {
    fprintf(stderr, "create main block %s failed. reason: %s\n",
            mainblock_path.c_str(), strerror(errno));
    delete mainblock;
    index_handle->remove(block_id);
    exit(-3);
  }

  // 3. 清理操作
  mainblock->close_file();
  index_handle->flush();
  delete mainblock;
  delete index_handle;

  return 0;
}
