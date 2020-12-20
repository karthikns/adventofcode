#include <algorithm>
#include <iostream>
#include <fstream>
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
        tileId(tileId)
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
    AdjoiningTile adjoiningTiles[4];
    string edges[4] = {};

};

Tile ParseTile(const size_t tileId, ifstream& inputFile)
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


    return Tile(tileId, tileContents);
}

vector<Tile> ParseInputFile(const string& inputFileName)
{
    vector<Tile> tiles;

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
                Tile tile = ParseTile(stoul(line.substr(5, 4)), inputFile);
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

void ProcessTileNeighbors(vector<Tile>& tiles)
{
    for (size_t outerIndex = 0; outerIndex < tiles.size() - 1; ++outerIndex)
        for (size_t innerIndex = outerIndex + 1; innerIndex < tiles.size(); ++innerIndex)
            tiles[outerIndex].ProcessTileNeighbor(tiles[innerIndex]);
}

uint64_t GetCornerTileIdsMultiplied(const vector<Tile>& tiles)
{
    uint64_t cornerTileIdsMultiplied = 1;

    for (const Tile& tile : tiles)
        if (tile.DoesTileHaveTwoNeighbors())
            cornerTileIdsMultiplied *= tile.GetTileId();

    return cornerTileIdsMultiplied;
}

int main()
{
    const string inputFileName(R"(20_in.txt)");
    //const string inputFileName(R"(20_sample_in.txt)");

    vector<Tile> tiles = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Tiles size: " << tiles.size() << endl;
    cout << endl;

    ProcessTileNeighbors(tiles);

    cout << "Get corner tile ids multiplied: " << GetCornerTileIdsMultiplied(tiles) << endl;
    cout << endl;

    return 0;
}
