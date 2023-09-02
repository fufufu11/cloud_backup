#pragma once
#include "util.hpp"
#include "config.hpp"
#include <unordered_map>
#include <vector>
namespace cloud
{
    class BackInfo
    {
    public:
        bool NewBackupInfo(const std::string& realpath)
        {
            FileUtil fu(realpath);
            if(fu.Exists() == false)
            {
                std::cout << "new backupinfo: file not exists!\n";
                return false;
            }
            Config* config = Config::get_instance();
            std::string packdir = config->GetPackDir();
			std::string packsuffix = config->GetPackFileSuffix();
			std::string download_prefix = config->GetDownloadPrefix();
            _pack_flag = false;
            _fsize = fu.FileSize();
            this->_mtime = fu.LastMTime();
			this->_atime = fu.LastATime();
            _real_path = realpath;
            _pack_path = packdir + fu.FileName() + packsuffix;
            _url = download_prefix + fu.FileName();
            return true;
        }
            
    public:
        bool _pack_flag;
        size_t _fsize;
        time_t _mtime;
        time_t _atime;
        std::string _real_path;
        std::string _pack_path;
        std::string _url;
    };
    class DataManager
    {
    public:
        DataManager()
        {
            _backup_file = Config::get_instance()->GetBackupFile();
            pthread_rwlock_init(&_rwlock,nullptr);
            InitLoad();
        }
        ~DataManager()
        {
            pthread_rwlock_destroy(&_rwlock);
        }
        bool Insert(const BackInfo& info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info._url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }
        bool Update(const BackInfo& info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info._url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }
        bool GetOneByURL(const std::string &url,BackInfo* info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.find(url);
            if(it == _table.end())
            {
                pthread_rwlock_unlock(&_rwlock);
                return false;
            }
            *info = it->second;
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }
        bool GetOneByRealPath(const std::string &realpath,BackInfo* info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.begin();
            for(;it != _table.end();++it)
            {
                if(it->second._real_path == realpath)
                {
                    *info = it->second;
                    pthread_rwlock_unlock(&_rwlock);
                    return true;
                }
            }
            pthread_rwlock_unlock(&_rwlock);
            return false;
        }
        bool GetAll(std::vector<BackInfo>* arry)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.begin();
            for (; it != _table.end(); ++it)
            {
                arry->push_back(it->second);
            }
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }
        bool Storage()
        {
            std::vector<BackInfo> arry;
            GetAll(&arry);
            Json::Value root;
            for(int i = 0;i < arry.size();i++)
            {
                Json::Value item;
                item["pack_flag"] = arry[i]._pack_flag;
                item["fsize"] = (Json::Int64)arry[i]._fsize;
                item["atime"] = (Json::Int64)arry[i]._atime;
                item["mtime"] = (Json::Int64)arry[i]._mtime;
                item["real_path"] = arry[i]._real_path;
                item["pack_path"] = arry[i]._pack_path;
                item["url"] = arry[i]._url;
                root.append(item);
            }
            std::string body;
            JsonUtil::Serialize(root,&body);
            FileUtil fu(_backup_file);
            fu.SetContent(body);
            return true;
        }
    bool InitLoad()
    {
        FileUtil fu(_backup_file);
        if(fu.Exists() == false) return false;
        std::string body;
        fu.GetContent(&body);
        Json::Value root;
        JsonUtil::UnSerialize(body,&root);
        for(int i = 0;i < root.size();i++)
        {
            BackInfo info;
            info._pack_flag = root[i]["pack_flag"].asBool();
            info._fsize = root[i]["fsize"].asInt64();
            info._atime = root[i]["atime"].asInt64();
            info._mtime = root[i]["mtime"].asInt64();
            info._pack_path = root[i]["pack_path"].asString();
            info._real_path = root[i]["real_path"].asString();
            info._url = root[i]["url"].asString();
            Insert(info);
        }
        return true;
    }
    private:
        std::string _backup_file;
        pthread_rwlock_t _rwlock;
        std::unordered_map<std::string,BackInfo> _table;
    };
};