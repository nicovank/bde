#include <iostream>

#include <bdlma_sequentialallocator.h>
#include <bdlma_multipoolallocator.h>
#include <bslma_newdeleteallocator.h>

#include <bsl_vector.h>

#define N 10

using namespace BloombergLP;

int main() {
    bdlma::SequentialPool allocator;
    bsl::vector<void *> active;

    for (int i = 0; i < N; ++i) {
        void *p = allocator.allocate(i);
        std::cout << i << " " << p << std::endl;
        active.push_back(p);
    }
}
