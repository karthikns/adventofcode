#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

size_t c_numberOfSides = 4;

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

    void SetCornerTileToTopLeft()
    {
        // assert DoesTileHaveTwoNeighbors()

        size_t numberOfLeftRotations = 1;

        // 0 is top, 3 is left
        // 1 is right, 2 is bottom
        if (adjoiningTiles[3].tileId == 0 && adjoiningTiles[0].tileId == 0)
            return;
        else if (adjoiningTiles[0].tileId == 0 && adjoiningTiles[1].tileId == 0)
            numberOfLeftRotations = 1;
        else if (adjoiningTiles[1].tileId == 0 && adjoiningTiles[2].tileId == 0)
            numberOfLeftRotations = 2;
        else if (adjoiningTiles[2].tileId == 0 && adjoiningTiles[3].tileId == 0)
            numberOfLeftRotations = 3;

        // TODO: rotate left number of times
    }

    bool HasTileNeighbor(size_t tileId) const
    {
        for (size_t index = 0; index < c_numberOfSides; ++index)
            if (adjoiningTiles[index].tileId == tileId)
                return true;

        return false;
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

size_t CountNumberOfHashesNotInMonster(const vector<shared_ptr<Tile>>& tiles)
{
    const size_t gridSize = static_cast<size_t>(sqrt(tiles.size()));

    vector<shared_ptr<Tile>> tilesToBeProcessed = tiles;
    const size_t cornerTileIndex = GetACornerTileIndex(tilesToBeProcessed);
    const shared_ptr<Tile> cornerTile = tilesToBeProcessed[cornerTileIndex];

    tilesToBeProcessed.erase(tilesToBeProcessed.begin() + cornerTileIndex);

    size_t assembledPuzzle[15][15] = {};
    assembledPuzzle[1][1] = cornerTile->GetTileId();

    for (size_t y = 1; y <= gridSize; ++y)
    {
        for (size_t x = 1; x <= gridSize; ++x)
        {
            if (y == 1 && x == 1)
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

            assembledPuzzle[y][x] = (*findIterator)->GetTileId();
            tilesToBeProcessed.erase(findIterator);
        }
    }

    cornerTile->SetCornerTileToTopLeft();

    return gridSize;
}

int main()
{
    //const string inputFileName(R"(20_in.txt)");
    const string inputFileName(R"(20_sample_in.txt)");

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
