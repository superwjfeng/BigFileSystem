#include "mmap_file.h"

static int debug = 1;  // debug mode close if 0, open if 1

namespace wjfeng {
namespace largefile {
MMapFile::MMapFile() : size_(0), fd_(-1), data_(nullptr) {}
MMapFile::MMapFile(const int &fd) : size_(0), fd_(fd), data_(nullptr) {}
MMapFile::MMapFile(const MMapOption &mmap_option, const int &fd)
    : size_(0), fd_(fd), data_(nullptr) {
  mmap_file_option_.max_mmap_size_ = mmap_option.max_mmap_size_;
  mmap_file_option_.first_mmap_size_ = mmap_option.first_mmap_size_;
  mmap_file_option_.per_mmap_size_ = mmap_option.per_mmap_size_;
}
MMapFile::~MMapFile() {
  if (data_) {
    if (debug) {
      std::cout << "mmap file destruct, fd: " << fd_
                << ", maped size: " << size_ << ", data: " << data_
                << std::endl;
    }
    msync(data_, size_, MS_SYNC);
    munmap(data_, size_);

    size_ = 0;
    data_ = nullptr;
    fd_ = -1;
    mmap_file_option_.max_mmap_size_ = 0;
    mmap_file_option_.first_mmap_size_ = 0;
    mmap_file_option_.per_mmap_size_ = 0;
  }
}

bool MMapFile::sync_file() {
  if (data_ != nullptr && size_ > 0) {
    return msync(data_, size_, MS_ASYNC) == 0;
  }
  return true;  // no sync
}

bool MMapFile::map_file(const bool &write) {
  int flags = PROT_READ;
  if (write) flags |= PROT_WRITE;

  // check parameters for mmap
  if (fd_ < 0) return false;
  if (mmap_file_option_.max_mmap_size_ == 0) return false;
  if (size_ < mmap_file_option_.max_mmap_size_)
    size_ = mmap_file_option_.first_mmap_size_;
  else
    size_ = mmap_file_option_.max_mmap_size_;

  if (!ensure_file_size(size_)) {
    fprintf(stderr, "ensure file size failed in map_file, size: %d\n", size_);
    return false;
  }
  data_ = mmap(0, size_, flags, MAP_SHARED, fd_, 0);
  if (MAP_FAILED == data_) {
    fprintf(stderr, "map file failed: %s\n", strerror(errno));
    size_ = 0;
    data_ = nullptr;
    fd_ = -1;
    return false;
  }
  if (debug) {
    std::cout << "mmap file successed, fd: " << fd_ << ", maped size: " << size_
              << ", data: " << data_ << std::endl;
  }
  return true;
}

void *MMapFile::get_data() const { return data_; }
int32_t MMapFile::get_size() const { return size_; }

bool MMapFile::munmap_file() {
  if (munmap(data_, size_) == 0)
    return true;
  else
    return false;
}

bool MMapFile::remap_file() {
  if (fd_ < 0 || data_ == nullptr) {
    fprintf(stderr, "mremap not mapped yet\n");
    return false;
  }
  if (mmap_file_option_.max_mmap_size_ == size_) {
    fprintf(stderr, "already mapped max size, now size: %d, max size: %d\n",
            size_, mmap_file_option_.max_mmap_size_);
    return false;
  }
  int new_size = size_ + mmap_file_option_.per_mmap_size_;
  if (new_size > mmap_file_option_.max_mmap_size_) {
    new_size = mmap_file_option_.max_mmap_size_;
  }
  if (!ensure_file_size(new_size)) {
    fprintf(stderr, "ensure file size failed in remap_file, new_size: %d\n",
            new_size);
    return false;
  }

  if (debug) {
    std::cout << "mmap file destruct, fd: " << fd_ << ", maped size: " << size_
              << ", data: " << data_ << std::endl;
  }
  void *new_map_data = mremap(data_, size_, new_size, MREMAP_MAYMOVE);
  if (new_map_data == MAP_FAILED) {
    fprintf(stderr, "mremap failed, fd: %d, new size: %d, error desc: %s\n",
            fd_, new_size, strerror(errno));
    return false;
  } else {
    if (debug) {
      std::cout << "remap file successed, fd: " << fd_
                << ", maped size: " << size_ << ", data: " << data_
                << std::endl;
    }
  }
  data_ = new_map_data;
  size_ = new_size;
  return true;
}

bool MMapFile::ensure_file_size(const int32_t &size) const {
  struct stat s;
  if (fstat(fd_, &s) < 0) {
    fprintf(stderr, "fstat error, error desc: %s\n", strerror(errno));
    return false;
  }
  if (s.st_size < size) {
    if (ftruncate(fd_, size)) {  // 扩容
      fprintf(stderr, "ftruncate error, size: %d, error desc: %s\n", size,
              strerror(errno));
      return false;
    }
  }
  return true;
}
}  // namespace largefile
}  // namespace wjfeng