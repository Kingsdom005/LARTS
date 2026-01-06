project(rtems_application)
cmake_minimum_required(VERSION 3.12)

# 设置编译器
#set(CMAKE_C_COMPILER    /home/dongl/code/RTEMS/toolchain/out/aarch64/bin/aarch64-linux-musl-gcc)
#set(CMAKE_CXX_COMPILER  /home/dongl/code/RTEMS/toolchain/out/aarch64/bin/aarch64-linux-musl-g++)
#set(CMAKE_C_COMPILER   /home/dongl/code/RTEMS/toolchain/out/arm/bin/arm-linux-musleabihf-gcc)
#set(CMAKE_CXX_COMPILER /home/dongl/code/RTEMS/toolchain/out/arm/bin/arm-linux-musleabihf-g++)
#set(CMAKE_C_COMPILER   /home/dongl/code/RTEMS/toolchain/out/loongarch/bin/loongarch64-linux-musl-gcc)
#set(CMAKE_CXX_COMPILER /home/dongl/code/RTEMS/toolchain/out/loongarch/bin/loongarch64-linux-musl-g++)

#set(CMAKE_C_COMPILER   /rtp/code/toolchain/out/arm/bin/arm-linux-musleabihf-gcc)
#set(CMAKE_CXX_COMPILER /rtp/code/toolchain/out/arm/bin/arm-linux-musleabihf-g++)
set(CMAKE_C_COMPILER    /rtp/code/toolchain/out/aarch64/bin/aarch64-linux-musl-gcc)
set(CMAKE_CXX_COMPILER  /rtp/code/toolchain/out/aarch64/bin/aarch64-linux-musl-g++)

### rtems_application
# 添加可执行文件
add_executable(rtems_application main.c)
# 链接到自定义的标准库
target_link_libraries(rtems_application libc.a)
# 设置连接器脚本
#target_link_options(rtems_application PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(rtems_application PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")


### image0
# 添加可执行文件
add_executable(image0 image0.c)
# 链接到自定义的标准库
target_link_libraries(image0 libc.a)
# 设置连接器脚本
#target_link_options(image0 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(image0 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### image1
# 添加可执行文件
add_executable(image1 image1.c)
# 链接到自定义的标准库
target_link_libraries(image1 libc.a)
# 设置连接器脚本
#target_link_options(image1 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(image1 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### image2
# 添加可执行文件
add_executable(image2 image2.c)
# 链接到自定义的标准库
target_link_libraries(image2 libc.a)
# 设置连接器脚本
#target_link_options(image2 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(image2 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")


### image3
# 添加可执行文件
add_executable(image3 image3.c)
# 链接到自定义的标准库
target_link_libraries(image3 libc.a)
# 设置连接器脚本
#target_link_options(image3 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(image3 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### image4
# 添加可执行文件
add_executable(image4 image4.c)
# 链接到自定义的标准库
target_link_libraries(image4 libc.a)
# 设置连接器脚本
#target_link_options(image4 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(image4 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### image5
# 添加可执行文件
add_executable(image5 image5.c)
# 链接到自定义的标准库
target_link_libraries(image5 libc.a)
# 设置连接器脚本
#target_link_options(image5 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(image5 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### ini
# 添加头文件索引路径
include_directories(./ini)
# 添加可执行文件
add_executable(ini_image ./ini/main.c ./ini/minIni.c)
# 为 ini_image 目标添加编译选项
target_compile_options(ini_image PRIVATE
#        -Os                    # 优化尺寸
#        -ffunction-sections    # 函数分离（便于GC）
#        -fdata-sections        # 数据分离（便于GC）
#        -fno-unwind-tables     # 禁用异常表（不影响加载）
#        -fno-asynchronous-unwind-tables # 禁用异步展开表
)
target_compile_definitions(ini_image PRIVATE
        NDEBUG                 # 禁用调试
        INI_READONLY           # 可读
#        INI_NOFLOAT            # 禁用浮点支持
##        INI_NOBROWSE           # 禁用浏览功能（如果不需要）
#        INI_BUFFERSIZE=256     # 减小缓冲区大小
)
# 链接到自定义的标准库
target_link_libraries(ini_image libc.a)
# 设置连接器脚本
target_link_options(ini_image PRIVATE
#        "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds"
        "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds"
#        -Wl,--gc-sections      # 垃圾回收未使用段（关键优化）
#        -Wl,--strip-debug      # 只剥离调试符号（保留基本符号）
)

# 添加可执行文件
add_executable(ini_image1 ./ini/main1.c ./ini/minIni.c)
# 为 ini_image 目标添加编译选项
target_compile_options(ini_image PRIVATE
        #        -Os                    # 优化尺寸
        #        -ffunction-sections    # 函数分离（便于GC）
        #        -fdata-sections        # 数据分离（便于GC）
        #        -fno-unwind-tables     # 禁用异常表（不影响加载）
        #        -fno-asynchronous-unwind-tables # 禁用异步展开表
)
target_compile_definitions(ini_image1 PRIVATE
        NDEBUG                 # 禁用调试
        INI_READONLY           # 可读
        #        INI_NOFLOAT            # 禁用浮点支持
        ##        INI_NOBROWSE           # 禁用浏览功能（如果不需要）
        #        INI_BUFFERSIZE=256     # 减小缓冲区大小
)
# 链接到自定义的标准库
target_link_libraries(ini_image1 libc.a)
# 设置连接器脚本
target_link_options(ini_image1 PRIVATE
#        "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds"
        "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds"
        #        -Wl,--gc-sections      # 垃圾回收未使用段（关键优化）
        #        -Wl,--strip-debug      # 只剥离调试符号（保留基本符号）
)


### rt
include_directories(./rt)
add_executable(rt0 ./rt/rt0.c)
add_executable(rt1 ./rt/rt1.c)
target_link_libraries(rt0 libc.a)
target_link_libraries(rt1 libc.a)
#target_link_options(rt0 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
#target_link_options(rt1 PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(rt0 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")
target_link_options(rt1 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### test thread
add_executable(thread_image ./thread-test/main.c)
target_link_libraries(thread_image libc.a)
#target_link_options(thread_image PRIVATE "-T/home/dongl/code/RTEMS/rtems/rtems-enhance/testsuites/samples/rtems_application/linkcmds")
target_link_options(thread_image PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)