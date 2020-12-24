#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Bucket
{
    vector<size_t> bucketContents;
    size_t minValueInBucket = 0;
    size_t maxValueInBucket = 0;

    void UpdateMinAndMax()
    {
        if (bucketContents.empty())
        {
            cout << "Error: recomputing max and min when bucket is empty" << endl;
            exit(-1);
        }

        maxValueInBucket = *max_element(begin(bucketContents), end(bucketContents));
        minValueInBucket = *min_element(begin(bucketContents), end(bucketContents));
    }
};

struct BucketsPosition
{
    size_t bucketIndex = 0;
    size_t positionInBucket = 0;
};

class Buckets
{
public:
    Buckets(const size_t bucketSize, const size_t minBucketSize) :
        bucketSize(bucketSize),
        minBucketSize(minBucketSize)
    {
    }

    size_t GetFirstValue()
    {
        return GetValueAtBucketsPosition({0, 0});
    }

    size_t GetNextValue(const size_t value) const
    {
        const BucketsPosition valuePosition = GetValuePosition(value);
        const BucketsPosition nextPosition = GetNextPosition(valuePosition);
        return GetValueAtBucketsPosition(nextPosition);
    }

    void AppendValue(const size_t value)
    {
        const size_t fillSize = minBucketSize + (bucketSize - minBucketSize) * 0.5;
        //if (buckets.empty() || buckets[buckets.size() - 1].bucketContents.size() >= bucketSize)
        if (buckets.empty() || buckets[buckets.size() - 1].bucketContents.size() >= fillSize)
        {
            Bucket newBucket;
            newBucket.minValueInBucket = newBucket.maxValueInBucket = value;
            newBucket.bucketContents.emplace_back(value);
            buckets.emplace_back(newBucket);
            return;
        }

        Bucket& lastBucket = buckets[buckets.size() - 1];
        lastBucket.minValueInBucket = min(value, lastBucket.minValueInBucket);
        lastBucket.maxValueInBucket = max(value, lastBucket.maxValueInBucket);
        lastBucket.bucketContents.emplace_back(value);
    }

    void InsertAfter(const size_t value, const vector<size_t>& valuesToInsert)
    {
        BucketsPosition bucketPosition = GetValuePosition(value);
        Bucket& bucketToInsert = buckets[bucketPosition.bucketIndex];
        bucketToInsert.bucketContents.insert(
            begin(bucketToInsert.bucketContents) + bucketPosition.positionInBucket + 1,
            begin(valuesToInsert),
            end(valuesToInsert));

        if (bucketToInsert.bucketContents.size() <= bucketSize)
        {
            bucketToInsert.UpdateMinAndMax();
            return;
        }

        // Break up the bucket if it exceeds max
        SplitBucketIfItExceedsMax(bucketPosition.bucketIndex);
        return;
    }

    size_t RemoveAfter(const size_t value)
    {
        BucketsPosition valuePosition = GetValuePosition(value);
        BucketsPosition removePosition = GetNextPosition(valuePosition);

        const size_t valueToRemove = buckets[removePosition.bucketIndex].bucketContents[removePosition.positionInBucket];
        buckets[removePosition.bucketIndex].bucketContents.erase(
            begin(buckets[removePosition.bucketIndex].bucketContents) + removePosition.positionInBucket);

        // Nothing more to do if the value removed doesn't exceed the bucket's minimum size
        if (buckets[removePosition.bucketIndex].bucketContents.size() >= minBucketSize)
        {
            buckets[removePosition.bucketIndex].UpdateMinAndMax();
            return valueToRemove;
        }

        // If last bucket and has more than value, nothing more to do, return
        // If the last bucket has no values remaining, delete the bucket
        if (removePosition.bucketIndex == buckets.size() - 1)
        {
            if (buckets[removePosition.bucketIndex].bucketContents.empty())
            {
                buckets.erase(begin(buckets) + removePosition.bucketIndex);
            }
            else
            {
                buckets[removePosition.bucketIndex].UpdateMinAndMax();
            }

            return valueToRemove;
        }

        // The current bucket has less than minimum required elements
        // 1. merge contents of next bucket with this bucket
        // 2. delete next bucket
        // 3. split the current bucket if size is greater than max

        const size_t nextBucketIndex = removePosition.bucketIndex + 1;
        buckets[removePosition.bucketIndex].bucketContents.insert(
            end(buckets[removePosition.bucketIndex].bucketContents),
            begin(buckets[nextBucketIndex].bucketContents),
            end(buckets[nextBucketIndex].bucketContents)
        );

        buckets.erase(begin(buckets) + nextBucketIndex);
        SplitBucketIfItExceedsMax(removePosition.bucketIndex);

        return valueToRemove;
    }

    // TODO: Test
    vector<size_t> RemoveThreeAfter(const size_t value)
    {
        vector<size_t> valuesRemoved;
        valuesRemoved.push_back(RemoveAfter(value));
        valuesRemoved.push_back(RemoveAfter(value));
        valuesRemoved.push_back(RemoveAfter(value));

        return valuesRemoved;
    }

    bool IsValuePresent(const size_t value)
    {
        BucketsPosition bucketPosition = GetValuePosition(value);
        return (bucketPosition.bucketIndex == SIZE_MAX) ? false : true;
    }

    size_t GetNumberOfBuckets() const
    {
        return buckets.size();
    }

private:
    void SplitBucketIfItExceedsMax(size_t bucketIndex)
    {
        Bucket& bucketToSplit = buckets[bucketIndex];
        if (bucketToSplit.bucketContents.size() <= bucketSize)
        {
            buckets[bucketIndex].UpdateMinAndMax();
            return;
        }

        Bucket newBucket;
        const size_t startIndexForTransfer = bucketToSplit.bucketContents.size() / 2;
        vector<size_t>::iterator beginIterator = begin(bucketToSplit.bucketContents) + startIndexForTransfer;
        vector<size_t>::iterator endIterator = end(bucketToSplit.bucketContents);
        newBucket.bucketContents.insert(end(newBucket.bucketContents), beginIterator, endIterator);
        bucketToSplit.bucketContents.erase(beginIterator, endIterator);

        newBucket.UpdateMinAndMax();
        bucketToSplit.UpdateMinAndMax();

        buckets.insert(begin(buckets) + bucketIndex + 1, newBucket);
    }

    size_t GetValueAtBucketsPosition(const BucketsPosition& position) const
    {
        return buckets[position.bucketIndex].bucketContents[position.positionInBucket];
    }

    BucketsPosition GetNextPosition(const BucketsPosition& position) const
    {
        BucketsPosition nextPosition = { position.bucketIndex, position.positionInBucket + 1 };
        if (nextPosition.positionInBucket >= buckets[nextPosition.bucketIndex].bucketContents.size())
        {
            nextPosition.bucketIndex = (nextPosition.bucketIndex + 1) % buckets.size();
            nextPosition.positionInBucket = 0;
        }

        return nextPosition;
    }

    BucketsPosition GetValuePosition(const size_t value) const
    {
        for (size_t bucketIndex = 0; bucketIndex < buckets.size(); ++bucketIndex)
        {
            const Bucket& currentBucket = buckets[bucketIndex];
            if (value >= currentBucket.minValueInBucket &&
                value <= currentBucket.maxValueInBucket)
            {
                vector<size_t>::const_iterator findIterator = find(
                    cbegin(currentBucket.bucketContents),
                    cend(currentBucket.bucketContents),
                    value);

                if (findIterator == end(currentBucket.bucketContents))
                    continue;

                const size_t positionInBucket = distance(begin(currentBucket.bucketContents), findIterator);
                return { bucketIndex, positionInBucket };
            }
        }

        return { SIZE_MAX, SIZE_MAX };
    }

private:
    vector<Bucket> buckets;
    size_t bucketSize;
    size_t minBucketSize;
};

void TestBuckets()
{
    Buckets buckets(10, 5);

    buckets.AppendValue(1234);
    for (size_t index = 0; index < 50; ++index)
    {
        buckets.AppendValue(index);
    }

    buckets.AppendValue(50);

    size_t value = {};
    value = buckets.GetNextValue(8);
    value = buckets.GetNextValue(9);
    value = buckets.GetNextValue(50);

    value = buckets.GetFirstValue();

    buckets.InsertAfter(1234, { 2345 });

    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;
    value = buckets.RemoveAfter(50);
    cout << value << endl;


    vector<size_t> removedElements;
    removedElements = buckets.RemoveThreeAfter(20);
    removedElements = buckets.RemoveThreeAfter(19);
    cout << "Removed" << endl;
}

size_t PlayCrabCupsOptimized(string input, const size_t inputSize, const size_t numberOfTurns)
{
    Buckets cups(1000, 500);

    size_t maxElement = 0;
    for (char& cup : input)
    {
        const size_t value = cup - '1';
        cups.AppendValue(value);
        maxElement = max(value, maxElement);
    }

    for (size_t index = maxElement + 1; index < inputSize; ++index)
        cups.AppendValue(index);

    size_t currentValue = cups.GetFirstValue();
    for (size_t turnNumber = 0; turnNumber < numberOfTurns; ++turnNumber)
    {
        if (turnNumber % 100'000 == 0)
        {
            std::cout << "Processing turn " << turnNumber << " of " << numberOfTurns << endl;
            std::cout << "    Number of buckets: " << cups.GetNumberOfBuckets() << endl;
        }

        //const size_t firstValue = cups.GetFirstValue();
        //size_t value = firstValue;
        //do {
        //    value = cups.GetNextValue(value);
        //    if(value == currentValue)
        //        cout << "(" << value + 1 << ")";
        //    else
        //        cout << value + 1;

        //    cout << " ";
        //} while (value != firstValue);
        //cout << endl;

        const vector<size_t> valuesRemoved = cups.RemoveThreeAfter(currentValue);

        size_t destinationValue = (inputSize + currentValue - 1) % inputSize;
        while (!cups.IsValuePresent(destinationValue))
            destinationValue = (inputSize + destinationValue - 1) % inputSize;

        cups.InsertAfter(destinationValue, valuesRemoved);

        currentValue = cups.GetNextValue(currentValue);
    }

    //cout << "Final configuration:" << endl;
    //const size_t firstValue = cups.GetFirstValue();
    //size_t value = firstValue;
    //do {
    //    value = cups.GetNextValue(value);
    //    if (value == currentValue)
    //        cout << "(" << value + 1 << ")";
    //    else
    //        cout << value + 1;

    //    cout << " ";
    //} while (value != firstValue);
    //cout << endl;



    const size_t firstNumber = cups.GetNextValue(0) + 1;
    const size_t secondNumber = cups.GetNextValue(firstNumber - 1) + 1;

    return firstNumber * secondNumber;
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

vector<size_t>::iterator FindDestinationCup(vector<size_t>& cups, const size_t currentCupValue, const size_t inputSize)
{
    size_t destinationCupValue = currentCupValue;
    vector<size_t>::iterator destinationCupIterator;

    while (true)
    {
        destinationCupValue = (inputSize + destinationCupValue - 1) % inputSize;

        destinationCupIterator = find(begin(cups), end(cups), destinationCupValue);
        if (destinationCupIterator != end(cups))
            return destinationCupIterator;
    }

    return destinationCupIterator;
}

vector<size_t> PlayCrabCupsPart1(const string input, const size_t inputSize, const size_t numberOfTurns)
{
    vector<size_t> cups;
    for (const char& cup : input)
        cups.push_back(cup - '1');

    const size_t maxElement = *max_element(begin(cups), end(cups));
    for (size_t index = maxElement + 1; index < inputSize; ++index)
        cups.push_back(index);

    size_t currentCupValue = cups[0];
    for (size_t turnNumber = 0; turnNumber < numberOfTurns; ++turnNumber)
    {
        if (turnNumber % 1'000 == 0)
            std::cout << "Processing turn " << turnNumber + 1<< " of " << numberOfTurns << endl;

        //std::cout << "-- move " << turnNumber + 1 << " --" << endl;
        //std::cout << "cups: " << GetDebugString(cups, currentCupValue) << endl;

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

        //std::cout << "pick up: " << 1 + removedCups[0] << ", " << 1 + removedCups[1] << ", " << 1 + removedCups[2] << endl;

        vector<size_t>::iterator destinationCupIterator = FindDestinationCup(cups, currentCupValue, inputSize);
        const size_t destinationValue = *destinationCupIterator;

        //std::cout << "destination: " << 1 + *destinationCupIterator << endl;

        cups.insert(destinationCupIterator + 1, begin(removedCups), end(removedCups));

        auto newCurrentCupIterator = find(begin(cups), end(cups), currentCupValue);
        size_t newCurrentCupIndex = (inputSize + distance(begin(cups), newCurrentCupIterator) + 1) % inputSize;
        currentCupValue = cups[newCurrentCupIndex];

        //std::cout << endl;
    }
    std::cout << endl;

    return cups;
}

string GetCupsStringForPart1(const vector<size_t>& cups)
{
    auto zeroIterator = find(begin(cups), end(cups), 0);
    vector<size_t> answer(zeroIterator + 1, end(cups));
    if (answer.size() < 8)
        answer.insert(end(answer), begin(cups), zeroIterator);

    string answerString;
    transform(
        begin(answer), end(answer),
        back_inserter(answerString), [](const size_t value) -> char
        {
            return '1' + value;
        });

    return answerString;
}

size_t GetAnswerForPart2(const vector<size_t>& cups)
{
    auto zeroIterator = find(begin(cups), end(cups), 0);
    zeroIterator++;
    if (zeroIterator == end(cups))
        zeroIterator = begin(cups);

    const size_t firstNumber = *zeroIterator + 1;

    zeroIterator++;
    if (zeroIterator == end(cups))
        zeroIterator = begin(cups);

    const size_t secondNumber = *zeroIterator + 1;

    return firstNumber * secondNumber;
}

int main()
{
    const string input("158937462");
    const string sample_input("389125467");

    const string& initialCupsString = input;

    vector<size_t> cupsPart1 = PlayCrabCupsPart1(initialCupsString, 9, 100);
    string stringForPart1Answer = GetCupsStringForPart1(cupsPart1);
    std::cout << "Crab cups (Part 1): " << stringForPart1Answer << endl;
    std::cout << endl;

    //vector<size_t> cupsPart2 = PlayCrabCupsPart1(initialCupsString, 1'000'000, 10'000'000);
    //std::cout << "Crab cups (Part 2): " << GetAnswerForPart2(cupsPart2) << endl;
    //std::cout << endl;

    //TestBuckets();

    //std::cout << "Crab cups (Part 2): " << PlayCrabCupsOptimized(initialCupsString, 9, 100) << endl;
    std::cout << "Crab cups (Part 2): " << PlayCrabCupsOptimized(initialCupsString, 1'000'000, 10'000'000) << endl;
    std::cout << endl;

    return 0;
}
