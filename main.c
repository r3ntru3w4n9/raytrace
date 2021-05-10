#include <assert.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char const* argv[]) {
    // This region runs in parallel.
#pragma omp parallel default(none)
    {
        // #pragma omp for
    }
    return 0;
}
