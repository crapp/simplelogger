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

#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <chrono>
#include <ctime>
#include <string>
#include <vector>

#include <ealogger/global.h>

namespace ealogger
{
/**
 * @brief Log message struct
 * @details
 *
 * A log message in ealogger stores lots of information besides the message string
 * a user has provided.
 *
 * First of all there are two types of messages in ealogger. LogMessage::LOGTYPE
 * has two members. STACK messages are messages that are containing a stacktrace.
 *
 * Additionally a LogMessage stores the message severity, the file from where the
 * message was issued as well as the line number and the function name. All these
 * properties are exposed with appropriate getter functions.
 */
struct LogMessage {
public:
    /**
     * @brief The LOGTYPE enum stands for the LogMessage type
     */
    enum LOGTYPE {
        DEFAULT = 0, /**< Normal log message */
        STACK        /**< Log message with a vector of stack elements */

    };

    /**
     * @brief A constant iterator typedef that is internally used
     */
    typedef std::vector<std::string>::const_iterator msg_vec_it;

    /**
     * @brief Initializes a log message object
     *
     * @param severity Severity of the message
     * @param message Message as std::string
     * @param log_type LogMessage#LOGTYPE
     * @param file File from where this log message was issued
     * @param lnumber Line number in file from where this log message was issued
     * @param func Function from where this log message was issued
     */
    LogMessage(ealogger::constants::LOG_LEVEL severity, std::string message,
               LOGTYPE log_type, std::string file, int lnumber, std::string func)
        : severity(severity),
          message(std::move(message)),
          log_type(log_type),
          call_file(std::move(file)),
          call_file_line_num(lnumber),
          call_func(std::move(func))
    {
        this->t = std::chrono::system_clock::now();
    }
    /**
     * @brief Initializes a log message object with a vector of message strings
     *
     * @param severity Severity of the message
     * @param message_vec A vector<std::string> containing the stack elements
     * @param log_type LogMessage#LOGTYPE
     * @param file File from where this log message was issued
     * @param lnumber Line number in file from where this log message was issued
     * @param func Function from where this log message was issued
     */
    LogMessage(ealogger::constants::LOG_LEVEL severity,
               std::vector<std::string> message_vec, LOGTYPE log_type,
               std::string file, int lnumber, std::string func)
        : severity(severity),
          message_vec(std::move(message_vec)),
          log_type(log_type),
          call_file(std::move(file)),
          call_file_line_num(lnumber),
          call_func(std::move(func))
    {
        this->t = std::chrono::system_clock::now();
        this->message = "";
    }

    /**
     * @brief Return the time_point when this message was created
     * @return std::time_t object
     */
    std::time_t get_timepoint()
    {
        return std::chrono::system_clock::to_time_t(this->t);
    }
    /**
     * @brief Returns the severity of the message
     * @return Return severity
     */
    ealogger::constants::LOG_LEVEL get_severity() { return this->severity; }
    /**
     * @brief Get the log message
     * @return Log message as std::string
     */
    std::string get_message() { return this->message; }
    /**
     * @brief Get the log message type
     * @return LogMessage#LOGTYPE
     */
    LOGTYPE get_log_type() { return this->log_type; }
    /**
     * @brief Returns a constant iterator pointing the begin of the message vector
     * @return #msg_vec_it
     */
    msg_vec_it get_msg_vec_begin() { return this->message_vec.cbegin(); }
    /**
     * @brief Returns a constant iterator pointing the end of the message vector
     * @return #msg_vec_it
     */
    msg_vec_it get_msg_vec_end() { return this->message_vec.cend(); }
    /**
     * @brief Return file from where this log message was issued
     * @return
     */
    std::string get_call_file() { return this->call_file; }
    /**
     * @brief Return line number in file from where this log message was issued
     * @return
     */
    int get_call_file_line() { return this->call_file_line_num; }
    /**
     * @brief Return function name from where this log message was issued
     * @return
     */
    std::string get_call_func() { return this->call_func; }
private:
    /** Time Point when this log message was created*/
    std::chrono::system_clock::time_point t;
    /** Severity of this message */
    ealogger::constants::LOG_LEVEL severity;
    /** The log message */
    std::string message;
    /** A vector of messages */
    std::vector<std::string> message_vec;
    /** The log message type */
    LOGTYPE log_type;
    std::string
        call_file; /**< The source file from which the logger was called */
    int call_file_line_num; /**< Line number in the source file */
    std::string call_func;  /**< function from which the logger was called */
};
}

#endif  // LOGMESSAGE_H
