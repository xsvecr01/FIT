/**
 * @file FileParser.cpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Implementacni soubor
 * 
 */

#include "FileParser.hpp"

FileParser::FileParser(){};

FileParser::FileParser(std::string filename)
{
    _filename = filename;
    if(!fileExists())
    {
        return;
    }
    _domains = getList();
    
}


bool FileParser::isBlacklisted(std::string address)
{
    std::vector<std::string> parts; 
    std::stringstream ss(address); 
    std::string s; 
      
    while(getline(ss, s, '.'))
    {
        parts.push_back(s);
    }

    s = parts.back();
    while(!parts.empty())
    {
        auto found = _domains.find(s);
        if(found != _domains.end())
        {
            return true;
        }
        parts.pop_back();
        if(!parts.empty())
        {
            s = parts.back() + "." + s;
        }
    }

    return false;
}

bool FileParser::isEmpty()
{
    if(_domains.empty())
    {
        return true;
    }
    return false;
}


bool FileParser::fileExists()
{
    std::ifstream ifile(_filename);
    if(ifile)
    {
        ifile.close();
        return true;
    }
    else
    {
        return false;
    }
}


std::unordered_set<std::string> FileParser::getList()
{
    std::ifstream readFile(_filename);
    std::string line;
    
    std::unordered_set<std::string> domains;
    
    while(getline (readFile, line))
    {
        line = removeSpaces(line);
        if(line[0] != '#' && line[0] != '\0')
        {
            domains.insert(line);
        }
    }
    readFile.close();
    return domains;
}

std::string FileParser::removeSpaces(std::string str) 
{ 
    int count = 0; 
    for (int i = 0; str[i]; i++) 
        if (str[i] != ' ') 
            str[count++] = str[i];

    str[count] = '\0'; 
    return str;
} 
