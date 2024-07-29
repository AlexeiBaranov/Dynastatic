/*
Copyright(c) 2024, Alexei Baranov  https://github.com/AlexeiBaranov/dynastatic
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __DYNASTATIC_H__
#define __DYNASTATIC_H__

#include <stdlib.h>
#include <string.h>

/**
 * @brief Declares public <t>_calloc and <t>_free dynastatic functions. Use DEFINE_DYNASTATIC to define <t>_calloc and <t>_free functions body
 * @param  t - data type
 */
#define DECLARE_DYNASTATIC(t) \
t *t##_calloc(void); \
void t##_free(t *ptr);

/**
 * @brief Allocate instance of variable from the static list (if available), or return calloc-ed memory
 * @param  t - data type
 */
#define DYNASTATIC_CALLOC(t) \
 (t##_DYNASTATIC_FREED)?(memset(&(t##_DYNASTATIC_DATA[t##_DYNASTATIC_AVAILABLE[--t##_DYNASTATIC_FREED]]), 0, sizeof(t))):( \
 (t##_DYNASTATIC_ALLOCATED < t##_DYNASTATIC_CAPACITY)?(memset(&(t##_DYNASTATIC_DATA[t##_DYNASTATIC_ALLOCATED++]), 0, sizeof(t))):( \
 calloc(1, sizeof(t)) \
 ))

/**
 * @brief Returnes allocated variable to the static list of available elements, or frees calloc-ed memory
 * @param  t - data type
 * @param  v - pointer returned from the previous call to DYNASTATIC_CALLOC
 */
#define DYNASTATIC_FREE(t, v) \
   do {\
      if((char *)(void *)(t *)v >= t##_DYNASTATIC_FIRST && (char *)(void *)(t *)v <= t##_DYNASTATIC_LAST) \
         t##_DYNASTATIC_AVAILABLE[t##_DYNASTATIC_FREED++]=((unsigned int)((char *)(void *)v - t##_DYNASTATIC_FIRST)/sizeof(t)); \
      else free(v); \
   } while(0)

/**
 * @brief Defines and allocates static structures for subsequent dynamic allocations
 * @param  t - data type
 * @param  n - initial number of elements in static array
 */
#define DEFINE_DYNASTATIC(t, n) \
static t  t##_DYNASTATIC_DATA[n] = {0}; \
static char *t##_DYNASTATIC_FIRST = (void *)&(t##_DYNASTATIC_DATA[0]); \
static char *t##_DYNASTATIC_LAST  = (void *)&(t##_DYNASTATIC_DATA[n-1]); \
static unsigned int t##_DYNASTATIC_AVAILABLE[n]       = {0}; \
static unsigned int t##_DYNASTATIC_ALLOCATED          = 0; \
static unsigned int t##_DYNASTATIC_FREED              = 0; \
static const unsigned int t##_DYNASTATIC_CAPACITY     = n; \
t *t##_calloc(void) { \
   return DYNASTATIC_CALLOC(t); \
} \
void t##_free(t *ptr) { \
   DYNASTATIC_FREE(t, ptr); \
}

#endif
