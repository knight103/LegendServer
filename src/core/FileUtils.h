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

#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <string>
#include <vector>
#include "core/Path.h"

class FileUtils {
public:
    static FileUtils* getInstance();

    static void destroyInstance();

    static void setDelegate(FileUtils *delegate);

    virtual ~FileUtils();
    
    /**
     *  Gets string from a file.
     */
    virtual std::string getStringFromFile(const std::string& filename);

    /**
     *  Returns the fullpath for a given filename.
     */
    virtual std::string fullPathForFilename(const std::string &filename) const;

    /**
     *  Checks whether the path is an absolute path.
     */
    virtual bool isAbsolutePath(const std::string& path) const;
    
    /**
     *  Checks whether a file exists.
     */
    virtual bool isFileExist(const std::string& filename) const;
    
    /**
     *  Checks whether a file exists without considering search paths and resolution orders.
     */
    virtual bool isFileExistInternal(const std::string& filePath) const;
    
    /**
     *  添加搜索路径
     */
    void addSearchPath(const std::string& searchpath, const bool front=false);
    
    /**
     * Set default resource root path.
     */
    void setDefaultResourceRootPath(const std::string& path);
    
public:
    /**
     *  连接两个路径，返回一个完整路径
     */
    static std::string pathJoin(const std::string& base, const std::string& add);
    
protected:
    /**
     *  获取文件的全路径
     */
    virtual std::string getPathForFilename(const std::string& filename, const std::string& searchPath) const;
    
    /**
     *  获取文件的全路径
     */
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const;

protected:
    FileUtils();

    virtual bool init();

    /**
     * 唯一实例
     */
    static FileUtils* _instance;
    
    /**
     * 默认工作根目录
     */
    Path _defaultResRootPath;
    
    /**
     * 使用setSearchPaths、addSearchPath方法会存放在这
     */
    std::vector<Path> _originalSearchPaths;
    
    std::vector<Path> _searchPathArray;
};

#endif
