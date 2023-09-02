#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "util.hpp"
#include "data.hpp"
#include "cloud.hpp"
using namespace std;

#define BACKUP_FILE "./backup.dat"
#define BACKUP_DIR "./backup/"
int main()
{
	cloud::Backup backup(BACKUP_DIR, BACKUP_FILE);
	backup.RunModule();
	return 0;
}