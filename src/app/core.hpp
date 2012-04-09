/*
 * @section lcns Licensing
 * This program is free software; you can redistribute it and/or
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
 */
/**
 * @file core.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 */

#ifndef WINTERMUTE_CORE_HPP
#define WINTERMUTE_CORE_HPP

// Qt
#include <QObject>
#include <QVariantMap>

// Local
#include <app/global.hpp>
#include <app/diagnoser.hpp>

class QApplication;
class QSocketNotifier;

namespace Wintermute
{

struct CorePrivate;
/**
 * @brief The central management class of Wintermute.
 *
 * The Core holds the vital activities of Wintermute's startup. It
 * handles the actions from the outside system environment and, with a little
 * Qt magic and hours of coding, transfers control to the parts of Wintermute
 * that would be appropriate to be under control.
 *
 * @nonreentrant
 * @class Core wintermute.hpp "core.hpp"
 */
/// @todo Implement a means of having this class catch the signals of the system.
class Core : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Core)
    WINTER_SINGLETON(Core)

public:

    /**
     * @brief Default constructor.
     *
     * @attention This constructor should NEVER be used, it's called by the main()
     * method of Wintermute, and needs never to be called again.
     *
     * @fn Core
     * @param argc The command line argument passed representing the number of given arguments.
     * @param argv The command line argument passed representing the value of each argument.
     * @internal
     */
    explicit Core (int& p_argv, char** p_argc);

    /**
     * @brief Obtains a pointer to the current arguments.
     *
     * This map contains a normalized list of the arguments that were passed on the
     * command line. This is used mainly to save the ease of plug-ins having to check
     * the string list given by Qt.
     *
     * @note This list only contains the qualified arguments in a list. Support for
     *       arbitrary arguments is coming soon.
     * @todo Add support for arbitrary arguments.
     * @fn arguments
     */
    static QVariantMap arguments();

    /**
     * @brief Ends the program; with an optional exit code.
     * @param p_exitCode The exit code to report to the system.
     * @param p_closeRootApplication Whether or not ALL Wintermute instances should be closed.
     * @fn exit
     */
    static void exit (const int p_exitCode = 0,
                      const bool p_closeRootApplication = false);

    /**
     * @brief Ends the program with success code.
     */
    static void quit();

signals:
    /**
     * @brief Raised once the core's ready to go.
     *
     * This signal is emitted when the core's done loading prerequisites.
     * This is <b>after</b> all plug-ins have been loaded and the inter-
     * process communication system is active.
     *
     * @fn initialized
     * @see Wintermute::Core::Initialize()
     */
    void started() const;

    /**
     * @brief Raised once the core's ready to shut down.
     *
     * This signal is emitted when the core's ready to shutdown the system.
     * This is <b>before</b> any plug-ins are unloaded, but not before they're
     * deinitialized.
     *
     * @fn deinitialized
     */
    void stopped() const;

protected:
    /**
     * @brief Configures Wintermute's core.
     *
     * This method holds the instructions to set-up some imperative command line arguments.
     * It then parses each argument and saves it to the global cache of arguments for other
     * utilities to use.
     *
     * @fn Configure
     * @param p_argc The command line argument passed representing the number of given arguments.
     * @param p_argv The command line argument passed representing the value of each argument.
     * @see Wintermute::Core::Initialize()
     */
    static void configure (int& p_argc, char** p_argv);

protected slots:

    /**
     * @brief Initializes the system.
     *
     * Does the first bit of initialization work for the core process of Wintermute by
     * loading the plug-ins and then the data system.
     *
     * @see Wintermute::Core::initialized
     * @fn Initialize
     */
    static void start();

    /**
     * @brief Deinitializes the system.
     *
     * Cleans up all of the work for the core processes and runs the approriate disconnection methods.
     *
     * @see Wintermute::Core::deinitialized
     * @fn Deinitialize
     */
    static void stop ();

private:
    QScopedPointer<CorePrivate> d_ptr;
};
} // namespace

#endif /* WINTERMUTE_CORE_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;