# LARTS: Language Abstractions for Real-Time and Secure Systems

This repository provides an artifact package accompanying our research on the LARTS real-time system framework.

Due to confidentiality agreement restrictions, the complete source code for the LARTS framework is currently unavailable for public access. Instead, this repository includes a limited set of representative experimental cases, together with their corresponding test configurations, execution scripts, and evaluation procedures, as used in the paper.

The released materials are designed to:

- Illustrate the experimental methodology adopted in the paper.
- Enable partial reproduction and validation of reported results.
- Demonstrate how different categories of experiments are constructed and executed.

## 🛠️ Framework

The LARTS programming model is outlined as shown in the figure below.

<img src="https://github.com/Kingsdom005/LARTS/blob/main/overview.png" alt="overview" style="zoom:90%;" />

---

## Repository Structure

Below is a high-level view of the repository layout (focused on the artifact and reproduction components):

```text
LARTS/
├── cross_architecture/
│   ├── Armv7/                  # Prebuilt images and instructions for ARMv7 (Cortex-A9 on QEMU)
│   ├── Armv8/                  # Prebuilt images and instructions for ARMv8 (Cortex-A72 on QEMU)
│   └── README.md
├── error_handling/             # Robustness & fault-handling artifact case(s)
│   ├── image/                  # Prebuilt images
│   ├── rtems_application/      # user source code
│   └── README.md
├── memory_usage/               # Memory footprint artifact case(s)
│   ├── image/                  # Prebuilt images
│   ├── rtems_application/      # user source code
│   └── README.md
├── time_calculation/           # time cost
│   ├── context-switch/         # time cost of context switch
│   ├── syscall/                # time cost of syscall
│   └── README.md
├── projectMigration/           # Migration effort artifact case(s) (e.g., Linux → LARTS)
│   ├── image/                  # Prebuilt images
│   ├── rtems_application/      # user source code
│   └── README.md
├── other/*                     # other materials (if any)
└── testsuites/*                # Test suite sources used for evaluation
...

```

## 📋 Quick Start

The project's source code is currently not open-sourced. We have made available certain binary programs compiled using the LARTS kernel.

- How to use Docker to build a runtime environment for pre-compiled binaries ? (Verified)

```shell
# Recommended: Ubuntu 20.04
# create docker container
# optional(Port Forwarding/File mapping)
# b7bab04fd9aa (replace your ubuntu20.04 image)
docker run -it --name LARTS b7bab04fd9aa /bin/bash
# in docker ubuntu20.04
# Install dependencies and required components
apt update && apt install sudo -y
sudo apt upgrade
sudo apt-get install -y autoconf automake libtool pkg-config
sudo apt-get install -y cmake
sudo apt install -y build-essential bison flex unzip
sudo apt install -y texinfo
sudo apt install -y python3.8 python3.8-dev
sudo apt install -y python-is-python3
sudo apt install -y libgmp-dev libmpfr-dev libmpc-dev
# C++ Dependencies
sudo apt-get install -y qemu qemu-user qemu-user-static qemu-system qemu-system-misc binfmt-support
# If not installed, libraries may not be found on armv8.
sudo apt-get install -y g++-aarch64-linux-gnu
cp /usr/aarch64-linux-gnu/lib/* /lib/
# If using WSL2 in windows10/11, omitting this step will cause paths similar to Windows to be loaded by default, resulting in errors.
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
# Clone Repository
mkdir -p /LARTS && cd /LARTS
git clone https://github.com/Kingsdom005/LARTS.git
cd ./LARTS
# For armv7 testing, please refer to ./Armv7/README.md
# For armv8 testing, please refer to ./Armv8/README.md
```

## 📊 Baselines for Reproduction

To reproduce comparative results, the following open-source baseline implementations are available:

- **FreeRTOS**: https://github.com/FreeRTOS/FreeRTOS-Kernel
- **Zephyr**: https://github.com/zephyrproject-rtos/zephyr
- **RT-Thread**: https://github.com/RT-Thread/rt-thread
- **RTEMS**: https://github.com/RTEMS/rtems/tree/5.1
- **Linux-RT**: https://github.com/clrkwllms/rt-linux

Commercial and private resources and repositories will not be made public.

## 🔧 Compilation Toolchain

The open-source toolchain and libraries currently in use are shared below:

- **RSB**: https://gitlab.rtems.org/rtems/tools/rtems-source-builder.git (Branch: 5)
- **Musl-Cross-Make**: https://github.com/richfelker/musl-cross-make.git (Branch: master)
- **Loongarch64 Related**: https://github.com/loongson
  - **binutils & gdb**: https://github.com/loongson/binutils-gdb (Branch: default)
  - **gcc**: https://github.com/loongson/gcc (Branch: default)
  - **newlib**: https://github.com/loongson/newlib (Branch: default)

Certain private toolchain resources and repositories (e.g., musl ulib) will not be made publicly available at this time. Additional toolchain resources will be made available in due course.

## 📈 Available Resources

Currently available resources:

- [LARTS Demo(armv7&armv8)](https://github.com/Kingsdom005/LARTS/releases/tag/Test)

We will gradually make more resources available.

## 🔄 Future Releases

We currently provide partial support for the **Loongarch64 architecture**. Resources related to the Loongson architecture are scheduled for release in March 2026, with plans to support additional architectures and expand functionality in subsequent phases.

All current development and testing is conducted using QEMU. Moving forward, our team will conduct testing on **physical hardware** and publish relevant test data to advance global operating system research and development.
