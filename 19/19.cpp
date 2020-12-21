#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

enum class Part
{
    Part1,
    Part2
};

tuple<vector<string>, vector<string>> ParseInputFile(const string& inputFileName)
{
    vector<string> ruleStrings;
    vector<string> messagesToValidate;

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
            ruleStrings.emplace_back(line);
        }

        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, line);
            //cout << "line2: " << line << endl;

            if (line.empty())
                break;

            // Enter parsing code here
            messagesToValidate.emplace_back(line);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return { ruleStrings, messagesToValidate };
}

struct Rule
{
    vector<pair<int, int>> sequences;
};


class AllRules
{
public:
    AllRules(const vector<string>& rulesStrings, Part part):
        rules(500),
        rulesRegEx(500),
        numRulesProcessed(0),
        totalNumRules(0),
        part(part)
    {
        ProcessRuleStrings(rulesStrings);
        ProcessRules();
    }

    bool CheckIfMessageIsValid(const string& message) const
    {
        regex regex("^" + rulesRegEx[0] + "$");
        return regex_match(message, regex);
    }

    string GetRegexString() const
    {
        return "^" + rulesRegEx[0] + "$";
    }

private:
    void ProcessRuleStrings(const vector<string>& rulesStrings)
    {
        size_t ruleId = 0;
        for (size_t index = 0; index < rulesStrings.size(); ++index)
        {
            const string& ruleString = rulesStrings[index];

            if (const size_t pos = ruleString.find("id") != string::npos)
            {
                ruleId = stoul(ruleString.substr(pos + 1));
                ++totalNumRules;
                continue;
            }


            if (const size_t pos = ruleString.find("two") != string::npos)
            {
                const string payload = ruleString.substr(4);
                const size_t spacePos = payload.find(' ');
                const int first = stoi(payload.substr(0, spacePos));
                const int second = stoi(payload.substr(spacePos + 1));
                rules[ruleId].sequences.push_back({ first, second });
                continue;
            }


            if (const size_t pos = ruleString.find("\"") != string::npos)
            {
                rulesRegEx[ruleId] = ruleString[1];
                ++numRulesProcessed;
                continue;
            }
        }

        rules.resize(totalNumRules);
        rulesRegEx.resize(totalNumRules);
    }

    void ProcessRules()
    {
        while (numRulesProcessed < totalNumRules)
        {
            for (size_t id = 0; id < totalNumRules; ++id)
            {
                if (rules[id].sequences.empty())
                    continue;

                if (!AreAllSequencesProcessed(rules[id]))
                    continue;

                if (rules[id].sequences.size() > 3)
                    cout << "~~~~ ERROR ~~~~";

                string ruleRegEx;
                if (rules[id].sequences.size() == 1)
                    ruleRegEx = "(" + ProcessSequence(rules[id].sequences[0], id) + ")";
                else
                {
                    ruleRegEx = "((" + ProcessSequence(rules[id].sequences[0], id) + ")|(" + ProcessSequence(rules[id].sequences[1], id) + "))";
                }

                if (id == 8 && part == Part::Part2)
                {
                    ruleRegEx = "(" + ruleRegEx + "+)";
                }

                ++numRulesProcessed;
                rules[id].sequences.clear();
                rulesRegEx[id] = ruleRegEx;
            }
        }
    }

    string ProcessSequence(const pair<int, int>& sequence, const size_t ruleId)
    {
        if (sequence.second == 256)
            return rulesRegEx[sequence.first];

        string returnString = rulesRegEx[sequence.first] + rulesRegEx[sequence.second];

        if (ruleId == 11)
        {
            returnString = 
                "(" + rulesRegEx[sequence.first] + "){1}" +
                "(" + rulesRegEx[sequence.second] + "){1}";
        }

        return returnString;
    }

    bool AreAllSequencesProcessed(const Rule& rule)
    {
        for (const pair<int, int>& sequence : rule.sequences)
        {
            if (rulesRegEx[sequence.first].empty())
                return false;

            if (sequence.second != 256 && rulesRegEx[sequence.second].empty())
                return false;
        }

        return true;
    }

private:
    vector<Rule> rules;
    vector<string> rulesRegEx;
    size_t numRulesProcessed;
    size_t totalNumRules;
    Part part;

};

size_t CountOfValidMessages(vector<string>& messagesToValidate, const AllRules& allRules)
{
    size_t numberOfValidMessages = 0;

    vector<string> invalidMessages;

    for (size_t index = 0; index < messagesToValidate.size(); ++index)
    {
        //if ((index + 1) % 10 == 0)
        //    cout << "Validating message " << index + 1 << " of " << messagesToValidate.size() << endl;

        if (allRules.CheckIfMessageIsValid(messagesToValidate[index]))
            ++numberOfValidMessages;
        else
            invalidMessages.push_back(messagesToValidate[index]);
    }

    messagesToValidate = move(invalidMessages);
    return numberOfValidMessages;
}

size_t CountOfInvalidMessagesPart2(vector<string>& messagesToValidate, const AllRules& allRules)
{
    const size_t messagesSize = messagesToValidate.size();

    const string regexString = allRules.GetRegexString();
    for (size_t index = 1; index <= 9; ++index)
    {
        vector<string> invalidStrings;

        string regexStringCopy = regexString;
        char newChar = '0' + static_cast<char>(index);
        replace(begin(regexStringCopy), end(regexStringCopy), '1', newChar);

        regex regularExpression(regexStringCopy);

        //cout << "Validating with instances: " << index << endl;
        for (size_t index = 0; index < messagesToValidate.size(); ++index)
        {
            //if((index + 1) % 10 == 0)
            //    cout << "Validating message " << index + 1 << " of " << messagesToValidate.size() << endl;

            if (!regex_match(messagesToValidate[index], regularExpression))
                invalidStrings.push_back(messagesToValidate[index]);
        }
        //cout << endl;

        messagesToValidate = move(invalidStrings);
    }

    return messagesToValidate.size();
}

// Part 2
// Incorrect: 235
int main()
{
    const string inputFileName(R"(19_processed_in.txt)");
    //const string inputFileName(R"(19_sample_processed_in.txt)");

    vector<string> ruleStrings;
    vector<string> messagesToValidate;

    tie(ruleStrings, messagesToValidate) = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Rule strings size: " << ruleStrings.size() << endl;
    const size_t totalMessagesSize = messagesToValidate.size();
    cout << "Messages to validate size: " << totalMessagesSize << endl;
    cout << endl;

    AllRules allRules(ruleStrings, Part::Part1);
    cout << "Count of valid messages (Part 1): " << CountOfValidMessages(messagesToValidate, allRules);
    cout << endl << endl;

    AllRules allRules2(ruleStrings, Part::Part2);
    cout << "Count of valid messages (Part 2): " << totalMessagesSize - CountOfInvalidMessagesPart2(messagesToValidate, allRules2);
    cout << endl;

    return 0;
}
