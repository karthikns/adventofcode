#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

vector<map<string, string>> GetInput(const string& inputFileName)
{
    //fstream fs(inputFileName);
    //string s;
    //fs >> s;
    //cout << s;
    //return {};

    vector<map<string, string>> passports;

    try
    {
        fstream inputFile(inputFileName);

        map<string, string> passport;
        while (inputFile.good())
        {
            string passportField;
            inputFile >> passportField;

            if (passportField == "~")
            {
                passports.push_back(passport);
                passport.clear();
                continue;
            }

            const size_t delimiterPosition = passportField.find(':');
            const string key = passportField.substr(0, delimiterPosition);
            const string value = passportField.substr(delimiterPosition + 1);

            if (!(key == "byr" || key == "iyr" || key == "eyr" || key == "hgt" || key == "hcl" || key == "ecl" || key == "pid" || key == "cid"))
            {
                cout << "Invalid key detected" << endl;
            }

            if (passport.find(key) != end(passport))
            {
                cout << "Duplicate" << endl;
            }

            passport.emplace(key, value);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return passports;
}

bool ValidatePassportFields(const map<string, string>& passport)
{
    {
        auto it = passport.find("pid");
        if (!regex_match(it->second, regex("^[0-9]{9}$")))
        {
            //cout << it->second << endl;
            return false;
        }
    }

    {
        auto it = passport.find("ecl");
        if (!regex_match(it->second, regex("^(amb)|(blu)|(brn)|(gry)|(grn)|(hzl)|(oth)$")))
        {
            //cout << it->second << endl;
            return false;
        }
    }

    {
        auto it = passport.find("hcl");
        if (!regex_match(it->second, regex("#[0-9a-f]{6}")))
        {
            //cout << it->second << endl;
            return false;
        }
    }

    {
        // BUG BUG
        auto it = passport.find("hgt");
        if (!regex_match(it->second, regex("([0-9]{3}(cm))|([0-9]{2}(in))")))
        {
            //cout << it->second << endl;
            return false;
        }
    }

    {
        auto it = passport.find("eyr");
        if (!regex_match(it->second, regex("20((2[0-9])|(30))")))
        {
            //cout << it->second << endl;
            return false;
        }
    }

    {
        auto it = passport.find("iyr");
        if (!regex_match(it->second, regex("20((1[0-9])|(20))")))
        {
            //cout << it->second << endl;
            return false;
        }
    }

    {
        auto byr = passport.find("byr");
        if (!regex_match(byr->second, regex("[0-9]{4}")))
        {
            //cout << it->second << endl;
            return false;
        }

        int byrNumber = stoi(byr->second);
        if (byrNumber < 1920 || byrNumber > 2002)
            return false;
    }

    return true;
}

bool IsPassportValid(const map<string, string>& passport)
{
    if (passport.size() == 8)
        return ValidatePassportFields(passport);

    if (passport.size() == 7 && passport.find("cid") == end(passport))
        return ValidatePassportFields(passport);

    //cout << passport.size() << ": ";
    //decltype(passport.begin()) it = passport.find("pid");
    //string pid;
    //if (it != end(passport))
    //{
    //    pid = it -> second;
    //    cout << pid << endl;
    //}
    //else
    //{
    //    cout << "no pid" << endl;
    //}

    return false;
}

size_t CountValidPassports(const vector<map<string, string>>& passports)
{
    size_t numberOfValidPassports = 0;

    for (const map<string, string>& passport : passports)
    {
        if(IsPassportValid(passport))
        {
            ++numberOfValidPassports;
        }
    }

    return numberOfValidPassports;
}

// 171 is less

int main()
{
    const string inputFileName(R"(4_in_processed.txt)");
    //const string inputFileName(R"(4_sample_in_processed.txt)");
    vector<map<string, string>> passports = GetInput(inputFileName);

    cout << passports.size() << endl;

    const size_t numberOfValidPassports = CountValidPassports(passports);

    cout << endl << numberOfValidPassports << endl;

    return 0;
}
