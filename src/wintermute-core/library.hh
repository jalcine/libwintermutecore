/*
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

#include <uv.h>
#include <string>
#include <unordered_map>
#include <wintermute-core/library.hpp>

using std::string;

namespace Wintermute {
  class LibraryPrivate {
    public:
      typedef uv_lib_t HandleType;
      typedef SharedPtr<HandleType> HandlePtr;

      explicit LibraryPrivate();
      virtual ~LibraryPrivate();
      bool claimHandle(const string& filename);
      bool freeHandle();

      HandlePtr handle;
      string filename;
      Library::LoadState loadState;

    private:
      static std::unordered_map<string, HandlePtr> libraryHandles;
  };
}
