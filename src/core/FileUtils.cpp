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
{
}

FileUtils::~FileUtils()
{
}

bool FileUtils::init()
{
    _searchPathArray.push_back(_defaultResRootPath);
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

std::string FileUtils::fullPathForFilename(const std::string &filename) const {
    if (filename.empty()) {
        return "";
    }
    
    if (isAbsolutePath(filename)) {
        return filename;
    }
    
    // TODO: 从缓存读?? 暂时没做

    std::string fullpath;
    for (const auto& searchIt : _searchPathArray)
    {
        fullpath = this->getPathForFilename(filename, searchIt.str());
        
        if (!fullpath.empty())
        {
            // Using the filename passed in as key.
//            _fullPathCache.emplace(filename, fullpath);
            return fullpath;
        }
    }

    return "";
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
    
    std::string fullpath = filePath;
    
    FILE *fp = fopen(fullpath.c_str(), "rb");
    
    ret = fp != NULL;
    
    if (ret) {
        fclose(fp);
    }
    
    return ret;
}

void FileUtils::addSearchPath(const std::string& searchpath, const bool front) {
    std::string prefix;
    if (!isAbsolutePath(searchpath))
        prefix = _defaultResRootPath.str();
    
    std::string path = pathJoin(prefix, searchpath);
    
    if (front) {
        _originalSearchPaths.insert(_originalSearchPaths.begin(), searchpath);
        _searchPathArray.insert(_searchPathArray.begin(), path);
    } else {
        _originalSearchPaths.push_back(searchpath);
        _searchPathArray.push_back(path);
    }
}

void FileUtils::setDefaultResourceRootPath(const std::string& path)
{
    if (_defaultResRootPath.str() != path)
    {
        _defaultResRootPath = Path(path);
    }
}

std::string FileUtils::getPathForFilename(const std::string& filename, const std::string& searchPath) const {
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path
    std::string path = searchPath;
    path += file_path;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    return path;
}

std::string FileUtils::getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const {
    std::string ret = directory;
    if (directory.size() && directory[directory.size()-1] != '/'){
        ret += '/';
    }
    ret += filename;
    
    // if the file doesn't exist, return an empty string
    if (!isFileExistInternal(ret)) {
        ret = "";
    }
    return ret;
}

std::string FileUtils::pathJoin(const std::string& base, const std::string& add) {
    std::string ret = base;
    
    if (ret[ret.length()-1] != '/') {
        ret += '/';
    }
    
    return ret + add;
}
