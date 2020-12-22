#include <deque>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class CombatGame
{
private:
    static size_t s_gameIndex;

public:
    CombatGame() :
        gameIndex(s_gameIndex++)
    {

    }

    void AddCardToBackOfPlayerOneDeck(uint32_t value)
    {
        decks[0].emplace_back(value);
    }

    void AddCardToBackOfPlayerTwoDeck(uint32_t value)
    {
        decks[1].emplace_back(value);
    }

    void AddCardToBackOfPlayerOneDeck(deque<uint32_t>::iterator begin, deque<uint32_t>::iterator end)
    {
        decks[0].insert(decks[0].end(), begin, end);
    }

    void AddCardToBackOfPlayerTwoDeck(deque<uint32_t>::iterator begin, deque<uint32_t>::iterator end)
    {
        decks[1].insert(decks[1].end(), begin, end);
    }

    size_t RunGameTillEndPart1()
    {
        uint32_t turnNumber = 1;
        while (!decks[0].empty() && !decks[1].empty())
        {
            //cout << "TURN: " << turnNumber << endl;
            ++turnNumber;

            PlayATurnPart1();
        }

        return decks[0].empty() ? 1 : 0;
    }

    size_t RunGameTillEndPart2()
    {
        //cout << "=== Game " << gameIndex << " ===" << endl;
        //cout << endl;

        size_t turnNumber = 1;
        while (!decks[0].empty() && !decks[1].empty())
        {
            //cout << "-- Round " << turnNumber << " (Game " << gameIndex << ") --" << endl;
            //cout << endl;
            ++turnNumber;

            history[0].push_back(decks[0]);
            history[1].push_back(decks[1]);

            PlayATurnPart2(turnNumber);

            if (find(begin(history[0]), end(history[0]), decks[0]) != end(history[0]) &&
                find(begin(history[1]), end(history[1]), decks[1]) != end(history[1]))
                return 0;
        }

        return decks[0].empty() ? 1 : 0;
    }

    uint32_t GetScoreOfWinner() const
    {
        if (!(decks[0].empty() || decks[1].empty()))
        {
            cout << "Error: Calculating winner when game is not over" << endl;
            cout << endl;

            return 0;
        }

        const deque<uint32_t>& winnerDeck = decks[0].empty() ? decks[1] : decks[0];

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
    void PlayATurnPart1()
    {
        if (decks[0].empty() || decks[1].empty())
        {
            cout << "No cards to process. Skipping turn" << endl;
            return;
        }

        const uint32_t playerOneTopCard = decks[0].front();
        decks[0].pop_front();

        const uint32_t playerTwoTopCard = decks[1].front();
        decks[1].pop_front();

        if (playerOneTopCard > playerTwoTopCard)
        {
            decks[0].emplace_back(playerOneTopCard);
            decks[0].emplace_back(playerTwoTopCard);
        }
        else if (playerTwoTopCard > playerOneTopCard)
        {
            decks[1].emplace_back(playerTwoTopCard);
            decks[1].emplace_back(playerOneTopCard);
        }
        else
        {
            cout << "Error: duplicate cards" << endl;
            cout << endl;
        }
    }

    void PlayATurnPart2(const size_t turnNumber)
    {
        if (decks[0].empty() || decks[1].empty())
        {
            cout << "No cards to process. Skipping turn" << endl;
            return;
        }

        const uint32_t topCards[2] = { decks[0].front(), decks[1].front() };

        decks[0].pop_front();
        decks[1].pop_front();

        size_t winnerIndex = 0;
        if (decks[0].size() >= topCards[0] && decks[1].size() >= topCards[1])
        {
            //cout << "Playing a sub-game to determine the winner..." << endl << endl;

            CombatGame subGame;
            subGame.AddCardToBackOfPlayerOneDeck(decks[0].begin(), decks[0].begin() + topCards[0]);
            subGame.AddCardToBackOfPlayerTwoDeck(decks[1].begin(), decks[1].begin() + topCards[1]);

            winnerIndex = subGame.RunGameTillEndPart2();

            //cout << "...anyway, back to game " << gameIndex << "." << endl;
            //cout << "Player " << winnerIndex + 1 << " wins round " << turnNumber << " of game " << gameIndex << "!" << endl;
            //cout << endl;
        }
        else
        {
            if(topCards[0] > topCards[1])
                winnerIndex = 0;
            else if(topCards[1] > topCards[0])
                winnerIndex = 1;
            else
                cout << "Error: duplicate cards" << endl << endl;
        }

        decks[winnerIndex].emplace_back(topCards[winnerIndex]);
        decks[winnerIndex].emplace_back(topCards[(winnerIndex + 1) % 2]);
    }

private:
    deque<uint32_t> decks[2];
    vector<deque<uint32_t>> history[2];
    size_t gameIndex;
};

size_t CombatGame::s_gameIndex = 1;

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
            //cout << "line1: " << line << endl;

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
            //cout << "line2: " << line << endl;

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

// Part 2 incorrect:
// 34766 too high
// 31926 too low
int main()
{
    const string inputFileName(R"(22_in.txt)");

    CombatGame combatGame = ParseInputFile(inputFileName);

    cout << "Input parsed" << endl;
    cout << endl;

    CombatGame combatGameForPart1 = combatGame;
    combatGameForPart1.RunGameTillEndPart1();
    cout << "Score of winner (Part 1): " << combatGameForPart1.GetScoreOfWinner() << endl;
    cout << endl;

    CombatGame combatGameForPart2 = combatGame;
    combatGameForPart2.RunGameTillEndPart2();
    cout << "Score of winner (Part 2): " << combatGameForPart2.GetScoreOfWinner() << endl;
    cout << endl;

    return 0;
}
