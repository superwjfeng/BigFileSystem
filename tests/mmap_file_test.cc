#include "mmap_file.h"

static const mode_t OPEN_MODE = 0644;  // mode_t 封装了 unsigned int
static const wjfeng::largefile::MMapOption mmap_option = {
    10485760, 4096, 4096};  // 10MB, 4KB, 4KB

int open_file(std::string file_name, int open_flags) {
  int fd = open(file_name.c_str(), open_flags, OPEN_MODE);
  if (fd < 0) return -errno;
  return fd;
}

int unlink_file(const std::string &file_name) {
  return unlink(file_name.c_str());
}

int main() {
  const char *filename = "mapfile_test.txt";

  int fd = open_file(filename, O_RDWR | O_CREAT | O_LARGEFILE);
  if (fd < 0) {
    // errno是一个全局的变量，防止多线程下被覆盖，所以采用-fd的方式
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
  unlink_file(filename);
  
  return 0;
}