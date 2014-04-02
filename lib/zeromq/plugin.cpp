/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include "plugin.hpp"
#include "globals.hpp"
#include "module.hpp"
#include "plugin.moc"

using Wintermute::ZeroMQ::Plugin;
using Wintermute::ZeroMQ::Module;
using Wintermute::Version;

Plugin::Plugin() : module ( new ZeroMQ::Module ( this ) )
{
}


void
Plugin::stop()
{
  module->stop();
}

void
Plugin::start()
{
  module->start();
}

Q_EXPORT_PLUGIN2 ( wintermute-zeromq, Wintermute::ZeroMQ::Plugin );