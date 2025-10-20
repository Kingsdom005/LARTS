# LARTS: Language Abstractions for Real-Time and Secure Systems
This repository contains evaluation scripts and resources for the real-time system framework presented in our research. The framework integrates four core subsystems to deliver robust, secure, and predictable real-time performance.

## 🛠️ Framework 

The framework is built on a layered architecture:

<img src="C:\Users\liyanqi\AppData\Roaming\Typora\typora-user-images\image-20251020225827483.png" alt="image-20251020225827483" style="zoom:90%;" />

**Programming Model & Tooling Layer**

- Application development using LARTS abstractions
- LARTS loader for compilation and load-time binding
- Unified Process-Thread model 
- Deterministic Memory Domains
- Deterministic Channels
- Secure Runtime Semantics

**Execution Environment Layer**

- Deterministic execution with isolation domains
- Predictable and secure application output
- Real-time guarantees enforcement

**Underlying Foundation**

- Hardware support for ARM Cortex-A platforms
- Low-level system primitives and isolation mechanisms

The evaluation is divided into three stages:

1. **Static Analysis**: Compute system dependencies and timing characteristics
2. **Kernel Compilation**: Build the real-time enhanced kernel with specific configurations
3. **Benchmark Execution**: Run comprehensive real-time performance tests

## 📋 Quick Start Example

The project's source code is currently not open-sourced. New resources will be gradually open-sourced in the future.

## 📊 Baselines for Reproduction

To reproduce comparative results, the following baseline implementations are available:

- **Reference RTOS A**: https://github.com/baseline-rtos/rtos-a
- **Reference Framework B**: https://gitlab.com/rt-frameworks/framework-b
- **Comparative System C**: https://bitbucket.org/rt-systems/system-c

## 🔧 Evaluation Scripts

We will gradually release the tools and scripts for testing.

## 📈 Available Resources

We will gradually make thesis-related resources available.

## 🔄 Future Releases

Thesis-related resources will be available in the future.

