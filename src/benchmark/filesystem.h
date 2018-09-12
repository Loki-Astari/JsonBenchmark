#ifndef THORS_ANVIL_FILE_SYSTEM_FS
#define THORS_ANVIL_FILE_SYSTEM_FS

#include "benchmarkConfig.h"
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>
#ifdef HAVE_DIRENT_H
# include <dirent.h>
#else
# define dirent direct
# ifdef HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# ifdef HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# ifdef HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#ifdef HAVE_STRUCT_DIRENT_D_NAMLEN
# define NAMLEN(dirent) ((dirent)->d_namlen)
#else
#include <string.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#endif

#define QUOTE_(Val)     #Val
#define QUOTE(Val)      QUOTE_(Val)

namespace ThorsAnvil
{
    namespace FileSystem
    {

class Path
{
    std::string path;
    public:
        Path(Path const& parent, std::string const& name)
            : path(parent.path)
        {
            if (path.size() != 0 && path.back() != '/')
            {   path.append("/");
            }
            path.append(name);
        }
        Path(char const* path)
            : path(path)
        {}
        Path(std::string const& path)
            : path(path)
        {}

        std::string const& str() const
        {
            return path;
        }
};

class DirectoryEntry
{
    Path        parent;
    std::string objectName;
    bool        dir;
    public:
        DirectoryEntry(Path const& parent, std::string&& name, bool dir)
            : parent(parent)
            , objectName(std::move(name))
            , dir(dir)
        {}

        bool isDir()        const   {return dir;}
        bool isFile()       const   {return !dir;}
        bool isNormalDir()  const   {return dir && objectName[0] != '.';}
        bool isNormalFile() const   {return !dir && objectName[0] != '.';}

        Path                path()  const   {return Path(parent, objectName);}
        std::string const&  name()  const   {return objectName;}

        friend std::ostream& operator<<(std::ostream& s, DirectoryEntry const& data)
        {
            return s << "Name: " << data.objectName << " : " << (data.dir ? "Directory" : "File");;
        }
};

class DirectoryIterator
{
    Path    path;
    DIR*    directory;
    class Iterator;
    public:
        DirectoryIterator(Path const& path)
            : path(path)
        {
            directory = opendir(path.str().c_str());
            if (directory == nullptr)
            {
                throw std::runtime_error("ThorsAnvil::FileSystem::DirectoryIterator: DirectoryFailed to Create Dir Object");
            }
        }
        ~DirectoryIterator()
        {
            closedir(directory);
        }
        Iterator begin() const
        {
            return Iterator(*this);
        }
        Iterator end() const
        {
            return Iterator{};
        }
    private:
        class Iterator
        {
            DirectoryIterator const*    parent;
            dirent*                     last;

            using value_type = DirectoryEntry;

            public:
                Iterator(DirectoryIterator const& initParent)
                    : parent(&initParent)
                    , last(readdir(parent->directory))
                {}
                Iterator()
                    : parent(nullptr)
                    , last(nullptr)
                {}
                DirectoryEntry operator*() const
                {
                    return DirectoryEntry(parent->path, std::string(last->d_name, last->d_name + NAMLEN(last)), last->d_type == DT_DIR);
                }
                Iterator& operator++()
                {
                    last = readdir(parent->directory);
                    return *this;
                }
                Iterator operator++(int)
                {
                    Iterator result(*this);
                    ++(*this);
                    return result;
                }
                bool operator==(Iterator const& rhs) const
                {
                    return last == rhs.last;
                }
                bool operator!=(Iterator const& rhs) const
                {
                    return !(*this == rhs);
                }
        };

};

    }
}

#endif
