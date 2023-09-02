#include <iostream>
#include "hot.hpp"

using namespace std;
cloud::DataManager* _data;
int main()
{
    _data = new cloud::DataManager();
    cloud::HotManger hot;
    hot.RunModule();
    return 0;
}