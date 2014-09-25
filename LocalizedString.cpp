//
//  LocalizedString.cpp
//  Magic Forge
//
//  Created by Phil Maevskiy on 12/07/14.
//
//

#include "LocalizedString.h"

#include "cocos2d.h"
#include <iostream>
#include <sstream>
#include <map>

USING_NS_CC;

static std::map<std::string, std::string> localizedStrings;

void purgeLocalizedStringsCache()
{
    localizedStrings.clear();
}

std::string localize(const std::string& key)
{
    std::string fileName = "locales/";
    
    LanguageType curLanguage = Application::getInstance()->getCurrentLanguage();
    switch (curLanguage) {

        case LanguageType::ENGLISH:
        fileName += "Locale_en";
        break;
     
    case LanguageType::RUSSIAN:
        fileName += "Locale_ru";
        break;
/*
    case LanguageType::CHINESE:
        fileName += "Locale_zh";
        break;
    
     case LanguageType::FRENCH:
        fileName += "Locale_fr";
        break;
        
     case LanguageType::ITALIAN:
        fileName += "Locale_it";
        break;
        
     case LanguageType::GERMAN:
        fileName += "Locale_ge";
        break;
     
    case LanguageType::SPANISH:
        fileName += "Locale_sp";
        break;
             
     case LanguageType::KOREAN:
        fileName += "Locale_ko";
        break;
             
     case LanguageType::JAPANESE:
        fileName += "Locale_jp";
        break;
            
     case LanguageType::HUNGARIAN:
        fileName += "Locale_hu";
        break;
*/
    default:
        break;
    }

    std::string resultStr;

    if (localizedStrings.empty()) {
        std::string line;
        std::istringstream fileStringStream(FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(fileName)));

        // Get file contents line by line
        while (std::getline(fileStringStream, line)) {
            if (line.find("/*", 0) == std::string::npos && line.find("//", 0) == std::string::npos) //filter the valid string of one line
            {
                std::string::size_type validPos = line.find('=', 0);
                if (validPos != std::string::npos) {
                    std::string keyStr = line.substr(0, validPos - 1);
                    std::string subStr = line.substr(validPos + 1, line.size() - 1); // get valid string

                    //trim space
                    keyStr.erase(0, keyStr.find_first_not_of(" \t"));
                    keyStr.erase(keyStr.find_last_not_of(" \t") + 1);

                    subStr.erase(0, subStr.find_first_not_of(" \t"));
                    subStr.erase(subStr.find_last_not_of(" \t") + 1);

                    //trim \"
                    keyStr.erase(0, keyStr.find_first_not_of("\""));
                    keyStr.erase(keyStr.find_last_not_of("\"") + 1);

                    subStr.erase(0, subStr.find_first_not_of("\""));

                    //trim ; character and last \" character
                    subStr.erase(subStr.find_last_not_of(";") + 1);
                    subStr.erase(subStr.find_last_not_of("\"") + 1);

                    //replace line feed with \n
                    std::string::size_type pos(0);
                    std::string old_value("\\n");
                    if ((pos = subStr.find(old_value)) != std::string::npos) {
                        for (; pos != std::string::npos; pos += 1) {
                            if ((pos = subStr.find(old_value, pos)) != std::string::npos) {
                                subStr.erase(pos, 2);
                                subStr.insert(pos, 1, '\n');
                            } else
                                break;
                        }
                    }

                    localizedStrings.insert(std::pair<std::string, std::string>(keyStr, subStr));
                }
            }
        }
    }

    auto it = localizedStrings.find(key);
    if (it != localizedStrings.end()) {
        resultStr = it->second;
        if (resultStr.length() > 0) {
            return it->second;
        }
    }

    return key;
}