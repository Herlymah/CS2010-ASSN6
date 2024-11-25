// memalloc.cpp
#include "memalloc.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

void displayMemoryState(const freelist& freel, const alloclist& aloc, int clock, int requestedSize) {
    std::cout << "\n=== Time: " << std::setw(4) << clock << " ===\n";

    // Display memory demand if there's a request
    if (requestedSize > 0) {
        std::cout << "Memory Requested: " << std::setw(4) << requestedSize << " units\n";
    }

    // Display free memory blocks
    std::cout << "Free Memory:     ";
    int totalFree = 0;
    for (const auto& block : freel) {
        std::cout << "[" << std::setw(4) << block.first << ":" << std::setw(4) << block.second << "] ";
        totalFree += block.second;
    }
    std::cout << "\nTotal Free: " << totalFree << " units\n";

    // Display allocated memory blocks
    std::cout << "Allocated Memory:";
    int totalAllocated = 0;
    for (const auto& alloc : aloc) {
        std::cout << "[" << std::setw(4) << alloc.first.first << ":"
            << std::setw(4) << alloc.first.second << " until "
            << std::setw(4) << alloc.second << "] ";
        totalAllocated += alloc.first.second;
    }
    std::cout << "\nTotal Allocated: " << totalAllocated << " units\n";
    std::cout << "----------------------------------------\n";
}

// Insert a block into the free list, maintaining sorted order by starting address
void insertIntoFreeList(freelist& freel, const range& newBlock) {
    auto it = std::lower_bound(freel.begin(), freel.end(), newBlock,
        [](const range& a, const range& b) { return a.first < b.first; });
    freel.insert(it, newBlock);
}

// Insert a block into the allocated list, maintaining sorted order by lease expiry
void insertIntoAllocatedList(alloclist& aloc, const alloc& newAlloc) {
    auto it = std::lower_bound(aloc.begin(), aloc.end(), newAlloc,
        [](const alloc& a, const alloc& b) { return a.second < b.second; });
    aloc.insert(it, newAlloc);
}

// Function to generate a random memory request
range generateRequest(int minSize, int maxSize, int& leaseDuration, int minLease, int maxLease) {
    int blockSize = rand() % (maxSize - minSize + 1) + minSize;
    leaseDuration = rand() % (maxLease - minLease + 1) + minLease;
    return { 0, blockSize };  // Start is determined during allocation
}

// Allocate memory block using first-fit strategy
bool allocateBlock(freelist& freel, alloclist& aloc, int blockSize, int currentTime, int leaseDuration) {
    for (auto it = freel.begin(); it != freel.end(); ++it) {
        if (it->second >= blockSize) {  // First-fit: Find first block large enough
            int startAddress = it->first;
            aloc.push_back({ {startAddress, blockSize}, currentTime + leaseDuration });

            // Update the free list (split block if needed)
            if (it->second > blockSize) {
                it->first += blockSize;
                it->second -= blockSize;
            }
            else {
                freel.erase(it);  // Exact fit
            }
            return true;
        }
    }
    return false;  // Allocation failed
}

// Check for expired leases and free the blocks
void checkExpiredLeases(alloclist& aloc, freelist& freel, int currentTime) {
    auto it = aloc.begin();
    while (it != aloc.end()) {
        if (it->second <= currentTime) {  // Lease expired
            insertIntoFreeList(freel, it->first);  // Return block to free list
            it = aloc.erase(it);  // Remove from allocated list
        }
        else {
            ++it;
        }
    }
}

// Merge adjacent free blocks
void mergeFreeBlocks(freelist& freel) {
    if (freel.empty()) return;

    std::sort(freel.begin(), freel.end());  // Sort by starting address
    auto it = freel.begin();
    auto nextIt = it + 1;

    while (nextIt != freel.end()) {
        if (it->first + it->second == nextIt->first) {  // Adjacent blocks
            it->second += nextIt->second;  // Merge sizes
            nextIt = freel.erase(nextIt);  // Remove merged block
        }
        else {
            ++it;
            ++nextIt;
        }
    }
}

// Generate the final simulation report
void generateReport(int totalRequests, int satisfiedRequests, int unsatisfiedRequests,
    int minBlock, int maxBlock, int totalBlockSize,
    int minLease, int maxLease, int totalLeaseDuration, int mergeCount,
    freelist& freel, alloclist& aloc) {

    std::cout << "\nSimulation Report:\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Total Requests: " << totalRequests << "\n";
    std::cout << "Satisfied Requests: " << satisfiedRequests << "\n";
    std::cout << "Unsatisfied Requests: " << unsatisfiedRequests << "\n";
    std::cout << "Block Size - Min: " << minBlock << "\n";
    std::cout << "Block Size - Max: " << maxBlock << "\n";
    std::cout << "Block Size - Avg: " << (totalBlockSize / totalRequests) << "\n";
    std::cout << "Lease Duration - Max: " << maxLease << "\n";
    std::cout << "Lease Duration - Min: " << minLease << "\n";
    std::cout << "Lease Duration - Avg: " << (totalLeaseDuration / totalRequests) << "\n";
    std::cout << "Number of Merges: " << mergeCount << "\n";
    std::cout << "----------------------------------------\n";

    // Display final memory state
    displayMemoryState(freel, aloc, 0);
}