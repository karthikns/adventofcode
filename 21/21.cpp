#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Item
{
public:
    Item() = default;

    Item(const string& rawInput)
    {
        const size_t delimiterPosition = rawInput.find('(');
        const string ingredientsRawString = rawInput.substr(0, delimiterPosition);
        const string AllergensRawString = rawInput.substr(delimiterPosition + 1, rawInput.length() - delimiterPosition - 2);

        cout << ingredientsRawString << endl;
        cout << AllergensRawString << endl;
        cout << endl;

        stringstream ingredientsStream(ingredientsRawString);
        while (!ingredientsStream.eof())
        {
            string ingredient;
            ingredientsStream >> ingredient;

            if (ingredient.empty())
                continue;

            ingredients.emplace_back(ingredient);
        }
        sort(begin(ingredients), end(ingredients));

        stringstream allergensStream(AllergensRawString);
        while (!allergensStream.eof())
        {
            string allergen;
            allergensStream >> allergen;

            if (allergen.empty())
                continue;

            allergens.emplace_back(allergen);
        }
        sort(begin(allergens), end(allergens));
    }

    vector<string> GetIngredients() const
    {
        return ingredients;
    }

    vector<string> GetAllergens() const
    {
        return allergens;
    }

    bool HasIngredient(const string& ingredient) const
    {
        return find(begin(ingredients), end(ingredients), ingredient) != end(ingredients);
    }

    bool HasAllergen(const string& Allergen) const
    {
        return find(begin(allergens), end(allergens), Allergen) != end(allergens);
    }

    void RemoveIngredient(const string& ingredient)
    {
        if (!HasIngredient(ingredient))
        {
            return;
        }

        ingredients.erase(find(begin(ingredients), end(ingredients), ingredient));
    }

    void RemoveAllergen(const string& allergen)
    {
        if (!HasAllergen(allergen))
        {
            return;
        }

        ingredients.erase(find(begin(allergens), end(allergens), allergen));
    }

    size_t GetIngredientCount() const
    {
        return ingredients.size();
    }

private:
    vector<string> ingredients;
    vector<string> allergens;
 };

vector<Item> ParseInputFile(const string& inputFileName)
{
    vector<Item> items;

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
            Item item(line);
            items.emplace_back(item);
        }
    }
    catch (...)
    {
        cout << "File parse error" << endl;
        exit(-1);
    }

    return items;
}

vector<string> GetIngredientsInItems(const vector<Item>& items)
{
    set<string> allIngredientsSet;
    for (const Item& item : items)
    {
        vector<string> itemIngredient = item.GetIngredients();
        allIngredientsSet.insert(begin(itemIngredient), end(itemIngredient));
    }

    vector<string> allIngredientsVector(begin(allIngredientsSet), end(allIngredientsSet));
    sort(begin(allIngredientsVector), end(allIngredientsVector));
    return allIngredientsVector;
}

void RemoveIngredientFromItems(const string& ingredientToRemove, vector<Item>& items)
{
    for (Item& item : items)
        item.RemoveIngredient(ingredientToRemove);
}

size_t GetSumOfIngedientsCountInItems(const vector<Item>& items)
{
    size_t sumOfIngredientsCountInItems = 0;
    for (const Item& item : items)
        sumOfIngredientsCountInItems += item.GetIngredientCount();

    return sumOfIngredientsCountInItems;
}

map<string, string> BuildAllergenIngredientMapping(const vector<Item>& items)
{
    set<string> allAllergens;
    for (const Item& item : items)
    {
        vector<string> itemAllergens = item.GetAllergens();
        allAllergens.insert(begin(itemAllergens), end(itemAllergens));
    }

    map<string, string> allergenToIngredientMap;

    vector<Item> itemsToProcess = items;
    set<string> allergensToProcess = allAllergens;

    while (!allergensToProcess.empty())
    {
        bool isAllergenFound = false;
        string allergenFound, ingredientFound;
        for (const string allergen : allergensToProcess)
        {
            vector<string> potentialIngredients = GetIngredientsInItems(itemsToProcess);
            for (const Item& item : itemsToProcess)
            {
                if (!item.HasAllergen(allergen))
                    continue;

                vector<string> itemIngredients = item.GetIngredients();
                vector<string> intersectionIngredients(itemIngredients.size());
                vector<string>::iterator nextPlaceToInsert = set_intersection(
                    begin(potentialIngredients), end(potentialIngredients),
                    begin(itemIngredients), end(itemIngredients),
                    begin(intersectionIngredients));

                const size_t numberOfItemsInIntersection = distance(begin(intersectionIngredients), nextPlaceToInsert);
                intersectionIngredients.resize(numberOfItemsInIntersection);

                potentialIngredients = move(intersectionIngredients);

                if (potentialIngredients.size() == 1)
                {
                    isAllergenFound = true;
                    allergenFound = allergen;
                    ingredientFound = potentialIngredients[0];
                    //cout << allergen << " " << potentialIngredients[0] << endl;
                    break;
                }
            }

            if (isAllergenFound)
            {
                break;
            }
        }

        if (!isAllergenFound)
        {
            cout << "Potential infinite loop" << endl;
            return {};
        }

        allergenToIngredientMap[allergenFound] = ingredientFound;
        allergensToProcess.erase(allergensToProcess.find(allergenFound));
        RemoveIngredientFromItems(ingredientFound, itemsToProcess);
    }

    return allergenToIngredientMap;
}

size_t GetNumberOfIngredientsThatDontHaveAnAllergen(const vector<Item>& items, const map<string, string>& allergenToIngredientMap)
{
    vector<Item> itemsWithoutAllergenIngredients = items;
    for (auto mapIterator : allergenToIngredientMap)
        RemoveIngredientFromItems(mapIterator.second, itemsWithoutAllergenIngredients);

    return GetSumOfIngedientsCountInItems(itemsWithoutAllergenIngredients);
}

string ConcatenateIngredientNames(const map<string, string>& allergenToIngredientMap)
{
    string result;

    if (allergenToIngredientMap.empty())
        return {};

    for (auto allergenToIngredientPair : allergenToIngredientMap)
    {
        result += allergenToIngredientPair.second + ',';
    }

    return result.substr(0, result.size() - 1);
}

int main()
{
    const string inputFileName(R"(21_processed_in.txt)");

    vector<Item> items = ParseInputFile(inputFileName);
    cout << endl;

    cout << "Items size: " << items.size() << endl;
    cout << endl;

    map<string, string> allergenToIngredientMap = BuildAllergenIngredientMapping(items);

    cout << "Number Of Ingredients That Dont Have An Allergen: " << GetNumberOfIngredientsThatDontHaveAnAllergen(items, allergenToIngredientMap) << endl;
    cout << endl;

    cout << "Ingredient names sorted by their allergen names lexicographically: " << ConcatenateIngredientNames(allergenToIngredientMap);
    cout << endl;

    return 0;
}
