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

#include "core/Path.h"

Path::Path(const std::string& path):
_path(path) {
}

Path Path::operator / (const Path& p1) const {
    Path p(_path + p1.str());
    return p;
}

const std::string& Path::str() const {
    return _path;
}

void Path::join(const Path& p) {
    _path + p.str();
    end();
}

void Path::join(const char* p) {
    _path + p;
    end();
}

void Path::end() {
    if (_path.empty()) {
        return;
    }

    if (_path.at(_path.length() - 1) != PATH_SEPARATOR_CHAR) {
        _path.append(PATH_SEPARATOR);
    }
}
