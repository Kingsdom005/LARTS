#!/bin/sh

make clean || echo "Clean failed or not needed - continuing"
cmake ..
make
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

mkdir -p bin
cd ./bin

for f in image0 image1 image2 image3 image4 image5 rtems_application ini_image ini_image1 rt0 rt1 thread_image; do
    # out=/home/dongl/code/RTEMS/rtems/rtems-enhance/cpukit/include/rtems/enhancetask/image/${f}.h
    out=/rtp/code/rtems/cpukit/include/rtems/enhancetask/image/${f}.h
    xxd -i "$f" > "$out"
    sed -i 's/unsigned int \('"$f"_len'\)/const unsigned int \1/' "$out"
done

echo "Build succeeded"