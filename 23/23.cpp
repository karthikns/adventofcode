#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<size_t>::iterator FindDestinationCup(vector<size_t>& cups, size_t currentCupValue)
{
    size_t destinationCupValue = currentCupValue;
    vector<size_t>::iterator destinationCupIterator;

    while (true)
    {
        destinationCupValue = (9 + destinationCupValue - 1) % 9;

        destinationCupIterator = find(begin(cups), end(cups), destinationCupValue);
        if (destinationCupIterator != end(cups))
            return destinationCupIterator;
    }

    return destinationCupIterator;
}

string GetDebugString(const vector<size_t>& cups, const size_t currentValue)
{
    string debugString;
    for (size_t index = 0; index < cups.size(); ++index)
    {
        const size_t cupValue = cups[index];
        string cupValueString;
        cupValueString = ('1' + cupValue);

        if (cupValue == currentValue)
            debugString += "(" + cupValueString + ")";
        else
            debugString += cupValueString;

        debugString += ' ';
    }

    return debugString;
}

string PlayCrabCups(string input)
{
    size_t c_numberOfTurns = 100;

    vector<size_t> cups;

    for (char& cup : input)
    {
        cups.push_back(cup - '1');
    }

    size_t currentCupValue = cups[0];
    for (size_t turnNumber = 0; turnNumber < c_numberOfTurns; ++turnNumber)
    {
        cout << "-- move " << turnNumber + 1 << " --" << endl;
        cout << "cups: " << GetDebugString(cups, currentCupValue) << endl;

        auto currentCupIterator = find(begin(cups), end(cups), currentCupValue);
        size_t currentCupIndex = distance(begin(cups), currentCupIterator);

        size_t cupToRemoveIndex = 0;
        vector<size_t> removedCups;
        cupToRemoveIndex = (currentCupIndex + 1) % cups.size();
        removedCups.push_back(cups[cupToRemoveIndex]);
        cupToRemoveIndex = (currentCupIndex + 2) % cups.size();
        removedCups.push_back(cups[cupToRemoveIndex]);
        cupToRemoveIndex = (currentCupIndex + 3) % cups.size();
        removedCups.push_back(cups[cupToRemoveIndex]);

        cups.erase(remove(cups.begin(), cups.end(), removedCups[0]), cups.end());
        cups.erase(remove(cups.begin(), cups.end(), removedCups[1]), cups.end());
        cups.erase(remove(cups.begin(), cups.end(), removedCups[2]), cups.end());

        cout << "pick up: " << 1 + removedCups[0] << ", " << 1 + removedCups[1] << ", " << 1 + removedCups[2] << endl;

        vector<size_t>::iterator destinationCupIterator = FindDestinationCup(cups, currentCupValue);
        const size_t destinationValue = *destinationCupIterator;

        cout << "destination: " << 1 + *destinationCupIterator << endl;

        cups.insert(destinationCupIterator + 1, begin(removedCups), end(removedCups));

        auto newCurrentCupIterator = find(begin(cups), end(cups), currentCupValue);
        size_t newCurrentCupIndex = (9 + distance(begin(cups), newCurrentCupIterator) + 1) % 9;
        currentCupValue = cups[newCurrentCupIndex];

        cout << endl;
    }

    cout << "--final--" << endl;
    cout << "cups: " << GetDebugString(cups, currentCupValue) << endl;
    cout << endl;

    auto zeroIterator = find(begin(cups), end(cups), 0);
    vector<size_t> answer(zeroIterator + 1, end(cups));
    if (answer.size() < 8)
    {
        answer.insert(end(answer), begin(cups), zeroIterator);
    }

    string answerString;
    transform(
        begin(answer), end(answer),
        back_inserter(answerString), [](const size_t value) -> char
        {
            return '1' + value;
        });

    return answerString;
}

int main()
{
    const string input("158937462");
    const string samplel_input("389125467");

    const string& cups = input;

    cout << "Crab cups (Part 1): " << PlayCrabCups(cups) << endl;
    cout << endl;

    return 0;
}
