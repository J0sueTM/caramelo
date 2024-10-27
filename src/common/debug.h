#ifndef FD_DEBUG_H
#define FD_DEBUG_H

#include "../../vendor/log.c/src/log.h"

#define fd_assert_msg(expr, action, msg, ...)  \
  if (!(expr)) {                               \
    log_fatal(msg, ##__VA_ARGS__);             \
    do { action; } while (0);                  \
  }

#define fd_assert_msg_noop(expr, msg, ...) \
  fd_assert_msg(                           \
    #expr,                                 \
    { exit(1); },                          \
    msg,                                   \
    #__VA_ARGS__                           \
  )

#define fd_assert(expr, action) \
  fd_assert_msg(                \
    expr,                       \
    action,                     \
    "Assertion %s failed",      \
    #expr                       \
  )

#define fd_assert_noop(expr) \
  fd_assert_msg_noop(        \
    expr,                    \
    "Assertion %s failed",   \
    #expr                    \
  )

#endif
