#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

uint64_t CountTreesHit(const vector<string>& lineVector, const size_t numberOfXSteps, const size_t numberOfYSteps)
{
    const size_t numberOfCharacterPerLine = lineVector[0].length();
    uint64_t numberOfTreesHit = 0;
    size_t x = 0;

    for(size_t y = 0; y < lineVector.size(); y += numberOfYSteps)
    //for (const string& line : lineVector)
    {
        //cout << x << " ";
        if (lineVector[y][x] == '#')
        {
            //cout << "hit" << endl;
            ++numberOfTreesHit;
        }
        else
        {
            //cout << "miss" << endl;
        }

        x = (x + numberOfXSteps) % numberOfCharacterPerLine;
    }

    return numberOfTreesHit;
}

int main()
{
    const string inputFileName("3_in.txt");
    //const string inputFileName("3_sample_in.txt");

    vector<string> lineVector;
    try
    {
        fstream fs(inputFileName);

        while (fs.good())
        {
            char buffer[200] = {};
            fs.getline(buffer, sizeof(buffer) / sizeof(buffer[0]) - 1);

            if (!buffer[0])
            {
                break;
            }

            lineVector.emplace_back(buffer);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        return -1;
    }

    if (lineVector.empty())
    {
        cout << "No entries" << endl;
        return -1;
    }

    cout << CountTreesHit(lineVector, 1, 1) << endl;
    cout << CountTreesHit(lineVector, 3, 1) << endl;
    cout << CountTreesHit(lineVector, 5, 1) << endl;
    cout << CountTreesHit(lineVector, 7, 1) << endl;
    cout << CountTreesHit(lineVector, 1, 2) << endl;

    uint64_t multiplyResult =
        CountTreesHit(lineVector, 1, 1) *
        CountTreesHit(lineVector, 3, 1) *
        CountTreesHit(lineVector, 5, 1) *
        CountTreesHit(lineVector, 7, 1) *
        CountTreesHit(lineVector, 1, 2);

    cout << endl;
    cout << multiplyResult << endl;

    return 0;
}
