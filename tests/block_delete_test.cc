/*
 * @Author: wjfeng wj.feng@tum.de
 * @Date: 2023-11-28 01:39:02
 * @LastEditors: wjfeng wj.feng@tum.de
 * @LastEditTime: 2023-11-28 02:47:40
 * @FilePath: /LargeFileSystem/tests/block_delete_test.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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

  // 2. 删除文件的MetaInfo
  uint64_t file_id = 0;
  std::cout << "Type your file_id" << std::endl;
  std::cin >> block_id;

  if (block_id < 1) {
    std::cerr << "Invalid blockid, exit." << std::endl;
    exit(-2);
  }

  ret = index_handle->delete_segment_meta(file_id);

  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "delete index failed, file_id: %lu, ret: %d\n", file_id,
            ret);
    exit(-3);
  }

  ret = index_handle->flush();
  if (ret != wjfeng::largefile::LFS_SUCCESS) {
    fprintf(stderr, "flush mainblock failed, file_id: %lu, ret: %d\n", file_id,
            ret);
    exit(-4);
  }

  printf("delete successfully!\n");
  delete index_handle;

  return 0;
}