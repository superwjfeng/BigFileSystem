#include "mmap_file_op.h"

#include "common.h"

static const wjfeng::largefile::MMapOption mmap_option = {
    10485760, 4096, 4096};  // 10MB, 4KB, 4KB

int main() {
  int ret = 0;
  const char *filename = "mmap_file_op.txt";
  wjfeng::largefile::MMapFileOperation *mmfo =
      new wjfeng::largefile::MMapFileOperation{filename};

  // test open 
  int fd = mmfo->open_file();
  if (fd < 0) {
    fprintf(stderr, "open file %s failed, reason: %s\n", filename,
            strerror(-fd));
    exit(-1);
  }

  // test mmap
  ret = mmfo->mmap_file(mmap_option);
  if (ret == wjfeng::largefile::LFS_ERROR) {
    fprintf(stderr, "mmap_file failed, reason: %s\n", strerror(errno));
    mmfo->close_file();
    exit(-2);
  }

  char buffer[128 + 1];
  memset(buffer, '6', 128);

  // test pwrite
  ret = mmfo->pwrite_file(buffer, 128, 8);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "write_file: read length is less than required!");
    } else {
      fprintf(stderr, "write file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  }

  // test pread
  memset(buffer, 0, 128);
  ret = mmfo->pread_file(buffer, 128, 8);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "pread_file: read length is less than required!");
    } else {
      fprintf(stderr, "pread file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  } else {
    buffer[128] = '\0';
    printf("read: %s\n", buffer);
  }

  // test flush
  ret = mmfo->flush_file();
  if (ret == wjfeng::largefile::LFS_ERROR) {
    fprintf(stderr, "flush file %s failed, reason: %s\n", filename,
            strerror(-fd));
  }

  mmfo->munmap_file();
  mmfo->unlink_file();
  mmfo->close_file();
  return 0;

  return 0;
}