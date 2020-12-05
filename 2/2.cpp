// 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Record
{
    int first;
    int second;
    char letter;
    string password;
};

int main()
{
    //fstream fs("C:\\Users\\karth\\Desktop\\input.txt");
    fstream fs("2_in.txt");

    vector<Record> recordVector;

    try
    {
        while (fs.good())
        {
            Record r;
            fs >> r.first >> r.second >> r.letter >> r.password;
            recordVector.emplace_back(r);
        }
    }
    catch (...)
    {
        cout << "File parse error";
        return 0;
    }

    {
        int numberOfValidPasswords = 0;
        for (const Record& record : recordVector)
        {
            int numberOfOccurances = count(begin(record.password), end(record.password), record.letter);

            if (numberOfOccurances >= record.first && numberOfOccurances <= record.second)
                ++numberOfValidPasswords;
        }

        cout << numberOfValidPasswords << endl;
    }

    {
        int numberOfValidPasswords = 0;
        for (const Record& record : recordVector)
        {
            bool isFirstMatching = false;
            bool isSecondMatching = false;

            const int passwordLength = record.password.length();

            if (record.first - 1 < passwordLength && record.password[record.first - 1] == record.letter)
            {
                isFirstMatching = true;
            }

            if (record.second - 1 < passwordLength && record.password[record.second - 1] == record.letter)
            {
                isSecondMatching = true;
            }

            const bool isValidPassword = (isFirstMatching ^ isSecondMatching);
            numberOfValidPasswords += isValidPassword;

            //cout << record.first << " " << record.second << " " << record.letter << " " << record.password << endl;
            //cout << isValidPassword << endl << endl;
        }

        cout << numberOfValidPasswords << endl;
    }

    return 0;
}
