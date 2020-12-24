#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> ParseInputFile(const string& inputFileName)
{
    vector<string> instructions;

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
            instructions.emplace_back(line);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return instructions;
}

struct Point
{
    size_t x = 0;
    size_t y = 0;
};

bool PointComparator(const Point lhs, const Point rhs)
{
    if (lhs.y < rhs.y)
        return true;
    else if (lhs.y > rhs.y)
        return false;

    return lhs.x < rhs.x;
}

const size_t c_gridSize = 400;
const char c_white = '.';
const char c_black = '#';

vector<string> GetNewFloor()
{
    const string str(c_gridSize, c_white);
    vector<string> floor(c_gridSize, str);
    return floor;
}

Point ProcessInstructionString(const string& instructions)
{
    stringstream stream(instructions);

    const size_t startXPosition = c_gridSize / 2;
    const size_t startYPosition = c_gridSize / 2;

    size_t xPosition = startXPosition;
    size_t yPosition = startYPosition;

    size_t maxx = 0;
    size_t maxy = 0;

    while (!stream.eof())
    {
        string singleInstruction;
        stream >> singleInstruction;

        if (singleInstruction == "e")
            xPosition += 2;
        else if (singleInstruction == "w")
            xPosition -= 2;
        else if (singleInstruction == "ne")
        {
            xPosition++;
            yPosition--;
        }
        else if (singleInstruction == "se")
        {
            xPosition++;
            yPosition++;
        }
        else if (singleInstruction == "nw")
        {
            xPosition--;
            yPosition--;
        }
        else if (singleInstruction == "sw")
        {
            xPosition--;
            yPosition++;
        }
        else
        {
            break;
        }

        maxx = max(maxx, xPosition);
        maxy = max(maxy, yPosition);

        //cout << xPosition << ", " << yPosition << endl;
    }

    return { xPosition, yPosition };
}

size_t ProcessAllInstructionsPart1(vector<string> allInstructions)
{
    bool(*pointComparatorFunction)(Point, Point) = PointComparator;
    map<Point, char, bool(*)(Point, Point)> blackTilesCoordinates(pointComparatorFunction);

    for (const string& instructions : allInstructions)
    {
        Point finalPosition = ProcessInstructionString(instructions);

        bool(*fn_pt)(Point, Point) = PointComparator;
        map<Point, char>::iterator findIterator = blackTilesCoordinates.find(finalPosition);
        if (findIterator != end(blackTilesCoordinates))
            blackTilesCoordinates.erase(findIterator);
        else
            blackTilesCoordinates[finalPosition] = 'b';
    }

    return blackTilesCoordinates.size();
}

size_t CountBlackTiles(const vector<string>& floor)
{
    size_t countOfBlackTiles = 0;
    for (const string& floorLine : floor)
    {
        //cout << floorLine << endl;
        countOfBlackTiles += count(begin(floorLine), end(floorLine), c_black);
    }

    return countOfBlackTiles;
}

size_t GetCountOfAdjacentBlackTiles(const size_t yIndex, const size_t xIndex, const vector<string> floor)
{
    // x = first
    // y = second
    vector<pair<int ,int>> adjacentTiles = {
        {  2,  0 },
        {  1,  1 },
        { -1,  1 },
        { -2,  0 },
        { -1, -1 },
        {  1, -1 },
    };

    size_t countOfAdjacentBlackTiles = 0;
    for (const pair<int, int> adjacentTile : adjacentTiles)
        if (floor[yIndex + adjacentTile.second][xIndex + adjacentTile.first] == c_black)
            ++countOfAdjacentBlackTiles;

    return countOfAdjacentBlackTiles;
}

// Incorrect:
// 1690 - too low
// 3459 - too low
size_t ProcessAllInstructionsPart2(vector<string> allInstructions)
{
    vector<string> floor = GetNewFloor();
    for (const string& instructions : allInstructions)
    {
        Point finalPosition = ProcessInstructionString(instructions);

        if (floor[finalPosition.y][finalPosition.x] == c_black)
            floor[finalPosition.y][finalPosition.x] = c_white;
        else if (floor[finalPosition.y][finalPosition.x] == c_white)
            floor[finalPosition.y][finalPosition.x] = c_black;
    }

    vector<string> newFloor = GetNewFloor();
    for (size_t index = 0; index < 100; ++index)
    {
        size_t countOfBlackTiles = 0;
        for (size_t yIndex = 2; yIndex < c_gridSize - 2; yIndex++)
        {
            for (size_t xIndex = 2 + (yIndex % 2); xIndex < c_gridSize - 2; xIndex += 2)
            {
                size_t numberOfAdjacentBlackTiles = GetCountOfAdjacentBlackTiles(yIndex, xIndex, floor);
                newFloor[yIndex][xIndex] = floor[yIndex][xIndex];

                if (floor[yIndex][xIndex] == c_black && (numberOfAdjacentBlackTiles == 0 || numberOfAdjacentBlackTiles > 2))
                    newFloor[yIndex][xIndex] = c_white;
                else if (floor[yIndex][xIndex] == c_white && numberOfAdjacentBlackTiles == 2)
                    newFloor[yIndex][xIndex] = c_black;

                if (newFloor[yIndex][xIndex] == c_black)
                    ++countOfBlackTiles;
            }
        }

        vector<string> temp = move(floor);
        floor = move(newFloor);
        newFloor = move(temp);

        cout << "Day " << index + 1 << ": " << countOfBlackTiles << endl;
    }
    cout << endl;

    return CountBlackTiles(floor);
}


int main()
{
    const string inputFileName(R"(24_processed_in.txt)");
    //const string inputFileName(R"(24_sample_processed_in.txt)");

    vector<string> instructions = ParseInputFile(inputFileName);
    cout << endl;

    cout << "All instructions size: " << instructions.size() << endl;
    cout << endl;

    cout << "Black tiles with face up (Part 1): " << ProcessAllInstructionsPart1(instructions) << endl;
    cout << endl;

    cout << "Black tiles with face up (Part 2): " << ProcessAllInstructionsPart2(instructions) << endl;
    cout << endl;

    return 0;
}
