#pragma once
#include "util.hpp"

namespace cloud
{
#define CONFIG_FILE "./cloud.conf"
    class Config
    {
    public:
        static Config* get_instance()
        {
            static Config _config;
            return &_config;
        }
        int GetHotTime()
        {
            return _hot_time;
        }
        int GetServerPort()
        {
            return _server_port;
        }
        std::string GetServerIP()
        {
            return _server_ip;
        }
        std::string GetDownloadPrefix()
        {
            return _download_prefix;
        }
        std::string GetPackFileSuffix()
        {
            return _packfile_suffix;
        }
        std::string GetPackDir()
        {
            return _pack_dir;
        }
        std::string GetBackDir()
        {
            return _back_dir;
        }
        std::string GetBackupFile()
        {
            return _backup_file;
        }
    private:
        bool ReadConfigFile()
        {
            FileUtil fu(CONFIG_FILE);
            std::string body;
            if(fu.GetContent(&body) == false)
            {
                std::cout << "load config file failed\n";
                return false;
            }
            Json::Value root;
            if(JsonUtil::UnSerialize(body,&root) == false)
            {
                std::cout << "parse config file failed\n";
                return false;
            }
            _hot_time = root["hot_time"].asInt();
            _server_port = root["server_port"].asInt();
            _server_ip = root["server_ip"].asString();
            _download_prefix = root["download_prefix"].asString();
            _packfile_suffix = root["packfile_suffix"].asString();
            _pack_dir = root["pack_dir"].asString();
            _back_dir = root["back_dir"].asString();
             _backup_file = root["backup_file"].asString();
        }
    private:
        int _hot_time;
        int _server_port;
        std::string _server_ip;
        std::string _download_prefix;
        std::string _packfile_suffix;
        std::string _pack_dir;
        std::string _back_dir;
        std::string _backup_file;
    private:
        Config()
        {
            ReadConfigFile();
        }
        Config(const Config& c) = delete;
        Config& operator=(const Config& c) = delete;
    };
};