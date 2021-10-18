#include <iostream>
#include "Hash_table_class.h"
#include <string>
#include <fstream>

using namespace std;

Hash_Table MakeNew()
{
    Hash_Table table;
    Person person(10, "8800");
    table.Insert("Fedor", person);
    return table;
}
int main()
{
    Hash_Table table = MakeNew();
    string name1 = "Fedor";
    cout << table.At(name1).second.age << endl;
    ifstream input;
    input.open("input.txt");
    for (int i = 0; i < 275; i++)
    {
        Person chel;
        chel.age = i;
        chel.phonenumber = (char)(i + 1);
        chel.phonenumber += (char)(i + 1) * (i + 1);
        string name;
        getline(input, name);
        table.Insert(name, chel);
    }
    Hash_Table table1 = table;
    table1.Swap(table);
    cout << table["Adam Hunter"].second.phonenumber << endl;
    cout << table.Erase("Adam Hunter") << endl;
    cout << table.Contains("Adam Hunter") << endl;
    cout << table["Adam Hunter"].second.phonenumber << endl;
    //cout << table.At("Adam Hunter").second.age << endl;
    table1.Clear();
    input.close();
}
