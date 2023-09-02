#include "util.hpp"

using namespace std;

//文件工具测试
int main(int argc,char* args[])
{
    string filename = args[1];
    cloud::FileUtil fu(filename);
    // fu.SetContent("你好");
    // string body;
    // fu.GetPosLen(&body,0,6);
    // cout << body << endl;
    // body.clear();
    // fu.GetContent(&body);
    // cout << body << endl;

    // cout << fu.FileSize() << endl;
    // cout << fu.LastMTime() << endl;
    // cout << fu.LastATime() << endl;
    // cout << fu.FileName() << endl;

    // string packname = filename + ".lz";
    // fu.Compress(packname);
    // fu.UnCompress("test.txt");
    fu.CreateDirectory();
    vector<string> vs;
    fu.ScanDirectory(&vs);
    for(auto& i : vs)
    {
        cout << i << endl; 
    }
    return 0;
}

//Json工具测试
// int main()
// {
//     const char* name = "小明";
//     int age = 18;
//     double score[] = {99.5,100,90};
//     Json::Value root;
//     root["姓名"] = name;
//     root["年龄"] = age;
//     root["成绩"].append(score[0]);
//     root["成绩"].append(score[1]);
//     root["成绩"].append(score[2]);
//     string str;
//     cloud::JsonUtil::Serialize(root,&str);
//     cout << str << endl;

//     Json::Value val;
//     cloud::JsonUtil::UnSerialize(str,&val);
//     cout << val["姓名"] << endl;
//     cout << val["年龄"] << endl;
//     cout << val["成绩"][0] << endl;
//     cout << val["成绩"][1] << endl;
//     cout << val["成绩"][2] << endl;
//     return 0;
// }