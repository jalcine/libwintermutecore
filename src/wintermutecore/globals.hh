/*
 * vim: set ft=cpp :
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef WINTERMUTE_CORE_GLOBALS_HH_
# define WINTERMUTE_CORE_GLOBALS_HH_

#define W_CHECK_UV(r, call)   \
  if (r) { \
    printf("\n[%s] E: %s - %s\n", call, uv_err_name(r), uv_strerror(r)); \
    abort();\
  } \


#endif
