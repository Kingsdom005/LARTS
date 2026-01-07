#
# Samples
#

ACLOCAL_AMFLAGS = -I ../aclocal

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am

samples =
sample_screens =
sample_docs =

support_includes = -I$(top_srcdir)/../support/include

# 编译内核
if TEST_rtems_kernel
    samples += rtems_kernel
    sample_screens += rtems_kernel/rtems_kernel.scn
    sample_docs += rtems_kernel/rtems_kernel.doc
    rtems_kernel_SOURCES = rtems_kernel/init.c
    rtems_kernel_CPPFLAGS = $(AM_CPPFLAGS) $(TEST_FLAGS_rtems_kernel) \
         $(support_includes)
endif

#if TEST_rtems_rtp
#    samples += rtems_rtp
#    sample_screens += rtems_rtp/rtems_rtp.scn
#    sample_docs += rtems_rtp/rtems_rtp.doc
#    rtems_rtp_SOURCES = rtems_rtp/init.c
#    rtems_rtp_CPPFLAGS = $(AM_CPPFLAGS) $(TEST_FLAGS_rtems_rtp) \
#         $(support_includes) \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/xilinx_zynq_a9_qemu/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/cpukit/include/rtems \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/bsps/arm/include
#endif


#if TEST_rtems_disk
#    samples += rtems_disk
#    sample_screens += rtems_disk/rtems_disk.scn
#    sample_docs += rtems_disk/rtems_disk.doc
#    rtems_disk_SOURCES = rtems_disk/init.c
#    rtems_disk_CPPFLAGS = $(AM_CPPFLAGS) $(TEST_FLAGS_rtems_disk) \
#         $(support_includes) \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/xilinx_zynq_a9_qemu/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/cpukit/include/rtems \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/bsps/arm/include
#endif
#
#
#if TEST_rtems_context
#    samples += rtems_context
#    sample_screens += rtems_context/rtems_context.scn
#    sample_docs += rtems_context/rtems_context.doc
#    rtems_context_SOURCES = rtems_context/init.c
#    rtems_context_CPPFLAGS = $(AM_CPPFLAGS) $(TEST_FLAGS_rtems_context) \
#         $(support_includes) \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/xilinx_zynq_a9_qemu/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/cpukit/include/rtems \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/bsps/arm/include
#endif


#if TEST_rtems_kernel_separation
#    samples += rtems_kernel_separation
#    sample_screens += rtems_kernel_separation/rtems_kernel_separation.scn
#    sample_docs += rtems_kernel_separation/rtems_kernel_separation.doc
#    rtems_kernel_separation_SOURCES = rtems_kernel_separation/application.c
#    rtems_kernel_separation_CPPFLAGS = $(AM_CPPFLAGS) $(TEST_FLAGS_rtems_kernel_separationt) \
#         $(support_includes) \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/xilinx_zynq_a9_qemu/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/cpukit/include/rtems \
#         -I/mnt/code/development/rtems/5/arm/arm-rtems5/lib/include \
#         -I/mnt/code/development/rtems/rtems-rtp/bsps/arm/include
#endif

noinst_PROGRAMS = $(samples)
