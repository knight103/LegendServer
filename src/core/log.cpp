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

#include "log.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#if LOG_IMPLEMENT == LOG_IMPLEMENT_PLOG
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

static plog::ConsoleAppender<plog::TxtFormatter> plogOut;
#endif

static const int MAX_LOG_LENGTH = 16*1024;

void _log(int level, const char* fmt, ...) {
    int bufferSize = MAX_LOG_LENGTH;
    char* buf = nullptr;
    int nret = 0;
    va_list args;
    do
    {
        buf = new char[bufferSize];
        if (buf == nullptr)
            return;
        /*
        pitfall: The behavior of vsnprintf between VS2013 and VS2015/2017 is different
        VS2013 or Unix-Like System will return -1 when buffer not enough, but VS2015/2017 will return the actural needed length for buffer at this station
        The _vsnprintf behavior is compatible API which always return -1 when buffer isn't enough at VS2013/2015/2017
        Yes, The vsnprintf is more efficient implemented by MSVC 19.0 or later, AND it's also standard-compliant, see reference: http://www.cplusplus.com/reference/cstdio/vsnprintf/
        */
        va_start(args, fmt);
        nret = vsnprintf(buf, bufferSize - 3, fmt, args);
        va_end(args);

        if (nret >= 0)
        { // VS2015/2017
            if (nret <= bufferSize - 3)
            {// success, so don't need to realloc
                break;
            }
            else
            {
                bufferSize = nret + 3;
                delete[] buf;
            }
        }
        else // < 0
        {	// VS2013 or Unix-like System(GCC)
            bufferSize *= 2;
            delete[] buf;
        }
    } while (true);
    buf[nret] = '\n';
    buf[++nret] = '\0';

#if LOG_IMPLEMENT == LOG_IMPLEMENT_DEFAULT
	fprintf(stdout, "%s", buf);
	fflush(stdout);
#elif LOG_IMPLEMENT == LOG_IMPLEMENT_PLOG
	plog::init(plog::verbose, &plogOut);
	LOG_(PLOG_DEFAULT_INSTANCE, plog::Severity(level)) << buf;
#endif

    delete[] buf;
}
