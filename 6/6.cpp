#include <iostream>
#include <fstream>
#include <set>
#include <string>

using namespace std;

// Part 1
size_t SumOfCountOfUniqueAnswersInGroups(const string& inputFileName)
{
    size_t sumOfCountOfUniqueAnswersInGroups = 0;

    try
    {
        fstream inputFile(inputFileName);

        if (inputFile.bad())
        {
            cout << "File Open Error" << endl;
            exit(-1);
        }

        string answersByPerson;
        set<char> answersInGroup;
        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, answersByPerson);

            if (answersByPerson.size() == 0)
            {
                //cout << answersInGroup.size() << endl;
                sumOfCountOfUniqueAnswersInGroups += answersInGroup.size();
                answersInGroup.clear();
            }
            else
            {
                answersInGroup.insert(begin(answersByPerson), end(answersByPerson));
            }

            //cout << "line: " << answersByPerson << endl;
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return sumOfCountOfUniqueAnswersInGroups;
}

// Part 2
size_t SumOfCountOfUniqueAnswersInGroups2(const string& inputFileName)
{
    size_t sumOfCountOfUniqueAnswersInGroups = 0;

    try
    {
        fstream inputFile(inputFileName);

        if (inputFile.bad())
        {
            cout << "File Open Error" << endl;
            exit(-1);
        }

        string answersByPerson;
        set<char> answersInGroup;
        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, answersByPerson);

            if (answersByPerson.size() == 0)
            {
                cout << answersInGroup.size() << endl;
                sumOfCountOfUniqueAnswersInGroups += answersInGroup.size();
                answersInGroup.clear();
            }
            else
            {
                answersInGroup.insert(begin(answersByPerson), end(answersByPerson));
            }

            cout << "line: " << answersByPerson << endl;
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return sumOfCountOfUniqueAnswersInGroups;
}

int main()
{
    const string inputFileName(R"(6_in.txt)");

    cout << SumOfCountOfUniqueAnswersInGroups(inputFileName) << endl;

    return 0;
}
