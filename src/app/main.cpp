/***
 * @file main.cpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 *
 * @section lcns Licensing
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
 *
 */

// Local
#include "app/logging.hpp"
#include "app/core.hpp"
#include "app/core.hxx"

// Qt
#include <QApplication>

using namespace Wintermute;

int main (int p_argc, char** p_argv)
{
    qInstallMsgHandler (Logging::catchQDebugMessage);
    Core core (p_argc, p_argv);
    return QApplication::exec();
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
