#ifndef CRM_DEBUG_H
#define CRM_DEBUG_H

#include "../../vendor/log.c/src/log.h"

#define crm_assert_msg(expr, action, msg, ...)  \
  if (!(expr)) {                                \
    log_fatal(msg, ##__VA_ARGS__);              \
    do { action; } while (0);                   \
  }

#define crm_assert_msg_noop(expr, msg, ...) \
  crm_assert_msg(                           \
    #expr,                                  \
    { exit(1); },                           \
    msg,                                    \
    #__VA_ARGS__                            \
  )

#define crm_assert(expr, action) \
  crm_assert_msg(                \
    expr,                        \
    action,                      \
    "Assertion %s failed",       \
    #expr                  \
  )

#define crm_assert_noop(expr) \
  crm_assert_msg_noop(        \
    expr,                    \
    "Assertion %s failed",    \
    #expr                     \
  )

#endif
