## Description

In this project, I orchestrate multiple coders competing for limited USB dongles using POSIX threads, mutexes, and smart scheduling.

**This project allowed me to greatly improve the following skills:**
- Command-line argument parsing
- Resources management & starvation prevention
- Scheduling
- System design trade-off analysis
- Synchronization primitives
- Multithreading

## Instructions

## Resources

## Blocking cases handled
section describing all the concurrency issues addressed in
your solution (e.g., deadlock prevention and Coffman’s conditions, starvation prevention,
cooldown handling, precise burnout detection, and log serialization)

## Thread synchronisation mechanisms
section explaining the specific threading prim-
itives used in your implementation (pthread_mutex_t, pthread_cond_t, custom event
implementation) and how they coordinate access to shared resources (dongles, logging,
monitor state). Include examples of how race conditions are prevented and how thread-safe
communication is achieved between coders and the monitor