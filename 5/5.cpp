#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

uint16_t HighestSeatId(const string& inputFileName, vector<uint16_t>& list)
{
    uint16_t highestSeatId = 0;

    try
    {
        fstream inputFile(inputFileName);

        while (inputFile.good())
        {
            string seatString;
            inputFile >> seatString;

            if (seatString.empty())
            {
                break;
            }

            uint16_t seatId = 0;
            for (char c : seatString)
            {
                uint16_t bit = (c == 'B' || c == 'R') ? 1 : 0;
                seatId = (seatId << 1) | bit;
            }

            list.emplace_back(seatId);

            if (seatId > highestSeatId)
                highestSeatId = seatId;
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return highestSeatId;
}

int main()
{
    const string inputFileName(R"(5_in.txt)");

    vector<uint16_t> seatIds;
    cout << HighestSeatId(inputFileName, seatIds) << endl;

    sort(begin(seatIds), end(seatIds));

    uint16_t missingSeatId = 0;
    for (size_t i = 1; i < seatIds.size(); ++i)
    {
        //cout << seatIds[i] << endl;

        if (seatIds[i] != seatIds[i - 1] + 1)
        {
            missingSeatId  = seatIds[i] - 1;
            break;
        }
    }

    cout << missingSeatId << endl;

    return 0;
}
