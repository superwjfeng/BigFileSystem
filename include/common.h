#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

namespace wjfeng {
namespace largefile {
const int32_t TFS_SUCCESS = 0;
const int32_t EXIT_DISK_OPER_INCOMPLETE =
    -8012;  // read or write length is less than required
}
}  // namespace wjfeng
#endif  // _COMMON_H_INCLUDED_