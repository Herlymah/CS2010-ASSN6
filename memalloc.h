#ifndef MEMORY_SIMULATION_H
#define MEMORY_SIMULATION_H

#include <vector>
#include <utility>  // For std::pair

// Type definitions
using range = std::pair<int, int>;  // Represents a memory block {start, size}
using alloc = std::pair<range, int>;  // Represents an allocated block and its lease expiry
using freelist = std::vector<range>;
using alloclist = std::vector<alloc>;

// Function declarations
void displayMemoryState(const freelist& freel, const alloclist& aloc, int clock, int requestedSize = 0);
void insertIntoFreeList(freelist& freel, const range& newBlock);
void insertIntoAllocatedList(alloclist& aloc, const alloc& newAlloc);
void checkExpiredLeases(alloclist& aloc, freelist& freel, int currentTime);
range generateRequest(int minSize, int maxSize, int& leaseDuration, int minLease, int maxLease);
bool allocateBlock(freelist& freel, alloclist& aloc, int blockSize, int currentTime, int leaseDuration);
void mergeFreeBlocks(freelist& freel);
void generateReport(int totalRequests, int satisfiedRequests, int unsatisfiedRequests,
    int minBlock, int maxBlock, int totalBlockSize,
    int minLease, int maxLease, int totalLeaseDuration, int mergeCount,
    freelist& freel, alloclist& aloc);

#endif  // MEMORY_SIMULATION_H