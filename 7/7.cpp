#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

map<string, vector<string>> ParsePart1(const string& inputFileName)
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

struct InnerBag
{
    size_t innerBagCount;
    string innerBagColor;
};

map<string, vector<InnerBag>> ParsePart2(const string& inputFileName)
{
    map<string, vector<InnerBag>> bagList;

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
            //cout << "line: " << line << endl;

            size_t delimiterPosition = line.find('|');
            const string outerBagColor = line.substr(0, delimiterPosition);
            cout << outerBagColor << endl;

            bagList[outerBagColor] = {};

            while (true)
            {
                size_t colorStartPosition = delimiterPosition + 1;
                delimiterPosition = line.find('|', colorStartPosition);

                if (delimiterPosition == string::npos)
                {
                    break;
                }

                const string innerBag = line.substr(colorStartPosition, delimiterPosition - colorStartPosition);

                if (innerBag == "no other")
                    continue;

                size_t numberDelimiterPosition = innerBag.find('-');
                string numberString = innerBag.substr(0, numberDelimiterPosition);
                size_t number = stoi(numberString);

                string innerColor = innerBag.substr(numberDelimiterPosition + 1);

                bagList[outerBagColor].push_back({number, innerColor});

                cout << "    " << numberString << " " << innerColor << endl;
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

size_t NumberOfInnerBagsInShinyGoldOuterBag(const map<string, vector<InnerBag>>& bagList)
{
    map<string, size_t> nestedBagCounts;

    // Infinite loop if the input is not a DAG
    while (bagList.size() != nestedBagCounts.size())
    {
        for (const auto& bagContents : bagList)
        {
            bool canCalculateBagCount = true;
            size_t bagSize = 0;
            for (const auto& innerBag : bagContents.second)
            {
                auto it = nestedBagCounts.find(innerBag.innerBagColor);
                if (it == end(nestedBagCounts))
                {
                    canCalculateBagCount = false;
                    break;
                }

                bagSize += it->second * innerBag.innerBagCount;
            }

            if (canCalculateBagCount)
            {
                // +1 to add the bag which is a direct child
                nestedBagCounts[bagContents.first] = bagSize + 1;
            }
        }
    }

    // -1 to exclude the shiny gold outer bag
    return nestedBagCounts["shiny gold"] - 1;
}

int main()
{
    {
        const string inputFileName(R"(7_1_in_processed.txt)");
        map<string, vector<string>> bagList = ParsePart1(inputFileName);
        cout << bagList.size() << endl;
        cout << NumberOfOuterBagsThatContainShinyGoldInnerBag(bagList) << endl << endl;
    }

    {
        const string inputFileName(R"(7_2_in_processed.txt)");
        map<string, vector<InnerBag>> bagList = ParsePart2(inputFileName);
        cout << bagList.size() << endl;
        cout << NumberOfInnerBagsInShinyGoldOuterBag(bagList) << endl << endl;
    }

    return 0;
}
