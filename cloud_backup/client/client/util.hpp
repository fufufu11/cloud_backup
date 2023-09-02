#pragma once
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include<experimental/filesystem>
#include <memory>
namespace cloud
{
    namespace fs = std::experimental::filesystem;
    //�ļ���������
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
        size_t FileSize()//��ȡ�ļ���С
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "get file size fail\n";
                return -1;
            }
            return st.st_size;
        }
        time_t LastMTime()//���һ���޸�ʱ��
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "get file LastMtime fail\n";
                return -1;
            }
            return st.st_mtime;
        }
        time_t LastATime()//���һ�η���ʱ��
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "get file LastAtime fail\n";
                return -1;
            }
            return st.st_atime;
        }
        std::string FileName()//��ȡ�ļ�����
        {
            size_t pos = _filename.find_last_of("\\");
            if (pos == std::string::npos) return _filename;
            return _filename.substr(pos + 1);
        }
        bool GetPosLen(std::string* body, size_t pos, size_t len)//�õ�ָ�����ȵ����ݣ��ŵ�����Ͳ���body����
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
            ifs.seekg(pos, std::ios::beg);//���ļ��л�ȡ���ݵķ�Χ
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
        bool Exists()//�ж��ļ��Ƿ����
        {
            return fs::exists(_filename);
        }
        bool createDirectory() //����һ��Ŀ¼
        {
            if (this->Exists()) return true;
            return fs::create_directories(_filename);
        }
        bool ScanDirectory(std::vector<std::string>* array) //���һ��Ŀ¼
        {
            this->createDirectory();
            for (auto& p : fs::directory_iterator(_filename))
            {
                if (fs::is_directory(p) == true) continue;
                //relative_path ����·�����ļ���
                array->push_back(fs::path(p).relative_path().string());
            }
            return true;
        }
    };
}