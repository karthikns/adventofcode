#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<int> ParseInputFile(const string& inputFileName)
{
    vector<int> parseResult{};

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
    const string inputFileName(R"(template_in.txt)");

    vector<int> result = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Parse result: " << result.size() << endl;
    cout << endl;

    return 0;
}
