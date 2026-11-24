# Description

The goal of this project is to simulate multiple coders competing for a limited number of USB dongles. Each coder is represented by a POSIX thread and needs two adjacent dongles simultaneously to compile.

Coders alternate between three activities: Compiling ; Debugging ; Refactoring.

The simulation introduces resource contention, timing constraints and scheduling policies. It aims to prevent deadlocks, ensure fair access to shared resources and detect when a coder exceeds their burnout deadline.

Two scheduling policies are supported:

- **FIFO (First In, First Out):** requests are handled according to their arrival order.
- **EDF (Earliest Deadline First):** requests are prioritized according to the earliest burnout deadline.

**This project allowed me to greatly improve the following skills:**
- Command-line argument parsing
- Resources management & starvation prevention
- Scheduling
- System design trade-off analysis
- Synchronization primitives
- Multithreading

# Instructions

Clone the repository and enter the project directory. Then compile the program using the Makefile:

```bash
make
```

The executable generated is named `codexion`.

Other available Makefile rules:

```bash
make clean
make fclean
make re
```

The project includes Valgrind and Helgrind targets:

```bash
make valgrind
make helgrind
```

Helgrind can be used to detect potential data races and synchronization problems involving shared resources.

The program takes eight mandatory arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders and dongles |
| `time_to_burnout` | Maximum time allowed between two compilation starts |
| `time_to_compile` | Duration of a compilation |
| `time_to_debug` | Duration of debugging |
| `time_to_refactor` | Duration of refactoring |
| `number_of_compiles_required` | Number of successful compilations required per coder |
| `dongle_cooldown` | Minimum cooldown period before a released dongle can be used again |
| `scheduler` | Scheduling policy: `fifo` or `edf` |

Example:

```bash
./codexion 5 100 20 20 10 5 10 edf
```

# Blocking cases handled

## Deadlock prevention

Coders compete for adjacent dongles, which creates a risk of deadlock when several threads request shared resources concurrently. The implementation protects individual dongle states using mutexes. Access to shared queue entries is also synchronized. The acquisition and release procedures must ensure that concurrent requests cannot result in duplicated dongle ownership.

## Starvation prevention and scheduling

The project supports two scheduling policies:

- FIFO prioritizes requests according to their arrival order.
- EDF prioritizes the coder with the earliest burnout deadline.

The deadline for EDF scheduling is calculated using:

```text
deadline = last_compile_start + time_to_burnout
```

The scheduling mechanism is responsible for deciding which waiting coder has priority when multiple coders request the same dongle.

## Dongle cooldown

After a compilation, dongles cannot immediately be reused. Each dongle stores an availability timestamp (`available_at_ms`). A dongle becomes available again only after its cooldown period has elapsed.

## Burnout detection

Each coder has a timestamp recording the beginning of its last compilation. The elapsed time is compared with the configured burnout limit. When the deadline is exceeded, the simulation stop and the burnout event is displayed within the required 10 ms tolerance.

## Log serialization

All status messages are protected by a dedicated printing mutex. This prevents messages from different threads from being mixed together or interleaved in the terminal output.

# Thread synchronisation mechanisms

## pthread_mutex_t

A mutex ensures that only one thread at a time can access the protected data in a critical section. Several mutexes are used to protect shared data:

- **Dongle mutexes (`mutex_dongle`):** protects dongle availability, cooldown timestamps and waiting queue entries.
- **Coder mutexes (`mutex_coder`):** protects coder state and compilation timestamps.
- **Printer mutex (`mutex_printer`):** protects terminal output.
- **Starter mutex (`mutex_starter`):** protects the simulation state.

## Thread-safe communication

Coders and the monitoring thread share simulation data through the simulation structure. Shared state is accessed through mutex-protected operations. This allows the monitoring logic to inspect coder states while coder threads update their own states. The printing mutex also ensures that status messages remain readable even when several threads execute concurrently.


# Resources

- Peer learning. BIG thanks to Jolyne, Luka, Ady <3
- [Project comprehension](https://dev.to/yel-bakk/thread-in-c-codexion-42-1ao)
- [Programmation concurrente](https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/)
- [Librairie threads](https://koor.fr/C/cthreads/Index.wp)
- [Mutexes](https://medium.com/@sherniiazov.da/mutexes-in-c-ac2b0f1a6d34)
- [Valgrind - Helgrind](https://valgrind.org/docs/manual/hg-manual.html)

## AI usage
No AI was use during this project.
