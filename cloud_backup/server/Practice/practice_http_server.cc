#include "httplib.h"
using namespace httplib;
int main()
{
    Server svr;
    svr.Get("/hi",[](const Request& req,Response& rsp){
        rsp.set_content("Hello World!","text/plain");
    });
    svr.Get(R"(/numbers/(\d+))",[&](const Request& req,Response& rsp){
        auto numbers = req.matches[1];
        rsp.set_content(numbers,"text/plain");
    });
    svr.Post("/multipart",[&](auto& req,auto& rsp){
        auto size = req.files.size();
        auto ret = req.has_file("file");
        if(ret == false)
        {
            std::cout << "not file upload\n";
            rsp.status = 400;
            return;
        }
        const auto& file = req.get_file_value("file");
        rsp.body.clear();
        rsp.body = file.filename;//文件名称
        rsp.body += "\n";
        rsp.body += file.content;//文件内容
        rsp.set_header("Content-Type","text/plain");
        rsp.status = 200;
    });
    svr.listen("0.0.0.0",9091);
    return 0;
}