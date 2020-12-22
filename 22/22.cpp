#include <deque>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class CombatGame
{
public:
    CombatGame() = default;

    void AddCardToBackOfPlayerOneDeck(uint32_t value)
    {
        playerOneDeck.emplace_back(value);
    }

    void AddCardToBackOfPlayerTwoDeck(uint32_t value)
    {
        playerTwoDeck.emplace_back(value);
    }

    void RunGameTillEnd()
    {
        uint32_t turnNumber = 1;
        while (!playerOneDeck.empty() && !playerTwoDeck.empty())
        {
            cout << "TURN: " << turnNumber++ << endl;
            PlayATurn();
        }
    }

    uint32_t GetScoreOfWinner() const
    {
        if (!(playerOneDeck.empty() || playerTwoDeck.empty()))
        {
            cout << "Error: Calculating winner when game is not over" << endl;
            cout << endl;

            return 0;
        }

        const deque<uint32_t>& winnerDeck = playerOneDeck.empty() ? playerTwoDeck : playerOneDeck;

        uint32_t scoreOfWinner = 0;
        uint32_t scoreWeight = static_cast<uint32_t>(winnerDeck.size());
        for (int index = 0; index < winnerDeck.size(); ++index)
        {
            scoreOfWinner += (winnerDeck[index] * scoreWeight);
            scoreWeight--;
        }

        return scoreOfWinner;
    }

private:
    void PlayATurn()
    {
        if (playerOneDeck.empty() || playerTwoDeck.empty())
        {
            cout << "No cards to process. Skipping turn" << endl;
            return;
        }

        const uint32_t playerOneTopCard = playerOneDeck.front();
        playerOneDeck.pop_front();

        const uint32_t playerTwoTopCard = playerTwoDeck.front();
        playerTwoDeck.pop_front();

        if (playerOneTopCard > playerTwoTopCard)
        {
            playerOneDeck.emplace_back(playerOneTopCard);
            playerOneDeck.emplace_back(playerTwoTopCard);
        }
        else if (playerTwoTopCard > playerOneTopCard)
        {
            playerTwoDeck.emplace_back(playerTwoTopCard);
            playerTwoDeck.emplace_back(playerOneTopCard);
        }
        else
        {
            cout << "Error: duplicate cards" << endl;
            cout << endl;
        }
    }

    deque<uint32_t> playerOneDeck;
    deque<uint32_t> playerTwoDeck;
};

CombatGame ParseInputFile(const string& inputFileName)
{
    CombatGame combatGame;

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
            cout << "line1: " << line << endl;

            if (line.empty())
                break;

            // Enter parsing code here
            if (line == "Player 1:")
                continue;

            combatGame.AddCardToBackOfPlayerOneDeck(stoul(line));
        }

        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, line);
            cout << "line2: " << line << endl;

            if (line.empty())
                break;

            // Enter parsing code here
            if (line == "Player 2:")
                continue;

            combatGame.AddCardToBackOfPlayerTwoDeck(stoul(line));
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return combatGame;
}

int main()
{
    const string inputFileName(R"(22_in.txt)");

    CombatGame combatGame = ParseInputFile(inputFileName);

    cout << "Input parsed" << endl;
    cout << endl;

    combatGame.RunGameTillEnd();
    cout << "Score of winner: " << combatGame.GetScoreOfWinner() << endl;
    cout << endl;

    return 0;
}
