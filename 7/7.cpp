#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

map<string, vector<string>> Parse(const string& inputFileName)
{
    map<string, vector<string>> bagList;

    try
    {
        ifstream inputFile(inputFileName, ios::in);

        if (inputFile.fail())
        {
            cout << "File Open Error" << endl;
            exit(-1);
        }

        string line;
        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, line);
            cout << "line: " << line << endl;

            size_t delimiterPosition = line.find('|');
            const string outerBagColor = line.substr(0, delimiterPosition);
            cout << outerBagColor << endl;

            while (true)
            {
                size_t colorStartPosition = delimiterPosition + 1;
                delimiterPosition = line.find('|', colorStartPosition);

                if (delimiterPosition == string::npos)
                {
                    break;
                }

                const string innerBagColor = line.substr(colorStartPosition, delimiterPosition - colorStartPosition);
                cout << "    " << innerBagColor << endl;

                if (innerBagColor == "no other")
                    continue;

                bagList[innerBagColor].emplace_back(outerBagColor);
            }
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return bagList;
}

size_t NumberOfOuterBagsThatContainShinyGoldInnerBag(const map<string, vector<string>>& bagList)
{
    set<string> outerBagsThatContainShinyGoldInnerBag;

    set<string> bagsToSearch;
    bagsToSearch.emplace("shiny gold");

    while (!bagsToSearch.empty())
    {
        auto firstBagIterator = bagsToSearch.begin();
        const string innerBagColor = *firstBagIterator;
        bagsToSearch.erase(firstBagIterator);

        // If this color has been evaluated, continue
        if (outerBagsThatContainShinyGoldInnerBag.find(innerBagColor) != end(outerBagsThatContainShinyGoldInnerBag))
        {
            continue;
        }

        outerBagsThatContainShinyGoldInnerBag.emplace(innerBagColor);

        auto bagListIterator = bagList.find(innerBagColor);
        if (bagListIterator == end(bagList))
        {
            //cout << "Bag not found in bagList: " << innerBagColor << endl;
            continue;
        }

        bagsToSearch.insert(begin(bagListIterator->second), end(bagListIterator->second));
    }

    // -1 to remove the gold bag that is added initially
    return outerBagsThatContainShinyGoldInnerBag.size() - 1;
}

int main()
{
    const string inputFileName(R"(7_1_in_processed.txt)");
    map<string, vector<string>> bagList = Parse(inputFileName);



    cout << endl;
    cout << bagList.size() << endl << endl;

    cout << NumberOfOuterBagsThatContainShinyGoldInnerBag(bagList) << endl << endl;

    return 0;
}
