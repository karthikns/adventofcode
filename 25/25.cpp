#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<uint64_t> ParseInputFile(const string& inputFileName)
{
    vector<uint64_t> parseResult{};

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
            parseResult.push_back(stoull(line));
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

// Incorrect:
// 6164813 too high
uint64_t GetEncryptionKey(const uint64_t card, const uint64_t door)
{
    const uint64_t modder = 20201227ull;

    uint64_t value = 1;
    uint64_t encryptionKey = 1;
    while (value != card)
    {
        value = (value * 7) % modder;
        encryptionKey = (encryptionKey * door) % modder;
    }

    return encryptionKey;
}

int main()
{
    const vector<uint64_t> sampleKeys{ 5764801, 17807724 };

    const string inputFileName(R"(25_in.txt)");

    const vector<uint64_t> actualKeys = ParseInputFile(inputFileName);
    cout << endl;

    const vector<uint64_t>& inputKeys = actualKeys;
    //const vector<uint64_t>& inputKeys = sampleKeys;

    cout << "Number of keys: " << actualKeys.size() << endl;
    for (const uint64_t key : actualKeys)
        cout << "Key: " << key << endl;
    cout << endl;

    cout << "Encryption key: " << GetEncryptionKey(inputKeys[0], inputKeys[1]) << endl;
    cout << endl;

    return 0;
}
