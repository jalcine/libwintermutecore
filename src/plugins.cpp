/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @legalese
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 */

#include <QDir>
#include <QtDebug>
#include <QCoreApplication>
#include "config.hpp"
#include "plugins.hpp"
#include "core.hpp"

using namespace std;

namespace Wintermute {
    namespace Plugins {
        PluginList Factory::s_allPlgns;

        /// @todo Find a means of storing plugins that should be loaded.
        /// @todo Load the plugins designated to be loaded.
        /// @todo Have plugins be found in "${PREFIX_LIB}/plugins" (/usr/lib/wintermute/plugins via their corresponding ".plgn". They should only load this information into Wintermute, and only load plugins marked to be loaded.
        void Factory::Startup () {
            qDebug() << "(core) [Factory] Starting up...";
            QCoreApplication::addLibraryPath (WINTER_PLUGINS_PATH);
            QDir *d = new QDir(WINTER_PLUGINS_PATH);
            d->setFilter (QDir::Files | QDir::Readable | QDir::NoSymLinks);
            d->setSorting (QDir::Name);

            QStringList l_pls = d->entryList();
            foreach (QString l_pd, l_pls)
                Factory::loadPlugin (string(WINTER_PLUGINS_PATH) + string("/") + l_pd.toStdString ());

#ifdef WINTERMUTE_USING_GUI
            Factory::loadPlugin ("/usr/lib/libwntrgui.so");
#endif
        }

        /// @todo Unload every loaded plugin and free all resources.
        void Factory::Shutdown () {
            qDebug() << "(core) [Factory] Unloading plugins..";
            foreach (PluginBase* l_plgnBs, s_allPlgns){
                qDebug() << "(core) [Factory] Unloaded" << l_plgnBs->name () << ".";
                l_plgnBs->deinitialize ();
            }
        }

        const PluginBase* Factory::loadPlugin (const string &p_pth) {
            return loadPlugin((new QFile(QString::fromStdString (p_pth))));
        }

        void Factory::unloadPlugin(const QString& p_uuid){
            PluginBase* l_plgn = Factory::s_allPlgns.take (p_uuid);
            delete l_plgn;
        }

        /// @todo Improve dependency checking.
        const PluginBase* Factory::loadPlugin (const QFile *p_fl){
            if (!p_fl->exists ()){
                qWarning() << "(core) [Factory] Failed to load " << p_fl->fileName () << p_fl->errorString ();
                return NULL;
            }

            //QSettings* l_plgnSpec = new QSettings(p_fl->fileName (),QSettings::IniFormat);

            QPluginLoader* l_plgnLdr = new QPluginLoader(p_fl->fileName ());
            l_plgnLdr->setLoadHints (QLibrary::ExportExternalSymbolsHint | QLibrary::ResolveAllSymbolsHint);
            if (l_plgnLdr->load ()){
                PluginBase* l_plgnBase = dynamic_cast<PluginBase*>(l_plgnLdr->instance ());
                l_plgnBase->m_plgnLdr = l_plgnLdr;

                if (!l_plgnBase->isSupported ()){
                    qWarning() << "(core) [Factory] The plugin" << l_plgnBase->name () << "is incompatiable with this version of Wintermute.";
                    return NULL;
                } else
                    qDebug() << "(core) [Factory] Plugin" << l_plgnBase->name () << "v." << l_plgnBase->version() << "loaded.";

                qDebug() << "(core) [Factory] Dependency checking skipped.";

                l_plgnBase->doInitializing ();
                QObject::connect (Core::instance (),SIGNAL(deinitialized()),l_plgnBase,SLOT(doDeinitialize()));

                Factory::s_allPlgns.insert (l_plgnBase->uuid (),l_plgnBase);
                return l_plgnBase;
            } else {
                qWarning() << "(core) [Factory] Err loading plugin:" << l_plgnLdr->errorString ();
                return NULL;
            }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
