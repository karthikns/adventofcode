#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

struct AddressValue
{
    uint64_t index;
    uint64_t value;
};

struct Input
{
    string mask;
    vector<AddressValue> addressValues;
};

vector<Input> ParseInputFile(const string& inputFileName)
{
    vector<Input> parseResult{};

    uint64_t maxAddress = 0;

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

            if (line.empty())
                break;

            // Enter parsing code here
            if (line.find("mask = ") != string::npos)
            {
                Input input{};
                input.mask = line.substr(7);
                //cout << "    " << input.mask << endl;

                parseResult.emplace_back(input);
                continue;
            }

            if (line.find("mem[") != string::npos)
            {
                //cout << "    " << line << endl;
                smatch matchValues;
                regex memRegEx("mem\\[(\\d+)\\] = (\\d+)");

                regex_search(line, matchValues, memRegEx);

                const string addressString = matchValues[1].str();
                const string valueString = matchValues[2].str();
                uint64_t addressNumber = stoull(addressString);
                uint64_t valueNumber = stoull(valueString);

                maxAddress = valueNumber > maxAddress ? valueNumber : maxAddress;

                parseResult.back().addressValues.push_back({ addressNumber, valueNumber });
            }
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

tuple<uint64_t, uint64_t> GetAndAndOrMasks(const string& mask)
{
    string orMaskString = mask;
    replace(begin(orMaskString), end(orMaskString), 'X', '0');
    uint64_t orMask = stoull(orMaskString, nullptr, 2);

    string andMaskString = mask;
    replace(begin(andMaskString), end(andMaskString), 'X', '1');
    uint64_t andMask = stoull(andMaskString, nullptr, 2);

    return make_tuple(orMask, andMask);
}

uint64_t ProcessBitmask(const uint64_t initialValue, const uint64_t orMask, const uint64_t andMask)
{
    uint64_t value = initialValue;
    value |= orMask;
    value &= andMask;
    return value;
}

uint64_t ProcessBitmasksAndAddValues(const vector<Input>& inputs)
{
    map<uint64_t, uint64_t> processedAddressValueMap;

    for (const Input& input : inputs)
    {
        uint64_t orMask, andMask;
        tie(orMask, andMask) = GetAndAndOrMasks(input.mask);
        for (const AddressValue& addressValue : input.addressValues)
        {
            processedAddressValueMap[addressValue.index] = ProcessBitmask(addressValue.value, orMask, andMask);
        }
    }

    uint64_t sumOfAllValues = 0;
    for (const pair<uint64_t, uint64_t>& mapEntry : processedAddressValueMap)
    {
        sumOfAllValues += mapEntry.second;
    }

    return sumOfAllValues;
}

int main()
{
    const string inputFileName(R"(14_in.txt)");

    vector<Input> parseResult = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Parse result Size: " << parseResult.size() << endl;
    cout << endl;

    uint64_t sumOfValuesAfterApplyingBitmasks =  ProcessBitmasksAndAddValues(parseResult);
    cout << "sumOfValuesAfterApplyingBitmasks: " << sumOfValuesAfterApplyingBitmasks << endl;
    cout << endl;

    return 0;
}
