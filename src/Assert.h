#ifndef ASSERT_H
#define ASSERT_H

#include <cassert>

#define ACROSS_ASSERT(expr, message) assert(expr && message)

#endif