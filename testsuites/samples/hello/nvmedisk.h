/**
 * NVMe Disk Block Device API
 */

#ifndef _RTEMS_NVMEDISK_H
#define _RTEMS_NVMEDISK_H

// Maxul 2023-4-12 
// XXX: use a single function for nvmedisk_write/read
// XXX: use a worker thread for async I/O

#include <rtems.h>
#include <rtems/blkdev.h>

typedef struct rtems_nvmedisk_config
{
    uint32_t block_size;
    rtems_blkdev_bnum block_num;
} rtems_nvmedisk_config;

extern rtems_nvmedisk_config rtems_nvmedisk_configuration[];
extern size_t rtems_nvmedisk_configuration_size;

rtems_device_driver nvmedisk_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *arg);

/**
 * NVMe disk driver table entry.
 */
#define NVMEDISK_DRIVER_TABLE_ENTRY                  \
    {                                                \
        .initialization_entry = nvmedisk_initialize, \
        RTEMS_GENERIC_BLOCK_DEVICE_DRIVER_ENTRIES    \
    }

#define NVMEDISK_DEVICE_BASE_NAME "/dev/nvme"

typedef struct nvmedisk
{
    uint32_t block_size;
    rtems_blkdev_bnum block_num;
} nvmedisk;

int nvmedisk_ioctl(rtems_disk_device *dd, uint32_t req, void *argp);

rtems_device_driver
nvmedisk_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor __attribute__((unused)),
    void *arg __attribute__((unused)))
{
    rtems_device_minor_number i;
    rtems_nvmedisk_config *c = rtems_nvmedisk_configuration;
    struct nvmedisk *r;
    rtems_status_code rc;

    rc = rtems_disk_io_initialize();
    if (rc != RTEMS_SUCCESSFUL)
        return rc;

    /*
     * Coverity Id 27 notes that this calloc() is a resource leak.
     *
     * This is allocating memory for a NVMe disk which will persist for
     * the life of the system. RTEMS has no "de-initialize" driver call
     * so there is no corresponding free(r).  Coverity is correct that
     * it is never freed but this is not a problem.
     */
    r = calloc(rtems_nvmedisk_configuration_size, sizeof(struct nvmedisk));
    for (i = 0; i < rtems_nvmedisk_configuration_size; i++, c++, r++)
    {
        dev_t dev = rtems_filesystem_make_dev_t(major, i);
        char name[] = NVMEDISK_DEVICE_BASE_NAME "0";
        name[sizeof(NVMEDISK_DEVICE_BASE_NAME) - 1] += i;
        r->block_size = c->block_size;
        r->block_num = c->block_num;

        rc = rtems_disk_create_phys(dev, c->block_size, c->block_num,
                                    nvmedisk_ioctl, r, name);

        if (rc != RTEMS_SUCCESSFUL)
            return rc;
    }
    printk("%s\n", __func__);
    nvme_scan(); // XXX
    return RTEMS_SUCCESSFUL;
}

// #define RTEMS_NVMEDISK_TRACE
static void
rtems_nvmedisk_printf(const nvmedisk *nvmed, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("nvmedisk:");
    vprintf(format, args);
    printf("\n");
}

static int
nvmedisk_read(struct nvmedisk *nvmed, rtems_blkdev_request *req)
{
    uint32_t i;
    rtems_blkdev_sg_buffer *sg;

#ifdef RTEMS_NVMEDISK_TRACE
    rtems_nvmedisk_printf(nvmed, "nvmedisk read: start=%d, blocks=%d",
                          req->bufs[0].block, req->bufnum);
#endif

    for (i = 0, sg = req->bufs; i < req->bufnum; i++, sg++)
    {
#ifdef RTEMS_NVMEDISK_TRACE
        rtems_nvmedisk_printf(nvmed, "nvmedisk read: block=%d length=%d off=%d addr=%p",
                              sg->block, sg->length, sg->block * nvmed->block_size,
                              sg->block * nvmed->block_size);
#endif
        // memcpy(sg->buffer, from + (sg->block * nvmed->block_size), sg->length);
        // nvme_prpl_xfer(ns, sg->block, sg->buffer, sg->length / ns->block_size, 0);
        {
            int id;
            int cnt = sg->length / ns->block_size;
            u64 lba = sg->block;
            for (id = 0; id < cnt;)
            {
                u16 blocks_remaining = cnt - id;
                char *op_buf = sg->buffer + id * ns->block_size;
                int blocks = nvme_prpl_xfer(ns, lba + id, op_buf, blocks_remaining, 0);
                if (blocks < 0)
                    return -1;
                id += blocks;
            }
        }
    }
    rtems_blkdev_request_done(req, RTEMS_SUCCESSFUL);
    return 0;
}

static int
nvmedisk_write(struct nvmedisk *nvmed, rtems_blkdev_request *req)
{
    uint32_t i;
    rtems_blkdev_sg_buffer *sg;

#ifdef RTEMS_NVMEDISK_TRACE
    rtems_nvmedisk_printf(nvmed, "nvmedisk write: start=%d, blocks=%d",
                          req->bufs[0].block, req->bufnum);
#endif
    for (i = 0, sg = req->bufs; i < req->bufnum; i++, sg++)
    {
#ifdef RTEMS_NVMEDISK_TRACE
        rtems_nvmedisk_printf(nvmed, "nvmedisk write: block=%d length=%d off=%d addr=%p",
                              sg->block, sg->length, sg->block * nvmed->block_size,
                              sg->block * nvmed->block_size);
#endif
        // memcpy(to + (sg->block * nvmed->block_size), sg->buffer, sg->length);
        // nvme_prpl_xfer(ns, sg->block, sg->buffer, sg->length / ns->block_size, 1);
        {
            int id;
            int cnt = sg->length / ns->block_size;
            u64 lba = sg->block;
            for (id = 0; id < cnt;)
            {
                u16 blocks_remaining = cnt - id;
                char *op_buf = sg->buffer + id * ns->block_size;
                int blocks = nvme_prpl_xfer(ns, lba + id, op_buf, blocks_remaining, 1);
                if (blocks < 0)
                    return -1;
                id += blocks;
            }
        }
    }
    rtems_blkdev_request_done(req, RTEMS_SUCCESSFUL);
    return 0;
}

int nvmedisk_ioctl(rtems_disk_device *dd, uint32_t req, void *argp)
{
    struct nvmedisk *nvmed = rtems_disk_get_driver_data(dd);

    switch (req)
    {
    case RTEMS_BLKIO_REQUEST:
    {
        rtems_blkdev_request *r = argp;

        switch (r->req)
        {
        case RTEMS_BLKDEV_REQ_READ:
            return nvmedisk_read(nvmed, r);

        case RTEMS_BLKDEV_REQ_WRITE:
            return nvmedisk_write(nvmed, r);

        default:
            errno = EINVAL;
            return -1;
        }
        break;
    }

    default:
        return rtems_blkdev_ioctl(dd, req, argp);
        break;
    }

    errno = EINVAL;
    return -1;
}

#endif /* _RTEMS_NVMEDISK_H */

