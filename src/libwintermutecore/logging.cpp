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

// TODO Extract timing code to its own utility library.

#include "logging.hpp"
#include <chrono>
#include <ctime>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/ttcclayout.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>

using Wintermute::Logging;
using log4cxx::LoggerPtr;

W_DECLARE_SINGLETON(Logging)

string get_current_time_as_string()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
	currentTime = std::chrono::high_resolution_clock::now();
	time_t currentTimeInt = std::chrono::high_resolution_clock::to_time_t(currentTime);
	return std::ctime(&currentTimeInt);
}

LoggerPtr obtain_logger(const string& name)
{
	LoggerPtr logger;
	if (name == "root")
	{
		logger = log4cxx::Logger::getRootLogger();
	}
	else
	{
		logger = log4cxx::Logger::getLogger(name);
	}

	return logger;
}

Logging::Logging()
{
	log4cxx::BasicConfigurator::configure();
	LoggerPtr rootLogger = obtain_logger("root");

	log4cxx::LayoutPtr layoutPtr(new log4cxx::TTCCLayout);
	log4cxx::ConsoleAppender* consoleAppender = new log4cxx::ConsoleAppender(layoutPtr);
	log4cxx::FileAppender* fileAppender = new log4cxx::FileAppender(layoutPtr, "wintermute.log");

	consoleAppender->setTarget(log4cxx::ConsoleAppender::getSystemOut());

	log4cxx::AppenderPtr consoleAppenderPtr(consoleAppender);
	log4cxx::AppenderPtr fileAppenderPtr(fileAppender);
	rootLogger->addAppender(consoleAppenderPtr);
	rootLogger->addAppender(fileAppenderPtr);

	info("Started logging session at " + get_current_time_as_string());
}

Logging::~Logging()
{
	info("Terminated logging session at " + get_current_time_as_string());
}

void Logging::setLevel(const Logging::Level& level)
{
	const int log4cxx_level = (uint) level * 1000;
	LoggerPtr logger = log4cxx::Logger::getRootLogger();
	logger->setLevel(log4cxx::Level::toLevel(log4cxx_level));
}

Logging::Level Logging::level() const
{
	LoggerPtr logger = log4cxx::Logger::getRootLogger();
	const int my_level = logger->getLevel()->toInt() / 1000;
	return (Logging::Level) my_level;
}

void Logging::error(const string& message, const string& name)
{
	obtain_logger(name)->error(message);
}

void Logging::debug(const string& message, const string& name)
{
#ifdef WINTERMUTE_DEBUG 
	obtain_logger(name)->debug(message);
#endif
}

void Logging::info(const string& message, const string& name)
{
	obtain_logger(name)->info(message);
}

void Logging::warn(const string& message, const string& name)
{
	obtain_logger(name)->warn(message);
}

void Logging::trace(const string& message, const string& name)
{
#ifdef WINTERMUTE_DEBUG
	obtain_logger(name)->trace(message);
#endif
}