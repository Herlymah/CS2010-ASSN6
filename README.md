Name: Halimah Abdulrasheed Instructor: Franco Carlacci CS2010 Assignment 5

Memory Allocation Simulator
Overview
This project implements a memory allocation simulator that demonstrates the dynamic allocation and deallocation of memory blocks using a first-fit allocation strategy. The simulation includes features such as lease-based memory management, block merging, and detailed memory state tracking.

Simulation Parameters
The simulation can be configured using the following parameters in main.cpp:

MIN_LEASE (40): Minimum lease duration
MAX_LEASE (70): Maximum lease duration
MIN_SIZE (50): Minimum block size
MAX_SIZE (350): Maximum block size
TIME_LIMIT (1000): Total simulation duration
REQUEST_INTERVAL (50): Interval between memory requests
MEMORY_SIZE (1000): Total size of simulated memory

Output Information
The simulation provides detailed output including:

Real-time Memory State

Current simulation time
Free memory blocks and their sizes
Allocated memory blocks and their lease expiry times
Total free and allocated memory


Event Information

New memory requests
Allocation success/failure
Lease expirations
Block merging operations


Final Statistics

Total requests processed
Request satisfaction rate
Block size statistics (min/max/average)
Lease duration statistics
Merge operation count



Data Structures

range: Represents a memory block with start address and size
alloc: Represents an allocated block with its lease expiry time
freelist: List of available memory blocks
alloclist: List of allocated memory blocks

Key Functions
Memory Management

allocateBlock(): Implements first-fit allocation strategy
checkExpiredLeases(): Manages lease expiration
mergeFreeBlocks(): Combines adjacent free blocks

Display and Reporting

displayMemoryState(): Shows current memory state
generateReport(): Produces final simulation statistics

Utility Functions

generateRequest(): Creates random memory requests
insertIntoFreeList(): Maintains sorted free block list
insertIntoAllocatedList(): Maintains sorted allocated block list

Example Run
=== Time: 50 ===
Memory Requested: 200 units
Free Memory:     [   0:1000] 
Total Free: 1000 units
Allocated Memory:
Total Allocated: 0 units
----------------------------------------
Request for 200 units with lease 55 was SATISFIED
=== Time: 50 ===
Free Memory:     [ 200: 800] 
Total Free: 800 units
Allocated Memory:[ 0: 200 until  105] 
Total Allocated: 200 units
----------------------------------------
