/****************************************************************************
Copyright (c) 2018 harry.zhu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#define LOG_IMPLEMENT_DEFAULT	0
#define LOG_IMPLEMENT_PLOG		1

#define LOG_IMPLEMENT LOG_IMPLEMENT_DEFAULT

enum LogLevel {
    none = 0,
    fatal = 1,
    error = 2,
    warning = 3,
    info = 4,
    debug = 5,
    verbose = 6
};

void _log(int level, const char* fmt, ...);


#define log_verbose(fmt, ...) _log(LogLevel::verbose, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) _log(LogLevel::debug, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) _log(LogLevel::info, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _log(LogLevel::warning, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) _log(LogLevel::error, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) _log(LogLevel::fatal, fmt, ##__VA_ARGS__)
#define log_none(fmt, ...) _log(LogLevel::none, fmt, ##__VA_ARGS__)
