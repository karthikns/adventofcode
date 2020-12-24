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
            cout << "line: " << line << endl;

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

Point ProcessInstructionString(const string& instructions)
{
    stringstream stream(instructions);

    const size_t startXPosition = 100;
    const size_t startYPosition = 100;

    size_t xPosition = startXPosition;
    size_t yPosition = startYPosition;

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

int main()
{
    const string inputFileName(R"(24_processed_in.txt)");

    vector<string> instructions = ParseInputFile(inputFileName);
    cout << endl;

    cout << "All instructions size: " << instructions.size() << endl;
    cout << endl;

    cout << "Black tiles with face up (Part 1): " << ProcessAllInstructionsPart1(instructions);
    cout << endl;

    return 0;
}
