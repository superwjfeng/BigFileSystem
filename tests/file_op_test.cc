#include "file_op.h"

int main() {
  const char *filename = "file_op.txt";
  wjfeng::largefile::FileOperation *fileOP =
      new wjfeng::largefile::FileOperation(filename,
                                           O_CREAT | O_RDWR | O_LARGEFILE);
  int fd = fileOP->open_file();
  if (fd < 0) {
    fprintf(stderr, "open file %s failed, reason: %s\n", filename,
            strerror(-fd));
    exit(-1);
  }

  // write test

  // char buffer[4096];
  // memset(buffer, '6', 4096);
  // int ret = fileOP->pwrite_file(buffer, 4096, 128);
  char buffer[64];
  memset(buffer, '8', 64);
  int ret = fileOP->pwrite_file(buffer, 64, 128);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "pwrite_file: read length is less than required!");
    } else {
      fprintf(stderr, "pwrite file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  }

  // read test

  memset(buffer, 0, 64);
  ret = fileOP->pread_file(buffer, 64, 128);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "pread_file: read length is less than required!");
    } else {
      fprintf(stderr, "pread file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  } else {
    buffer[64] = '\0';
    printf("read: %s\n", buffer);
  }

  // test write, 文件指针从头开始
  memset(buffer, '9', 64);
  ret = fileOP->write_file(buffer, 64);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "write_file: read length is less than required!");
    } else {
      fprintf(stderr, "write file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  }

  fileOP->unlink_file();
  fileOP->close_file();

  return 0;
}