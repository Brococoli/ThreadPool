 
cmake_minimum_required(VERSION 2.8)

project(ThreadPool)

add_compile_options(-std=c++11)
add_compile_options(-g3)

include_directories(include)
aux_source_directory(. SRC_LIST)

set(THREAD_POOL src/thread_pool.cc src/thread.cc src/utils.cc ./main.cc)
# add_subdirectory(src)

add_executable(thread_pool ${SRC_LIST} ${THREAD_POOL})
# add_executable(thread_pook ${SRC_LIST})
target_link_libraries(thread_pool pthread)

