project(rtems_application)
cmake_minimum_required(VERSION 3.12)

# Set up the compiler
#set(CMAKE_C_COMPILER   /rtp/code/toolchain/out/arm/bin/arm-linux-musleabihf-gcc)
#set(CMAKE_CXX_COMPILER /rtp/code/toolchain/out/arm/bin/arm-linux-musleabihf-g++)
set(CMAKE_C_COMPILER    /rtp/code/toolchain/out/aarch64/bin/aarch64-linux-musl-gcc)
set(CMAKE_CXX_COMPILER  /rtp/code/toolchain/out/aarch64/bin/aarch64-linux-musl-g++)
#set(CMAKE_C_COMPILER   /rtp/code/toolchain/out/loongarch/bin/loongarch64-linux-musl-gcc)
#set(CMAKE_CXX_COMPILER /rtp/code/toolchain/out/loongarch/bin/loongarch64-linux-musl-g++)

### image0
# Add executable file
add_executable(image0 image0.c)
# Link to a custom standard library
target_link_libraries(image0 libc.a)
# Set up the link script
target_link_options(image0 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### image1
# Add executable file
add_executable(image1 image1.c)
# Link to a custom standard library
target_link_libraries(image1 libc.a)
# Set up the link script
target_link_options(image1 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

### test thread
add_executable(thread_image ./thread-test/main.c)
target_link_libraries(thread_image libc.a)
target_link_options(thread_image PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")

# Set the output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)