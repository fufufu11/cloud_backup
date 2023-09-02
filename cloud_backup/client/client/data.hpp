#pragma once
#include <unordered_map>
#include <sstream>
#include "util.hpp"

namespace cloud
{
	class DataManager
	{
	public:
		DataManager(const std::string& backup_file) : _backup_file(backup_file)
		{
			InitLoad();
		}
		bool Storage()
		{
			//1.获取所有备份信息
			std::stringstream ss;
			auto it = _table.begin();
			for (; it != _table.end(); it++)
			{
				//2.将所有信息进行指定持久化格式的组织
				ss << it->first << " " << it->second << "\n";
			}
			//3.持久化存储
			FileUtil fu(_backup_file);
			fu.SetContent(ss.str());
			return true;
		}
		int Split(const std::string& str, const std::string& sep, std::vector<std::string>* arry) {
			int count = 0;
			size_t pos = 0, idx = 0;
			while (1) {
				pos = str.find(sep, idx);
				if (pos == std::string::npos) {
					break;
				}
				if (pos == idx) {
					idx = pos + sep.size();  
					continue;
				}
				std::string tmp = str.substr(idx, pos - idx);
				arry->push_back(tmp);
				count++;
				idx = pos + sep.size();
			}
			if (idx < str.size()) {
				arry->push_back(str.substr(idx));
				count++;
			}
			return count;
		}

		bool InitLoad() {
			//1. 从文件中读取所有数据
			FileUtil fu(_backup_file);
			std::string body;
			fu.GetContent(&body);
			//2. 进行数据解析，添加到表中
			std::vector<std::string> arry;
			Split(body, "\n", &arry);
			for (auto& a : arry) {
				// b.txt b.txt-34657-345636
				std::vector<std::string> tmp;
				Split(a, " ", &tmp);
				if (tmp.size() != 2) {
					continue;
				}
				_table[tmp[0]] = tmp[1];
			}
			return true;
		}
		bool Insert(const std::string& key, const std::string& val) {
			_table[key] = val;
			Storage();
			return true;
		}
		bool Update(const std::string& key, const std::string& val) {
			_table[key] = val;
			Storage();
			return true;
		}
		bool GetOneByKey(const std::string& key, std::string* val) {
			auto it = _table.find(key);
			if (it == _table.end()) {
				return false;
			}
			*val = it->second;
			return true;
		}
	private:
		std::string _backup_file;//备份信息的持久化存储文件
		std::unordered_map<std::string, std::string> _table;
	};
};