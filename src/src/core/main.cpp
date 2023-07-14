#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AppX.h"

int main()
{
    UI_init();

    const std::string fileJSON = "appx.json";

    APPX_FILE appxf;
    appxf.strFileGlobal = extractNameFileJSON(fileJSON,"global");
    appxf.strFileModule = extractNameFileJSON(fileJSON,"module");
    appxf.strFileStyle  = extractNameFileJSON(fileJSON,"style");

    std::map<std::string, std::string> values = extractValues(fileJSON);
/*
    // Display the values
    std::cout << "Name: " << values["name"] << std::endl;
    std::cout << "Version: " << values["version"] << std::endl;
	std::cout << "tar: " << values["tar"] << std::endl;
*/
    enginejs(appxf);

    return 0;
}




