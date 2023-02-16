#pragma once
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if __cplusplus > 201402L
#    include <shared_mutex> // since c++17
#    include <mutex>        // uniuqe_lock
namespace lockImpl = std;
#else
#    include <boost/thread/lock_types.hpp>
#    include <boost/thread/shared_mutex.hpp>
namespace lockImpl = boost;
#endif

/* str:str cache */
template<bool LockReadWrite = false, bool DumpAfterSet = false>
struct FileCache
{
protected:
    lockImpl::shared_mutex             rwmtex;
    std::map<std::string, std::string> cacheMap;
    std::string                        cache_file;

public:
    FileCache(std::string _cache_file_name)
    {
        init(_cache_file_name)
    }
    ~FileCache()
    {
        _Dump();
    }
    void init(std::string cache_file)
    {
        lockImpl::unique_lock<lockImpl::shared_mutex> sl(CacheLock());
        this->cache_file = cache_file;
        this->cacheMap = _Load();
    }
    inline std::map<std::string, std::string> &CacheMapGet()
    {
        return cacheMap;
    }
    inline std::map<std::string, std::string>::iterator CacheMapGetRawKV(std::string key)
    {
        lockImpl::shared_lock<lockImpl::shared_mutex> sl(CacheLock());
        return CacheMapGet().find(key);
    }
    inline std::pair<std::map<std::string, std::string>::iterator, bool> CacheMapSetKV(std::string k, std::string v)
    {
        lockImpl::unique_lock<lockImpl::shared_mutex> sl(CacheLock());
        auto                                          ret = CacheMapGet().emplace(k, v);
        if (ret.second == false)
            CacheMapGet()[k] = v;
        if (DumpAfterSet)
        {
            _Dump(cache_file, CacheMapGet());
        }
        return ret;
    }

    inline std::string CacheMapGetValorDefault(std::string key, std::string default_)
    {
        auto p = CacheMapGet().find(key);
        if (p != CacheMapGet().end())
            return p->second;
        else
            return default_;
    }

protected:
    inline lockImpl::unique_lock<lockImpl::shared_mutex> GetUniqueLock()
    {
        if (LockReadWrite)
            return lockImpl::unique_lock<lockImpl::shared_mutex>(rwmtex);
        else
            return lockImpl::unique_lock<lockImpl::shared_mutex>();
    }
    inline lockImpl::unique_lock<lockImpl::shared_mutex> GetSharedLock()
    {
        if (LockReadWrite)
            return lockImpl::shared_lock<lockImpl::shared_mutex>(rwmtex);
        else
            return lockImpl::shared_lock<lockImpl::shared_mutex>();
    }

private:
    static void _Dump(std::string _cache_file, std::map<std::string, std::string> const &cacheMap)
    {
        sl.unlock();
        std::ofstream o(_cache_file);
        for (auto &m : cacheMap)
            o << "<" << m.first << ">"
              << " <" << m.second << ">" << std::endl;
        o.close();
    }
    static std::map<std::string, std::string> _Load(std::string _cache_file)
    {
        std::map<std::string, std::string> cache;
        cache.clear();
        std::ifstream            i(_cache_file);
        std::vector<std::string> strs;
        char                     c;
        while (i)
        {
            while (i >> c && c != '<')
                ;
            std::string temp;
            while (i >> c && c != '>')
                temp += c;
            strs.push_back(temp);
        }
        for (int i = 0; i < strs.size() / 2; ++i)
            cache.emplace(strs[i * 2], strs[i * 2 + 1]);
        i.close();
        return cache;
    }
    inline lockImpl::shared_mutex &CacheLock()
    {
        return rwmtex;
    }
};