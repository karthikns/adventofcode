#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>

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
size_t SumOfCountOfCommonAnswersInGroups(const string& inputFileName)
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
        const vector<char> allLetters = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', };
        vector<char> answersIntersectionInGroup = allLetters;
        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, answersByPerson);

            if (answersByPerson.size() == 0)
            {
                //cout << "    " << answersIntersectionInGroup.size() << endl;
                sumOfCountOfUniqueAnswersInGroups += answersIntersectionInGroup.size();
                answersIntersectionInGroup = allLetters;
            }
            else
            {
                vector<char> intersectionTemp(26);

                sort(begin(answersByPerson), end(answersByPerson));

                set_intersection(
                    begin(answersByPerson), end(answersByPerson),
                    begin(answersIntersectionInGroup), end(answersIntersectionInGroup),
                    begin(intersectionTemp)
                );

                answersIntersectionInGroup = move(intersectionTemp);
                answersIntersectionInGroup.erase(remove(begin(answersIntersectionInGroup), end(answersIntersectionInGroup), 0), end(answersIntersectionInGroup));
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

int main()
{
    const string inputFileName(R"(6_in.txt)");

    cout << SumOfCountOfUniqueAnswersInGroups(inputFileName) << endl << endl;
    cout << SumOfCountOfCommonAnswersInGroups(inputFileName) << endl;

    return 0;
}
