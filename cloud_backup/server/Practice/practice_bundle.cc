#include <iostream>
#include <string>
#include <fstream>
#include "bundle.h"

int main(int argc, char *argv[])
{
  //压缩
  // std::cout <<"argv[1] 是原始文件路径名称\n";
  // std::cout <<"argv[2] 是压缩包名称\n";
  // if (argc < 3) return -1;
  // std::string ifilename = argv[1];
  // std::string ofilename = argv[2];
  // std::ifstream ifs;
  // ifs.open(ifilename, std::ios::binary);//打开原始文件
  // ifs.seekg(0, std::ios::end);//跳转读写位置到末尾
  // size_t fsize = ifs.tellg();//获取末尾偏移量--文件长度
  // ifs.seekg(0, std::ios::beg);//跳转到文件起始
  // std::string body;
  // body.resize(fsize);//调整body大小为文件大小
  // ifs.read(&body[0], fsize);//读取文件所有数据到body找给你
  // std::string packed = bundle::pack(bundle::LZIP, body);//以lzip格式压缩文件数据
  // std::ofstream ofs;
  // ofs.open(ofilename, std::ios::binary);//打开压缩包文件
  // ofs.write(&packed[0], packed.size());//将压缩后的数据写入压缩包文件
  // ifs.close();
  // ofs.close();

  ///////////////////////////////////////////////////////////////////////////////////////////
  //解压
  if (argc < 3)
  {
    printf("argv[1]是压缩包名称\n");
    printf("argv[2]是解压后的文件名称\n");
    return -1;
  }
  std::string ifilename = argv[1]; // 压缩包名

  std::string ofilename = argv[2]; // 解压缩后文件名

  std::ifstream ifs;
  ifs.open(ifilename, std::ios::binary);
  ifs.seekg(0, std::ios::end);
  size_t fsize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  std::string body;
  body.resize(fsize);
  ifs.read(&body[0], fsize);
  ifs.close();
  std::string unpacked = bundle::unpack(body); // 对压缩包数据解压缩

  std::ofstream ofs;
  ofs.open(ofilename, std::ios::binary);
  ofs.write(&unpacked[0], unpacked.size());
  ofs.close();
  return 0;
}

