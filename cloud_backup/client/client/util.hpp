#pragma once
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include<experimental/filesystem>
#include <memory>
namespace cloud
{
    namespace fs = std::experimental::filesystem;
    //文件操作工具
    class FileUtil
    {
    private:
        std::string _filename;
    public:
        FileUtil(const std::string& filename) : _filename(filename) {}
        bool Remove()
        {
            if (Exists() == false)
            {
                return false;
            }
            remove(_filename.c_str());
            return true;
        }
        size_t FileSize()//获取文件大小
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "get file size fail\n";
                return -1;
            }
            return st.st_size;
        }
        time_t LastMTime()//最后一次修改时间
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "get file LastMtime fail\n";
                return -1;
            }
            return st.st_mtime;
        }
        time_t LastATime()//最后一次访问时间
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "get file LastAtime fail\n";
                return -1;
            }
            return st.st_atime;
        }
        std::string FileName()//获取文件名称
        {
            size_t pos = _filename.find_last_of("\\");
            if (pos == std::string::npos) return _filename;
            return _filename.substr(pos + 1);
        }
        bool GetPosLen(std::string* body, size_t pos, size_t len)//得到指定长度的数据，放到输出型参数body里面
        {
            size_t fsize = this->FileSize();
            if (pos + len > fsize)
            {
                std::cout << "get file len is error\n";
                return false;
            }
            std::ifstream ifs;
            ifs.open(_filename, std::ios::binary);
            if (ifs.is_open() == false)
            {
                std::cout << "read open file failed\n";
                return false;
            }
            ifs.seekg(pos, std::ios::beg);//从文件中获取数据的范围
            body->resize(len);
            ifs.read(&(*body)[0], len);
            if (ifs.good() == false)
            {
                std::cout << "get file content failed\n";
                ifs.close();
                return false;
            }
            ifs.close();
            return true;
        }
        bool GetContent(std::string* body)
        {
            size_t fsize = this->FileSize();
            return GetPosLen(body, 0, fsize);
        }
        bool SetContent(const std::string& body)
        {
            std::ofstream ofs;
            ofs.open(_filename, std::ios::binary);
            if (ofs.is_open() == false)
            {
                std::cout << "write open content failed\n";
                ofs.close();
                return false;
            }
            ofs.write(&body[0], body.size());
            if (ofs.good() == false)
            {
                std::cout << "write file content failed\n";
                ofs.close();
                return false;
            }
            ofs.close();
            return true;
        }
        bool Exists()//判断文件是否存在
        {
            return fs::exists(_filename);
        }
        bool createDirectory() //创建一个目录
        {
            if (this->Exists()) return true;
            return fs::create_directories(_filename);
        }
        bool ScanDirectory(std::vector<std::string>* array) //浏览一个目录
        {
            this->createDirectory();
            for (auto& p : fs::directory_iterator(_filename))
            {
                if (fs::is_directory(p) == true) continue;
                //relative_path 带有路径的文件名
                array->push_back(fs::path(p).relative_path().string());
            }
            return true;
        }
    };
}