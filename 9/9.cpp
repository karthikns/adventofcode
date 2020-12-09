#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

vector<uint64_t> ParseInputFile(const string& inputFileName)
{
    vector<uint64_t> inputNumbers{};

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

            inputNumbers.emplace_back(stoull(line));
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return inputNumbers;
}

bool DoAnyTwoNumbersAddToTarget(vector<uint64_t>::const_iterator begin, vector<uint64_t>::const_iterator end, uint64_t amount)
{
    for (auto first = begin; first != end; ++first)
    {
        auto second = find(first, end, amount - *first);
        if (second != end)
        {
            return true;
        }
    }

    return false;
}

uint64_t FindIncorrectNumber(const vector<uint64_t> inputNumbers)
{
    for (size_t candidateIndex = 25; candidateIndex < inputNumbers.size(); ++candidateIndex)
    {
        const uint64_t target = inputNumbers[candidateIndex];
        vector<uint64_t>::const_iterator inputNumbersBeginIterator = inputNumbers.begin();
        if (!DoAnyTwoNumbersAddToTarget(inputNumbers.begin() + candidateIndex - 25, inputNumbers.begin() + candidateIndex, target))
        {
            return inputNumbers[candidateIndex];
        }
    }

    return 0;
}

uint64_t FindEncryptionWeakness(const vector<uint64_t> inputNumbers, const uint64_t incorrectNumber)
{

    for (size_t rangeBeginIndex = 0; rangeBeginIndex < inputNumbers.size(); ++rangeBeginIndex)
    {
        uint64_t accumulator = inputNumbers[rangeBeginIndex];
        size_t rangeEndIndex = rangeBeginIndex + 1;
        bool found = false;
        for (; rangeEndIndex < inputNumbers.size(); ++rangeEndIndex)
        {
            accumulator += inputNumbers[rangeEndIndex];

            if (accumulator == incorrectNumber)
            {
                found = true;
                break;
            }
            else if (accumulator > incorrectNumber)
            {
                break;
            }
        }

        if (found)
        {
            const uint64_t minInRange = *min_element(inputNumbers.cbegin() + rangeBeginIndex, inputNumbers.cbegin() + rangeEndIndex + 1);
            const uint64_t maxInRange = *max_element(inputNumbers.cbegin() + rangeBeginIndex, inputNumbers.cbegin() + rangeEndIndex + 1);

            cout << minInRange << ", " << maxInRange << endl;

            return minInRange + maxInRange;
        }
    }

    return 0;
}

// part 2: 3184168 too low
int main()
{
    const string inputFileName(R"(9_in.txt)");

    vector<uint64_t> inputNumbers = ParseInputFile(inputFileName);
    cout << "Input size: " << inputNumbers.size() << endl << endl;

    const uint64_t incorrectNumber = FindIncorrectNumber(inputNumbers);
    cout << "Incorrect number: " << incorrectNumber << endl << endl;

    const uint64_t encryptionWeakness = FindEncryptionWeakness(inputNumbers, incorrectNumber);
    cout << "Part 2: " << encryptionWeakness << endl << endl;

    return 0;
}
