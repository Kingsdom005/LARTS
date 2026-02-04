# LARTS: Language Abstractions for Real-Time and Secure Systems

## 🛠️ Framework

The LARTS programming model is outlined as shown in the figure below.

<img src="https://github.com/Kingsdom005/LARTS/blob/main/overview.png" alt="overview" style="zoom:90%;" />

---

## 📊 Testcase Description

The error_handling directory contains the compiled binary files for the user program (./rtems_application/build/bin/), the source code for the user program test cases (./rtems_application/), and the LARTS kernel executable (./image).

The LARTS kernel can handle and capture numerous error types, including but not limited to cross-domain access and null pointer exceptions. The implementation and source code definitions for certain error types are as follows.

```
const char *const fatal_source_text[] = {
        [SIGHUP]    = "SIGHUP    | Hangup detected on controlling terminal, usually caused by session termination or terminal disconnection.",
        [SIGINT]    = "SIGINT    | Keyboard interrupt (Ctrl-C), user requests termination of the foreground domain.",
        [SIGQUIT]   = "SIGQUIT   | Keyboard quit (Ctrl-\\), forcibly terminates the domain and generates a core dump.",
        [SIGILL]    = "SIGILL    | Illegal instruction exception, CPU fails to decode the current instruction stream.",
        [SIGTRAP]   = "SIGTRAP   | Breakpoint or single-step trap, triggered by debugger or exception detection mechanisms.",
        [SIGABRT]   = "SIGABRT   | The domain invokes abort() to proactively terminate with an exception, generating a core dump.",

        [SIGEMT]    = "SIGEMT    | Emulator trap or hardware platform-specific exception.",
        [SIGFPE]    = "SIGFPE    | Arithmetic exception (divide-by-zero, overflow, or invalid floating-point operation).",
        [SIGKILL]   = "SIGKILL   | Forced termination; cannot be caught, blocked, or ignored.",
        [SIGBUS]    = "SIGBUS    | Bus error, caused by invalid address alignment or physical access failure.",
        [SIGSEGV]   = "SIGSEGV   | Segmentation fault, illegal memory access resulting in a core dump.",
        [SIGSYS]    = "SIGSYS    | Illegal system call, due to ABI mismatch or insufficient privileges.",
        [SIGPIPE]   = "SIGPIPE   | Broken pipe, write attempted on a closed pipe.",
        [SIGALRM]   = "SIGALRM   | Alarm clock expired, timer event triggered.",
        [SIGTERM]   = "SIGTERM   | Termination request; domain may catch the signal and exit gracefully.",
        [SIGURG]    = "SIGURG    | Urgent data available on a socket.",
        [SIGSTOP]   = "SIGSTOP   | Stops the domain; cannot be caught, blocked, or ignored.",
        [SIGTSTP]   = "SIGTSTP   | User-initiated stop (Ctrl-Z), can be caught.",
        [SIGCONT]   = "SIGCONT   | Continues execution of a stopped domain.",
        [SIGCHLD]   = "SIGCHLD   | Child thread state has changed.",
        [SIGCLD]    = "SIGCLD    | Alias compatible with SIGCHLD.",
        [SIGTTIN]   = "SIGTTIN   | Background domain attempted to read from controlling terminal.",
        [SIGTTOU]   = "SIGTTOU   | Background domain attempted to write to controlling terminal.",
        [SIGIO]     = "SIGIO     | Asynchronous I/O event notification.",
        [SIGPOLL]   = "SIGPOLL   | Semantically equivalent to SIGIO.",
        [SIGWINCH]  = "SIGWINCH  | Terminal window size changed.",
        [SIGUSR1]   = "SIGUSR1   | User-defined signal 1.",
        [SIGUSR2]   = "SIGUSR2   | User-defined signal 2.",

        [0 << 8] = "ENHANCE_FATAL_EXIT     | Real-time Domain Exit",
        [1 << 8] = "ENHANCE_FATAL_UNDEF    | Real-time Domain encounters an UNDEF exception",
        [2 << 8] = "SWI | SVC",
        [3 << 8] = "ENHANCE_FATAL_PABORT   | Real-time Domain encounters a PREFETCH ABORT exception",
        [4 << 8] = "ENHANCE_FATAL_DABORT   | Real-time Domain encounters a DATA ABORT exception",
        [5 << 8] = "ENHANCE_FATAL_RESERVED | Real-time Domain encounters an ENHANCE_FATAL_RESERVED exception",
        [8 << 8] = "ENHANCE_FATAL_UABORT   | Real-time Domain encounters a USER ABORT exception",
        [9 << 8] = "ENHANCE_FATAL_MAX      | Maximum descriptor for Real-time Domain exceptions",
};

```
