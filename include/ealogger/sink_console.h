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

#ifndef SINK_CONSOLE_H
#define SINK_CONSOLE_H

#include <iostream>

#include "sink.h"

class SinkConsole : public Sink
{
public:
    SinkConsole(std::string msg_pattern, std::string dateime_pattern,
                bool enabled);
    virtual ~SinkConsole();

private:
    void write_message(const std::string &msg);
};

#endif /* SINK_CONSOLE_H */
