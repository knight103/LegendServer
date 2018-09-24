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

#include "FileUtils.h"
#include <sys/stat.h>
#include "Constant.h"

typedef enum
{
    SAX_NONE = 0,
    SAX_KEY,
    SAX_DICT,
    SAX_INT,
    SAX_REAL,
    SAX_STRING,
    SAX_ARRAY
}SAXState;

typedef enum
{
    SAX_RESULT_NONE = 0,
    SAX_RESULT_DICT,
    SAX_RESULT_ARRAY
}SAXResult;

FileUtils* FileUtils::_instance = nullptr;

FileUtils* FileUtils::getInstance(void)
{
    if (!_instance)
    {
        _instance = new FileUtils();
        _instance->init();
    }
    return _instance;
}

void FileUtils::destroyInstance()
{
    CC_SAFE_DELETE(_instance);
}


void FileUtils::setDelegate(FileUtils *delegate)
{
    if (_instance)
        delete _instance;

    _instance = delegate;
}

FileUtils::FileUtils()
    : _writablePath("")
{
}

FileUtils::~FileUtils()
{
}

bool FileUtils::init()
{
    _searchPathArray.push_back(_defaultResRootPath);
    _searchResolutionsOrderArray.push_back("");
    return true;
}

std::string FileUtils::getStringFromFile(const std::string& filename)
{
    std::string s = "";
    std::string fullPath = fullPathForFilename(filename);

    FILE *fp = fopen(fullPath.c_str(), "rb");
    if (fp) {
        auto descriptor = fileno(fp);
        struct stat statBuf;
        if (fstat(descriptor, &statBuf) == -1) {
            fclose(fp);
            return "";
        }
        size_t size = statBuf.st_size;

        char* buffer = new char[size];

        size_t readsize = fread(buffer, 1, size, fp);
        fclose(fp);

        s.assign(buffer, readsize);
        delete[] buffer;
    }
    return s;
}

std::string FileUtils::fullPathForFilename(const std::string &filename) const
{
    if (filename.empty())
    {
        return "";
    }

    if (isAbsolutePath(filename))
    {
        return filename;
    }

    std::string fullpath;
    
    fullpath = _defaultResRootPath + filename;

    return fullpath;
}

bool FileUtils::isAbsolutePath(const std::string& path) const
{
    return (path[0] == '/');
}

bool FileUtils::isFileExist(const std::string& filename) const
{
    if (isAbsolutePath(filename))
    {
        return isFileExistInternal(filename);
    }
    else
    {
        std::string fullpath = fullPathForFilename(filename);
        if (fullpath.empty())
            return false;
        else
            return true;
    }
}

bool FileUtils::isFileExistInternal(const std::string& filePath) const
{
    if (filePath.empty())
    {
        return false;
    }
    
    bool ret = false;
    
    std::string fullpath = fullPathForFilename(filePath);
    
    FILE *fp = fopen(fullpath.c_str(), "rb");
    
    ret = fp != NULL;
    
    if (ret) {
        fclose(fp);
    }
    
    return ret;
}

void FileUtils::setDefaultResourceRootPath(const std::string& path)
{
    if (_defaultResRootPath != path)
    {
        _defaultResRootPath = path;
        if (!_defaultResRootPath.empty() && _defaultResRootPath[_defaultResRootPath.length()-1] != '/')
        {
            _defaultResRootPath += '/';
        }
    }
}
