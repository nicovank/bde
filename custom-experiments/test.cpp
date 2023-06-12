#include <iostream>

#include <bdlma_multipoolallocator.h>

#include <bsl_vector.h>

#define N 10

using namespace BloombergLP;

int main() {
  bdlma::Multipool allocator;
  bsl::vector<void *> active;

  for (int i = 0; i < N; ++i) {
    void *p = allocator.allocate(i);
    std::cout << i << " " << p << std::endl;
    active.push_back(p);
  }
}
