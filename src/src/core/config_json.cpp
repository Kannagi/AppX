#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> extractNameFileJSON(const std::string& filejson,const std::string& groupname)
{
    std::ifstream file(filejson);
    std::string contentJSON, line;

    while (std::getline(file, line))
    {
        contentJSON += line;
    }

    std::vector<std::string> nameFile;

    std::size_t position = contentJSON.find("\""+groupname+"\"");

    if (position == std::string::npos)
		return nameFile;

	position = contentJSON.find("[", position);
	if (position == std::string::npos)
		return nameFile;


	std::size_t endTab = contentJSON.find("]", position);
	if (endTab == std::string::npos)
		return nameFile;


	std::string filesJSON = contentJSON.substr(position + 1, endTab - position - 1);

	std::size_t beginName = filesJSON.find("\"");
	std::size_t endName;
	while (beginName != std::string::npos)
	{
		endName = filesJSON.find("\"", beginName + 1);
		if (endName != std::string::npos)
		{
			std::string tfile = filesJSON.substr(beginName + 1, endName - beginName - 1);
			nameFile.push_back(tfile);
			beginName = filesJSON.find("\"", endName + 1);
		}
		else
		{
			break;
		}
	}





    return nameFile;
}

std::map<std::string, std::string> extractValues(const std::string& jsonFile)
{
    std::ifstream file(jsonFile);
    std::string jsonContent, line;

    // Read the JSON file line by line
    while (std::getline(file, line))
        jsonContent += line;

    std::map<std::string, std::string> values;

    // Search for keys and extract the corresponding values
    std::size_t position = 0;
    while (true)
    {
        std::size_t startKey = jsonContent.find("\"", position);
        if (startKey != std::string::npos)
        {
            std::size_t endKey = jsonContent.find("\"", startKey + 1);
            if (endKey != std::string::npos)
            {
                std::string key = jsonContent.substr(startKey + 1, endKey - startKey - 1);

                std::size_t startValue = jsonContent.find("\"", endKey + 1);
                if (startValue != std::string::npos)
                {
                    std::size_t endValue = jsonContent.find("\"", startValue + 1);
                    if (endValue != std::string::npos)
                    {
                        std::string value = jsonContent.substr(startValue + 1, endValue - startValue - 1);
                        values[key] = value;
                    }
                }

                position = endKey + 1;
            }
        }
        else
        {
            break;
        }
    }

    return values;
}


