#pragma once 
#include "data.hpp"
#include <unistd.h>
extern cloud::DataManager* _data;
namespace cloud
{
    class HotManger
    {
    private:
        std::string _back_dir;
        std::string _pack_dir;
        std::string _pack_suffix;
        int _hot_time;
    private:
        //非热点文件返回真，热点文件返回假
        bool HotJudge(const std::string& filename)
        {
            FileUtil fu(filename);
            time_t last_atime = fu.LastATime();
            time_t cur_time = time(nullptr);
            if(cur_time - last_atime > _hot_time)
            {
                return true;
            }
            return false;
        }
    public:
        HotManger()
        {
            Config *config = Config::get_instance();
            _back_dir = config->GetBackDir();
            _pack_dir = config->GetPackDir();
            _pack_suffix = config->GetPackFileSuffix();
            _hot_time = config->GetHotTime();
            FileUtil tmp1(_back_dir);
            FileUtil tmp2(_pack_dir);
            tmp1.CreateDirectory();
            tmp2.CreateDirectory();
        }
        bool RunModule()
        {
            while(1)
            {
                //1.遍历备份目录，获取所有文件名
                FileUtil fu(_back_dir);
                std::vector<std::string> arry;
                fu.ScanDirectory(&arry);
                //2.遍历判断文件是否是非热点文件
                for(auto& a : arry)
                {
                    if(HotJudge(a) == false)
                    {
                        continue;//热点文件不需要特别处理
                    }
                    // 3.获取文件的备份信息
                    BackInfo bi;
                    if (_data->GetOneByRealPath(a,&bi) == false)
                    {
                        //现在有一个文件存在，但是没有备份信息
                        bi.NewBackupInfo(a);
                    }
                    // 3.对非热点文件进行压缩处理
                    FileUtil tmp(a);
                    tmp.Compress(bi._pack_path);
                    //4.删除源文件，修改备份信息
                    tmp.Remove();
                    bi._pack_flag = true;
                    _data->Update(bi);
                }
                usleep(1000);
            }
            return true;
        }
    };
};