#include "config.hpp"

using namespace std;

int main()
{
    cloud::Config* config = cloud::Config::get_instance();
    cout << config->GetHotTime() << endl;
    cout << config->GetServerPort() << endl;
    cout << config->GetServerIP() << endl;
    cout << config->GetDownloadPrefix() << endl;
    cout << config->GetPackFileSuffix() << endl;
    cout << config->GetPackDir() << endl;
    cout << config->GetBackDir() << endl;
    cout << config->GetBackupFile() << endl;
    return 0;
}