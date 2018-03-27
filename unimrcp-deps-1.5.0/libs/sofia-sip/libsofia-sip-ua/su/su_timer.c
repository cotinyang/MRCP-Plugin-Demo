/*
 * This file is part of the Sofia-SIP package
 *
 * Copyright (C) 2005 Nokia Corporation.
 *
 * Contact: Pekka Pessi <pekka.pessi@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

/**@CFILE su_timer.c
 *
 * Timer interface for su_root.
 *
 * @author Pekka Pessi <Pekka.Pessi@nokia.com>
 * Created: Fri Apr 28 15:45:41 2000 ppessi
 */

#include "config.h"

#include <sys/types.h>

#if USE_HEAP_TIMER_QUEUE

#include "sofia-sip/heap.h"

typedef union {
  void *private;
  /* Use for debugging */
  struct timers_priv {
    size_t _size, _used;
    struct su_timer_s * _heap[2];
  } *actual;
} su_timer_heap_t;

#define SU_TIMER_QUEUE_T su_timer_heap_t

#else /* default rb-tree queue */

#include "sofia-sip/rbtree.h"

struct su_timer_queue_s {
  struct {
    struct su_timer_s *first;
    struct su_timer_s *tree;
  } *queue;
};

#define SU_TIMER_QUEUE_T struct su_timer_queue_s

#endif

#include "sofia-sip/su.h"
#include "su_port.h"
#include "sofia-sip/su_wait.h"
#include "sofia-sip/su_alloc.h"
#include "sofia-sip/rbtree.h"

#include "su_module_debug.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define SU_DUR64(d) (1000000 * (su_dur64_t)(d))

/**@ingroup su_wait
 *
 * @page su_timer_t Timer Objects
 *
 *  Timers are used to schedule some task to be executed at given time or
 *  after a default interval. The default interval is specified when the
 *  timer is created. We call timer activation "setting the timer", and
 *  deactivation "resetting the timer" (as in SDL). When the given time has
 *  arrived or the default interval has elapsed, the timer expires and
 *  it is ready for execution.
 *
 *  The functions used to create, destroy, activate, and manage timers are
 *  as follows:
 *   - su_timer_create(),
 *   - su_timer_destroy(),
 *   - su_timer_set_interval(),
 *   - su_timer_set_at(),
 *   - su_timer_set(),
 *   - su_timer_set_for_ever(),
 *   - su_timer_run(),
 *   - su_timer_reset(),
 *   - su_timer_latest(), and
 *   - su_timer_root().
 *
 * @note
 * Timers use poll() to wake up waiting thread. On Linux, the timer
 * granularity is determined by HZ kernel parameter, which decided when the
 * kernel was compiled. With kernel 2.4 the default granularity is 10
 * milliseconds, and minimum duration of a timer is approximately 20
 * milliseconds. Naturally, using RTC would give better timing results, but
 * RTC usage above 64 Hz is privileged operation.
 *
 * @par
 * On Windows, the granularity is determined by the real-time clock timer.
 * By default, it uses the 18.78 Hz granularity.  That timer can be adjusted
 * up to 1000 Hz using Windows multimedia library.
 *
 * @section su_timer_usage Using Timers
 *
 * A timer is created by calling su_timer_create():
 * @code
 *   timer = su_timer_create(su_root_task(root), 200);
 * @endcode
 * The default duration is given in milliseconds.
 *
 * Usually, timer wakeup function should be called at regular intervals. In
 * such case, the timer is activated using function su_timer_set_for_ever().
 * When the timer is activated it is given the wakeup function and pointer to
 * context data:
 * @code
 *   su_timer_set_for_ever(timer, timer_wakeup, args);
 * @endcode
 *
 * When the interval has passed, the root event loop calls the wakeup
 * function:
 * @code
 *   timer_wakeup(root, timer, args);
 * @endcode
 *
 * If the number of calls to callback function is important, use
 * su_timer_run() instead. The run timer tries to compensate for missed time
 * and invokes the callback function several times if needed. (Because the
 * real-time clock can be adjusted or the program suspended, e.g., while
 * debugged, the callback function can be called thousends of times in a
 * row.) Note that while the timer tries to compensate for delays occurred
 * before and during the callback, it cannot be used as an exact source of
 * timing information.
 *
 * Timer ceases running when su_timer_reset() is called.
 *
 * Alternatively, the timer can be @b set for one-time event invocation.
 * When the timer is set, it is given the wakeup function and pointer to
 * context data.
 * @code su_timer_set(timer, timer_wakeup, args);
 * @endcode
 *
 * The duration for one-time invocation can be specified using
 * su_timer_set_interval().
 *
 * When the timer expires, the root event loop calls the wakeup function:
 * @code
 *   timer_wakeup(root, timer, args);
 * @endcode
 *
 * If the timed event is not needed anymore, the timer can be reset:
 * @code
 *   su_timer_reset(timer);
 * @endcode
 *
 * If the timer is expected to be called at regular intervals, it is
 * possible to set ro run continously with su_timer_run().  While such a
 * continously running timer is active it @b must @b not @b be @b set using
 * su_timer_set() or su_timer_set_at().
 *
 * When the timer is not needed anymore, the timer object itself should be
 * destroyed:
 * @code
 *   su_timer_destroy(timer);
 * @endcode
 */

struct su_timer_s {
  su_task_r       sut_task;	/**< Task reference */
  su_time64_t     sut_when;	/**< When timer should be waken up next time */
  su_dur64_t      sut_dur64;	/**< Timer duration */
  su_timer_f      sut_wakeup;	/**< Function to call when waken up */
  su_timer_arg_t *sut_arg;	/**< Pointer to argument data */
  unsigned        sut_woken;	/**< Timer has been woken up this many times */

  unsigned        sut_running:2;/**< Timer is running */
  unsigned        sut_deferrable:1;/**< Timer is deferrable */

#if USE_HEAP_TIMER_QUEUE

  unsigned        :0;
  size_t          sut_set;	/**< Timer is set (inserted in heap) */

#else

  unsigned        sut_set:1;	/**< Timer is set (inserted in tree) */

  unsigned        _sut_black:1, :0;
  su_timer_t     *_sut_dad, *_sut_left, *_sut_right;

#endif
};

/** Timer running status */
enum sut_running {
  reset = 0,		/**< Timer is not running */
  run_at_intervals = 1, /**< Compensate missed wakeup calls */
  run_for_ever = 2	/**< Do not compensate  */
};

#define SU_TIMER_IS_SET(sut) ((sut)->sut_set != 0)

#if USE_HEAP_TIMER_QUEUE

HEAP_DECLARE(su_inline, su_timer_queue_t, timers_, su_timer_t *);

su_inline void timers_set(su_timer_t **array, size_t index, su_timer_t *t)
{
  array[t->sut_set = index] = t;
}

su_inline int timers_less(su_timer_t *a, su_timer_t *b)
{
  return a->sut_when < b->sut_when;
}

su_inline void *timers_alloc(void *argument, void *memory, size_t size)
{
  (void)argument;

  if (size)
    return realloc(memory, size);
  else
    return free(memory), NULL;
}

HEAP_BODIES(su_inline, su_timer_queue_t, timers_, su_timer_t *,
	    timers_less, timers_set, timers_alloc, NULL);

#else

/* Define macros for rbtree implementation */
#define SUT_LEFT(t) ((t)->_sut_left)
#define SUT_RIGHT(t) ((t)->_sut_right)
#define SUT_PARENT(t) ((t)->_sut_dad)
#define SUT_SET_RED(t) ((t)->_sut_black = 0)
#define SUT_SET_BLACK(t) ((t)->_sut_black = 1)
#define SUT_IS_RED(t) ((t) && (t)->_sut_black == 0)
#define SUT_IS_BLACK(t) (!(t) || (t)->_sut_black == 1)
#define SUT_COPY_COLOR(dst, src) ((dst)->_sut_black = (src)->_sut_black)
#define SUT_INSERT(t) ((t)->sut_set = 1)
#define SUT_REMOVE(t) ((t)->_sut_left = NULL, (t)->_sut_right = NULL,	\
		       (t)->_sut_dad = NULL, (t)->sut_set = 0)

su_inline int sut_cmp(su_timer_t const *a, su_timer_t const *b)
{
  if (a->sut_when < b->sut_when)
    return -1;
  else if (a->sut_when == b->sut_when)
    return 0;
  else
    return 1;
}

RBTREE_PROTOS(su_inline, timer, su_timer_t);

RBTREE_BODIES(su_inline, timer, su_timer_t,
	      SUT_LEFT, SUT_RIGHT, SUT_PARENT,
	      SUT_IS_RED, SUT_SET_RED,
	      SUT_IS_BLACK, SUT_SET_BLACK,
	      SUT_COPY_COLOR, sut_cmp, SUT_INSERT, SUT_REMOVE);

#endif

/**@internal Get current time. */
su_inline su_time64_t
su_timer_current(su_timer_t const *t)
{
  (void)t;

  return su_now64();
}

/**@internal Get current time. */
su_inline su_dur64_t
su_timer_offset(su_timer_t const *t)
{
  return 0;
}

/**@internal Get current time. */
su_inline su_timer_t *
su_timer_queue_first(su_timer_queue_t const *timers)
{
#if USE_HEAP_TIMER_QUEUE
  return timers ? timers_get(timers[0], 1) : NULL;
#else
  return timers && timers->queue ? timers->queue->first : NULL;
#endif
}

/**@internal Set the timer.
 *
 * @retval 0 when successful (always)
 */
su_inline int
su_timer_set0(su_timer_queue_t *timers,
	      su_timer_t *t,
	      su_timer_f wakeup,
	      su_wakeup_arg_t *arg,
	      su_time64_t when,
	      su_dur64_t offset)
{
  if (timers == NULL)
    return -1;

#if USE_HEAP_TIMER_QUEUE

  if (SU_TIMER_IS_SET(t))
    timers_remove(timers[0], t->sut_set);

#else

  if (SU_TIMER_IS_SET(t)) {
    if (t == timers->queue->first)
      timers->queue->first = timer_succ(t);
    timer_remove(&timers->queue->tree, t);
  }

#endif

  t->sut_wakeup = wakeup;
  t->sut_arg = arg;

  if (when == 0)
    when = su_timer_current(t);

  t->sut_when = when + offset;

#if USE_HEAP_TIMER_QUEUE

  if (timers_is_full(timers[0])) {
    timers_resize(NULL, timers, 0);
    assert(!timers_is_full(timers[0]));
    if (timers_is_full(timers[0]))
      return -1;
  }

  return timers_add(timers[0], t);

#else

  timer_append(&timers->queue->tree, t);

  if (timer_prec(t) == NULL)
    timers->queue->first = t;

  return 0;

#endif
}

/**@internal Validate timer @a t and return pointer to per-port timer tree.
 *
 * @retval pointer to pointer to timer tree when successful
 * @retval NULL upon an error
 */
static
su_timer_queue_t *su_timer_queue(su_timer_t const *t,
				 int use_sut_dur64,
				 char const *caller)
{
  su_timer_queue_t *timers;

  if (t == NULL) {
    SU_DEBUG_1(("%s(%p): %s\n", caller, (void *)t,
		"NULL argument"));
    return NULL;
  }

  if (use_sut_dur64 && t->sut_dur64 == 0) {
    SU_DEBUG_1(("%s(%p): %s\n", caller, (void *)t,
		"timer without default duration"));
    return NULL;
  }

  if (t->sut_deferrable)
    timers = su_task_deferrable(t->sut_task);
  else
    timers = su_task_timers(t->sut_task);

  if (timers == NULL) {
    SU_DEBUG_1(("%s(%p): %s\n", caller, (void *)t, "invalid timer"));
    return NULL;
  }

#if USE_HEAP_TIMER_QUEUE

  if (timers_is_full(timers[0]) && timers_resize(NULL, timers, 0) == -1) {
    SU_DEBUG_1(("%s(%p): %s\n", caller, (void *)t, "timer queue failed"));
    return NULL;
  }

#else

  if (timers->queue == NULL) {
    timers->queue = calloc(1, sizeof *timers->queue);
    if (timers->queue == NULL)
      return NULL;
  }

#endif

  return timers;
}

su_inline void su_timer_dequeue(su_timer_queue_t *timers,
				su_timer_t *t)
{
  if (!SU_TIMER_IS_SET(t))
    return;

#if USE_HEAP_TIMER_QUEUE
  timers_remove(timers[0], t->sut_set);
#else
  if (t == timers->queue->first)
    timers->queue->first = timer_succ(t);
  timer_remove(&timers->queue->tree, t);
#endif
}

/**Create a timer.
 *
 * Allocate and initialize an instance of su_timer_t.
 *
 * @param task a task for root object with which the timer will be associated
 * @param msec the default duration of the timer in milliseconds
 *
 * @return A pointer to allocated timer instance, NULL on error.
 */
su_timer_t *su_timer_create(su_task_r const task, su_duration_t msec)
{
  su_timer_t *retval;

  assert(msec >= 0);

  if (!su_task_cmp(task, su_task_null))
    return NULL;

  if (msec < 0)
    return NULL;

  retval = su_zalloc(NULL, sizeof(*retval));
  if (retval) {
    su_task_copy(retval->sut_task, task);
    retval->sut_dur64 = SU_DUR64(msec);
  }

  return retval;
}


/** Destroy a timer.
 *
 * Deinitialize and free an instance of su_timer_t.
 *
 * @param t pointer to the timer object
 */
void su_timer_destroy(su_timer_t *t)
{
  if (t) {
    su_timer_reset(t);
    su_task_deinit(t->sut_task);
    su_free(NULL, t);
  }
}

/** Check if the timer has been set.
 *
 * @param t pointer to a timer object
 *
 * @return Nonzero if set, zero if reset.
 *
 * @NEW_1_12_11
 */
int su_timer_is_set(su_timer_t const *t)
{
  return t && t->sut_set != 0;
}

/** Check if the timer is running.
 *
 * @param t pointer to a timer object
 *
 * @return Nonzero if running.
 *
 * @NEW_UNRELEASED
 */
int su_timer_is_running(su_timer_t const *t)
{
  return t && t->sut_running != 0;
}

/**Return when the timer has been last expired.
 *
 * @param t pointer to a timer object
 *
 * @return Timestamp (as returned by su_time()).
 *
 * @note If the timer is running (set with su_timer_run()), the returned
 * timestamp not the actual time but it is rather calculated from the
 * initial timestamp.
 *
 * @NEW_1_12_11
 */
su_time_t su_timer_latest(su_timer_t const *t)
{
  su_time_t tv = { 0, 0 };
  su_dur64_t offset;

  if (t == NULL)
    return tv;

  offset = su_timer_offset(t);

  return su_time64_to_time(t->sut_when + offset);
}

/** Set the timer for the given @a interval.
 *
 *  Sets (starts) the given timer to expire after the specified duration.
 *
 * @param t       pointer to the timer object
 * @param wakeup  pointer to the wakeup function
 * @param arg     argument given to the wakeup function
 * @param interval duration in milliseconds before timer wakeup is called
 *
 * @return 0 if successful, -1 otherwise.
 */
int su_timer_set_interval(su_timer_t *t,
			  su_timer_f wakeup,
			  su_timer_arg_t *arg,
			  su_duration_t interval)
{
  su_timer_queue_t *timers = su_timer_queue(t, 0, "su_timer_set_interval");

  return su_timer_set0(timers, t, wakeup, arg, 0, SU_DUR64(interval));
}

/** Set the timer for the given @a interval.
 *
 *  Sets (starts) the given timer to expire after the specified duration.
 *
 * @param t       pointer to the timer object
 * @param wakeup  pointer to the wakeup function
 * @param arg     argument given to the wakeup function
 * @param interval duration in nanoseconds before timer wakeup is called
 *
 * @return 0 if successful, -1 otherwise.
 *
 * @NEW_UNRELEASED
 */
int su_timer_set_interval64(su_timer_t *t,
			    su_timer_f wakeup,
			    su_timer_arg_t *arg,
			    su_dur64_t interval)
{
  su_timer_queue_t *timers = su_timer_queue(t, 0, "su_timer_set_interval64");

  return su_timer_set0(timers, t, wakeup, arg, 0, interval);
}

/** Set the timer for the default interval.
 *
 *  Sets (starts) the given timer to expire after the default duration.
 *
 *  The timer must have an default duration.
 *
 * @param t       pointer to the timer object
 * @param wakeup  pointer to the wakeup function
 * @param arg     argument given to the wakeup function
 *
 * @return 0 if successful, -1 otherwise.
 */
int su_timer_set(su_timer_t *t,
		 su_timer_f wakeup,
		 su_timer_arg_t *arg)
{
  su_timer_queue_t *timers = su_timer_queue(t, 1, "su_timer_set");

  return su_timer_set0(timers, t, wakeup, arg, 0, t->sut_dur64);
}

/** Set timer at known time.
 *
 * Sets the timer to expire at given time.
 *
 * @param t       pointer to the timer object
 * @param wakeup  pointer to the wakeup function
 * @param arg     argument given to the wakeup function
 * @param at      time structure defining the wakeup time
 *
 * @return 0 if successful, -1 otherwise.
 */
int su_timer_set_at(su_timer_t *t,
		    su_timer_f wakeup,
		    su_wakeup_arg_t *arg,
		    su_time_t at)
{
  su_timer_queue_t *timers = su_timer_queue(t, 0, "su_timer_set_at");
  su_time64_t when;
  su_dur64_t offset;

  if (timers == NULL)
    return -1;

  when = su_time_to_time64(at);
  offset = -su_timer_offset(t);

  return su_timer_set0(timers, t, wakeup, arg, when, offset);
}

/** Set the timer for regular intervals.
 *
 * Run the given timer continuously, call wakeup function repeately in
 * the default interval. If a wakeup call is latye or missed, timer
 * tries to make it up.
 *
 * The timer must have an non-zero default interval.
 *
 * @param t       pointer to the timer object
 * @param wakeup  pointer to the wakeup function
 * @param arg     argument given to the wakeup function
 *
 * @return 0 if successful, -1 otherwise.
 */
int su_timer_run(su_timer_t *t,
		 su_timer_f wakeup,
		 su_timer_arg_t *arg)
{
  su_timer_queue_t *timers = su_timer_queue(t, 1, "su_timer_run");

  if (timers == NULL)
    return -1;

  t->sut_running = run_at_intervals;
  t->sut_woken = 0;

  return su_timer_set0(timers, t, wakeup, arg, 0, t->sut_dur64);
}

/**Set the timer for regular intervals.
 *
 * Run the given timer continuously, call wakeup function repeately in
 * the default interval. Unlike su_timer_run(), set-for-ever timer does
 * not try to catchup missed callbacks.
 *
 * The timer must have an non-zero default interval.
 *
 * @param t       pointer to the timer object
 * @param wakeup  pointer to the wakeup function
 * @param arg     argument given to the wakeup function
 *
 * @return 0 if successful, -1 otherwise.
 */
int su_timer_set_for_ever(su_timer_t *t,
			  su_timer_f wakeup,
			  su_timer_arg_t *arg)
{
  su_timer_queue_t *timers = su_timer_queue(t, 1, "su_timer_set_for_ever");

  if (timers == NULL)
    return -1;

  t->sut_running = run_for_ever;
  t->sut_woken = 0;

  return su_timer_set0(timers, t, wakeup, arg, 0, t->sut_dur64);
}

/**Reset the timer.
 *
 * Resets (stops) the given timer.
 *
 * @param t  pointer to the timer object
 *
 * @return 0 if successful, -1 otherwise.
 */
int su_timer_reset(su_timer_t *t)
{
  su_timer_queue_t *timers;

  if (t == NULL)
    return -1;

  timers = su_timer_queue(t, 0, "su_timer_reset");
  if (timers == NULL)
    return -1;

  su_timer_dequeue(timers, t);

  t->sut_wakeup = NULL;
  t->sut_arg = NULL;
  t->sut_running = reset;

  return 0;
}

/** @internal Expire one timer. */
static int
expire_one(su_timer_queue_t *timers, su_timer_t *t, su_time64_t now)
{
  su_timer_f f;

  f = t->sut_wakeup; t->sut_wakeup = NULL;
  assert(f);

  if (!t->sut_running) {
    t->sut_when = now;

    f(su_root_magic(su_task_root(t->sut_task)), t, t->sut_arg);

    return 1;
  }

  if (t->sut_running == run_for_ever) {
    t->sut_woken++;
    t->sut_when = now;

    f(su_root_magic(su_task_root(t->sut_task)), t, t->sut_arg);

    if (t->sut_running == run_for_ever && t->sut_set == 0)
      su_timer_set0(timers, t, f, t->sut_arg, now, t->sut_dur64);

    return 1;
  } else {
    int n;

    for (n = 0;
	 t->sut_running == run_at_intervals &&
	   t->sut_set == 0 &&
	   t->sut_dur64 > 0;
	 n++) {
      if (t->sut_when > now) {
	su_timer_set0(timers, t, f, t->sut_arg, t->sut_when, 0);
	break;
      }
      t->sut_when += t->sut_dur64;
      t->sut_woken++;
      f(su_root_magic(su_task_root(t->sut_task)), t, t->sut_arg);
    }

    return n;
  }
}

/** @internal Check for expired timers in queue.
 *
 * Remove and executes expired timers from the queue.
 *
 * @param timers   pointer to the timer queue
 * @param until_wait maximum number of timers to expire at one run
 *
 * @return The number of expired timers.
 *
 * @NEW_UNRELEASED
 */
int
su_timer_queue_expire(su_timer_queue_t *timers, int until_wait)
{
  su_timer_t *t;
  int n;
  su_time64_t when;

  t = su_timer_queue_first(timers);
  if (t == NULL)
    return 0;

  when = su_timer_current(t);

#if USE_HEAP_TIMER_QUEUE
  while ((t = timers_get(timers[0], 1))) {
    if (t->sut_when > when)
      break;

    timers_remove(timers[0], 1);

    n += expire_one(timers, t, when);

    if (until_wait <= n)
      break;
  }

#else

  for (n = 0; timers->queue->first;) {
    t = timers->queue->first;

    if (t->sut_when > when)
      break;

    timers->queue->first = timer_succ(t);

    timer_remove(&timers->queue->tree, t);

    n += expire_one(timers, t, when);

    if (until_wait <= n)
      break;
  }

#endif

  return n;
}

/** @internal Check for expired timers in queue.
 *
 * The function su_timer_expire() checks a timer queue and executes and
 * removes expired timers from the queue. It also calculates the time when
 * the next timer expires.
 *
 * @param timers   pointer to the timer queue
 * @param timeout  timeout in milliseconds [IN/OUT]
 * @param now      current timestamp (ignored)
 *
 * @return
 * The number of expired timers.
 */
int su_timer_expire(su_timer_queue_t *timers,
		    su_duration_t *timeout,
		    su_time_t now)
{
  int n = su_timer_queue_expire(timers, INT_MAX);

  if (timeout != NULL) {
    su_duration_t d = su_timer_queue_timeout(timers);

    if (d < SU_DURATION_MAX && (d < *timeout || *timeout < 0))
      *timeout = d;
  }

  return n;
}

/** Calculate duration in milliseconds until next timer expires.
 *
 * @NEW_UNRELEASED
 */
su_duration_t
su_timer_queue_timeout(su_timer_queue_t const *timers)
{
  su_timer_t const *t = su_timer_queue_first(timers);
  su_time64_t next, now;

  if (t == NULL)
    return SU_DURATION_MAX;

  next = t->sut_when;
  now = su_timer_current(t);

  if (next < now)
    return 0;

  if (next > now + 1000000 * (su_dur64_t)SU_DURATION_MAX)
    return SU_DURATION_MAX;

  return (su_duration_t)((next - now) / 1000000);
}

/** Calculate duration in milliseconds until next timer expires. */
su_duration_t
su_timer_next_expires(su_timer_queue_t const *timers, su_time_t now)
{
  return su_timer_queue_timeout(timers);
}

/**
 * Resets and frees all timers belonging to a task.
 *
 * Resets and frees all timers belonging to the specified task in the
 * queue.
 *
 * @param timers   pointer to the timers
 * @param task     task owning the timers
 *
 * @return Number of timers reset.
 */
int su_timer_reset_all(su_timer_queue_t *timers, su_task_r task)
{
#if USE_HEAP_TIMER_QUEUE
  int n = 0;
  size_t i;

  timers_sort(timers[0]);

  for (i = timers_used(timers[0]); i > 0; i--) {
    su_timer_t *t = timers_get(timers[0], i);

    if (su_task_cmp(task, t->sut_task))
      continue;

    timers_remove(timers[0], i);

    su_task_deinit(t->sut_task);

    su_free(NULL, t);
    n++;
  }

  if (!timers_used(timers[0]))
    free(timers->private), timers->private = NULL;

  return n;
#else
  int n = 0;
  su_timer_t *t, *succ, *all = NULL;

  if (timers == NULL || timers->queue == NULL)
    return 0;

  for (t = timers->queue->first; t; t = succ) {
    succ = timer_succ(t);

    if (su_task_cmp(task, t->sut_task))
      continue;

    timer_remove(&timers->queue->tree, t);

    t->_sut_right = all, all = t;

    n++;
  }

  for (t = all; t; t = succ) {
    succ = t->_sut_right;

    su_task_deinit(t->sut_task);

    su_free(NULL, t);
  }

  if (timers->queue->tree != NULL)
    timers->queue->first = timer_first(timers->queue->tree);
  else
    free(timers->queue), timers->queue = NULL;

  return n;
#endif
}

/** Get the root object owning the timer.
 *
 * Return pointer to the root object owning the timer.
 *
 * @param t pointer to the timer
 *
 * @return Pointer to the root object.
 */
su_root_t *su_timer_root(su_timer_t const *t)
{
  return t ? su_task_root(t->sut_task) : NULL;
}


/** Change timer as deferrable (or as undeferrable).
 *
 * A deferrable timer is executed after the given timeout, however, the task
 * tries to avoid being woken up only because the timeout. Deferable timers
 * have their own queue and timers there are ignored when calculating the
 * timeout for epoll()/select()/whatever unless the timeout would exceed the
 * maximum defer time. The maximum defer time is 15 seconds by default, but
 * it can be modified by su_root_set_max_defer().
 *
 * @param t pointer to the timer
 * @param value make timer deferrable if true, undeferrable if false
 *
 * @return 0 if succesful, -1 upon an error
 *
 * @sa su_root_set_max_defer()
 *
 * @NEW_1_12_7
 */
int su_timer_deferrable(su_timer_t *t, int value)
{
  int live_timer;

  if (t == NULL || su_task_deferrable(t->sut_task) == NULL)
    return errno = EINVAL, -1;

  value = value != 0;

  /* Check for fast path: the value is unchanged */
  if (t->sut_deferrable == (unsigned int) value)
    return 0;

  live_timer = SU_TIMER_IS_SET(t);

  /* If the timer is live, remove it from the old queue */
  if (live_timer) {
    su_timer_queue_t *timers = su_timer_queue(t, 0, "su_timer_deferrable");
    su_timer_dequeue(timers, t);
  }

  t->sut_deferrable = value;

  /* If the timer is live, insert it into the proper queue */
  if (live_timer) {
    su_timer_queue_t *timers = su_timer_queue(t, 0, "su_timer_deferrable");
    return su_timer_set0(timers, t, t->sut_wakeup, t->sut_arg, t->sut_when, 0);
  }

  return 0;
}
