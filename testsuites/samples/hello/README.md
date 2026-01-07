# Porting NVMe to RTEMS 5.1

## How to use?

The folder contains the following files, using header files only to test nvme.

```
rtems-nvme-bsp
    ├── fs-test.h
    ├── init.c
    ├── io.h
    ├── nvme-bsp.h
    ├── nvmedisk.h
    └── README.md
```

## How to run?

```
dd if=/dev/zero of=nvme.img bs=1M count=256

qemu-system-i386 -m 512M \
    -drive file=nvme.img,if=none,id=nvme \
    -device nvme,serial=deadbeef,drive=nvme \
    -kernel ./build/i386-rtems5/c/pc386/testsuites/samples/nvme.exe

qemu-system-i386 -nographic -no-reboot -m 512M -append "--video=off --console=/dev/com1"\
                                 -drive file=nvme.img,if=none,id=nvme \
                                 -device nvme,serial=deadbeef,drive=nvme \
                                 -kernel ./rtems/kernels-5/pcp-386/i386-rtems5/c/pc386/testsuites/samples/hello.exe
```

Comment out `#define TEST_WRITE` in `fs-test.h` to check if persistence works.

## Notes

currently only i386 is tested. arm is not supported as PCI bus is required.

qemu-system-arm will yell: `No 'PCI' bus found for device 'nvme'`

Enjoy :-)
