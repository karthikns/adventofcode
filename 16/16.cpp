#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Range
{
    uint32_t rangeBegin = 0;
    uint32_t rangeEnd = 0;

    bool IsValueInRange(const uint32_t value) const
    {
        if (value >= rangeBegin && value <= rangeEnd)
            return true;

        return false;
    }
};

struct Field
{
    string fieldName;
    Range firstRange;
    Range secondRange;

    bool IsValueInField(const uint32_t value) const
    {
        return firstRange.IsValueInRange(value) || secondRange.IsValueInRange(value);
    }
};

struct Ticket
{
    vector<uint32_t> values;
};

struct Input
{
    vector<Field> fields;
    Ticket yourTicket;
    vector<Ticket> nearbyTickets;
};

enum class InputToProcess
{
    Field,
    YourTicket,
    NearbyTickets
};

Ticket ParseTicketValues(string ticketString)
{
    std::replace(begin(ticketString), end(ticketString), ',', ' ');

    Ticket ticket;
    stringstream stream(ticketString);
    uint32_t value = 0;
    while (stream >> value)
        ticket.values.emplace_back(value);

    return ticket;
}

Input ParseInputFile(const string& inputFileName)
{
    Input input;

    try
    {
        ifstream inputFile(inputFileName, ios::in);

        if (inputFile.fail())
        {
            cout << "File Open Error" << endl;
            exit(-1);
        }

        InputToProcess inputToProcess = InputToProcess::Field;
        string line;
        while (inputFile.good() && !inputFile.eof())
        {
            getline(inputFile, line);
            //cout << "line: " << line << endl;

            if (line.empty())
            {
                continue;
            }

            // Enter parsing code here
            if (line == "your ticket:")
            {
                inputToProcess = InputToProcess::YourTicket;
                continue;
            }
            else if (line == "nearby tickets:")
            {
                inputToProcess = InputToProcess::NearbyTickets;
                continue;
            }

            if (inputToProcess == InputToProcess::Field)
            {
                smatch matchValues;
                regex memRegEx("(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)");

                regex_search(line, matchValues, memRegEx);

                //cout << matchValues[0] << endl;
                Field field;
                field.fieldName = matchValues[1];
                field.firstRange.rangeBegin = stoul(matchValues[2]);
                field.firstRange.rangeEnd = stoul(matchValues[3]);
                field.secondRange.rangeBegin = stoul(matchValues[4]);
                field.secondRange.rangeEnd = stoul(matchValues[5]);

                input.fields.emplace_back(field);
                continue;
            }

            if (inputToProcess == InputToProcess::YourTicket)
            {
                input.yourTicket = ParseTicketValues(line);
                continue;
            }

            // inputToProcess == InputToProcess::NearbyTickets
            input.nearbyTickets.emplace_back(ParseTicketValues(line));
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return input;
}

bool IsTicketValid(Ticket ticket, vector<Field> fields)
{
    for (const int value : ticket.values)
    {
        bool isValueValid = false;
        for (const Field& field : fields)
        {
            if (field.IsValueInField(value))
            {
                isValueValid = true;
                break;
            }
        }

        if (!isValueValid)
            return false;
    }

    return true;
}

// Incorrect:
// 622419: Too high
// 130414: Too high
// 25895: correct
uint32_t GetSumOfInvalidFields(const Input& input)
{
    uint32_t sumOfInvalidValues = 0;

    for (const Ticket& ticket : input.nearbyTickets)
    {
        //cout << boolalpha << IsTicketValid(ticket, input.fields) << endl;
        for (const int value : ticket.values)
        {
            bool isValueValid = false;
            for (const Field& field : input.fields)
            {
                if (field.IsValueInField(value))
                {
                    isValueValid = true;
                    break;
                }
            }

            if (!isValueValid)
            {
                //cout << "    " << value << endl;
                sumOfInvalidValues += value;
            }
        }
    }

    return sumOfInvalidValues;
}

vector<Ticket> GetValidTickets(const Input& input)
{
    vector<Ticket> validTickets = input.nearbyTickets;
    auto iterator = remove_if(begin(validTickets),
        end(validTickets),
        [&](auto ticket)->bool 
        {
            return !IsTicketValid(ticket, input.fields);
        });


    //cout << "Distance: " << distance(begin(validTickets), iterator) << endl;

    validTickets.erase(iterator, end(validTickets));
    //cout << "Final size: " << validTickets.size() << endl;


    for (const Ticket& ticket : validTickets)
    {
        if (!IsTicketValid(ticket, input.fields))
            cout << "**** SCREAM *****" << endl;
    }

    return validTickets;
}

bool IsColumnInField(const uint32_t column, const vector<Ticket>& tickets, const Field& field)
{
    for (const Ticket& ticket : tickets)
        if (!field.IsValueInField(ticket.values[column]))
            return false;

    return true;
}

bool IsColumnFieldNameCandidatesEmpty(const vector<set<string>>& columnFieldNameCandidates)
{
    for (const set<string>& columnFieldNameNameCandidate : columnFieldNameCandidates)
        if (!columnFieldNameNameCandidate.empty())
            return false;

    return true;
}

void PurgeFieldNames(vector<set<string>>& columnFieldNameCandidates, const string& fieldName)
{
    for (set<string>& columnFieldNameNameCandidate : columnFieldNameCandidates)
    {
        columnFieldNameNameCandidate.erase(fieldName);
    }
}

// Incorrect:
// 3093368713 - too low
uint64_t GetDepartureFieldsMultiplied(const Input& input)
{
    vector<Ticket> validTickets = GetValidTickets(input);

    set<string> allFieldNames;
    for (const Field& field : input.fields)
        allFieldNames.emplace(field.fieldName);

    //for (const string& fieldName : allFieldNames)
    //    cout << fieldName << endl;


    vector<set<string>> columnFieldNameCandidates(validTickets[0].values.size());

    for (uint32_t columnNumber = 0; columnNumber < validTickets[0].values.size(); ++columnNumber)
    {
        //cout << columnNumber << " candidates" << endl;
        for (const Field& field : input.fields)
        {
            if (IsColumnInField(columnNumber, validTickets, field))
            {
                //cout << "    " << field.fieldName << endl;
                columnFieldNameCandidates[columnNumber].emplace(field.fieldName);
            }
        }
    }

    vector<string> columnLabels(validTickets[0].values.size());
    while (!IsColumnFieldNameCandidatesEmpty(columnFieldNameCandidates))
    {
        auto iterator = find_if(
            begin(columnFieldNameCandidates),
            end(columnFieldNameCandidates),
            [](auto candidateSet)->bool
            {
                return candidateSet.size() == 1; 
            });

        const string fieldName = *(iterator->begin());
        const size_t column = distance(begin(columnFieldNameCandidates), iterator);
        PurgeFieldNames(columnFieldNameCandidates, fieldName);

        columnLabels[column] = fieldName;
    }

    uint64_t departureFieldsMutliplied = 1;
    for (size_t index = 0; index < columnLabels.size(); ++index)
    {
        if (columnLabels[index].find("departure") != string::npos)
        {
            departureFieldsMutliplied *= static_cast<uint64_t>(input.yourTicket.values[index]);
        }
    }

    return departureFieldsMutliplied;
}

int main()
{
    const string inputFileName(R"(16_in.txt)");
    //const string inputFileName(R"(16_sample_in.txt)");

    Input input = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Sum of invalid fields: " << GetSumOfInvalidFields(input) << endl;
    cout << endl;

    cout << "Depature values multiplied: " << GetDepartureFieldsMultiplied(input) << endl;
    cout << endl;

    return 0;
}
