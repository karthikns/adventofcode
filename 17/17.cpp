#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const size_t MaxDimensionSize = 50;

class GridPart1
{
public:
    GridPart1(const vector<string>& initialState);
    void ComputeNextState();
    size_t GetActiveCubeCount() const;
    void DisplayGrid();

private:
    size_t GetCountOfActiveNeighbors(const size_t z, const size_t y, const size_t x);

    bool grid[MaxDimensionSize][MaxDimensionSize][MaxDimensionSize] = {};
    size_t countOfNeighbors[MaxDimensionSize][MaxDimensionSize][MaxDimensionSize] = {};
};

GridPart1::GridPart1(const vector<string>& initialState)
{
    const size_t halfDimensionSize = MaxDimensionSize / 2;
    const size_t yStart = (MaxDimensionSize - initialState.size()) / 2;
    const size_t xStart = (MaxDimensionSize - initialState[0].size()) / 2;

    for (size_t y = 0; y < initialState.size(); ++y)
        for (size_t x = 0; x < initialState[y].size(); ++x)
            if (initialState[y][x] == '#')
                grid[halfDimensionSize][yStart + y][xStart + x] = 1;
}

void GridPart1::ComputeNextState()
{
    //size_t countOfNeighbors[MaxDimensionSize][MaxDimensionSize][MaxDimensionSize] = {};
    memset(countOfNeighbors, 0, sizeof(countOfNeighbors));

    for (size_t z = 1; z < MaxDimensionSize - 1; ++z)
        for (size_t y = 1; y < MaxDimensionSize - 1; ++y)
            for (size_t x = 1; x < MaxDimensionSize - 1; ++x)
                countOfNeighbors[z][y][x] = GetCountOfActiveNeighbors(z, y, x);

    //bool oldGrid[MaxDimensionSize][MaxDimensionSize][MaxDimensionSize] = {};
    //memcpy(oldGrid, grid, sizeof(oldGrid));

    for (size_t z = 0; z < MaxDimensionSize; ++z)
        for (size_t y = 0; y < MaxDimensionSize; ++y)
            for (size_t x = 0; x < MaxDimensionSize; ++x)
            {
                const size_t neighborCount = countOfNeighbors[z][y][x];
                if (grid[z][y][x] == false && neighborCount == 3)
                    grid[z][y][x] = true;
                else if(grid[z][y][x] == true && (neighborCount < 2 || neighborCount > 3))
                    grid[z][y][x] = false;
            }
}

size_t GridPart1::GetActiveCubeCount() const
{
    size_t activeCubeCount = 0;
    for (int z = 0; z < MaxDimensionSize; ++z)
        for (int y = 0; y < MaxDimensionSize; ++y)
            for (int x = 0; x < MaxDimensionSize; ++x)
                activeCubeCount += grid[z][y][x];

    return activeCubeCount;
}

void GridPart1::DisplayGrid()
{
    for (int z = 0; z < MaxDimensionSize; ++z)
    {
        cout << z << endl;
        for (int y = 0; y < MaxDimensionSize; ++y)
        {
            for (int x = 0; x < MaxDimensionSize; ++x)
            {
                char characterToDisplay = '.';
                if (grid[z][y][x])
                    characterToDisplay = '#';

                cout << characterToDisplay << " ";
            }

            cout << endl;
        }

        cout << endl;
    }
}

size_t GridPart1::GetCountOfActiveNeighbors(const size_t z, const size_t y, const size_t x)
{
    size_t countOfActiveNeighbors = 0;
    for (int k = -1; k <= 1; ++k)
        for (int j = -1; j <= 1; ++j)
            for (int i = -1; i <= 1; ++i)
                countOfActiveNeighbors += grid[z + k][y + j][x + i];

    countOfActiveNeighbors -= grid[z][y][x];
    return countOfActiveNeighbors;
}


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

            parseResult.emplace_back(line);
            // Enter parsing code here
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

int main()
{
    const string inputFileName(R"(17_in.txt)");
    //const string inputFileName(R"(17_sample_in.txt)");

    vector<string> parseResult = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Parsed lines: " << parseResult.size() << endl;
    cout << endl;

    unique_ptr<GridPart1> grid = make_unique<GridPart1>(parseResult);

    for(size_t index = 0; index < 6; ++index)
        grid->ComputeNextState();

    //grid->DisplayGrid();

    cout << "Active cube count: " << grid->GetActiveCubeCount() << endl;

    return 0;
}
