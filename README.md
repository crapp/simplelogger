#ealogger#

The **e**asy **a**synchronous logger provides a simple to use yet powerful logging
functionality for c++ applications.
You can use it to log to **stdout**, to a **logfile** or **system log (syslog)**.
It was designed to be fast (uses Threads) and flexible. The library registers a
signal handler for SIGUSR1 and is compatible with logrotate (only available on
linux currently). It was tested on Linux, Windows and OS X.

**Table of Contents**  *generated with [DocToc](http://doctoc.herokuapp.com/)*

- [ealogger](#ealogger)
	- [Compilation](#compilation)
	- [Usage](#usage)
		- [Minimum Code example](#minimum-code-example)
		- [Colorized Logfiles with multitail](#colorized-logfiles-with-multitail)
	- [Source Code Documentation](#source-code-documentation)
	- [ToDo](#todo)
	- [FAQ](#faq)
	- [License](#license)


##Compilation##

the Library uses the [CMake](http://cmake.org/) build system. This way you
can easily build it on different platforms. No special dependencies are
required apart from CMake >= 2.8 and a **c++11** compatible compiler (e.g. gcc >=4.7;
Visual Studio 2013 (2010 and 2012 are not supported)).

Example for building a shared library without debug symbols. We are using an
out of source build here.
```shell
# clone the sources from github
git clone https://github.com/crapp/ealogger.git ealogger
# change directory and create build directory
cd ealogger
mkdir build
cd build
# run cmake from within the build directory
cmake ../ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make
# install the shared library
sudo make install
```

This will install libealogger to /usr/local/include on Unix systems.

You may also use [different generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
for other platforms. This is useful on OS X or Windows. CMake allows you for
example to generate Visual Studio or XCode project files in order to build the library.

##Usage##

Usage of this Library is quite simple. We provide a simple tester that you can
build with the library using the cmake command line argument `-DBUILD_TEST=ON`.

###Minimum Code example###

Here is a small example. First we instantiate a new Logger object, than we print
some messages.

```c++
std::unique_ptr<EALogger> log = std::unique_ptr<EALogger>(
    new EALogger(EALogger::logLevels::INFO,
                     true,
                     true,
                     false,
                     false,
                     "%H:%M:%S",
                     "logToMe.log"));
log->writeLog(EALogger::logLevels::DEBUG,
             "Do you see me?");
log->writeLog(EALogger::logLevels::WARNING,
             "A warning message");
log->writeLog(EALogger::logLevels::ERROR,
             "An error message");
log->writeLog(EALogger::logLevels::FATAL,
             "A fatal message");
log->printStackTrace(10);
```
This will output:
```shell
[17:55:28] WARNING: A warning message
[17:55:28] ERROR: An error message
[17:55:28] FATAL: A fatal message
[17:55:28] Stacktrace:
    .../libealogger.so(_ZN12EALogger15printStackTraceEj+0xb1) [0x7fb21db14ce3]
  .../logtest(main+0x6f6) [0x402cb3]
  /usr/lib/libc.so.6(__libc_start_main+0xf5) [0x7fb21cd3bb05]
  .../logtest() [0x4024f9]
```

As you can see the DEBUG level message is not printed. This is because of the minimum severity
we set when we created the object. At the end we are printing a Stacktrace.
c++ methods are not demangled currently.

###Colorized Logfiles with multitail###

We all know large logfiles are difficult to read. So some sort of color
highlighting might be useful. If you are using a Unix system you may try
[multitail](http://www.vanheusden.com/multitail/)

Here is a screenshot how this might look like
![ealogger multitail](http://crapp.github.io/simplelogger/screenshots/SimpleLoggerMultitail.jpeg "EALogger multitail")

The color scheme for multitail I used to generate the above colors in the
screenshot looks like this

    colorscheme:ealogger
    cs_re:,magenta,bold:^.+FATAL:.+
    cs_re:,red,bold:^.+ERROR:.+
    cs_re:,blue:^.+WARNING:.+
    cs_re:,green:^.+INFO:.+

Put it in ~/.multitailrc and start mutlitail
```shell
multitail -cS simpleLogger mylogfile.log
```
##Source Code Documentation##

Is available as doxygen generated html documentation. The doxygen project file is
located in the  [doc](https://github.com/crapp/ealogger/tree/master/doc) folder.
You may use it to generate the documentation.

##ToDo##

* Print stacktrace method does not demangle c++ method names. So the stacktrace
  is not so easy to read. I found a solution on the net and will implement it soon.
* Printing stacktrace does only work with gcc/llvm. There is a possible solution
  for Windows though.
* Support for other logrotation services like newsyslog or LogRotateWin

##FAQ##

**Yet another logger lib, why?**

I wanted to have a flexible and easy to use logger lib for my c++ projects.
Especially one that makes use of the new c++11 features.

**Why does it not have feature X?**

Feel free to fork the project and make a pull request!

##License##

```
Copyright (C) 2013 - 2016 Christian Rapp

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
