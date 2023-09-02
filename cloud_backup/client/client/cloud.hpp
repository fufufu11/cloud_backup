#pragma once
#include "data.hpp"
#include "httplib.h"
#include <windows.h>
namespace cloud
{
#define SERVER_ADDR "47.108.137.232"
#define SERVER_PORT 9092
	class Backup
	{
	private:
		std::string _back_dir;
		DataManager* _data;
	public:
		Backup(const std::string& back_dir, const std::string& back_file)
			:_back_dir(back_dir)
		{
			_data = new DataManager(back_file);
		}
		std::string GetFileIdentifier(const std::string& filename)
		{
			//a.txt-fsize-mtime
			FileUtil fu(filename);
			std::stringstream ss;
			ss << fu.FileName() << "-" << fu.FileSize() << "-" << fu.LastMTime();
			return ss.str();
		}
		bool Upload(const std::string& filename) {
			//1. ��ȡ�ļ�����
			FileUtil fu(filename);
			std::string body;
			fu.GetContent(&body);

			//2. �http�ͻ����ϴ��ļ�����
			httplib::Client client(SERVER_ADDR, SERVER_PORT);
			httplib::MultipartFormData item;
			item.content = body;
			item.filename = fu.FileName();
			item.name = "file";
			item.content_type = "application/octet-stream";
			httplib::MultipartFormDataItems items;
			items.push_back(item);

			auto res = client.Post("/upload", items);
			if (!res || res->status != 200) {
				return false;
			}
			return true;
		}
		bool IsNeedUpload(const std::string& filename) {
			// ��Ҫ�ϴ����ļ����ж��������ļ��������ģ������������Ǳ��޸Ĺ�
			// �ļ��������ģ���һ����û����ʷ������Ϣ
			// �����������Ǳ��޸Ĺ�������ʷ��Ϣ��������ʷ��Ψһ��ʶ�뵱ǰ���µ�Ψһ��ʶ��һ��
			std::string id;
			if (_data->GetOneByKey(filename, &id) != false) {
				//����ʷ��Ϣ
				std::string new_id = GetFileIdentifier(filename);
				if (new_id == id) {
					return false;//����Ҫ���ϴ�-�ϴ��ϴ���û�б��޸Ĺ�
				}
			}
			//һ���ļ��Ƚϴ�,��������Ŀ��������Ŀ¼�£�������Ҫһ�����̣�
			//���ÿ�α����򶼻��жϱ�ʶ��һ����Ҫ�ϴ�һ����ʮG���ļ����ϴ��ϰٴ�
			//���Ӧ���ж�һ���ļ�һ��ʱ�䶼û�б��޸Ĺ��ˣ�������ϴ�
			FileUtil fu(filename);
			if (time(NULL) - fu.LastMTime() < 3) {//3����֮�ڸ��޸Ĺ�--��Ϊ�ļ������޸���
				return false;
			}

			std::cout << filename << " need upload!\n";
			return true;
		}
		bool RunModule() {
			while (1) {
				//1. ������ȡָ���ļ����������ļ�
				FileUtil fu(_back_dir);
				std::vector<std::string> arry;
				fu.ScanDirectory(&arry);
				//2. ����ж��ļ��Ƿ���Ҫ�ϴ�
				for (auto& a : arry) {
					if (IsNeedUpload(a) == false) {
						continue;
					}
					//3. �������Ҫ�ϴ����ϴ��ļ�
					if (Upload(a) == true) {
						_data->Insert(a, GetFileIdentifier(a));//�����ļ�������Ϣ
						std::cout << a << " upload success!\n";
					}
				}
				Sleep(1);
				std::cout << "-------------------------loop end------------\n";
			}
		}

	};
};