#include <iostream>
#include "data.hpp"
using namespace std;

int main(int argc,char* argv[])
{
    // cloud::BackInfo backinfo;
    // backinfo.NewBackupInfo(argv[1]);
    // cout << backinfo._pack_flag << endl;
    // cout << backinfo._fsize << endl;
    // cout << backinfo._atime << endl;
    // cout << backinfo._mtime << endl;
    // cout << backinfo._real_path << endl;
    // cout << backinfo._pack_path << endl;
    // cout << backinfo._url << endl;

    // cloud::DataManager data;
    // cloud::BackInfo tmp;
    // data.Insert(backinfo);
    // vector<cloud::BackInfo> arry;
    // data.GetAll(&arry);
    // cout << "-----------------------GetALL-------------------------" << endl;
    // for (auto &i : arry)
    // {
    //     cout << i._pack_flag << endl;
    //     cout << i._fsize << endl;
    //     cout << i._atime << endl;
    //     cout << i._mtime << endl;
    //     cout << i._real_path << endl;
    //     cout << i._pack_path << endl;
    //     cout << i._url << endl;
    // }
    // cout << "----------------------GetOneByURL----------------------" << endl;
    // tmp.NewBackupInfo(argv[1]);
    // data.GetOneByURL("/download/util.hpp",&tmp);
    // cout << tmp._pack_flag << endl;
    // cout << tmp._fsize << endl;
    // cout << tmp._atime << endl;
    // cout << tmp._mtime << endl;
    // cout << tmp._real_path << endl;
    // cout << tmp._pack_path << endl;
    // cout << tmp._url << endl;

    // cout << "----------------------Update&&GetOneByRealpath----------------------" << endl;
    // tmp._pack_flag = true;
    // data.Update(tmp);
    // data.GetOneByRealPath("./util.hpp",&tmp);
    // cout << tmp._pack_flag << endl;
    // cout << tmp._fsize << endl;
    // cout << tmp._atime << endl;
    // cout << tmp._mtime << endl;
    // cout << tmp._real_path << endl;
    // cout << tmp._pack_path << endl;
    // cout << tmp._url << endl;

    //--------------------------------------InitLoad Test--------------------------------------------
    cloud::DataManager data;
    std::vector<cloud::BackInfo> arry;
    data.GetAll(&arry);
    for (auto &a : arry)
    {
        std::cout << a._pack_flag << std::endl;
        std::cout << a._fsize << std::endl;
        std::cout << a._mtime << std::endl;
        std::cout << a._atime << std::endl;
        std::cout << a._real_path << std::endl;
        std::cout << a._pack_path << std::endl;
        std::cout << a._url << std::endl;
    }
    return 0;
}