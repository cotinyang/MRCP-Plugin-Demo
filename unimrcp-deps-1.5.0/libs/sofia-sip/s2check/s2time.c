/*
 * This file is part of the Sofia-SIP package
 *
 * Copyright (C) 2009 Nokia Corporation.
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

#include "config.h"

#include "s2util.h"
#include <sofia-sip/su_time.h>

#include <string.h>
#include <stdio.h>

/* -- Delay scenarios --------------------------------------------------- */

static su_time64_t time64_offset;

extern void (*_su_time)(su_time_t *tv);
extern su_time64_t (*_su_nanotime)(su_time64_t *t);
extern su_time64_t (*_su_monotime)(su_time64_t *t);

static void _su_time_fast_forwarder(su_time_t *tv)
{
  su_time64_t t64 = su_time_to_time64(*tv);

  *tv = su_time64_to_time(t64 + time64_offset);
}

static su_time64_t _su_time64_fast_forwarder(su_time64_t *t)
{
  return *t = *t + time64_offset;
}

su_duration_t s2_quickstep(su_root_t *root,
			   su_duration_t step,
			   su_duration_t forward)
{
  _su_time = _su_time_fast_forwarder;
  _su_nanotime = _su_time64_fast_forwarder;
  _su_monotime = _su_time64_fast_forwarder;

  time64_offset += 1000000 * forward;

  return su_root_step(root, step);
}

void s2_fast_forward(unsigned long seconds,
		     su_root_t *root)
{
  _su_time = _su_time_fast_forwarder;
  _su_nanotime = _su_time64_fast_forwarder;
  _su_monotime = _su_time64_fast_forwarder;

  time64_offset += SU_E9 * (su_time64_t)seconds;

  if (root)
    su_root_step(root, 0);
}

void
s2_timed_logger(void *stream, char const *fmt, va_list ap)
{
  char buffer[4096];
  su_time_t now = su_now();
  size_t prefix, wrote;
  int n;

  snprintf(buffer, sizeof buffer,
	   "%02u:%02u:%02u.%06lu[+%.3f] ",
	   (unsigned)(now.tv_sec / 3600 % 24),
	   (unsigned)(now.tv_sec / 60 % 60),
	   (unsigned)(now.tv_sec % 60),
	   now.tv_usec,
	   time64_offset * 1e-9);
  prefix = strlen(buffer);
  n = vsnprintf(buffer + prefix, (sizeof buffer) - prefix, fmt, ap);
  if (n > 0)
    wrote = fwrite(buffer, prefix + n, 1, stream);
}
