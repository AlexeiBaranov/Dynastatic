#include <stdio.h>
#include <time.h>

#include "dynastatic.h"

typedef struct dyn_struct {
   char a;
   int b;
} dyn_struct;

DECLARE_DYNASTATIC(dyn_struct);
DEFINE_DYNASTATIC(dyn_struct, 1000);

// For manipulations with nanoseconds
#define SEC_TO_NS(sec) ((sec)*1000000000)

// Number of test cycles
#define NUMBER_OF_CYCLES 100000

int main(int argc, char *argv[]) {

   dyn_struct *d;

   struct timespec ts;
   unsigned long long nanoseconds_1;
   unsigned long long nanoseconds_2;
   unsigned long long nanoseconds_3;
   int return_code;
   int i;

   return_code = timespec_get(&ts, TIME_UTC);
   nanoseconds_1 = SEC_TO_NS((unsigned long long)ts.tv_sec) + (unsigned long long)ts.tv_nsec;

   for (i = 0; i < NUMBER_OF_CYCLES; i++) {
      d = DYNASTATIC_CALLOC(dyn_struct);
      DYNASTATIC_FREE(dyn_struct, d);
   }

   return_code = timespec_get(&ts, TIME_UTC);
   nanoseconds_2 = SEC_TO_NS((unsigned long long)ts.tv_sec) + (unsigned long long)ts.tv_nsec;

   for (i = 0; i < NUMBER_OF_CYCLES; i++) {
      d = calloc(1, sizeof(dyn_struct));
      free(d);
   }

   return_code = timespec_get(&ts, TIME_UTC);
   nanoseconds_3 = SEC_TO_NS((unsigned long long)ts.tv_sec) + (unsigned long long)ts.tv_nsec;

   printf("%lld calls to dynastatic calloc/free takes %lld nanoseconds\n", (unsigned long long) NUMBER_OF_CYCLES, (unsigned long long) nanoseconds_2 - nanoseconds_1);
   printf("%lld calls to stdlib's calloc/free takes %lld nanoseconds\n", (unsigned long long) NUMBER_OF_CYCLES, (unsigned long long) nanoseconds_3 - nanoseconds_2);
   printf("Dynastatic is %lld times faster\n", (unsigned long long) (nanoseconds_3 - nanoseconds_2)/((unsigned long long) nanoseconds_2 - nanoseconds_1));

   return 0;
}