# vim: set ts=2 sts=2 sw=2 fdm=indent
###############################################################################
# Author: Jacky Alciné <me@jalcine.me>
#
# Wintermute is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# Wintermute is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with Wintermute; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
###############################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

if (NOT DEFINED _wntr_func_deps)
  set(_wntr_func_deps ON)
else()
  return()
endif()

include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(basename CFE_basename)
CHECK_FUNCTION_EXISTS(getenv CFE_getenv)
CHECK_FUNCTION_EXISTS(setenv CFE_setenv)
CHECK_FUNCTION_EXISTS(secure_getenv CFE_getenv)
CHECK_FUNCTION_EXISTS(secure_setenv CFE_setenv)
