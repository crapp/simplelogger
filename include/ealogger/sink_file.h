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

#ifndef SINK_FILE_H
#define SINK_FILE_H

/** @file sink_file.h */

#include <fstream>

#include <ealogger/sink.h>

namespace ealogger
{
/**
 * @addtogroup SINK_GROUP
 * @{
 */

/**
 * @brief Sink to write to a log file
 * @details
 *
 * This sink allows you to write to a log file. It does not support rotating files
 * based on elapsed time or file size.
 * You also have to make sure the application hass appropriate write permissions
 * and the target directory exists.
 */
class SinkFile : public Sink
{
public:
    /**
     * @brief SinkFile constructor with additional parameters
     *
     * @param msg_template String with conversion specifiers
     * @param datetime_pattern Conversion specifiers for date time
     * @param enabled Whether or not this sink is enabled
     * @param min_lvl Minimum severity
     * @param log_file Log file
     * @param flush_buffer Flush the ofstream buffer with every message
     *
     * @details
     * Make sure you have write permissions for \p log_file and the corresponding
     * directories exist.
     *
     * The parameter \p flush_buffer can be used to influence the flushing of
     * internal buffers. Normally ealogger leaves this to the system. This means
     * not every message might be written immediately to the file. If you want a
     * different behaviour set flush_buffer to true at the cost of decreasing
     * performance.
     */
    SinkFile(std::string msg_template, std::string datetime_pattern,
             bool enabled, ealogger::constants::LOG_LEVEL min_lvl,
             std::string log_file, bool flush_buffer);
    virtual ~SinkFile();

    /**
     * @brief Set log file
     *
     * @param log_file
     * @warning
     * This is method is not exposed by the API currently. You may have the
     * possibility to work with Sink objects directly in the future. If you need
     * to change the logfile use Logger::init_file_sink to reinit the SinkFile
     * sink with a different logfile
     */
    void set_log_file(std::string log_file);

private:
    std::mutex mtx_file_stream;
    std::mutex mtx_log_file;

    std::ofstream file_stream;
    std::string log_file;
    bool flush_buffer;

    void write_message(const std::string &msg);
    /**
     * @brief Called when Sink::set_enabled was called
     * @details
     * Opens or closes logfile according to Sink#enabled
     */
    void config_changed();
    /**
     * @brief Open logfile
     */
    void open_file();
    /**
     * @brief Close logfile
     */
    void close_file();
};
/** @} */
}

#endif /* SINK_FILE_H */
