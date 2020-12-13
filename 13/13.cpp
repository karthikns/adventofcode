#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Input
{
    uint64_t arrivalTime;
    vector<uint32_t> busIds;
};

Input ParseInputFile(const string& inputFileName)
{
    Input parseResult{};

    try
    {
        ifstream inputFile(inputFileName, ios::in);

        if (inputFile.fail())
        {
            cout << "File Open Error" << endl;
            exit(-1);
        }

        string arrivalTimeString;
        getline(inputFile, arrivalTimeString);
        cout << arrivalTimeString << endl;

        parseResult.arrivalTime = stoull(arrivalTimeString);

        while (inputFile.good() && !inputFile.eof())
        {
            string busIdString;
            getline(inputFile, busIdString, ',');
            cout << "busId: " << busIdString << endl;

            if (busIdString.empty())
                break;

            // Enter parsing code here
            uint32_t busId = (busIdString == "x") ? 0 : stoul(busIdString);
            parseResult.busIds.emplace_back(busId);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

uint64_t BusIdOfFastestBusMultipliedByWaitTime(const Input& input)
{
    uint64_t quickestBusId = 0;
    uint64_t quickestWaitTime = UINT32_MAX;

    for (const uint32_t busId : input.busIds)
    {
        if (!busId)
            continue;

        const uint64_t lastDepartureTime = (input.arrivalTime / busId) * busId;
        const uint64_t nextDepartureTime = lastDepartureTime + busId;
        const uint64_t waitTime = nextDepartureTime - input.arrivalTime;

        if (waitTime < quickestWaitTime)
        {
            quickestBusId = busId;
            quickestWaitTime = waitTime;
        }
    }

    return quickestBusId * quickestWaitTime;
}

struct BusIdWithOffset
{
    uint64_t busId;
    uint64_t offset;
};

struct ModEquation
{
    int64_t divisor;
    int64_t remainder;
};

ModEquation ComputeEquations(const vector<ModEquation>& equations)
{
    vector<ModEquation> equationsToProcess = equations;
    
    if (equationsToProcess.empty())
        return {0, 0};

    sort(begin(equationsToProcess), end(equationsToProcess), [](auto first, auto second) -> bool { return first.divisor < second.divisor; });

    ModEquation biggestEquation = equationsToProcess.back();
    equationsToProcess.pop_back();

    while (!equationsToProcess.empty())
    {
        ModEquation nEquation = biggestEquation;
        ModEquation mEquation = equationsToProcess.back();
        equationsToProcess.pop_back();

        int64_t nRemainderMinusmRemainder = nEquation.remainder - mEquation.remainder;
        int64_t nCandidate = 0;
        for (; nCandidate < nEquation.divisor; ++nCandidate)
        {
            int64_t remainder = (nRemainderMinusmRemainder + nCandidate * nEquation.divisor) % mEquation.divisor;
            if (remainder == 0)
            {
                break;
            }
        }

        int64_t netRemainder = nEquation.remainder + nCandidate * nEquation.divisor;
        int64_t netDivisor = nEquation.divisor * mEquation.divisor;

        biggestEquation = { netDivisor , netRemainder};
    }

    return biggestEquation;
}

// Incorrect:
// 100000000385544
// 966184982077497 - too high
// 31953756881492
// 928869597984475
int64_t ComputePuzzleAnswer(const Input& input)
{
    vector<ModEquation> equations;
    for (size_t index = 0; index < input.busIds.size(); ++index)
        if (input.busIds[index] != 0)
            equations.push_back({ input.busIds[index], -(int64_t)index });

    return ComputeEquations(equations).remainder;
}

int main()
{
    const string inputFileName(R"(13_in.txt)");
    //const string inputFileName(R"(13_sample_in.txt)");

    Input input = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Bus Ids Size: " << input.busIds.size() << endl;
    cout << endl;

    cout << "Bus Id Of Fastest Bus Multiplied By Wait Time: " << BusIdOfFastestBusMultipliedByWaitTime(input) << endl;
    cout << endl;

    cout << "Puzzle Answer: " << ComputePuzzleAnswer(input) << endl;
    cout << endl;

    return 0;
}
