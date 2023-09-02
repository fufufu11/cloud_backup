#include <iostream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

#include <memory>
#include <sstream>
using namespace std;

int main()
{
    //序列化测试

    // const char* name = "小明";
    // int age = 18;
    // double score[] = {99.5,98.5,90};

    // Json::Value val;
    // val["姓名"] = "小明";
    // val["年龄"] = 18;
    // val["成绩"].append(score[0]);
    // val["成绩"].append(score[1]);
    // val["成绩"].append(score[2]);

    // Json::StreamWriterBuilder swb;
    // std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
    // std::ostringstream os;
    // sw->write(val,&os);
    // std::string str = os.str();
    // cout << str << endl;

/////////////////////////////////////////////////////////////////////////////////////
   //反序列化测试
    string str = R"({"姓名":"小明","年龄":18,"成绩":[100,98.5,90]})";
    Json::Value root;
    Json::CharReaderBuilder crb;
    unique_ptr<Json::CharReader> cr(crb.newCharReader());
    string err;
    int ret = cr->parse(str.c_str(),str.c_str() + str.size(),&root,&err);
    if(ret = false) cout << "err:" << err << endl;
    cout << root["姓名"].asString() << endl;
    cout << root["年龄"].asInt() << endl;
    cout << root["成绩"][0] << endl;
    cout << root["成绩"][1] << endl;
    cout << root["成绩"][2] << endl;
    return 0;
}