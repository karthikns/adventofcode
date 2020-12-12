#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Ship
{
public:
    Ship();

    void MoveEast(const int value);
    void MoveSouth(const int value);
    void MoveWest(const int value);
    void MoveNorth(const int value);
    void MoveForward(const int value);
    void TurnRight(const int degrees);
    void TurnLeft(const int degrees);

    int GetManhattanDistance() const;

    friend ostream& operator<<(ostream& stream, Ship& ship);

private:
    int m_horizontalPosition;
    int m_verticalPosition;
    int m_direction;
};

Ship::Ship() :
    m_horizontalPosition(0),
    m_verticalPosition(0),
    m_direction(0)
{
}

void Ship::MoveEast(const int value)
{
    m_horizontalPosition += value;
}

void Ship::MoveWest(const int value)
{
    MoveEast(-value);
}

void Ship::MoveNorth(const int value)
{
    m_verticalPosition += value;
}

void Ship::MoveSouth(const int value)
{
    MoveNorth(-value);
}

void Ship::TurnLeft(const int value)
{
    m_direction = (m_direction + (value % 360) + 360) % 360;;
}

void Ship::TurnRight(const int value)
{
    TurnLeft(-value);
}

int Ship::GetManhattanDistance() const
{
    return abs(m_horizontalPosition) + abs(m_verticalPosition);
}

ostream& operator<<(ostream& stream, Ship& ship)
{
    stream << "Horizontal Position: " << ship.m_horizontalPosition << endl;
    stream << "Vertical Position: " << ship.m_verticalPosition << endl;
    stream << "Direction: " << ship.m_direction << endl;
    return stream;
}

void Ship::MoveForward(const int value)
{
    if (m_direction % 90 != 0)
    {
        cout << "Error, cannot move in current direction: " << m_direction << endl;
    }

    switch ((m_direction % 360)/ 90)
    {
    case 0:
    {
        MoveEast(value);
        break;
    }
    case 1:
    {
        MoveNorth(value);
        break;
    }
    case 2:
    {
        MoveWest(value);
        break;
    }
    case 3:
    {
        MoveSouth(value);
        break;
    }
    }
}

struct ShipStep
{
    char instruction;
    int value;
};

vector<ShipStep> ParseInputFile(const string& inputFileName)
{
    vector<ShipStep> steps{};

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
            const char instruction = line[0];
            const int value = stoi(line.substr(1));
            //cout << "    " << instruction << ", " << value << endl;

            steps.push_back({instruction, value});
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return steps;
}

void MoveShip(Ship& ship, const vector<ShipStep>& steps)
{
    for (const ShipStep& shipStep : steps)
    {
        switch (shipStep.instruction)
        {
        case 'N':
            ship.MoveNorth(shipStep.value);
            break;
        case 'S':
            ship.MoveSouth(shipStep.value);
            break;
        case 'E':
            ship.MoveEast(shipStep.value);
            break;
        case 'W':
            ship.MoveWest(shipStep.value);
            break;
        case 'L':
            ship.TurnLeft(shipStep.value);
            break;
        case 'R':
            ship.TurnRight(shipStep.value);
            break;
        case 'F':
            ship.MoveForward(shipStep.value);
            break;
        default:
            cout << "Unrecognized instruction: " << shipStep.instruction << endl;
            break;
        }
    }
}

int main()
{
    const string inputFileName(R"(12_in.txt)");

    vector<ShipStep> steps = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Number of input steps: " << steps.size() << endl;
    cout << endl;

    Ship ship;
    MoveShip(ship, steps);
    cout << ship << endl;
    cout << endl;

    cout << "Manhattan Distance: " << ship.GetManhattanDistance() << endl;
    cout << endl;

    return 0;
}
