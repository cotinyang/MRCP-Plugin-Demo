/*
 * This file is part of the Sofia-SIP package
 *
 * Copyright (C) 2005, 2011 Nokia Corporation.
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

#ifndef SOFIA_SIP_SU_RANDOM_H
/** Defined when <sofia-sip/su_random.h> has been included. */
#define SOFIA_SIP_SU_RANDOM_H

/**@ingroup su_random
 * @file sofia-sip/su_random.h
 *
 * Random numbers.
 *
 * @author Pekka Pessi <Pekka.Pessi@nokia.com>
 */

#ifndef SU_TYPES_H
#include <sofia-sip/su_types.h>
#endif

SOFIA_BEGIN_DECLS

SOFIAPUBFUN int su_randint(int lb, int ub);

SOFIAPUBFUN int64_t su_randint64(int64_t lb, int64_t ub);

SOFIAPUBFUN void *su_randmem(void *mem, size_t siz);

SOFIAPUBFUN uint32_t su_random(void);

SOFIAPUBFUN uint64_t su_random64(void);

SOFIA_END_DECLS

#endif	/* !defined SOFIA_SIP_SU_RANDOM_H */
