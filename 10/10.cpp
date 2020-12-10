#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<uint32_t> ParseInputFile(const string& inputFileName)
{
    vector<uint32_t> parseResult{};

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

            if (line.empty())
                break;

            parseResult.emplace_back(stoul(line));
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

uint32_t MultiplyOneDifferencesWithThreeDifferences(vector<uint32_t>& joltages)
{
    sort(begin(joltages), end(joltages));
    joltages.insert(begin(joltages), 0);

    joltages.emplace_back(joltages[joltages.size() - 1] + 3);

    uint32_t numberOfOneDifferences = 0;
    uint32_t numberOfThreeDifferences = 0;
    for (size_t index = 0; index < joltages.size() - 1; ++index)
    {
        const uint32_t difference = joltages[index + 1] - joltages[index];
        if (difference == 1)
            ++numberOfOneDifferences;
        else if(difference == 3)
            ++numberOfThreeDifferences;
    }

    return numberOfOneDifferences * numberOfThreeDifferences;
}

uint64_t FindNumberOfCombinationsOfAdapters(const vector<uint32_t>& joltages)
{
    const size_t maxNumber = joltages[joltages.size() - 1];
    vector<uint64_t> numberOfCombinations(maxNumber + static_cast<size_t>(1));

    numberOfCombinations[maxNumber] = 1;
    numberOfCombinations[maxNumber - 1] = 0;
    numberOfCombinations[maxNumber - 2] = 0;
    for (int number = static_cast<int>(maxNumber - 3); number >= 0; --number)
    {
        const size_t plusOneIndex = static_cast<size_t>(number) + 1;
        const size_t plusTwoIndex = plusOneIndex + 1;
        const size_t plusThreeIndex = plusTwoIndex + 1;

        if (find(begin(joltages), end(joltages), number) == end(joltages))
        {
            numberOfCombinations[number] = 0;
        }
        else
        {
            numberOfCombinations[number] =
                numberOfCombinations[plusOneIndex] +
                numberOfCombinations[plusTwoIndex] +
                numberOfCombinations[plusThreeIndex];
        }
    }

    return numberOfCombinations[0];
}

int main()
{
    const string inputFileName(R"(10_in.txt)");

    vector<uint32_t> joltages = ParseInputFile(inputFileName);
    cout << endl;
    cout << "Joltages Size: " << joltages.size() << endl;
    cout << endl;

    const uint32_t multiplyOneDifferencesWithThreeDifferences = MultiplyOneDifferencesWithThreeDifferences(joltages);
    cout << "MultiplyOneDifferencesWithThreeDifferences: " << multiplyOneDifferencesWithThreeDifferences << endl;
    cout << endl;

    const uint64_t numberOfCombinationsOfAdapters = FindNumberOfCombinationsOfAdapters(joltages);
    cout << "FindNumberOfCombinationsOfAdapters: " << numberOfCombinationsOfAdapters << endl;
    cout << endl;

    return 0;
}
