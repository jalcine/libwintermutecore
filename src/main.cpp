/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/
/**
 * @namespace Wintermute
 * @brief     Represents the parent namespace of Wintermute
 */
/**
 * @namespace Wintermute::Procedure
 * @brief     Represents the parent namespace of Wintermute::Procedure.
 */
/**
 * @namespace Wintermute::Events
 * @brief     Represents the parent namespace of Wintermute::Events.
 */
/**
 * @defgroup rcp Remote Procedure Calling
 * @addtogroup Wintermute::Procedure::Module
 * @addtogroup Wintermute::Procedure::Dispatcher
 * @addtogroup Wintermute::Procedure::Receiver
 * @addtogroup Wintermute::Procedure::Message
 *
 * %Wintermute takes remote procedure calling to an interesting level. It
 * abstracts different messaging systems like D-Bus and ZeroMQ so that the
 * %Wintermute processes and its friends could send and receive information
 * regardless of location and/or speed. The basis of the RPC system is built
 * using `Dispatcher`, `Receiver` and `Message` classes.
 */

#include "Wintermute/Application"

using Wintermute::Application;

int main ( int argc, char** argv )
{
  return Application::run ( argc, argv );
}
