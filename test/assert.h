#ifndef TEST_ASSERT_H_
#define TEST_ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"

#define ASSERT_EQ(expected, actual) \
  if ((expected) != (actual)) {     \
    return 1;                       \
  }

#define ASSERT_NOT_EQ(expected, actual) \
  if ((expected) == (actual)) {         \
    return 1;                           \
  }

#ifdef __cplusplus
}
#endif

#endif /* TEST_ASSERT_H_ */
