#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

using SeatChangerFunction = void(char& newComputedSeat, const vector<string>& oldSeatingGrid, const int yIndex, const int xIndex);

vector<string> ParseInputFile(const string& inputFileName)
{
    vector<string> parseResult{};

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

            parseResult.emplace_back('.' + line + '.');
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

bool AreSeatingGridsSame(const vector<string>& first, const vector<string>& second)
{
    if (first.size() != second.size())
        return false;

    for (size_t yIndex = 0; yIndex < first.size(); ++yIndex)
        if (first[yIndex] != second[yIndex])
            return false;

    return true;
}

void SeatChangerPart1(char& newComputedSeat, const vector<string>& oldSeatingGrid, const int yIndex, const int xIndex)
{
    const int maxYSize = static_cast<int>(oldSeatingGrid.size());
    const int maxXSize = static_cast<int>(oldSeatingGrid[0].size());

    if (oldSeatingGrid[yIndex][xIndex] == '.')
        return;

    size_t numberOfAdjacentOccupiedSeats = 0;
    for (int yIncrement = -1; yIncrement <= 1; ++yIncrement)
    {
        for (int xIncrement = -1; xIncrement <= 1; ++xIncrement)
        {
            if (xIncrement == 0 && yIncrement == 0)
            {
                continue;
            }

            const size_t yIndexToCheck = yIndex + yIncrement;
            const size_t xIndexToCheck = xIndex + xIncrement;
            const char& oldSeat = oldSeatingGrid[yIndexToCheck][xIndexToCheck];
            if (oldSeat == '#')
            {
                ++numberOfAdjacentOccupiedSeats;
            }
        }
    }

    if (numberOfAdjacentOccupiedSeats == 0)
    {
        newComputedSeat = '#';
        return;
    }

    if (numberOfAdjacentOccupiedSeats >= 4)
    {
        newComputedSeat = 'L';
        return;
    }

    newComputedSeat = oldSeatingGrid[yIndex][xIndex];
}

char FindFirstSeatInDirection(const vector<string>& seatingGrid, const int yIndex, const int xIndex, const int yDirection, const int xDirection)
{
    int xPosToCheck = xIndex + xDirection;
    int yPosToCheck = yIndex + yDirection;

    const int maxY = static_cast<int>(seatingGrid.size());
    const int maxX = static_cast<int>(seatingGrid[0].size());

    while (true)
    {
        if (xPosToCheck < 0 || xPosToCheck >= maxX)
            return '.';

        if (yPosToCheck < 0 || yPosToCheck >= maxY)
            return '.';

        const char seatStatusAtPos = seatingGrid[yPosToCheck][xPosToCheck];
        if (seatStatusAtPos == 'L' || seatStatusAtPos == '#')
            return seatStatusAtPos;

        xPosToCheck += xDirection;
        yPosToCheck += yDirection;
    }

    return '.';
}

void SeatChangerPart2(char& newComputedSeat, const vector<string>& oldSeatingGrid, const int yIndex, const int xIndex)
{
    const int maxYSize = static_cast<int>(oldSeatingGrid.size());
    const int maxXSize = static_cast<int>(oldSeatingGrid[0].size());

    if (oldSeatingGrid[yIndex][xIndex] == '.')
        return;

    size_t numberOfAdjacentOccupiedSeats = 0;
    for (int yIncrement = -1; yIncrement <= 1; ++yIncrement)
    {
        for (int xIncrement = -1; xIncrement <= 1; ++xIncrement)
        {
            if (xIncrement == 0 && yIncrement == 0)
            {
                continue;
            }

            char oldSeatInDirection = FindFirstSeatInDirection(oldSeatingGrid, yIndex, xIndex, yIncrement, xIncrement);

            //const size_t yIndexToCheck = yIndex + yIncrement;
            //const size_t xIndexToCheck = xIndex + xIncrement;
            //const char& oldSeat = oldSeatingGrid[yIndexToCheck][xIndexToCheck];
            if (oldSeatInDirection == '#')
            {
                ++numberOfAdjacentOccupiedSeats;
            }
        }
    }

    if (numberOfAdjacentOccupiedSeats == 0)
    {
        newComputedSeat = '#';
        return;
    }

    if (numberOfAdjacentOccupiedSeats >= 5)
    {
        newComputedSeat = 'L';
        return;
    }

    newComputedSeat = oldSeatingGrid[yIndex][xIndex];
}

void PrintSeatingGrid(const vector<string>& seatingGrid)
{
    for (const string& row : seatingGrid)
    {
        cout << row << endl;
    }
}

size_t CountNumberOfOccupiedSeats(const vector<string>& seatingGrid)
{
    size_t numberOfOccupiedSeats = 0;
    for (const string& row : seatingGrid)
    {
        numberOfOccupiedSeats += count(begin(row), end(row), '#');
    }

    return numberOfOccupiedSeats;
}

size_t NumberOfOccupiedSeatsAfterStablization(vector<string> seatingGrid, SeatChangerFunction seatChangerFunction)
{
    vector<string> newSeatingGrid;

    while(true) {
        newSeatingGrid = seatingGrid;

        for (int yIndex = 1; yIndex < seatingGrid.size() - 1; ++yIndex)
        {
            for (int xIndex = 1; xIndex < seatingGrid[0].size() - 1; ++xIndex)
            {
                seatChangerFunction(newSeatingGrid[yIndex][xIndex], seatingGrid, yIndex, xIndex);
            }
        }

        //PrintSeatingGrid(newSeatingGrid);
        //cout << endl << endl;

        if (AreSeatingGridsSame(newSeatingGrid, seatingGrid))
            break;


        seatingGrid = newSeatingGrid;
    }

    return CountNumberOfOccupiedSeats(seatingGrid);
}

int main()
{
    const string inputFileName(R"(11_in.txt)");
    //const string inputFileName(R"(11_sample_in.txt)");

    vector<string> seatingGrid = ParseInputFile(inputFileName);
    cout << endl;

    string sentinel(seatingGrid[0].size(), '.');
    seatingGrid.push_back(sentinel);
    seatingGrid.insert(begin(seatingGrid), sentinel);

    cout << "Parse result: " << seatingGrid.size() << endl;
    cout << endl;

    auto seatChanger = SeatChangerPart1;
    cout << "Number occupied seats after stablization (part 1): " << NumberOfOccupiedSeatsAfterStablization(seatingGrid, seatChanger) << endl;

    seatChanger = SeatChangerPart2;
    cout << "Number occupied seats after stablization (part 2): " << NumberOfOccupiedSeatsAfterStablization(seatingGrid, seatChanger) << endl;

    return 0;
}
