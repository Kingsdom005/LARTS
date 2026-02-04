# LARTS: Language Abstractions for Real-Time and Secure Systems

## 🛠️ Framework

The LARTS programming model is outlined as shown in the figure below.

<img src="https://github.com/Kingsdom005/LARTS/blob/main/overview.png" alt="overview" style="zoom:90%;" />

---

## 📊 Testcase Description

The memory_usage directory contains the compiled binary files for the user program (./rtems_application/build/bin/), the source code for the user program test cases (./rtems_application/), and the LARTS kernel executable (./image).

We have implemented a memory monitor within the LARTS kernel that can be enabled or disabled via configuration, allowing for easy retrieval of the peak memory usage within the workspace.

Within the kernel, we provide the sampling frequency and sampling duration, which can be configured via PERIOD_TICKS and WINDOW_TICKS. Partial implementation and definitions are as follows:

```
/* Desired: sampling interval in ticks */
#define PERIOD_TICKS  50

/* Desired: total sampling window length (ticks) */
#define WINDOW_TICKS  4000

/* Maximum number of samples: round up, +1 to avoid boundary issues */
#define MAX_SAMPLES   ((WINDOW_TICKS / PERIOD_TICKS) + 2)

typedef struct {
  uint32_t elapsed_ticks;     /* Relative ticks since experiment start */
  uintptr_t total;
  uintptr_t used;
  uintptr_t free;
  uintptr_t min_free;
  uintptr_t largest_free;
  uint32_t usage_x1000;       /* xxx.xxx% => stored as value * 1000 */
} mem_sample_t;

static mem_sample_t g_samples[MAX_SAMPLES];
static uint32_t g_sample_count = 0;

static void take_one_sample(uint32_t elapsed_ticks)
{
  if (g_sample_count >= MAX_SAMPLES) {
    return; /* Defensive: avoid out-of-bounds */
  }

  Heap_Information_block info;
  bool ok = rtems_workspace_get_information(&info);
  if (!ok) {
    /* Record a sample even if sampling fails (or simply return) */
    g_samples[g_sample_count++] = (mem_sample_t){
      .elapsed_ticks = elapsed_ticks
    };
    return;
  }

  uintptr_t total = info.Stats.size;
  uintptr_t free  = info.Stats.free_size;
  uintptr_t used  = total - free;

  /* Usage: percentage * 1000 (keep three decimal places), avoid overflow */
  uint32_t usage_x1000 = 0;
  if (total != 0) {
    usage_x1000 = (uint32_t)((used * 100000ULL + total / 2) / total);
  }

  g_samples[g_sample_count++] = (mem_sample_t){
    .elapsed_ticks = elapsed_ticks,
    .total = total,
    .used  = used,
    .free  = free,
    .min_free = info.Stats.min_free_size,
    .largest_free = info.Free.largest,
    .usage_x1000 = usage_x1000
  };
}

static void dump_samples_once(void)
{
  printk("===== Workspace usage samples (%u samples) =====\n", g_sample_count);
  printk("period=%u ticks, window=%u ticks\n", (unsigned)PERIOD_TICKS, (unsigned)WINDOW_TICKS);
  printk("idx,elapsed_ticks,total,used,free,usage(%%),min_free,largest_free\n");

  for (uint32_t i = 0; i < g_sample_count; ++i) {
    const mem_sample_t *s = &g_samples[i];

    /* usage_x1000 -> A.BCD% */
    uint32_t ip = s->usage_x1000 / 1000;
    uint32_t fp = s->usage_x1000 % 1000;

    printk("%" PRIu32 ",%" PRIu32 ",%" PRIuPTR ",%" PRIuPTR ",%" PRIuPTR ",%" PRIu32 ".%03" PRIu32
           ",%" PRIuPTR ",%" PRIuPTR "\n",
           i,
           s->elapsed_ticks,
           s->total,
           s->used,
           s->free,
           ip, fp,
           s->min_free,
           s->largest_free);
  }

  printk("===== End =====\n");
}

/*
 * Call this function from your main process / Init task.
 * It will run until elapsed >= 4000 ticks, then print once and return.
 */
void record_workspace_for_4000ticks_then_print(void)
{
  g_sample_count = 0;

  rtems_interval base = rtems_clock_get_ticks_since_boot(); /* Initial value may be large, which is fine */
  rtems_interval next = base; /* Absolute tick time for the next sample */

  while (1) {
    rtems_interval now = rtems_clock_get_ticks_since_boot();
    rtems_interval elapsed = now - base; /* Key: use the difference */

    if (elapsed >= WINDOW_TICKS) {
      break;
    }

    if ((rtems_interval)(now - next) < (rtems_interval)0x80000000u) {
      /* now >= next (common unsigned wrap-around handling;
         here it could also simply be written as now >= next) */
      take_one_sample((uint32_t)elapsed);
      next += PERIOD_TICKS;
    }

    /* Reduce CPU usage: sleep at least 1 tick;
       alternatively, sleep until (next - now) if >= 1 */
    rtems_task_wake_after(1);
  }

  dump_samples_once();
}

```
