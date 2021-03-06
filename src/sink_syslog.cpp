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

#include <ealogger/sink_syslog.h>

namespace eal = ealogger;
namespace con = ealogger::constants;

eal::SinkSyslog::SinkSyslog(std::string msg_template,
                            std::string datetime_pattern, bool enabled,
                            con::LOG_LEVEL min_lvl)
    : eal::Sink(std::move(msg_template), std::move(datetime_pattern), enabled,
                min_lvl)
{
#ifdef EALOGGER_SYSLOG
    this->loglevel_syslog_lookup = {
        {con::LOG_LEVEL::EAL_DEBUG, LOG_DEBUG},
        {con::LOG_LEVEL::EAL_INFO, LOG_INFO},
        {con::LOG_LEVEL::EAL_WARNING, LOG_WARNING},
        {con::LOG_LEVEL::EAL_ERROR, LOG_ERR},
        {con::LOG_LEVEL::EAL_FATAL, LOG_CRIT},
        {con::LOG_LEVEL::EAL_STACK, LOG_CRIT},
        {con::LOG_LEVEL::EAL_INTERNAL,
         LOG_DEBUG}};  // mapping internal to syslog debug
#else
    this->loglevel_syslog_lookup = {};
#endif
}

eal::SinkSyslog::~SinkSyslog() {}
void eal::SinkSyslog::write_message(ATTR_UNUSED const std::string &msg)
{
#ifdef EALOGGER_SYSLOG
    std::lock_guard<std::mutex> lock(this->mtx_syslog);
    if (this->get_log_to_syslog())
        syslog(this->loglevel_syslog_lookup.at(msg_lvl), "%s", msg.c_str());
#endif
}

void eal::SinkSyslog::config_changed(){};
