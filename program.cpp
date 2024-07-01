#include <stdio.h>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

void toLowercase(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void countUniqueWords(string file)
{
    ifstream input(file);
    string line;
    map<string, map<string, int>> messBuf;

    map<string, int> globalInnerMap;
    messBuf.insert(make_pair("GLOBAL", globalInnerMap));

    if (!input.is_open())
    {
        std::cerr << "Cant read file";
        return;
    }

    while (getline(input, line))
    {
        size_t pos = line.find(':');
        if (pos != string::npos)
        {
            size_t secondPos = line.find(':', pos + 1);
            size_t senderNamePos = line.find("- ");

            if (senderNamePos != string::npos && secondPos != string::npos)
            {
                stringstream messageContent(line.substr(secondPos + 1));
                string senderName = line.substr(senderNamePos + 2, secondPos - senderNamePos - 2);
                string word;

                while (messageContent >> word)
                {
                    toLowercase(word);
                    auto nameExist = messBuf.find(senderName);

                    if (nameExist != messBuf.end())
                    {
                        auto entryExist = nameExist->second.find(word);

                        if (entryExist != nameExist->second.end())
                        {
                            entryExist->second = entryExist->second + 1;
                        }
                        else
                        {
                            nameExist->second.insert(pair<string, int>(word, 1));
                        }
                    }
                    else
                    {
                        map<string, int> innerMap;
                        innerMap[word] = 1;
                        messBuf.insert(make_pair(senderName, innerMap));
                    }

                    auto globalEntryExist = messBuf["GLOBAL"].find(word);

                    if (globalEntryExist != messBuf["GLOBAL"].end())
                    {
                        globalEntryExist->second = globalEntryExist->second + 1;
                    }
                    else
                    {
                        messBuf["GLOBAL"].insert(pair<string, int>(word, 1));
                    }
                }
            }
        }
    }

    input.close();

    vector<pair<string, vector<pair<string, int>>>> vecMap;

    ofstream output("output.txt", ios::trunc);

    for (const auto &userMessages : messBuf)
    {
        vector<pair<string, int>> innerVector;

        for(const auto &msgs : userMessages.second){
            if(msgs.second <= 3) continue;
            
            innerVector.push_back(make_pair(msgs.first, msgs.second));
        }

        vecMap.push_back(make_pair(userMessages.first, innerVector));
    }

    output.close();

    int i = 0;

    for (const auto &userMessages : vecMap)
    {
        // cout << userMessages.first << endl;

        // for(const auto &msgs : userMessages.second){
        //     cout << msgs.first << ": " << msgs.second << endl;

        //     if(i > 20) return;
        //     i++;
        // }
    }

    // for(const auto &entry : vecMap){
    //     cout << entry.first << ": ";
    //     for(const auto &elements : entry.second){
    //         cout << elements << endl;
    //     }
    // }

    // vector<pair<string, int>> sorted;
    // for (const auto &element : vecMap)
    // {
    //     if (element.second >= 3)
    //     {
    //         sorted.push_back(element);
    //     }
    // }

    // std::sort(sorted.begin(), sorted.end(), [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
    //           { return a.second < b.second; });

    // if (!output.is_open())
    // {
    //     cerr << "Couldnt write to file";
    //     return;
    // }

    // for (const auto &pair : sorted)
    // {
    //     output << pair.first << ": " << pair.second << endl;
    // }


    std::cout << "Done" << endl;
}

int main() {
    countUniqueWords("/home/mix/Downloads/chat.txt");
    return 0;
}