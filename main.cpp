#include "memalloc.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

int main() {
    // Simulation parameters
    const int MIN_LEASE = 40;
    const int MAX_LEASE = 70;
    const int MIN_SIZE = 50;
    const int MAX_SIZE = 350;
    const int TIME_LIMIT = 1000;
    const int REQUEST_INTERVAL = 50;  // Increased interval for clearer output
    const int MEMORY_SIZE = 1000;

    // Initialize clock and data structures
    long int clock = 0;
    freelist f = { {0, MEMORY_SIZE} };  // Initial free block
    alloclist a;                        // Empty allocated list

    // Statistics tracking
    int totalRequests = 0;
    int satisfiedRequests = 0;
    int unsatisfiedRequests = 0;
    int totalBlockSize = 0;
    int minBlockSize = MAX_SIZE;
    int maxBlockSize = MIN_SIZE;
    int totalLeaseDuration = 0;
    int minLeaseDuration = MAX_LEASE;
    int maxLeaseDuration = MIN_LEASE;
    int mergeCount = 0;

    srand(time(0));

    std::cout << "\n=== Memory Allocation Simulation Start ===\n";
    displayMemoryState(f, a, clock);

    // Main simulation loop
    while (++clock <= TIME_LIMIT) {
        // Check for expired leases
        bool leasesExpired = false;
        auto beforeSize = a.size();
        checkExpiredLeases(a, f, clock);
        if (a.size() != beforeSize) {
            mergeFreeBlocks(f);
            std::cout << "\n>>> Leases expired at time " << clock << " <<<\n";
            displayMemoryState(f, a, clock);
        }

        // Generate request at intervals
        if (clock % REQUEST_INTERVAL == 0) {
            int leaseDuration;
            range request = generateRequest(MIN_SIZE, MAX_SIZE, leaseDuration, MIN_LEASE, MAX_LEASE);
            totalRequests++;
            totalBlockSize += request.second;

            // Update statistics
            minBlockSize = std::min(minBlockSize, request.second);
            maxBlockSize = std::max(maxBlockSize, request.second);
            totalLeaseDuration += leaseDuration;
            minLeaseDuration = std::min(minLeaseDuration, leaseDuration);
            maxLeaseDuration = std::max(maxLeaseDuration, leaseDuration);

            std::cout << "\n>>> New request at time " << clock << " <<<\n";
            displayMemoryState(f, a, clock, request.second);

            // Try to allocate
            bool allocated = false;
            if (allocateBlock(f, a, request.second, clock, leaseDuration)) {
                satisfiedRequests++;
                allocated = true;
            }
            else {
                mergeFreeBlocks(f);
                mergeCount++;
                if (allocateBlock(f, a, request.second, clock, leaseDuration)) {
                    satisfiedRequests++;
                    allocated = true;
                }
                else {
                    unsatisfiedRequests++;
                }
            }

            std::cout << "Request for " << request.second << " units with lease "
                << leaseDuration << " was "
                << (allocated ? "SATISFIED" : "DENIED") << "\n";
            displayMemoryState(f, a, clock);
        }
    }

    // Generate final report
    std::cout << "\n=== Final Simulation State ===\n";
    displayMemoryState(f, a, clock);
    generateReport(totalRequests, satisfiedRequests, unsatisfiedRequests,
        minBlockSize, maxBlockSize, totalBlockSize,
        minLeaseDuration, maxLeaseDuration, totalLeaseDuration,
        mergeCount, f, a);

    return 0;
}