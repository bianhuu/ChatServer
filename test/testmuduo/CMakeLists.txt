#配置头文件搜索路径
include_directories(/home/lfz/packages/muduo-2.0.2)
#配置库文件搜索路径
link_directories(/home/lfz/packages/build/release-cpp11/lib)

#设置可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#设置需要编译的源文件列表
set(SRC_LIST ./testmuduo.cpp)

#把.指定路径下的所有源文件名字放入变量名SRC_LIST里面（若需要编译的源文件数量过多，避免一个个写入）
#aux_source_directory(. SRC_LIST)

#表示生成可执行文件server，由SRC_LIST变量所定义的源文件编译而来
add_executable(server ${SRC_LIST})
#表示server这个目标程序需链接muduo_net muduo_base pthread这三个库文件
target_link_libraries(server muduo_net muduo_base pthread)