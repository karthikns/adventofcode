#include <algorithm>
#include <bitset>
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
    uint64_t maxCountX = 0;

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

            // Enter parsing code here
            if (line.find("mask = ") != string::npos)
            {
                Input input{};
                input.mask = line.substr(7);
                //cout << "    " << input.mask << endl;

                const size_t countOfX = count(begin(input.mask), end(input.mask), 'X');
                maxCountX = max(countOfX, maxCountX);

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

                maxAddress = max(valueNumber, maxAddress);

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

tuple<uint64_t, uint64_t> GetAndAndOrMasksPart1(const string& mask)
{
    string orMaskString = mask;
    replace(begin(orMaskString), end(orMaskString), 'X', '0');
    uint64_t orMask = stoull(orMaskString, nullptr, 2);

    string andMaskString = mask;
    replace(begin(andMaskString), end(andMaskString), 'X', '1');
    uint64_t andMask = stoull(andMaskString, nullptr, 2);

    return make_tuple(orMask, andMask);
}

uint64_t ProcessBitmaskPart1(const uint64_t initialValue, const uint64_t orMask, const uint64_t andMask)
{
    uint64_t value = initialValue;
    value |= orMask;
    value &= andMask;
    return value;
}

uint64_t ProcessBitmasksAndAddValuesPart1(const vector<Input>& inputs)
{
    map<uint64_t, uint64_t> processedAddressValueMap;

    for (const Input& input : inputs)
    {
        uint64_t orMask, andMask;
        tie(orMask, andMask) = GetAndAndOrMasksPart1(input.mask);
        for (const AddressValue& addressValue : input.addressValues)
        {
            processedAddressValueMap[addressValue.index] = ProcessBitmaskPart1(addressValue.value, orMask, andMask);
        }
    }

    uint64_t sumOfAllValues = 0;
    for (const pair<uint64_t, uint64_t>& mapEntry : processedAddressValueMap)
    {
        sumOfAllValues += mapEntry.second;
    }

    return sumOfAllValues;
}

vector<uint64_t> GetAllAddressesPart2(const string& address)
{
    vector<string> candidates = { address };
    vector<uint64_t> addresses;

    while (!candidates.empty())
    {
        string candidate = candidates.back();
        candidates.pop_back();

        size_t xPosition = candidate.find('X');
        if (xPosition == string::npos)
        {
            addresses.push_back(stoull(candidate, nullptr, 2));
            continue;
        }

        string first = candidate;
        first[xPosition] = '0';
        candidates.emplace_back(first);

        candidate[xPosition] = '1';
        candidates.emplace_back(candidate);
    }

    return addresses;
}

string ComputeAddressWithAddressMaskPart2(uint64_t address, string addressMask)
{
    string addressString = bitset<36>(address).to_string();

    if (addressString.size() != addressMask.size())
    {
        cout << "Address mask and address string sizes dont match" << endl;
        cout << endl;
        exit(-1);
    }

    for (size_t index = 0; index < addressMask.length(); ++index)
    {
        if (addressMask[index] == 'X' || addressMask[index] == '1')
        {
            addressString[index] = addressMask[index];
        }
    }

    return addressString;
}

// Incorrect
// 1779474351292 = low
uint64_t ProcessBitmasksAndAddValuesPart2(const vector<Input>& inputs)
{
    map<uint64_t, uint64_t> processedAddressValueMap;

    for (const Input& input : inputs)
    {
        for (const AddressValue& addressValue : input.addressValues)
        {
            const uint64_t value = addressValue.value;
            const string addressWithMask = ComputeAddressWithAddressMaskPart2(addressValue.index, input.mask);
            const vector<uint64_t> addresses = GetAllAddressesPart2(addressWithMask);
            for (const uint64_t address : addresses)
            {
                processedAddressValueMap[address] = value;
            }
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

    uint64_t processBitmasksAndAddValuesPart1 =  ProcessBitmasksAndAddValuesPart1(parseResult);
    cout << "processBitmasksAndAddValuesPart1: " << processBitmasksAndAddValuesPart1 << endl;
    cout << endl;

    uint64_t processBitmasksAndAddValuesPart2 = ProcessBitmasksAndAddValuesPart2(parseResult);
    cout << "processBitmasksAndAddValuesPart2: " << processBitmasksAndAddValuesPart2 << endl;
    cout << endl;

    return 0;
}
