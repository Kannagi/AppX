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
	const std::string fileJSON = "appx.json";

	APPX_FILE appxf;
    appxf.strFileGlobal = extractNameFileJSON(fileJSON,"global");
    appxf.strFileModule = extractNameFileJSON(fileJSON,"module");
/*
    // Afficher les noms de fichiers
    for (const std::string& nomFichier : nomsFichiers) {
        std::cout << nomFichier << std::endl;
    }
*/

    std::map<std::string, std::string> values = extractValues(fileJSON);

    // Display the values
    std::cout << "Name: " << values["name"] << std::endl;
    std::cout << "Version: " << values["version"] << std::endl;


    enginejs(appxf);
    return 0;
}
