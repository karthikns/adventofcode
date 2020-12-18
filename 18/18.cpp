#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include <regex>
#include <string>
#include <vector>

using namespace std;

vector<string> ParseInputFile(const string& inputFileName)
{
    vector<string> parseResult{};

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
            auto it = remove(begin(line), end(line), ' ');
            line.erase(it, end(line));

            parseResult.emplace_back(line);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return parseResult;
}

struct Expression
{
    uint32_t number = 0;
    char op = '\0';
    struct Expression* left = nullptr;
    struct Expression* right = nullptr;
};

size_t FindOperandStringSize(const string& equationString, const size_t startIndex)
{
    const char startCharacter = equationString[startIndex];
    if (startCharacter >= '0' && startCharacter <= '9')
    {
        return 1;
    }

    if (startCharacter != '(')
    {
        cout << "~~~~~ ERROR ~~~~~" << endl;
        return 0;
    }

    // '('
    size_t numberOfOpenBrackets = 1;
    size_t index = startIndex + 1;
    for (; index < equationString.size(); ++index)
    {
        const char charAtIndex = equationString[index];
        if (charAtIndex == '(')
        {
            ++numberOfOpenBrackets;
        }
        else if (charAtIndex == ')')
        {
            --numberOfOpenBrackets;
            if (!numberOfOpenBrackets)
            {
                break;
            }
        }
    }

    //cout << equationString.substr(startIndex, index - startIndex + 1) << endl;
    return index - startIndex + 1;
}

uint64_t ComputeEquationPart1(const string& equationString)
{
    string equationStringToProcess = "+" + equationString;

    uint64_t equationValue = 0;
    char equationOperator = '+';
    for (size_t index = 0; index < equationStringToProcess.size();)
    {
        equationOperator = equationStringToProcess[index];
        ++index;

        const size_t lengthOfOperand = FindOperandStringSize(equationStringToProcess, index);
        uint64_t operandValue = 0;
        if (lengthOfOperand == 1)
        {
            const char charAtIndex = equationStringToProcess[index];
            const char zeroChar = '0';
            operandValue = static_cast<uint64_t>(charAtIndex - zeroChar);
        }
        else
        {
            operandValue = ComputeEquationPart1(equationStringToProcess.substr(index + 1, lengthOfOperand - 2));
        }

        index += lengthOfOperand;

        if (equationOperator == '+')
            equationValue += operandValue;
        else if (equationOperator == '*')
            equationValue *= operandValue;
        else
        {
            cout << "~~~~~ ERROR2 ~~~~~" << endl;
            return 0;
        }
    }

    //cout << equationStringToProcess << ": " << equationValue << endl;

    return equationValue;
}

struct OperandOp
{
    uint64_t number = 0;
    char op = '\0';
};

uint64_t ComputeEquationPart2(const string& equationString)
{
    string equationStringToProcess = equationString + '*';

    vector<OperandOp> operandOps;
    for (size_t index = 0; index < equationString.size();)
    {
        const size_t lengthOfOperand = FindOperandStringSize(equationStringToProcess, index);
        uint64_t operandValue = 0;
        if (lengthOfOperand == 1)
        {
            const char charAtIndex = equationStringToProcess[index];
            const char zeroChar = '0';
            operandValue = static_cast<uint64_t>(charAtIndex - zeroChar);
        }
        else
        {
            operandValue = ComputeEquationPart2(equationStringToProcess.substr(index + 1, lengthOfOperand - 2));
        }

        index += lengthOfOperand;

        const char op = equationStringToProcess[index];
        ++index;

        operandOps.push_back({operandValue, op});
    }

    const function<bool(const OperandOp& operandOp)> doesOperandOpContainPlusChecker =
        [](const OperandOp& operandOp) -> bool
        {
            return operandOp.op == '+';
        };

    // Process all +
    vector<OperandOp> multiplyOperandOps;
    for (size_t index = 0; index < operandOps.size(); ++index)
    {
        if (operandOps[index].op == '+')
        {
            operandOps[index + 1].number += operandOps[index].number;
            continue;
        }

        multiplyOperandOps.emplace_back(operandOps[index]);
    }


    // Process all *
    uint64_t equationValue = 1;
    for (const OperandOp& operandOp : multiplyOperandOps)
    {
        equationValue *= operandOp.number;
    }

    //cout << equationString << ": " << equationValue << endl;
    return equationValue;
}

uint64_t ComputeSumOfEquations(const vector<string>& equationsStrings, const function<uint64_t(const string&)>& computeEquationFunction)
{
    uint64_t sumOfEquations = 0;

    for (const string& equationString : equationsStrings)
    {
        sumOfEquations += computeEquationFunction(equationString);
    }

    return sumOfEquations;

}

int main()
{
    const string inputFileName(R"(18_in.txt)");

    vector<string> parseResult = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Parse result size: " << parseResult.size() << endl;
    cout << endl;

    function<uint64_t(const string&)> computeEquationFunction;

    computeEquationFunction = ComputeEquationPart1;
    cout << "Sum of equations (Part 1): " << ComputeSumOfEquations(parseResult, computeEquationFunction) << endl;
    cout << endl;

    computeEquationFunction = ComputeEquationPart2;
    cout << "Sum of equations (Part 2): " << ComputeSumOfEquations(parseResult, computeEquationFunction) << endl;
    cout << endl;

    return 0;
}
