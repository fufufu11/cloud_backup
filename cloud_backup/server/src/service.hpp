#pragma once
#include "httplib.h"
#include "data.hpp"
extern cloud::DataManager* _data;

namespace cloud
{
    class Service
    {
    private:
        int _server_port;
        std::string _server_ip;
        std::string _download_prefix;
        httplib::Server _server;
    private:
        static void Upload(const httplib::Request& req,httplib::Response& rsp)
        {
            // post /upload  文件数据在正文中（正文并不全是文件数据）
            auto ret = req.has_file("file");//判断有没有上传的文件区域
            if(ret == false)
            {
                rsp.status = 400;
                return;
            }
            const auto& file = req.get_file_value("file");
            //file.filename 文件名称  file.content 文件数据
            std::string back_dir = Config::get_instance()->GetBackDir();
            std::string realpath = back_dir + FileUtil(file.filename).FileName();
            FileUtil fu (realpath);
            fu.SetContent(file.content);
            BackInfo info;
            info.NewBackupInfo(realpath);
            _data->Insert(info);
            return;
        }
        static std::string TimetoStr(time_t t)
        {
            std::string tmp = std::ctime(&t);
            return tmp;
        }
        static void ListShow(const httplib::Request &req, httplib::Response &rsp)
        {
            // 1. 获取所有的文件备份信息
            std::vector<BackInfo> arry;
            _data->GetAll(&arry);
            // 2. 根据所有备份信息，组织html文件数据
            std::stringstream ss;
            ss << "<html><head><title>Download</title></head>";
            ss << "<body><h1>Download</h1><table>";
            for (auto &a : arry)
            {
                ss << "<tr>";
                std::string filename = FileUtil(a._real_path).FileName();
                ss << "<td><a href='" << a._url << "'>" << filename << "</a></td>";
                ss << "<td align='right'>" << TimetoStr(a._mtime) << "</td>";
                ss << "<td align='right'>" << a._fsize / 1024 << "k</td>";
                ss << "</tr>";
            }
            ss << "</table></body></html>";
            rsp.body = ss.str();
            rsp.set_header("Content-Type", "text/html");
            rsp.status = 200;
            return;
        }
        static std::string GetETag(const BackInfo &info)
        {
            // etg :  filename-fsize-mtime
            FileUtil fu(info._real_path);
            std::string etag = fu.FileName();
            etag += "-";
            etag += std::to_string(info._fsize);
            etag += "-";
            etag += std::to_string(info._mtime);
            return etag;
        }
        static void Download(const httplib::Request &req, httplib::Response &rsp)
        {
            // 1. 获取客户端请求的资源路径path   req.path
            // 2. 根据资源路径，获取文件备份信息
            BackInfo info;
            _data->GetOneByURL(req.path, &info);
            // 3. 判断文件是否被压缩，如果被压缩，要先解压缩,
            if (info._pack_flag == true)
            {
                FileUtil fu(info._pack_path);
                fu.UnCompress(info._real_path); // 将文件解压到备份目录下
                // 4. 删除压缩包，修改备份信息（已经没有被压缩）
                fu.Remove();
                info._pack_flag = false;
                _data->Update(info);
            }
            bool retrans = false;
            std::string old_etag;
            if(req.has_header("If-Range"))
            {
                old_etag = req.get_header_value("If-Range");
                //有If-Range字段且，这个字段的值与请求文件的最新etag一致则符合断点续传
                if(old_etag == GetETag(info))
                {
                    retrans = true;
                }
            }
            FileUtil fu(info._real_path);
            if (retrans == false)
            {
                fu.GetContent(&rsp.body);
                // 5. 设置响应头部字段： ETag， Accept-Ranges: bytes
                rsp.set_header("Accept-Ranges", "bytes");
                rsp.set_header("ETag", GetETag(info));
                rsp.set_header("Content-Type", "application/octet-stream");
                rsp.status = 200;
            }
            else
            {
                // httplib内部实现了对于区间请求也就是断点续传请求的处理
                // 只需要我们用户将文件所有数据读取到rsp.body中，它内部会自动根据请求
                // 区间，从body中取出指定区间数据进行响应
                  //std::string  range = req.get_header_val("Range"); bytes=start-end;
                fu.GetContent(&rsp.body);
                rsp.set_header("Accept-Ranges","bytes");
                rsp.set_header("ETag",GetETag(info));
                rsp.set_header("Content-Type","application/octet-stream");
                //rsp.set_header("Content-Range", "bytes start-end/fsize");
                rsp.status = 206;//区间请求响应的是206*****
            }
        }
        public:
            Service()
            {
                Config *config = Config::get_instance();
                _server_port = config->GetServerPort();
                _server_ip = config->GetServerIP();
                _download_prefix = config->GetDownloadPrefix();
            }
            bool RunModule()
            {
                _server.Post("/upload", Upload);
                _server.Post("/listshow", ListShow);
                _server.Get("/", ListShow);
                std::string download_url = _download_prefix + "(.*)";
                _server.Get(download_url, Download);
                _server.listen("0.0.0.0", _server_port);
                return true;
            }
        };
};