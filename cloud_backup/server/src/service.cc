#include <iostream>
#include <thread>
#include "service.hpp"
#include "hot.hpp"
using namespace std;
cloud::DataManager* _data;


void HotTest()
{
    cloud::HotManger hot;
    hot.RunModule();
}
void ServiceTest()
{
    cloud::Service srv;
    srv.RunModule();
}
int main()
{   
    _data = new cloud::DataManager();
    thread thread_hot(HotTest);
    thread thread_service(ServiceTest);
    thread_hot.join();
    thread_service.join();
    return 0;
}