project(rtems_application)
cmake_minimum_required(VERSION 3.12)

# Set up the compiler

set(CMAKE_C_COMPILER   /rtp/code/toolchain/out/arm/bin/arm-linux-musleabihf-gcc)
set(CMAKE_CXX_COMPILER /rtp/code/toolchain/out/arm/bin/arm-linux-musleabihf-g++)

#set(CMAKE_C_COMPILER    /rtp/code/toolchain/out/aarch64/bin/aarch64-linux-musl-gcc)
#set(CMAKE_CXX_COMPILER  /rtp/code/toolchain/out/aarch64/bin/aarch64-linux-musl-g++)

#set(CMAKE_C_COMPILER   /rtp/code/toolchain/out/loongarch/bin/loongarch64-linux-musl-gcc)
#set(CMAKE_CXX_COMPILER /rtp/code/toolchain/out/loongarch/bin/loongarch64-linux-musl-g++)


### image0
# Add executable file
add_executable(image0
  image0.c
)

# Link to a custom standard library
target_link_libraries(image0 libc.a)
#target_link_libraries(rtems_application libc.a libstdc++.a)

# Set up the link script
target_link_options(image0 PRIVATE "-T/rtp/code/rtems/testsuites/samples/rtems_application/linkcmds")


# Set the output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
