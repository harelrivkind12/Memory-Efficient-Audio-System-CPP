# AudioEngine-CPP: High-Performance Memory-Managed Session Orchestrator

## Overview
This project is a C++ simulation of a professional DJ track session manager. It is designed to handle complex audio resource orchestration within strict memory boundaries, simulating the hardware constraints of real-time embedded systems. The system manages a multi-layered hierarchy-from a persistent library to a fixed-capacity hardware cache and active mixing decks.

* **This project was developed as a joint academic effort at Ben-Gurion University in collaboration with a project partner.**

* **Final Version Showcase**: This repository contains the final, refactored version of the engine, optimized for performance and code clarity.
## Core Engineering Challenges

### 1. Deterministic Memory Ownership (Rule of 5)
I implemented a polymorphic AudioTrack hierarchy where memory safety and object lifecycle management are the highest priorities.
* **Dynamic Resource Handling**: To manage raw waveform data arrays, I implemented a complete Rule of 5, including the destructor, copy/move constructors, and assignment operators.
* **Efficiency**: Leveraged move semantics to "steal" resources during deck transitions, significantly reducing unnecessary memory allocations and improving performance.
* **Polymorphic Cloning**: Developed a robust cloning mechanism to ensure safe, independent track instances as they are transferred between system services.

### 2. Custom RAII Infrastructure (PointerWrapper)
To enforce strict RAII principles without relying on external libraries, I designed a custom template wrapper for pointer management.
* **Ownership Transfer**: Facilitates safe handovers between system layers using release and reset patterns.
* **Leak Prevention**: This wrapper guarantees that every dynamically allocated object is destroyed exactly once, ensuring a stable state even during complex service transitions.

### 3. Service-Oriented Architecture (SOA)
The system is divided into specialized services to maintain a clear separation of concerns:
* **LRU Cache Service**: A fixed-capacity storage layer implementing a Least Recently Used eviction policy. It simulates hardware cache limits by tracking access timestamps to prioritize frequently used tracks.
* **Mixing Engine Service**: Manages dual-deck operations with instant-transition logic. It includes automated BPM Synchronization and compatibility checking between active and incoming tracks.
* **Session Orchestrator**: A central component that coordinates the entire workflow, from configuration parsing to interactive session execution and detailed statistics reporting.

## Validation & Memory Safety
* **Modern C++ Standard**: Deep focus on pointer manipulation, references, and memory layout optimization.
* **Systematic Verification**: The implementation was rigorously tested for memory integrity. Using tools like Valgrind and GDB, I ensured that the entire lifecycle of tracks from library loading to deck unloading is 100% leak-free.
* **Robustness**: Integrated centralized logging and graceful recovery for configuration errors and cloning failures to ensure system stability.