#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

uint64_t MemoryGamePart1(vector<uint64_t>& history)
{
    //for (const uint64_t num : history)
    //{
    //    cout << num << endl;
    //}

    while (history.size() < 2020)
    {
        auto historyRBegin = crbegin(history);
        auto historyREnd = crend(history);

        uint64_t numberToInsert = 0;
        auto findIterator = find(historyRBegin + 1, historyREnd, history.back());
        if (findIterator != historyREnd)
            numberToInsert = static_cast<uint64_t>(distance(historyRBegin, findIterator));

        history.emplace_back(numberToInsert);
        //cout << history.back() << endl;
    }

    return history.back();
}

uint64_t MemoryGamePart2(const vector<uint64_t>& history)
{
    //for (const uint64_t num : history)
    //{
    //    cout << num << endl;
    //}

    unordered_map<uint64_t, uint64_t> numberLastIndexMap;
    for (uint64_t index = 0; index < history.size() - 1; ++index)
    {
        numberLastIndexMap[history[index]] = index;
    }

    uint64_t previousNumber = history.back();

    for (uint64_t index = history.size(); index < 30000000ull; ++index)
    {
        if (index % 1000000ull == 0)
            cout << "Processing index: " << index << endl;

        auto findIterator = numberLastIndexMap.find(previousNumber);

        uint64_t findIndex = 0;
        if (findIterator != numberLastIndexMap.end())
            findIndex = findIterator->second;

        numberLastIndexMap[previousNumber] = index - 1;

        if (findIterator == numberLastIndexMap.end())
            previousNumber = 0;
        else
            previousNumber = index - findIndex - 1;

        //cout << previousNumber << endl;
    }

    return previousNumber;
}

int main()
{
    {
        //vector<uint64_t> history = { 0, 3, 6 };
        vector<uint64_t> input = { 14, 3, 1, 0, 9, 5 };

        cout << "Part 1 answer: " << MemoryGamePart1(input) << endl;
        cout << endl;

        cout << "Part 2 answer: " << MemoryGamePart2(input) << endl;
        cout << endl;
    }

    return 0;
}
