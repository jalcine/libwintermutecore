/**
 *
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <Wintermute/Logging>
#include <Wintermute/PluginInterfaceable>

namespace Wintermute
{
class PluginPrivate
{
public:
  Q_DECLARE_PUBLIC ( Plugin );
  Plugin* q_ptr;
  QString id;
  QSettings* settings;
  QPluginLoader* loader;

  PluginPrivate ( Plugin* q ) : q_ptr ( q ), settings ( 0 ), loader ( 0 ) { }

  ~PluginPrivate() { }

  bool loadBinary() {
    loader->setLoadHints ( QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint );
    return loader->load();
  }

  bool unloadBinary() {
    return loader->unload();
  }

  PluginInterfaceable* tryLoad ( QPluginLoader* pluginLoader ) {
    Q_Q ( Plugin );
    this->loader = pluginLoader;
    if ( !this->loadBinary() ) {
      werr ( q_ptr, QString ( "Can't load binary!" ).arg ( pluginLoader->errorString() ) );
      return 0;
    } else {
      if ( !pluginLoader->isLoaded() ) {
        werr ( q_ptr, QString ( "Failed to load plugin binary. Error: %1" ).arg ( pluginLoader->errorString() ) );
        return 0;
      } else
        { winfo ( q_ptr, QString ( "Plugin interface loaded for %1" ).arg ( q->id() ) ); }
    }
    return this->getPluginInterface();
  }

  PluginInterfaceable* getPluginInterface() {
    QObject* obj = this->loader->instance();
    PluginInterfaceable* interface = 0;
    interface = dynamic_cast<PluginInterfaceable*> ( obj );
    return interface;
  }
};
}
