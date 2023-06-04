

typedef struct
{
    int delay_time;
    bool loop;

    std::vector<std::string> strFileModule;
    std::vector<std::string> strFileGlobal;
}APPX_FILE;


#include <fstream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> extractNameFileJSON(const std::string& filejson,const std::string& groupname);
std::map<std::string, std::string> extractValues(const std::string& jsonFile);

int enginejs(APPX_FILE &appxf);
