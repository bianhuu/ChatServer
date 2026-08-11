#把.（对应当前目录）指定路径下的所有源文件名字放入变量名SRC_LIST里面（若需要编译的源文件数量过多，避免一个个写入）
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)

#表示生成可执行文件ChatServer，由SRC_LIST,DB_LIST}变量所定义的源文件编译而来
add_executable(ChatServer ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})
#表示ChatServer这个目标程序需链接muduo_net muduo_base pthread这三个库文件
target_link_libraries(ChatServer muduo_net muduo_base mysqlclient hiredis pthread)