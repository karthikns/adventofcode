#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

size_t c_numberOfSides = 4;

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

void PrintGrid(const vector<string>& grid)
{
    for (const string& row : grid)
    {
        for (char c : row)
        {
            //if (c == '.')
            //    c = ' ';

            cout << c;
            //cout << " ";
        }

        cout << endl;
    }
}


void RotateGridClockwise(vector<string>& grid)
{
    const size_t c_gridSize = grid.size();
    const size_t c_numberOfLayers = c_gridSize / 2;

    for (size_t l = 0; l < c_numberOfLayers; ++l)
    {
        const size_t m = c_gridSize - 1 - l;
        for (size_t p = l; p < m; ++p)
        {
            const size_t q = c_gridSize - 1 - p;

            char temp = grid[l][p];
            grid[l][p] = grid[q][l];
            grid[q][l] = grid[m][q];
            grid[m][q] = grid[p][m];
            grid[p][m] = temp;
        }
    }
}

void FlipGridAlongYAxis(vector<string>& grid)
{
    for (string& line : grid)
        reverse(begin(line), end(line));
}

struct AdjoiningTile
{
    size_t tileId = 0;
    size_t side = 0;
};

class Tile
{
public:
    Tile(const size_t tileId, const vector<string>& tileContents) :
        tileId(tileId),
        tileContents(tileContents),
        adjoiningTiles(4),
        edges(4)
    {
        const size_t tileSize = tileContents.size();
        edges[0] = tileContents[0];
        edges[2] = tileContents[tileSize - 1];
        reverse(begin(edges[2]), end(edges[2]));

        for (size_t index = 0; index < tileContents.size(); ++index)
        {
            edges[3] = tileContents[index][0] + edges[3];
            edges[1] = edges[1] + tileContents[index][tileSize - 1];
        }
    }

    void ProcessTileNeighbor(Tile& tile)
    {
        for(size_t tileSideIndex = 0; tileSideIndex < c_numberOfSides; ++tileSideIndex)
            for(size_t incomingTileSideIndex = 0; incomingTileSideIndex < c_numberOfSides; ++incomingTileSideIndex)
                if (DoSidesMatch(edges[tileSideIndex], tile.edges[incomingTileSideIndex]))
                {
                    adjoiningTiles[tileSideIndex].tileId = tile.tileId;
                    adjoiningTiles[tileSideIndex].side = incomingTileSideIndex;

                    tile.adjoiningTiles[incomingTileSideIndex].tileId = tileId;
                    tile.adjoiningTiles[incomingTileSideIndex].side = tileSideIndex;
                }
    }

    bool DoesTileHaveTwoNeighbors() const
    {
        return GetNumberOfAdjoiningTiles() == 2;
    }

    size_t GetNumberOfAdjoiningTiles() const
    {
        size_t numberOfAdjoiningTiles = 0;
        for (size_t index = 0; index < c_numberOfSides; ++index)
            if (adjoiningTiles[index].tileId == 0)
                ++numberOfAdjoiningTiles;

        return numberOfAdjoiningTiles;
    }

    size_t GetTileId() const
    {
        return tileId;
    }

    size_t GetNeighborTileId(Direction direction) const
    {
        return adjoiningTiles[direction].tileId;
    }

    void PrintTile()
    {
        PrintGrid(tileContents);
    }

    void LogTile()
    {
        cout << "Tile Id: " << GetTileId() << endl;

        PrintTile();
        cout << endl;

        const vector<string> directionStrings = { "UP   ", "RIGHT", "DOWN ", "LEFT " };

        for (size_t directionIndex = 0; directionIndex < directionStrings.size(); ++directionIndex)
            cout << directionStrings[directionIndex] << ": " << edges[directionIndex] << endl;
        cout << endl;

        for (size_t directionIndex = 0; directionIndex < directionStrings.size(); ++directionIndex)
        {
            cout << directionStrings[directionIndex] << ": ";
            cout << adjoiningTiles[directionIndex].tileId << " ";
            cout << directionStrings[adjoiningTiles[directionIndex].side] << endl;
        }
    }

    string GetEdge(size_t direction) const
    {
        return edges[direction];
    }

    void RotateClockwise()
    {
        // Flip edges
        {
            string temp = edges[UP];
            edges[UP] = edges[LEFT];
            edges[LEFT] = edges[DOWN];
            edges[DOWN] = edges[RIGHT];
            edges[RIGHT] = temp;
        }

        // Flip adjoining tiles
        {
            AdjoiningTile temp = adjoiningTiles[UP];
            adjoiningTiles[UP] = adjoiningTiles[LEFT];
            adjoiningTiles[LEFT] = adjoiningTiles[DOWN];
            adjoiningTiles[DOWN] = adjoiningTiles[RIGHT];
            adjoiningTiles[RIGHT] = temp;
        }

        // Flip tile contents
        RotateGridClockwise(tileContents);
    }

    void FlipOnYAxis()
    {
        // Flip edges
        reverse(edges[Direction::UP].begin(), edges[Direction::UP].end());
        reverse(edges[Direction::DOWN].begin(), edges[Direction::DOWN].end());
        swap(edges[Direction::LEFT], edges[Direction::RIGHT]);
        reverse(edges[Direction::LEFT].begin(), edges[Direction::LEFT].end());
        reverse(edges[Direction::RIGHT].begin(), edges[Direction::RIGHT].end());

        // Flip adjoining tiles
        swap(adjoiningTiles[LEFT], adjoiningTiles[RIGHT]);

        // Flip tile contents
        for (size_t y = 0; y < tileContents.size(); ++y)
            reverse(tileContents[y].begin(), tileContents[y].end());
    }

    void FlipOnXAxis()
    {
        FlipOnYAxis();
        RotateClockwise();
        RotateClockwise();
    }

    void OrientToMatchTile(const Direction direction, const Tile& otherTile)
    {
        // assert has tile id

        while (adjoiningTiles[direction].tileId != otherTile.GetTileId())
            RotateClockwise();

        const size_t otherTileDirection = (direction + 2) % 4;
        if (GetEdge(direction) == otherTile.GetEdge(otherTileDirection))
            FlipOnYAxis();

        while (adjoiningTiles[direction].tileId != otherTile.GetTileId())
            RotateClockwise();
    }

    void SetCornerTileToTopLeft()
    {
        // assert DoesTileHaveTwoNeighbors()

        while (!(adjoiningTiles[LEFT].tileId == 0 && adjoiningTiles[UP].tileId == 0))
            RotateClockwise();
    }

    bool HasTileNeighbor(const size_t tileId) const
    {
        for (size_t index = 0; index < c_numberOfSides; ++index)
            if (adjoiningTiles[index].tileId == tileId)
                return true;

        return false;
    }

    string GetRowContents(size_t row) const
    {
        return tileContents[row];
    }

    string GetRowContentsWithoutCrust(size_t row) const
    {
        const size_t lengthToCopy = tileContents[row + 1].size() - 2;
        return tileContents[row + 1].substr(1, lengthToCopy);
    }

    size_t GetTileSize() const
    {
        return tileContents.size();
    }

    size_t GetTileSizeWithoutCrust() const
    {
        return GetTileSize() - 2;
    }

private:
    bool DoSidesMatch(const string& sideOne, const string& sideTwo)
    {
        if (sideOne == sideTwo)
            return true;

        string sideTwoReversed = sideTwo;
        reverse(begin(sideTwoReversed), end(sideTwoReversed));
        return sideOne == sideTwoReversed;
    }

    size_t tileId = 0;
    vector<AdjoiningTile> adjoiningTiles;
    vector<string> edges;
    vector<string> tileContents;

};

shared_ptr<Tile> ParseTile(const size_t tileId, ifstream& inputFile)
{
    vector<string> tileContents;

    while (inputFile.good() && !inputFile.eof())
    {
        string line;
        getline(inputFile, line);
        //cout << "tile: " << line << endl;

        if (line.empty())
            break;

        tileContents.emplace_back(line);
    }


    return make_shared<Tile>(tileId, tileContents);
}

vector<shared_ptr<Tile>> ParseInputFile(const string& inputFileName)
{
    vector<shared_ptr<Tile>> tiles;

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

            if (line.find("Tile") != string::npos)
            {
                shared_ptr<Tile> tile = ParseTile(stoul(line.substr(5, 4)), inputFile);
                tiles.emplace_back(tile);
            }
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return tiles;
}

void ProcessTileNeighbors(vector<shared_ptr<Tile>>& tiles)
{
    for (size_t outerIndex = 0; outerIndex < tiles.size() - 1; ++outerIndex)
        for (size_t innerIndex = outerIndex + 1; innerIndex < tiles.size(); ++innerIndex)
            tiles[outerIndex]->ProcessTileNeighbor(*tiles[innerIndex]);
}

uint64_t GetCornerTileIdsMultiplied(const vector<shared_ptr<Tile>>& tiles)
{
    uint64_t cornerTileIdsMultiplied = 1;

    for (const shared_ptr<Tile>& tile : tiles)
        if (tile->DoesTileHaveTwoNeighbors())
            cornerTileIdsMultiplied *= tile->GetTileId();

    return cornerTileIdsMultiplied;
}

uint64_t GetACornerTileIndex(const vector<shared_ptr<Tile>>& tiles)
{
    for (size_t index = 0; index < tiles.size(); ++index)
        if (tiles[index]->DoesTileHaveTwoNeighbors())
            return index;

    return 1000;
}

void EraseTileIfExists(const size_t tileId, vector<shared_ptr<Tile>>& tiles)
{
    auto it = find_if(begin(tiles), end(tiles), [tileId](auto tile) -> bool { return tile->GetTileId() == tileId; });

    if (it == tiles.end())
        return;

    tiles.erase(it);
}

vector<string> GetAssembledPuzzle(const vector<shared_ptr<Tile>>& tiles)
{
    const size_t c_gridSize = static_cast<size_t>(sqrt(tiles.size()));

    vector<shared_ptr<Tile>> tilesToBeProcessed = tiles;

    map<size_t, shared_ptr<Tile>> tileIdToTileMap;
    for (const shared_ptr<Tile>& tile : tiles)
        tileIdToTileMap[tile->GetTileId()] = tile;

    const size_t cornerTileIndex = GetACornerTileIndex(tilesToBeProcessed);
    const shared_ptr<Tile> cornerTile = tilesToBeProcessed[cornerTileIndex];
    tilesToBeProcessed.erase(tilesToBeProcessed.begin() + cornerTileIndex);

    cornerTile->FlipOnYAxis(); // NOT NEEDED
    cornerTile->SetCornerTileToTopLeft();

    const size_t rightOfCornerTileId = cornerTile->GetNeighborTileId(RIGHT);
    EraseTileIfExists(rightOfCornerTileId, tilesToBeProcessed);

    tileIdToTileMap[rightOfCornerTileId]->OrientToMatchTile(LEFT, *cornerTile);

    size_t assembledPuzzle[15][15] = {};
    assembledPuzzle[1][1] = cornerTile->GetTileId();
    assembledPuzzle[1][2] = rightOfCornerTileId;

    for (size_t y = 1; y <= c_gridSize; ++y)
    {
        for (size_t x = 1; x <= c_gridSize; ++x)
        {
            if (y == 1 && (x == 1 || x == 2))
                continue;

            const size_t topNeighborTileId = assembledPuzzle[y - 1][x];
            const size_t leftNeighborTileId = assembledPuzzle[y][x - 1];

            auto findIterator = find_if(
                begin(tilesToBeProcessed),
                end(tilesToBeProcessed),
                [topNeighborTileId, leftNeighborTileId](auto tile) -> bool {
                    return tile->HasTileNeighbor(topNeighborTileId) && tile->HasTileNeighbor(leftNeighborTileId);
                });

            // assert findIndex is not end
            shared_ptr<Tile> tile = *findIterator;
            tilesToBeProcessed.erase(findIterator);

            assembledPuzzle[y][x] = tile->GetTileId();

            if (x == 1)
            {
                const size_t otherTileId = assembledPuzzle[y - 1][x];
                tile->OrientToMatchTile(UP, *tileIdToTileMap[otherTileId]);
            }
            else
            {
                const size_t otherTileId = assembledPuzzle[y][x - 1];
                tile->OrientToMatchTile(LEFT, *tileIdToTileMap[otherTileId]);
            }
        }
    }

    // Debug
    //{
    //    const size_t c_tileSize = (*begin(tiles))->GetTileSize();
    //    for (size_t y = 1; y <= c_gridSize; ++y)
    //    {
    //        for (size_t row = 0; row < c_tileSize; ++row)
    //        {
    //            for (size_t x = 1; x <= c_gridSize; ++x)
    //                cout << tileIdToTileMap[assembledPuzzle[y][x]]->GetRowContents(row) << " ";

    //            cout << endl;
    //        }

    //        cout << endl;
    //    }
    //}

    // Combine to one grid
    const size_t c_tileSizeWithoutCrust = (*begin(tiles))->GetTileSizeWithoutCrust();
    vector<string> gridWithoutCrust(c_gridSize * c_tileSizeWithoutCrust);

    for (size_t y = 1; y <= c_gridSize; ++y)
        for (size_t row = 0; row < c_tileSizeWithoutCrust; ++row)
            for (size_t x = 1; x <= c_gridSize; ++x)
            {
                const size_t rowToInsert = (y - 1) * c_tileSizeWithoutCrust + row;
                gridWithoutCrust[rowToInsert] += tileIdToTileMap[assembledPuzzle[y][x]]->GetRowContentsWithoutCrust(row);
            }

    return gridWithoutCrust;
}

vector<vector<bool>> GetPuzzleMask(const vector<string>& puzzle)
{
    vector<vector<bool>> mask(puzzle.size(), vector<bool>(puzzle[0].size(), false));

    for (size_t y = 0; y < puzzle.size(); ++y)
        for (size_t x = 0; x < puzzle[y].size(); ++x)
            mask[y][x] = puzzle[y][x] == '#';

    return mask;
}

size_t CountTrueInPuzzleMask(const vector<vector<bool>>& mask)
{
    size_t countOfTrues = 0;

    for (const vector<bool>& row : mask)
        countOfTrues += count(begin(row), end(row), true);

    return countOfTrues;
}

struct Point
{
    size_t x = 0;
    size_t y = 0;
};

class SeaMonster
{
public:
    SeaMonster()
    {
        vector<string> seaMonsterString = {
            "                  # ",
            "#    ##    ##    ###",
            " #  #  #  #  #  #   ",
        };

        seaMonsterHeight = seaMonsterString.size();
        seaMonsterWidth = seaMonsterString[0].size();

        for (size_t y = 0; y < seaMonsterHeight; ++y)
            for (size_t x = 0; x < seaMonsterWidth; ++x)
                if(seaMonsterString[y][x] == '#')
                    seaMonsterBody.push_back({x, y});
    }

    size_t CheckForSeaMonstersInGridAndUpdateMask(const vector<string>& puzzle, vector<vector<bool>>& mask)
    {
        size_t numberOfSeaMonstersFound = 0;

        const size_t puzzleHeight = puzzle.size();
        const size_t puzzleWidth = puzzle[0].size();

        for (size_t y = 0; y < puzzleHeight - seaMonsterHeight; ++y)
            for (size_t x = 0; x < puzzleWidth - seaMonsterWidth; ++x)
                numberOfSeaMonstersFound += CheckForSeaMonsterInGridAndUpdateMask(y, x, puzzle, mask);

        return numberOfSeaMonstersFound;
    }

    bool CheckForSeaMonsterInGridAndUpdateMask(const size_t y, const size_t x, const vector<string>& puzzle, vector<vector<bool>>& mask)
    {
        for (const Point& seaMonsterPart : seaMonsterBody)
            if (puzzle[y + seaMonsterPart.y][x + seaMonsterPart.x] == '.')
                return false;

        for (const Point& seaMonsterPart : seaMonsterBody)
            mask[y + seaMonsterPart.y][x + seaMonsterPart.x] = false;

        return true;
    }

private:
    size_t seaMonsterWidth = 0;
    size_t seaMonsterHeight = 0;
    vector<Point> seaMonsterBody;
};

// Incorrect:
// HIGH: 2595
size_t CountNumberOfHashesNotInMonster(const vector<shared_ptr<Tile>>& tiles)
{
    vector<string> puzzle = GetAssembledPuzzle(tiles);
    //PrintGrid(puzzle);
    //cout << endl;

    vector<vector<bool>> mask;
    SeaMonster seaMonster;

    for (size_t index = 0; index < c_numberOfSides; ++index)
    {
        mask = GetPuzzleMask(puzzle);
        size_t seaMonsterCount = seaMonster.CheckForSeaMonstersInGridAndUpdateMask(puzzle, mask);

        if (seaMonsterCount)
            return CountTrueInPuzzleMask(mask);

        RotateGridClockwise(puzzle);
    }

    FlipGridAlongYAxis(puzzle);

    for (size_t index = 0; index < c_numberOfSides; ++index)
    {
        mask = GetPuzzleMask(puzzle);
        size_t seaMonsterCount = seaMonster.CheckForSeaMonstersInGridAndUpdateMask(puzzle, mask);

        if (seaMonsterCount)
            return CountTrueInPuzzleMask(mask);

        RotateGridClockwise(puzzle);
    }

    return 999999;
}

int main()
{
    const string inputFileName(R"(20_in.txt)");
    //const string inputFileName(R"(20_sample_in.txt)");

    vector<shared_ptr<Tile>> tiles = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Tiles size: " << tiles.size() << endl;
    cout << endl;

    ProcessTileNeighbors(tiles);

    cout << "Get corner tile ids multiplied: " << GetCornerTileIdsMultiplied(tiles) << endl;
    cout << endl;

    cout << "Count number of hashes not in monster: " << CountNumberOfHashesNotInMonster(tiles) << endl;
    cout << endl;

    return 0;
}
