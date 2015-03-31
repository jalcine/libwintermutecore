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

#include <list>
#include <string>
#include <libgen.h>
#include "logging.hpp"
#include "library.hpp"
#include "library.hh"

using std::string;
using std::list;
using Wintermute::Library;
using Wintermute::LibraryPrivate;

list<string> collectFilePaths(const string& filePath)
{
  const string baseFilePath = { basename(const_cast<char*>(filePath.c_str())) };
  const char* envWintermutePluginPath = getenv(WINTERMUTE_ENV_PLUGIN_PATH);
  const string ourPathFromEnv = (envWintermutePluginPath) ? envWintermutePluginPath : "";

  list<string> prefixes;
  // Compile a list of paths where this thing can be at.
  list<string> filePaths =
  {
    filePath,
    filePath + ".so",
    "lib" + filePath + ".so",
    "libwintermute" + filePath + ".so",

    baseFilePath,
    baseFilePath + ".so",
    "lib" + baseFilePath + ".so",
    "libwintermute" + baseFilePath + ".so",
  };
  list<string> originalPaths = filePaths;

  const bool hasAList = ourPathFromEnv.find(";") != string::npos;

  if (!hasAList)
  {
    prefixes.push_back(ourPathFromEnv);
  }

  for (string path : originalPaths)
  {
    for (string prefix : prefixes)
    {
      const string thePrefixedPath = prefix + "/" + path;
      filePaths.insert(filePaths.end(), thePrefixedPath);
    }
  }

  return filePaths;
}

Library::Library() : d_ptr(new LibraryPrivate)
{
  wdebug("Allocated an instance of a library.");
}

Library::Library(const string& preloadedFilename) : d_ptr(new LibraryPrivate)
{
  wdebug("Allocated an instance of a library; pre-loading library " + preloadedFilename + "...");
  load(preloadedFilename);
}

Library::~Library()
{
  if (loadedStatus() == LoadIsLoaded)
  {
    wdebug("Destroying this library for " + filename() + ", so unloading symbols first...");
    unload();
  }

  wdebug("Deallocated an instance of a library.");
}

Library::LoadState Library::load(const string& filenameToLoad)
{
  W_PRV(Library);

  if (filenameToLoad.empty())
  {
    return LoadUndefined;
  }

  wdebug("Attempting to load library from the filename '" + filenameToLoad + "'...");

  string errorMessage;
  auto libraryHandlePtr = d->claimHandleForFilename(filenameToLoad, errorMessage);

  if (!libraryHandlePtr)
  {
    d->handlePtr = nullptr;
    d->loadState = LoadNotLoaded;
    werror("Failed to claim handle for library: " + errorMessage);
    return LoadNotLoaded;
  }

  // Update the internals to reflect current state.
  d->handlePtr = libraryHandlePtr;
  d->loadState = LoadIsLoaded;
  d->filename = filenameToLoad;

  return LoadIsLoaded;
}

Library::LoadState Library::unload()
{
  W_PRV(Library);
  string errorMessage;
  const bool wasHandleFreed = d->freeHandle(errorMessage);

  if (!wasHandleFreed)
  {
    werror("Failed to free library handle for " + d->filename + ": " + errorMessage);
    return LoadNotLoaded;
  }

  wwarn("Library handle unloaded for " + d->filename + ".");
  d->filename = "";
  d->loadState = LoadNotLoaded;
  return LoadNotLoaded;
}

Library::LoadState Library::loadedStatus() const
{
  W_PRV(const Library);
  return d->loadState;
}

string Library::filename() const
{
  W_PRV(const Library);
  return d->filename;
}

Library::Ptr Library::find(const string& libraryQuery)
{
  Library::Ptr libraryPtr;
  list<string> filePathsToTry = collectFilePaths(libraryQuery);
  wdebug("Working over " + std::to_string(filePathsToTry.size()) + " paths...");

  for (auto path : filePathsToTry)
  {
    wdebug("Using path " + path + "...");
    Library::Ptr sampleLibraryPtr = make_shared<Library>(path);
    assert(sampleLibraryPtr);
    const bool wasLibraryLoaded = (sampleLibraryPtr->loadedStatus() == LoadIsLoaded);
    wdebug("Was library loaded from " + path + "? " + std::to_string(wasLibraryLoaded));

    if (wasLibraryLoaded)
    {
      libraryPtr = sampleLibraryPtr;
      break;
    }
  }

  return libraryPtr;
}

Library::FunctionPtr Library::resolveFunction(const string& functionName) const
{
  W_PRV(const Library);
  Library::FunctionPtr functionPtr;
  const char* errorMessage;

  wdebug("Resolving function " + functionName + " from " + d->filename);

  if (d->loadState != LoadIsLoaded)
  {
    werror("Can not resolve a function from an unloaded library.");
    return nullptr;
  }

  if (functionName.empty())
  {
    wwarn("Cannot resolve a function with no name.");
    return nullptr;
  }

  const int fcnResolved = uv_dlsym(d->handlePtr, functionName.c_str(), &functionPtr);

  if (fcnResolved < 0)
  {
    errorMessage = uv_dlerror(d->handlePtr);
    if (errorMessage != NULL)
    {
      wdebug("Failed to resolve function '" + functionName + " from " + d->filename + ": " + errorMessage);
    }
    else
    {
      wwarn("The function '" + functionName + "' was not found, empty reference obtained.");
    }

    return nullptr;
  }

  wtrace("Obtained function handle for '" + functionName + "' from " + d->filename + ".");
  return functionPtr;
}
