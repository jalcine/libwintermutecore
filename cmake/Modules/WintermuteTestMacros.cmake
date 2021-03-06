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

MACRO(wintermute_add_test _prefix _name _hdr)
  SET(_target ${_prefix}-${_name})
  CXXTEST_ADD_TEST(${_target} ${_target}_test.cc ${_hdr})
  WINTERMUTE_LINK_LIBRARIES(${_target})
  WINTERMUTE_ADD_TARGET_PROPERTIES(${_target})
  TARGET_LINK_LIBRARIES(${_target} wintermute-core)
  TARGET_INCLUDE_DIRECTORIES(${_target} PUBLIC ${WINTERMUTE_TEST_INCLUDE_DIRS})
ENDMACRO(wintermute_add_test)
