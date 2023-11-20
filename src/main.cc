#include "common.h"
#include "file_op.h"
#include "mmap_file.h"
#include "mmap_file_op.h"

static const mode_t OPEN_MODE = 0644;
static const wjfeng::largefile::MMapOption mmap_option = {10240000, 4096, 4096};

int open_file(std::string file_name, int open_flags) {
  int fd = open(file_name.c_str(), open_flags, OPEN_MODE);
  if (fd < 0) return -errno;
  return fd;
}

int test_mmap_file() {
  const char *filename = "./mapfile_test.txt";

  int fd = open_file(filename, O_RDWR | O_CREAT | O_LARGEFILE);
  if (fd < 0) {
    fprintf(stderr, "open file failed, filename: %s\n, error desc: %s\n",
            filename, strerror(-fd));
    return -1;
  }

  wjfeng::largefile::MMapFile *map_file =
      new wjfeng::largefile::MMapFile(mmap_option, fd);

  bool is_mapped = map_file->map_file(true);
  if (is_mapped) {
    map_file->remap_file();
    memset(map_file->get_data(), '9', map_file->get_size());
    map_file->sync_file();
    map_file->munmap_file();
  } else {
    fprintf(stderr, "map file failed\n");
  }

  close(fd);
  std::cout << "hello world" << std::endl;
}

int test_file_op() {
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

  char buffer[64];
  memset(buffer, '8', 4096);
  int ret = fileOP->pwrite_file(buffer, 64, 128);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "pwrite_file: read length is less than required!");
    } else {
      fprintf(stderr, "pwrite file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  }
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

  fileOP->close_file();
}

int test_mmap_file_op() {
  int ret = 0;
  const char *filename = "mmap_file_op.txt";
  wjfeng::largefile::MMapFileOperation *mmfo =
      new wjfeng::largefile::MMapFileOperation{filename};

  int fd = mmfo->open_file();
  if (fd < 0) {
    fprintf(stderr, "open file %s failed, reason: %s\n", filename,
            strerror(-fd));
    exit(-1);
  }
 
  ret = mmfo->mmap_file(mmap_option);
  if (ret == wjfeng::largefile::LFS_ERROR) {
    fprintf(stderr, "mmap_file failed, reason: %s\n", strerror(errno));
    mmfo->close_file();
    exit(-2);
  }

  char buffer[128 + 1];
  memset(buffer, '6', 128);

  ret = mmfo->pwrite_file(buffer, 128, 8);
  if (ret < 0) {
    if (ret == wjfeng::largefile::EXIT_DISK_OPER_INCOMPLETE) {
      fprintf(stderr, "write_file: read length is less than required!");
    } else {
      fprintf(stderr, "write file %s failed, reason: %s\n", filename,
              strerror(-fd));
    }
  }

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

  ret = mmfo->flush_file();
  if (ret == wjfeng::largefile::LFS_ERROR) {
    fprintf(stderr, "flush file %s failed, reason: %s\n", filename,
            strerror(-fd));
  }
  
  mmfo->munmap_file();
  mmfo->close_file();
  return 0;
}

int main() {
  // test_mmap_file();
  // test_file_op();
  test_mmap_file_op();
  return 0;
}