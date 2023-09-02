#include "httplib.h"
using namespace httplib;

int main()
{
    Client client("47.108.137.232",9091);
    MultipartFormData item;
    item.name = "file";
    item.filename = "hello.txt";
    item.content = "Hello World";//上传文件时，这里给的就是文件内容
    item.content_type = "text/plain";

    MultipartFormDataItems items;
    items.push_back(item);

    auto res = client.Post("/multipart",items);
    std::cout << res->status << std::endl;
    std::cout << res->body << std::endl;
    return 0;
}