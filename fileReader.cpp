#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

//Class FileReader used to read the configuration file
class FileReader {
public:
    //Read all parameters from the file and store them in a map<string , string>
    inline FileReader (std::string fileName)
    {   char* buffer =  new char[256];
        std::ifstream config(fileName.c_str());

        if(!config.is_open()){
            delete[] buffer;
            std::cerr << "Error: Failed to open " << fileName << " !" << std::endl;
        } else
        {   while (config.getline (buffer , 100))
            {   paraName = strtok (buffer , " ");
                paraVal = strtok (NULL , " ");
                paraMap.insert(std::map<std::string , std::string>::value_type(paraName , paraVal));
            }

            delete[] buffer;
            config.close();
        }
    }

    inline ~FileReader ()
    {
    }

    //Return the parameter value with the given name and type
    //Return an empty value and error message if failed
    template <typename T>
    inline T getParameter (std::string paraName)
    {   T res;
        std::map<std::string , std::string>::iterator it_map;
        it_map = paraMap.find (paraName);
        if (it_map != paraMap.end())
        {   std::stringstream sstr (it_map -> second);
            sstr >> res;
            sstr.clear();
        } else
        {   std::cout << "Parameter " << paraName << "not found in the configuration file!" << std::endl;
        }

        return res;
    }

private :
    std::map <std::string , std::string> paraMap;
    std::string paraName;
    std::string paraVal;
};
