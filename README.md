# Malloc
### `malloc`, `free`, `realloc` functions implementation using C


* Table of Contents
- [Description][[#description]
  - [[Data Structures][#Data structures]]
- [Authors][#authors]

# Description
## Data structures
When it comes to implementing a memory allocator, it is a good idea to know which data structure you will be using as it is crucial part of your memory allocator.
In this `malloc` implementation we are using **implicit linked list** to store chunks of memory obtained from `sbrk` system call.
# Authors
Tim Assavarat
Farrukh Akhrarov
