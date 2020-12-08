#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const string c_jmp("jmp");
const string c_nop("nop");
const string c_acc("acc");

struct Instruction
{
    string opcode;
    int32_t value;
};

vector<Instruction> Parse(const string& inputFileName)
{
    vector<Instruction> instructions;

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

            size_t spaceDelimiter = line.find(' ');
            string opcode;
            int32_t value{};
            if (spaceDelimiter == string::npos)
            {
                opcode = line;
            }
            else
            {
                opcode = line.substr(0, spaceDelimiter);
                string valueString = line.substr(spaceDelimiter + 1);
                value = stoi(valueString);
            }

            instructions.push_back({opcode, value});
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return instructions;
}

int32_t ProcessInstructionsPart1(const vector<Instruction>& instructions, const int32_t initialAccumulator)
{
    int32_t accumulator = initialAccumulator;
    vector<bool> instructionsVisited(instructions.size(), false);


    size_t ip = 0;
    while (true)
    {
        if (instructionsVisited[ip] == true)
        {
            cout << "Breaking when trying to revisit instruction " << ip << endl;
            break;
        }


        instructionsVisited[ip] = true;
        const Instruction& instruction = instructions[ip];

        //cout << ip << ": " << instruction.opcode << " " << instruction.value << " " << accumulator << endl;


        if (instruction.opcode == c_nop)
        {
            ++ip;
            continue;
        }

        if (instruction.opcode == c_acc)
        {
            ++ip;
            accumulator += instruction.value;
            continue;
        }

        if (instruction.opcode == c_jmp)
        {
            ip += instruction.value;
            continue;
        }

        cout << "Unknown instruction: " << instruction.value << endl;
    }


    return accumulator;
}

int32_t ProcessInstructionsPart2(const vector<Instruction>& instructions, const int32_t initialAccumulator)
{
    for (size_t index = 0; index < instructions.size(); ++index)
    {
        if (instructions[index].opcode == "c_acc")
            continue;

        int32_t accumulator = initialAccumulator;
        vector<bool> instructionsVisited(instructions.size(), false);
        
        size_t ip = 0;
        while (true)
        {
            if (ip == instructions.size())
            {
                cout << "Graceful exit. Changed instruction: " << index << ". Accumulator value: " << accumulator << endl;
                return accumulator;
                //break;
            }

            if (instructionsVisited[ip] == true)
            {
                //cout << "Breaking when trying to revisit instruction " << ip << endl;
                break;
            }


            instructionsVisited[ip] = true;
            Instruction instruction = instructions[ip];

            //cout << ip << ": " << instruction.opcode << " " << instruction.value << " " << accumulator << endl;
            if (ip == index)
            {
                if (instruction.opcode == c_nop)
                {
                    instruction.opcode = c_jmp;
                }
                else if (instruction.opcode == c_jmp)
                {
                    instruction.opcode = c_nop;
                }
            }

            if (instruction.opcode == c_nop)
            {
                ++ip;
                continue;
            }

            if (instruction.opcode == c_acc)
            {
                ++ip;
                accumulator += instruction.value;
                continue;
            }

            if (instruction.opcode == c_jmp)
            {
                ip += instruction.value;
                continue;
            }

            cout << "Unknown instruction: " << instruction.value << endl;
        }
    }

    cout << "Failed to halt program" << endl;
    return 0;
}

int main()
{
    const string inputFileName(R"(8_in.txt)");
    //const string inputFileName(R"(8_sample_in.txt)");

    vector<Instruction> instructions = Parse(inputFileName);
    cout << "Number of instructions: " << instructions.size() << endl << endl;

    {
        int32_t finalAccumulator = ProcessInstructionsPart1(instructions, 0);
        cout << "Final accumulator: " << finalAccumulator << endl << endl;
    }

    {
        int32_t finalAccumulator = ProcessInstructionsPart2(instructions, 0);
        cout << "Final accumulator: " << finalAccumulator << endl << endl;
    }

    return 0;
}
