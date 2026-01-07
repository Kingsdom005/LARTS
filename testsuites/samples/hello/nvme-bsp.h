// Low level NVMe disk access
//
// Copyright 2017 Amazon.com, Inc. or its affiliates.
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#ifndef __NVME_BSP_H__
#define __NVME_BSP_H__

////////////////////////////////////////////////////////////////////////////////

#include <bsp/irq.h>
#include <rtems/pci.h>
#include <rtems/malloc.h>
#include <rtems/pci.h>

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef signed char s8;
typedef signed short int s16;
typedef signed int s32;
typedef signed long long int s64;

#include "io.h"

/* Data structures */

/* The register file of a NVMe host controller. This struct follows the naming
   scheme in the NVMe specification. */
struct nvme_reg
{
    u64 cap;   /* controller capabilities */
    u32 vs;    /* version */
    u32 intms; /* interrupt mask set */
    u32 intmc; /* interrupt mask clear */
    u32 cc;    /* controller configuration */
    u32 _res0;
    u32 csts; /* controller status */
    u32 _res1;
    u32 aqa; /* admin queue attributes */
    u64 asq; /* admin submission queue base address */
    u64 acq; /* admin completion queue base address */
};

/* Submission queue entry */
struct nvme_sqe
{
    union
    {
        u32 dword[16];
        struct
        {
            u32 cdw0; /* Command DWORD 0 */
            u32 nsid; /* Namespace ID */
            u64 _res0;
            u64 mptr; /* metadata ptr */

            u64 dptr_prp1;
            u64 dptr_prp2;
        };
    };
};

/* Completion queue entry */
struct nvme_cqe
{
    union
    {
        u32 dword[4];
        struct
        {
            u32 cdw0;
            u32 _res0;
            u16 sq_head;
            u16 sq_id;
            u16 cid;
            u16 status;
        };
    };
};

/* The common part of every submission or completion queue. */
struct nvme_queue
{
    u32 *dbl; /* doorbell */
    u16 mask; /* length - 1 */
};

struct nvme_cq
{
    struct nvme_queue common;
    struct nvme_cqe *cqe;

    /* We have read upto (but not including) this entry in the queue. */
    u16 head;

    /* The current phase bit the controller uses to indicate that it has written
       a new entry. This is inverted after each wrap. */
    unsigned phase : 1;
};

struct nvme_sq
{
    struct nvme_queue common;
    struct nvme_sqe *sqe;

    /* Corresponding completion queue. We only support a single SQ per CQ. */
    struct nvme_cq *cq;

    /* The last entry the controller has fetched. */
    u16 head;

    /* The last value we have written to the tail doorbell. */
    u16 tail;
};

struct nvme_ctrl
{
    // struct pci_device *pci;
    void *pci;
    struct nvme_reg volatile *reg;

    u32 doorbell_stride; /* in bytes */

    struct nvme_sq admin_sq;
    struct nvme_cq admin_cq;

    u32 ns_count;

    struct nvme_sq io_sq;
    struct nvme_cq io_cq;
};

struct nvme_namespace
{
    // struct drive_s drive;
    struct nvme_ctrl *ctrl;

    u32 ns_id;

    u64 lba_count; /* The total amount of sectors. */

    u32 block_size;
    u32 metadata_size;
    u32 max_req_size;
};

/* Data structures for NVMe admin identify commands */

struct nvme_identify_ctrl
{
    u16 vid;
    u16 ssvid;
    char sn[20];
    char mn[40];
    char fr[8];

    u8 rab;
    u8 ieee[3];
    u8 cmic;
    u8 mdts;

    char _boring[516 - 78];

    u32 nn; /* number of namespaces */
};

struct nvme_identify_ns_list
{
    u32 ns_id[1024];
};

struct nvme_lba_format
{
    u16 ms;
    u8 lbads;
    u8 rp;
};

struct nvme_identify_ns
{
    u64 nsze;
    u64 ncap;
    u64 nuse;
    u8 nsfeat;
    u8 nlbaf;
    u8 flbas;

    char _boring[128 - 27];

    struct nvme_lba_format lbaf[16];
};

union nvme_identify
{
    struct nvme_identify_ns ns;
    struct nvme_identify_ctrl ctrl;
    struct nvme_identify_ns_list ns_list;
};

/* NVMe constants */

#define NVME_CAP_CSS_NVME (1ULL << 37)

#define NVME_CSTS_FATAL (1U << 1)
#define NVME_CSTS_RDY (1U << 0)

#define NVME_CC_EN (1U << 0)

#define NVME_SQE_OPC_ADMIN_CREATE_IO_SQ 1U
#define NVME_SQE_OPC_ADMIN_CREATE_IO_CQ 5U
#define NVME_SQE_OPC_ADMIN_IDENTIFY 6U

#define NVME_SQE_OPC_IO_WRITE 1U
#define NVME_SQE_OPC_IO_READ 2U

#define NVME_ADMIN_IDENTIFY_CNS_ID_NS 0U
#define NVME_ADMIN_IDENTIFY_CNS_ID_CTRL 1U
#define NVME_ADMIN_IDENTIFY_CNS_GET_NS_LIST 2U

#define NVME_CQE_DW3_P (1U << 16)

#define NVME_PAGE_SIZE 4096
#define NVME_PAGE_MASK ~(NVME_PAGE_SIZE - 1)

/* Length for the queue entries. */
#define NVME_SQE_SIZE_LOG 6
#define NVME_CQE_SIZE_LOG 4

#define dprintf(unused, fmt, ...) // printk(fmt, ##__VA_ARGS__)
#define warn_timeout()        \
    {                         \
        puts("warn_timeout"); \
    }
#define warn_noalloc()        \
    {                         \
        puts("warn_noalloc"); \
    }
#define warn_internalerror()        \
    {                               \
        puts("warn_internalerror"); \
    }

#define PCI_BASE_ADDRESS_0 0x10 /* 32 bits */
#define MAXDESCSIZE 80
#define DTYPE_NVME 0x91

static inline void mdelay(unsigned long t)
{
    Wait_X_ms(t);
}

// Page aligned "dma bounce buffer" of size NVME_PAGE_SIZE in high memory
static void *nvme_dma_buffer;
static struct nvme_namespace *ns;

static void *
zalloc_page_aligned(u32 size)
{
    void *res = NULL;
    posix_memalign(&res, NVME_PAGE_SIZE, size);
    if (res)
        memset(res, 0, size);
    return res;
}

static void
nvme_init_queue_common(struct nvme_ctrl *ctrl, struct nvme_queue *q, u16 q_idx,
                       u16 length)
{
    memset(q, 0, sizeof(*q));
    q->dbl = (u32 *)((char *)ctrl->reg + 0x1000 + q_idx * ctrl->doorbell_stride);
    dprintf(3, " queue %p q_idx %u dbl %p\n", q, q_idx, q->dbl);
    q->mask = length - 1;
}

static int
nvme_init_sq(struct nvme_ctrl *ctrl, struct nvme_sq *sq, u16 q_idx, u16 length,
             struct nvme_cq *cq)
{
    nvme_init_queue_common(ctrl, &sq->common, q_idx, length);
    sq->sqe = zalloc_page_aligned(sizeof(*sq->sqe) * length);

    if (!sq->sqe)
    {
        warn_noalloc();
        return -1;
    }

    dprintf(3, "sq %p q_idx %u sqe %p\n", sq, q_idx, sq->sqe);
    sq->cq = cq;
    sq->head = 0;
    sq->tail = 0;

    return 0;
}

static int
nvme_init_cq(struct nvme_ctrl *ctrl, struct nvme_cq *cq, u16 q_idx, u16 length)
{
    nvme_init_queue_common(ctrl, &cq->common, q_idx, length);
    cq->cqe = zalloc_page_aligned(sizeof(*cq->cqe) * length);
    if (!cq->cqe)
    {
        warn_noalloc();
        return -1;
    }

    cq->head = 0;

    /* All CQE phase bits are initialized to zero. This means initially we wait
       for the host controller to set these to 1. */
    cq->phase = 1;

    return 0;
}

static int
nvme_poll_cq(struct nvme_cq *cq)
{
    u32 dw3 = readl(&cq->cqe[cq->head].dword[3]);
    dprintf(1, "%p dw3 %u\n", &cq->cqe[cq->head].dword[3], dw3);
    return (!!(dw3 & NVME_CQE_DW3_P) == cq->phase);
}

static int
nvme_is_cqe_success(struct nvme_cqe const *cqe)
{
    return ((cqe->status >> 1) & 0xFF) == 0;
}

static struct nvme_cqe
nvme_error_cqe(void)
{
    struct nvme_cqe r;

    /* 0xFF is a vendor specific status code != success. Should be okay for
       indicating failure. */
    memset(&r, 0xFF, sizeof(r));
    return r;
}

static struct nvme_cqe
nvme_consume_cqe(struct nvme_sq *sq)
{
    struct nvme_cq *cq = sq->cq;

    if (!nvme_poll_cq(cq))
    {
        /* Cannot consume a completion queue entry, if there is none ready. */
        return nvme_error_cqe();
    }

    struct nvme_cqe *cqe = &cq->cqe[cq->head];
    u16 cq_next_head = (cq->head + 1) & cq->common.mask;
    dprintf(4, "cq %p head %u -> %u\n", cq, cq->head, cq_next_head);
    if (cq_next_head < cq->head)
    {
        dprintf(3, "cq %p wrap\n", cq);
        cq->phase = ~cq->phase;
    }
    cq->head = cq_next_head;

    /* Update the submission queue head. */
    if (cqe->sq_head != sq->head)
    {
        sq->head = cqe->sq_head;
        dprintf(4, "sq %p advanced to %u\n", sq, cqe->sq_head);
    }

    /* Tell the controller that we consumed the completion. */
    writel(cq->common.dbl, cq->head);

    return *cqe;
}

static struct nvme_cqe
nvme_wait(struct nvme_sq *sq)
{
    int nvme_timeout;
    for (nvme_timeout = 500; nvme_timeout > 0; nvme_timeout--)
    {
        mdelay(1);
        //        dprintf(1, "nvme_timeout %d, sq->cq %p\n", nvme_timeout, sq->cq);
        if (nvme_poll_cq(sq->cq))
            return nvme_consume_cqe(sq);
    }
    warn_timeout();
    return nvme_error_cqe();
}

/* Returns the next submission queue entry (or NULL if the queue is full). It
   also fills out Command Dword 0 and clears the rest. */
static struct nvme_sqe *
nvme_get_next_sqe(struct nvme_sq *sq, u8 opc, void *metadata, void *data, void *data2)
{
    if (((sq->head + 1) & sq->common.mask) == sq->tail)
    {
        dprintf(3, "submission queue is full\n");
        return NULL;
    }

    struct nvme_sqe *sqe = &sq->sqe[sq->tail];
    dprintf(4, "sq %p next_sqe %u\n", sq, sq->tail);

    memset(sqe, 0, sizeof(*sqe));
    sqe->cdw0 = opc | (sq->tail << 16 /* CID */);
    sqe->mptr = (u32)metadata;
    sqe->dptr_prp1 = (u32)data;
    sqe->dptr_prp2 = (u32)data2;

    return sqe;
}

/* Call this after you've filled out an sqe that you've got from nvme_get_next_sqe. */
static void
nvme_commit_sqe(struct nvme_sq *sq)
{
    dprintf(4, "sq %p commit_sqe %u\n", sq, sq->tail);
    sq->tail = (sq->tail + 1) & sq->common.mask;
    writel(sq->common.dbl, sq->tail);
}

/* Perform an identify command on the admin queue and return the resulting
   buffer. This may be a NULL pointer, if something failed. This function
   cannot be used after initialization, because it uses buffers in tmp zone. */
static union nvme_identify *
nvme_admin_identify(struct nvme_ctrl *ctrl, u8 cns, u32 nsid)
{
    union nvme_identify *identify_buf = zalloc_page_aligned(4096);
    if (!identify_buf)
    {
        /* Could not allocate identify buffer. */
        warn_internalerror();
        return NULL;
    }

    struct nvme_sqe *cmd_identify;
    cmd_identify = nvme_get_next_sqe(&ctrl->admin_sq,
                                     NVME_SQE_OPC_ADMIN_IDENTIFY, NULL,
                                     identify_buf, NULL);

    if (!cmd_identify)
    {
        warn_internalerror();
        goto error;
    }

    cmd_identify->nsid = nsid;
    cmd_identify->dword[10] = cns;

    nvme_commit_sqe(&ctrl->admin_sq);

    struct nvme_cqe cqe = nvme_wait(&ctrl->admin_sq);

    if (!nvme_is_cqe_success(&cqe))
    {
        goto error;
    }

    return identify_buf;
error:
    free(identify_buf);
    return NULL;
}

static struct nvme_identify_ctrl *
nvme_admin_identify_ctrl(struct nvme_ctrl *ctrl)
{
    return &nvme_admin_identify(ctrl, NVME_ADMIN_IDENTIFY_CNS_ID_CTRL, 0)->ctrl;
}

static struct nvme_identify_ns *
nvme_admin_identify_ns(struct nvme_ctrl *ctrl, u32 ns_id)
{
    return &nvme_admin_identify(ctrl, NVME_ADMIN_IDENTIFY_CNS_ID_NS,
                                ns_id)
                ->ns;
}

static void
nvme_probe_ns(struct nvme_ctrl *ctrl, u32 ns_idx, u8 mdts)
{
    u32 ns_id = ns_idx + 1;

    struct nvme_identify_ns *id = nvme_admin_identify_ns(ctrl, ns_id);
    if (!id)
    {
        dprintf(2, "NVMe couldn't identify namespace %u.\n", ns_id);
        goto free_buffer;
    }

    u8 current_lba_format = id->flbas & 0xF;
    if (current_lba_format > id->nlbaf)
    {
        dprintf(2, "NVMe NS %u: current LBA format %u is beyond what the "
                   " namespace supports (%u)?\n",
                ns_id, current_lba_format, id->nlbaf + 1);
        goto free_buffer;
    }

    if (!id->nsze)
    {
        dprintf(2, "NVMe NS %u is inactive.\n", ns_id);
        goto free_buffer;
    }

    if (!nvme_dma_buffer)
    {
        nvme_dma_buffer = zalloc_page_aligned(NVME_PAGE_SIZE);
        if (!nvme_dma_buffer)
        {
            warn_noalloc();
            goto free_buffer;
        }
    }
    dprintf(2, "NVMe nvme_dma_buffer %p .\n", nvme_dma_buffer);

    ns = malloc(sizeof(*ns)); // malloc_fseg
    if (!ns)
    {
        warn_noalloc();
        goto free_buffer;
    }
    memset(ns, 0, sizeof(*ns));
    ns->ctrl = ctrl;
    ns->ns_id = ns_id;
    ns->lba_count = id->nsze;

    struct nvme_lba_format *fmt = &id->lbaf[current_lba_format];

    ns->block_size = 1U << fmt->lbads;
    ns->metadata_size = fmt->ms;

    if (ns->block_size > NVME_PAGE_SIZE)
    {
        /* If we see devices that trigger this path, we need to increase our
           buffer size. */
        warn_internalerror();
        free(ns);
        goto free_buffer;
    }
#if 0
    ns->drive.cntl_id   = ns_idx;
    ns->drive.removable = 0;
    ns->drive.type      = DTYPE_NVME;
    ns->drive.blksize   = ns->block_size;
    ns->drive.sectors   = ns->lba_count;
#endif
    if (mdts)
    {
        ns->max_req_size = ((1U << mdts) * NVME_PAGE_SIZE) / ns->block_size;
        dprintf(3, "NVME NS %u max request size: %d sectors\n",
                ns_id, ns->max_req_size);
    }
    else
    {
        ns->max_req_size = -1U;
    }

    dprintf(1, "NVMe NS %u: %llu MiB (%llu %u-byte blocks + %u-byte metadata)",
            ns_id, (ns->lba_count * ns->block_size) >> 20,
            ns->lba_count, ns->block_size, ns->metadata_size);

free_buffer:
    free(id);
}

/* Release memory allocated for a completion queue */
static void
nvme_destroy_cq(struct nvme_cq *cq)
{
    free(cq->cqe);
    cq->cqe = NULL;
}

/* Release memory allocated for a submission queue */
static void
nvme_destroy_sq(struct nvme_sq *sq)
{
    free(sq->sqe);
    sq->sqe = NULL;
}

/* Returns 0 on success. */
static int
nvme_create_io_cq(struct nvme_ctrl *ctrl, struct nvme_cq *cq, u16 q_idx)
{
    int rc;
    struct nvme_sqe *cmd_create_cq;
    u32 length = 1 + (ctrl->reg->cap & 0xffff);
    if (length > NVME_PAGE_SIZE / sizeof(struct nvme_cqe))
        length = NVME_PAGE_SIZE / sizeof(struct nvme_cqe);

    rc = nvme_init_cq(ctrl, cq, q_idx, length);
    if (rc)
    {
        goto err;
    }

    cmd_create_cq = nvme_get_next_sqe(&ctrl->admin_sq,
                                      NVME_SQE_OPC_ADMIN_CREATE_IO_CQ, NULL,
                                      cq->cqe, NULL);
    if (!cmd_create_cq)
    {
        goto err_destroy_cq;
    }

    cmd_create_cq->dword[10] = (cq->common.mask << 16) | (q_idx >> 1);
    cmd_create_cq->dword[11] = 1 /* physically contiguous */;

    nvme_commit_sqe(&ctrl->admin_sq);

    struct nvme_cqe cqe = nvme_wait(&ctrl->admin_sq);

    if (!nvme_is_cqe_success(&cqe))
    {
        dprintf(2, "create io cq failed: %08x %08x %08x %08x\n",
                cqe.dword[0], cqe.dword[1], cqe.dword[2], cqe.dword[3]);

        goto err_destroy_cq;
    }

    return 0;

err_destroy_cq:
    nvme_destroy_cq(cq);
err:
    return -1;
}

/* Returns 0 on success. */
static int
nvme_create_io_sq(struct nvme_ctrl *ctrl, struct nvme_sq *sq, u16 q_idx, struct nvme_cq *cq)
{
    int rc;
    struct nvme_sqe *cmd_create_sq;
    u32 length = 1 + (ctrl->reg->cap & 0xffff);
    if (length > NVME_PAGE_SIZE / sizeof(struct nvme_cqe))
        length = NVME_PAGE_SIZE / sizeof(struct nvme_cqe);

    rc = nvme_init_sq(ctrl, sq, q_idx, length, cq);
    if (rc)
    {
        goto err;
    }

    cmd_create_sq = nvme_get_next_sqe(&ctrl->admin_sq,
                                      NVME_SQE_OPC_ADMIN_CREATE_IO_SQ, NULL,
                                      sq->sqe, NULL);
    if (!cmd_create_sq)
    {
        goto err_destroy_sq;
    }

    cmd_create_sq->dword[10] = (sq->common.mask << 16) | (q_idx >> 1);
    cmd_create_sq->dword[11] = (q_idx >> 1) << 16 | 1 /* contiguous */;
    dprintf(3, "sq %p create dword10 %08x dword11 %08x\n", sq,
            cmd_create_sq->dword[10], cmd_create_sq->dword[11]);

    nvme_commit_sqe(&ctrl->admin_sq);

    struct nvme_cqe cqe = nvme_wait(&ctrl->admin_sq);

    if (!nvme_is_cqe_success(&cqe))
    {
        dprintf(2, "create io sq failed: %08x %08x %08x %08x\n",
                cqe.dword[0], cqe.dword[1], cqe.dword[2], cqe.dword[3]);
        goto err_destroy_sq;
    }

    return 0;

err_destroy_sq:
    nvme_destroy_sq(sq);
err:
    return -1;
}

static int
nvme_create_io_queues(struct nvme_ctrl *ctrl)
{
    if (nvme_create_io_cq(ctrl, &ctrl->io_cq, 3))
        goto err;

    if (nvme_create_io_sq(ctrl, &ctrl->io_sq, 2, &ctrl->io_cq))
        goto err_free_cq;

    return 0;

err_free_cq:
    nvme_destroy_cq(&ctrl->io_cq);
err:
    return -1;
}

/* Waits for CSTS.RDY to match rdy. Returns 0 on success. */
static int
nvme_wait_csts_rdy(struct nvme_ctrl *ctrl, unsigned rdy)
{
    u32 max_to = 500 /* ms */ * ((ctrl->reg->cap >> 24) & 0xFFU);
    u32 csts;

    for (; max_to > 0; max_to--)
    {
        mdelay(1);
        if (rdy == ((csts = ctrl->reg->csts) & NVME_CSTS_RDY))
        {
            return 0;
        }
        if (csts & NVME_CSTS_FATAL)
        {
            dprintf(3, "NVMe fatal error during controller shutdown\n");
            return -1;
        }
    }

    warn_timeout();
    return -1;
}

/* Returns 0 on success. */
static int
nvme_controller_enable(struct nvme_ctrl *ctrl)
{
    int rc;

    /* Turn the controller off. */
    ctrl->reg->cc = 0;
    if (nvme_wait_csts_rdy(ctrl, 0))
    {
        dprintf(2, "NVMe fatal error during controller shutdown\n");
        return -1;
    }

    ctrl->doorbell_stride = 4U << ((ctrl->reg->cap >> 32) & 0xF);

    rc = nvme_init_cq(ctrl, &ctrl->admin_cq, 1,
                      NVME_PAGE_SIZE / sizeof(struct nvme_cqe));
    if (rc)
    {
        return -1;
    }

    rc = nvme_init_sq(ctrl, &ctrl->admin_sq, 0,
                      NVME_PAGE_SIZE / sizeof(struct nvme_sqe), &ctrl->admin_cq);
    if (rc)
    {
        goto err_destroy_admin_cq;
    }

    ctrl->reg->aqa = ctrl->admin_cq.common.mask << 16 | ctrl->admin_sq.common.mask;

    ctrl->reg->asq = (u32)ctrl->admin_sq.sqe;
    ctrl->reg->acq = (u32)ctrl->admin_cq.cqe;

    dprintf(3, "  admin submission queue: %p\n", ctrl->admin_sq.sqe);
    dprintf(3, "  admin completion queue: %p\n", ctrl->admin_cq.cqe);

    ctrl->reg->cc = NVME_CC_EN | (NVME_CQE_SIZE_LOG << 20) | (NVME_SQE_SIZE_LOG << 16 /* IOSQES */);

    if (nvme_wait_csts_rdy(ctrl, 1))
    {
        dprintf(2, "NVMe fatal error while enabling controller\n");
        goto err_destroy_admin_sq;
    }

    /* The admin queue is set up and the controller is ready. Let's figure out
       what namespaces we have. */

    struct nvme_identify_ctrl *identify = nvme_admin_identify_ctrl(ctrl);

    if (!identify)
    {
        dprintf(2, "NVMe couldn't identify controller.\n");
        goto err_destroy_admin_sq;
    }

    dprintf(3, "NVMe has %u namespace%s.\n",
            identify->nn, (identify->nn == 1) ? "" : "s");

    ctrl->ns_count = identify->nn;
    u8 mdts = identify->mdts;
    free(identify);

    if ((ctrl->ns_count == 0) || nvme_create_io_queues(ctrl))
    {
        /* No point to continue, if the controller says it doesn't have
           namespaces or we couldn't create I/O queues. */
        goto err_destroy_admin_sq;
    }

    /* Populate namespace IDs */
    int ns_idx;
    for (ns_idx = 0; ns_idx < ctrl->ns_count; ns_idx++)
    {
        nvme_probe_ns(ctrl, ns_idx, mdts);
    }

    dprintf(3, "NVMe initialization complete!\n");
    return 0;

err_destroy_admin_sq:
    nvme_destroy_sq(&ctrl->admin_sq);
err_destroy_admin_cq:
    nvme_destroy_cq(&ctrl->admin_cq);
    return -1;
}

/* Initialize an NVMe controller and detect its drives. */
static void
nvme_controller_setup(void *nvme_base)
{
    struct nvme_reg volatile *reg = nvme_base;
    if (!reg)
        return;
    dprintf(3, "nvme_reg %p, intms %x, intmc %x\n", reg, reg->intms, reg->intmc);

    u32 version = reg->vs;
    dprintf(3, "Found NVMe controller with version %u.%u.%u.\n",
            version >> 16, (version >> 8) & 0xFF, version & 0xFF);
    dprintf(3, "  Capabilities %016llx\n", reg->cap);

    if (~reg->cap & NVME_CAP_CSS_NVME)
    {
        dprintf(3, "Controller doesn't speak NVMe command set. Skipping.\n");
        goto err;
    }

    struct nvme_ctrl *ctrl = zalloc_page_aligned(sizeof(*ctrl)); // malloc_high
    if (!ctrl)
    {
        warn_noalloc();
        goto err;
    }

    memset(ctrl, 0, sizeof(*ctrl));

    ctrl->reg = reg;

    if (nvme_controller_enable(ctrl))
    {
        goto err_free_ctrl;
    }

    return;

err_free_ctrl:
    free(ctrl);
err:
    dprintf(2, "Failed to enable NVMe controller.\n");
}

/* Reads count sectors into buf. The buffer cannot cross page boundaries. */
static int
nvme_io_xfer(struct nvme_namespace *ns, u64 lba, void *prp1, void *prp2,
             u16 count, int write)
{
    if (((u32)prp1 & 0x3) || ((u32)prp2 & 0x3))
    {
        /* Buffer is misaligned */
        warn_internalerror();
        return -1;
    }

    struct nvme_sqe *io_read = nvme_get_next_sqe(&ns->ctrl->io_sq,
                                                 write ? NVME_SQE_OPC_IO_WRITE
                                                       : NVME_SQE_OPC_IO_READ,
                                                 NULL, prp1, prp2);
    io_read->nsid = ns->ns_id;
    io_read->dword[10] = (u32)lba;
    io_read->dword[11] = (u32)(lba >> 32);
    io_read->dword[12] = (1U << 31 /* limited retry */) | (count - 1);

    nvme_commit_sqe(&ns->ctrl->io_sq);

    struct nvme_cqe cqe = nvme_wait(&ns->ctrl->io_sq);

    if (!nvme_is_cqe_success(&cqe))
    {
        dprintf(2, "read io: %08x %08x %08x %08x\n",
                cqe.dword[0], cqe.dword[1], cqe.dword[2], cqe.dword[3]);

        return -1;
    }

    dprintf(5, "ns %u %s lba %llu+%u\n", ns->ns_id, write ? "write" : "read",
            lba, count);
    return count;
}

// Transfer up to one page of data using the internal dma bounce buffer
static int
nvme_bounce_xfer(struct nvme_namespace *ns, u64 lba, void *buf, u16 count,
                 int write)
{
    u16 const max_blocks = NVME_PAGE_SIZE / ns->block_size;
    u16 blocks = count < max_blocks ? count : max_blocks;

    if (write)
        memcpy(nvme_dma_buffer, buf, blocks * ns->block_size);

    int res = nvme_io_xfer(ns, lba, nvme_dma_buffer, NULL, blocks, write);

    if (!write && res >= 0)
        memcpy(buf, nvme_dma_buffer, res * ns->block_size);

    return res;
}

#define NVME_MAX_PRPL_ENTRIES 15 /* Allows requests up to 64kb */

// Transfer data using page list (if applicable)
static int
nvme_prpl_xfer(struct nvme_namespace *ns, u64 lba, void *buf, u16 count,
               int write)
{
    u32 base = (long)buf;
    s32 size;

    if (count > ns->max_req_size)
        count = ns->max_req_size;

    size = count * ns->block_size;
    /* Special case for transfers that fit into PRP1, but are unaligned */
    if (((size + (base & ~NVME_PAGE_MASK)) <= NVME_PAGE_SIZE))
        goto single;

    /* Every request has to be page aligned */
    if (base & ~NVME_PAGE_MASK)
        goto bounce;

    /* Make sure a full block fits into the last chunk */
    if (size & (ns->block_size - 1ULL))
        goto bounce;

    /* Build PRP list if we need to describe more than 2 pages */
    if ((ns->block_size * count) > (NVME_PAGE_SIZE * 2))
    {
        u32 prpl_len = 0;
        u64 *prpl = nvme_dma_buffer;
        int first_page = 1;
        for (; size > 0; base += NVME_PAGE_SIZE, size -= NVME_PAGE_SIZE)
        {
            if (first_page)
            {
                /* First page is special */
                first_page = 0;
                continue;
            }
            if (prpl_len >= NVME_MAX_PRPL_ENTRIES)
                goto bounce;
            prpl[prpl_len++] = base;
        }
        return nvme_io_xfer(ns, lba, buf, prpl, count, write);
    }

    /* Directly embed the 2nd page if we only need 2 pages */
    if ((ns->block_size * count) > NVME_PAGE_SIZE)
        return nvme_io_xfer(ns, lba, buf, buf + NVME_PAGE_SIZE, count, write);

single:
    //    printk("nvme_io_xfer %s\n", write == 1 ? "write" : "read");
    /* One page is enough, don't expose anything else */
    return nvme_io_xfer(ns, lba, buf, NULL, count, write);

bounce:
    //    printk("nvme_bounce_xfer %s\n", write == 1 ? "write" : "read");
    /* Use bounce buffer to make transfer */
    return nvme_bounce_xfer(ns, lba, buf, count, write);
}

#define PCI_CLASS_STORAGE_EXPRESS 0x010802
#define PCI_CLASS_STORAGE_NVME		0x0108
#define PCI_VENDOR_ID_REDHAT             0x15ad
#define PCI_DEVICE_ID_REDHAT_NVME        0x07f0

// Locate and init NVMe controllers
static void
nvme_scan(void)
{
    int pbus, pdev, pfun;

    // Scan PCI bus for NVMe adapters
    if (pci_find_device(PCI_VENDOR_ID_REDHAT, PCI_DEVICE_ID_REDHAT_NVME, 0,
        &pbus, &pdev, &pfun) != -1)
    {
        printk("NVMe class found on PCI bus\n");
    }
    else
    {
        printk("NVMe class not found on PCI bus\n");
        exit(0);
    }

    printk("bus 0x%x, dev 0x%x, func 0x%x\n", pbus, pdev, pfun);

    // pci_enable_membar
    uint32_t nvme_base;
    pci_read_config_dword(pbus, pdev, pfun, PCI_BASE_ADDRESS_0, &nvme_base);
    nvme_base &= PCI_BASE_ADDRESS_MEM_MASK;

    // pci_enable_busmaster
    uint32_t command;
    pci_read_config_dword(pbus, pdev, pfun, PCI_COMMAND, &command);
    command |= PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY;
    pci_write_config_dword(pbus, pdev, pfun, PCI_COMMAND, command);

    unsigned char irqline;
    pci_read_config_byte(pbus, pdev, pfun, PCI_INTERRUPT_LINE, &irqline);
    printk("NVMe device found @PCI 0x%08x, IRQ %i\n", nvme_base, irqline);

    nvme_controller_setup((void *)nvme_base);
}

////////////////////////////////////////////////////////////////////////////////
static int nvme_self_test(void)
{
    int sz = 512, order;
    for (order = 1; order < 16; order <<= 1)
    {
        sz <<= 1;
        printk("order %d sz %d\n", order, sz);
        char *data = malloc(sz), *buf = malloc(sz);
        memset(data, 0xac, sz);

        int i;
        int cnt;
        u64 lba = 0;
#if 1
        // write
        cnt = sz / ns->block_size;
        for (i = 0; i < cnt;)
        {
            u16 blocks_remaining = cnt - i;
            char *op_buf = data + i * ns->block_size;
            int blocks = nvme_prpl_xfer(ns, lba + i, op_buf, blocks_remaining, 1);
            if (blocks < 0)
                return -1;
            i += blocks;
        }
        printk("================== nvme write\n");
#endif
        // read
        cnt = sz / ns->block_size;
        for (i = 0; i < cnt;)
        {
            u16 blocks_remaining = cnt - i;
            char *op_buf = buf + i * ns->block_size;
            int blocks = nvme_prpl_xfer(ns, lba + i, op_buf, blocks_remaining, 0);
            if (blocks < 0)
                return -1;
            i += blocks;
        }
        printk("================== nvme read\n");
        printk("strncmp %d\n", 0 == strncmp(data, buf, sz));
        rtems_test_assert( 0 == strncmp(data, buf, sz) );
        // hexdump(buf, sz);
        free(data);
        free(buf);
    }
    printk("all done\n");

    return 0;
}

#endif

/* EOF */
