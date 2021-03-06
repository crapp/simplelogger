//   ealogger is a simple, asynchronous and powerful logger library for c++
//   Copyright 2013 - 2016 Christian Rapp
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef EALOGGER_H
#define EALOGGER_H

/**
 * @file ealogger.h
 */

#include <string>
/*
 * Mutual exclusion for threadsafe logger
 */
#include <csignal>
#include <iostream>
#include <mutex>
#include <stdexcept>
/*
 * Background logger thread
 */
#include <map>
#include <thread>
#include <vector>

#include <ealogger/global.h>
#include <ealogger/logmessage.h>
#include <ealogger/logqueue.h>
#include <ealogger/sink_console.h>
#include <ealogger/sink_file.h>
#include <ealogger/sink_syslog.h>
#include "config.h"

/**
 *@brief Main namespace for ealogger
 */
namespace ealogger
{
/**
 * @defgroup EALOGGER_GROUP Base
 * @brief The base group of ealogger describes the core functionality of the
 * logger lib
 * @sa
 * Logger
 *
 * @{
 */

// Define macros for all log levels and call public member write_log()
/**
 * @def eal_debug(msg)
 * @brief Write a debug message
 */
#define eal_debug(msg)                                                  \
    write_log(msg, ealogger::constants::LOG_LEVEL::EAL_DEBUG, __FILE__, \
              __LINE__, __func__)
/**
 * @def eal_info(msg)
 * @brief Write a info message
 */
#define eal_info(msg)                                                  \
    write_log(msg, ealogger::constants::LOG_LEVEL::EAL_INFO, __FILE__, \
              __LINE__, __func__)
/**
 * @def eal_warn(msg)
 * @brief Write a warning message
 */
#define eal_warn(msg)                                                     \
    write_log(msg, ealogger::constants::LOG_LEVEL::EAL_WARNING, __FILE__, \
              __LINE__, __func__)
/**
 * @def eal_error(msg)
 * @brief Write an error message
 */
#define eal_error(msg)                                                  \
    write_log(msg, ealogger::constants::LOG_LEVEL::EAL_ERROR, __FILE__, \
              __LINE__, __func__)
/**
 * @def eal_fatal(msg)
 * @brief Write a fatal message
 */
#define eal_fatal(msg)                                                  \
    write_log(msg, ealogger::constants::LOG_LEVEL::EAL_FATAL, __FILE__, \
              __LINE__, __func__)
/**
 * @def eal_stack()
 * @brief Write a message with a stacktrace
 */
#define eal_stack()                                                    \
    write_log("", ealogger::constants::LOG_LEVEL::EAL_STACK, __FILE__, \
              __LINE__, __func__)

/**
 * @brief ealogger main class
 * @author Christian Rapp (crapp)
 *
 * @details
 * The Logger class provides all the functionality you need for your application
 * to log messages. It can be as simple as in the following example.
 *
 * @code
 * #include <ealogger/ealogger.h>
 * int main() {
 *     ealogger::Logger logger;
 *     logger.init_console_sink();
 *     logger.eal_debug("My application is just awesome");
 *     return 0;
 * }
 * @endcode
 *
 * This will print *My application is awesome* to the console.
 *
 * ealogger uses sinks to write to different targets. Each Sink supports different
 * message [conversion patterns](@ref ConversionPattern) and a datetime pattern.
 * Each sink can be enabled or disabled independently and the minimum severity can be set.
 * You have to use Logger::init_console_* to init a sink. This methods define sane
 * defaults for their options. But of course you can change them to whatever you
 * want.
 *
 * The Methods set_msg_template, set_datetime_pattern, set_enabled and set_min_lvl
 * allow you to change the configuration of a sink. If the sink supports more
 * options you have to use the dedicated init method to reinitialize the sink.
 *
 * To make it easy to write messages with a specific severity there are some macro
 * functions for each log level and one for stacktrace (#eal_debug(msg) #eal_info(msg)
 * #eal_warn(msg) #eal_error(msg) #eal_fatal(msg) #eal_stack())
 * Logger::write_log allows you to write log messages without using these macros.
 *
 * ealogger and its sinks are threadsafe. Meaning if you use the same instance all
 * over your application it will make sure only one message at a time is written
 * to an iostream for example and the internal message queue is synchronized.
 */
class Logger
{
public:
    /**
     * @brief Logger constructor
     * @param async Boolean if activated ealogger uses a background thread to
     * write messages to a Sink
     *
     * @details
     * Use the Parameter async to activate a background logger thread. This way
     * logging will no longer slow down your application which is important for high
     * performance or time critical events. The only overhead is creating a LogMessage
     * object and pushing it on a std::queue.
     */
    Logger(bool async = true);
    ~Logger();

    /**
     * @brief Write a log message
     *
     * @param msg Message text
     * @param lvl Severity of the message
     * @param file File from where the method was called
     * @param lnumber Line number
     * @param func Function name
     *
     * @details
     *
     * This method is called by the macros that are defined in this header file
     * for the different log levels. You can of course call this method yourself
     * @code
     * mylogger.write_log("This is a warning", ealogger::constants::LOG_LEVEL::EAL_WARNING,
     *                    __FILE__, __LINE__, __func__);
     * mylogger.write_log("This is a warning without line file and func",
     *                    ealogger::constants::LOG_LEVEL::EAL_WARNING);
     * @endcode
     */
    void write_log(std::string msg, ealogger::constants::LOG_LEVEL lvl,
                   std::string file, int lnumber, std::string func);


     // overload void write_log(std::string, ealogger::constants::LOG_LEVEL, std::string, int, std::string)

    /**
     * @brief Write a log message
     *
     * @param msg Message text
     * @param lvl Severity of the message
     *
     * @note
     * Using conversion patterns for source file, line number or function with
     * this function will not give you the appropriate information as it is not
     * available
     */
    void write_log(std::string msg, ealogger::constants::LOG_LEVEL lvl);

    /**
     * @brief Init a syslog Sink
     *
     * @param enabled Choose whether this sink is enabled or not
     * @param min_lvl Minimum severity for this sink
     * @param msg_template Message template based on conversion patterns
     * @param datetime_pattern Datetime conversion patterns
     * @details
     *
     * This initializes a sink that allows you to write a message to the system
     * syslog.
     *
     * @note
     * This does only work on supported systems
     *
     * @sa
     * ConversionPattern and SinkSyslog
     */
    void init_syslog_sink(bool enabled = true,
                          ealogger::constants::LOG_LEVEL min_lvl =
                              ealogger::constants::LOG_LEVEL::EAL_DEBUG,
                          std::string msg_template = "%s: %m",
                          std::string datetime_pattern = "%F %T");
    /**
     * @brief Initialize the console Sink
     *
     * @param enabled Choose whether this sink is enabled or not
     * @param min_lvl Minimum severity for this sink
     * @param msg_template Message template based on conversion patterns
     * @param datetime_pattern Datetime conversion patterns
     * @details
     *
     * Initializes a sink to write a message to the console. The layout of the
     * message is based on \p msg_template.
     *
     * @sa
     * ConversionPattern and SinkConsole
     */
    void init_console_sink(bool enabled = true,
                           ealogger::constants::LOG_LEVEL min_lvl =
                               ealogger::constants::LOG_LEVEL::EAL_DEBUG,
                           std::string msg_template = "%d %s: %m",
                           std::string datetime_pattern = "%F %T");

    /**
     * @brief Initialize the simple file Sink
     *
     * @param enabled Choose whether this sink is enabled or not
     * @param min_lvl Minimum severity for this sink
     * @param msg_template Message template based on conversion patterns
     * @param datetime_pattern Datetime conversion patterns
     * @param logfile Logfile to use
     * @param flush_buffer Flush the ofstream buffer with every message
     * @details
     *
     * This method initializes a file sink. Using a file sink you can write to
     * a logfile that was specified with \p logfile. The file will be created if
     * it does not exist otherwise new messages will be appended.
     *
     * @note
     * ealogger will not create any directories for you and you have to make sure
     * the target location is writeable by the user that runs the application.
     *
     * @sa
     * ConversionPattern and SinkFile
     * Logger::discard_sink
     */
    void init_file_sink(bool enabled = true,
                        ealogger::constants::LOG_LEVEL min_lvl =
                            ealogger::constants::LOG_LEVEL::EAL_DEBUG,
                        std::string msg_template = "%d %s [%f:%l] %m",
                        std::string datetime_pattern = "%F %T",
                        std::string logfile = "ealogger_logfile.log",
                        bool flush_buffer = false);
    // void init_file_sink_rotating(bool enabled,
    // ealogger::constants::LOG_LEVEL min_lvl,
    // std::string msg_template,
    // std::string datetime_pattern,
    // std::string logfile);
    /**
     * @brief Discard a Sink and delete the object
     *
     * @param sink The Sink you want to discard
     * @details
     *
     * You can discard a Sink. This will reduce some overhead and can speed up
     * logging a little bit.
     */
    void discard_sink(ealogger::constants::LOGGER_SINK sink);

    /**
     * @brief Check if a Sink has been initialized already
     *
     * @param sink The Sink you want to check
     *
     * @return True if the Sink has been initialized
     */
    bool is_initialized(ealogger::constants::LOGGER_SINK sink);

    /**
     * @brief Set message template for a Sink
     *
     * @param sink
     * @param msg_template Template consisting of conversion patterns
     * @details
     * You can use conversion patterns and arbitrary strings to create a message
     * template for a Sink.
     *
     * @sa
     * ConversionPattern
     */
    void set_msg_template(ealogger::constants::LOGGER_SINK sink,
                          std::string msg_template);
    /**
     * @brief Set datetime conversion pattern for a Sink
     *
     * @param sink
     * @param datetime_pattern
     *
     * @details
     * Date time conversion is based on [std::strftime](http://en.cppreference.com/w/cpp/chrono/c/strftime)
     * You can use all the conversion specifiers mentioned there.
     */
    void set_datetime_pattern(ealogger::constants::LOGGER_SINK sink,
                              std::string datetime_pattern);
    /**
     * @brief Activate or deactivate a Sink
     *
     * @param sink
     * @param enabled
     */
    void set_enabled(ealogger::constants::LOGGER_SINK sink, bool enabled);
    /**
     * @brief Set the minimum log message severity for a Sink
     *
     * @param sink
     * @param min_level
     */
    void set_min_lvl(ealogger::constants::LOGGER_SINK sink,
                     ealogger::constants::LOG_LEVEL min_level);

    /**
     * @brief Check if the message queue is empty
     *
     * @return True if the message is empty, otherwise false.
     */
    bool queue_empty();

private:
    /** Mutex used when not in async mode */
    std::mutex mtx_logger_stop;

    static bool signal_SIGUSR1;

    bool async;

    /** Threadsafe queue for async mode */
    LogQueue log_msg_queue;
    /** Background thread */
    std::thread logger_thread;
    /** Controls background logger thread */
    bool logger_thread_stop;

    std::map<ealogger::constants::LOGGER_SINK, std::shared_ptr<Sink>>
        logger_sink_map;
    std::map<ealogger::constants::LOGGER_SINK, std::unique_ptr<std::mutex>>
        logger_mutex_map;

    /** Static Method to be registered for logrotate signal */
    static void logrotate(int signo);

    void thread_entry_point();

    /**
     * @brief This method writes the LogMessage to all activated sinks
     *
     * @param m LogMessage
     */
    void internal_log_routine(std::shared_ptr<LogMessage> m);

    /*
     * So far controlling the background logger thread is only possible for the
     * logger object itself.
     */
    bool get_logger_thread_stop();
    void set_logger_thread_stop(bool stop);
};
/** @} */
}

#endif  // EALOGGER_H
