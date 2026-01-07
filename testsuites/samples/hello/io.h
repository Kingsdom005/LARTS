#if defined(__i386__)

#define barrier() __asm__ __volatile__("": : :"memory")

static inline void outb(u8 value, u16 port) {
    __asm__ __volatile__("outb %b0, %w1" : : "a"(value), "Nd"(port));
}
static inline void outw(u16 value, u16 port) {
    __asm__ __volatile__("outw %w0, %w1" : : "a"(value), "Nd"(port));
}
static inline void outl(u32 value, u16 port) {
    __asm__ __volatile__("outl %0, %w1" : : "a"(value), "Nd"(port));
}
static inline u8 inb(u16 port) {
    u8 value;
    __asm__ __volatile__("inb %w1, %b0" : "=a"(value) : "Nd"(port));
    return value;
}
static inline u16 inw(u16 port) {
    u16 value;
    __asm__ __volatile__("inw %w1, %w0" : "=a"(value) : "Nd"(port));
    return value;
}
static inline u32 inl(u16 port) {
    u32 value;
    __asm__ __volatile__("inl %w1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void writel(void *addr, u32 val) {
    barrier();
    *(volatile u32 *)addr = val;
}
static inline void writew(void *addr, u16 val) {
    barrier();
    *(volatile u16 *)addr = val;
}
static inline void writeb(void *addr, u8 val) {
    barrier();
    *(volatile u8 *)addr = val;
}
static inline u64 readq(const void *addr) {
    u64 val = *(volatile const u64 *)addr;
    barrier();
    return val;
}
static inline u32 readl(const void *addr) {
    u32 val = *(volatile const u32 *)addr;
    barrier();
    return val;
}
static inline u16 readw(const void *addr) {
    u16 val = *(volatile const u16 *)addr;
    barrier();
    return val;
}
static inline u8 readb(const void *addr) {
    u8 val = *(volatile const u8 *)addr;
    barrier();
    return val;
}

#endif /* defined(__i386__) */
