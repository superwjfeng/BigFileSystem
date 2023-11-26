add_executable(mmap_file_test mmap_file_test.cc ../src/mmap_file.cc)
add_executable(file_op_test file_op_test.cc ../src/file_op.cc)
add_executable(mmap_file_op_test mmap_file_op_test.cc ../src/mmap_file_op.cc ../src/file_op.cc ../src/mmap_file.cc)