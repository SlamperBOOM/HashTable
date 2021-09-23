#include <string>
#include <iostream>
#include "Hash_table_class.h"

Hash_Table::Hash_Table()
{
	hashtablesize = 100;
	keys = new std::string[hashtablesize];
	hashtable = new Value[hashtablesize];
	indexes = new size_t[hashtablesize];
	usedplaces = 0;
}

Hash_Table::Hash_Table(const Hash_Table& table)
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	this->keys = new std::string[hashtablesize];
	this->hashtable = new Value[hashtablesize];
	this->indexes = new size_t[hashtablesize];
	for (size_t i = 0; i < hashtablesize; i++)
	{
		this->keys[i] = table.keys[i];
		this->hashtable[i] = table.hashtable[i];
		this->indexes[i] = table.indexes[i];
	}
}

Hash_Table::Hash_Table(Hash_Table&& table)
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	this->keys = table.keys;
	this->hashtable = table.hashtable;
	this->indexes = table.indexes;

	table.hashtablesize = 0;
	table.usedplaces = 0;
	table.indexes = nullptr;
	table.hashtable = nullptr;
	table.keys = nullptr;
}

Hash_Table& Hash_Table::operator=(const Hash_Table& table)
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	delete[] this->keys;
	this->keys = new std::string[hashtablesize];
	delete[] this->hashtable;
	this->hashtable = new Value[hashtablesize];
	delete[] this->indexes;
	this->indexes = new size_t[hashtablesize];
	for (size_t i = 0; i < hashtablesize; i++)
	{
		this->keys[i] = table.keys[i];
		this->hashtable[i] = table.hashtable[i];
		this->indexes[i] = table.indexes[i];
	}
	return *this;
}

Hash_Table& Hash_Table::operator=(Hash_Table&& table)
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	this->keys = table.keys;
	this->hashtable = table.hashtable;
	this->indexes = table.indexes;

	table.hashtablesize = 0;
	table.usedplaces = 0;
	table.indexes = nullptr;
	table.hashtable = nullptr;
	table.keys = nullptr;
	return *this;
}

Hash_Table::~Hash_Table()
{
	delete[] keys;
	delete[] hashtable;
	delete[] indexes;
}

void Hash_Table::swap(Hash_Table& table)
{
	Hash_Table a = table;
	table.hashtablesize = this->hashtablesize;
	table.usedplaces = this->usedplaces;
	table.keys = this->keys;
	table.indexes = this->indexes;
	table.hashtable = this->hashtable;

	this->hashtablesize = a.hashtablesize;
	this->usedplaces = a.usedplaces;
	this->keys = a.keys;
	this->indexes = a.indexes;
	this->hashtable = a.hashtable;
}

void Hash_Table::Clear()
{
	delete[] hashtable;
	delete[] keys;
	delete[] indexes;
	hashtablesize = 100;
	hashtable = new Value[hashtablesize];
	keys = new std::string[hashtablesize];
	indexes = new size_t[hashtablesize];
}

void Hash_Table::Insert(Key name, Value data)
{
	if (usedplaces >= hashtablesize * 0.75)
	{
		DoubleHashTableSize();
	}
	keys[usedplaces] = name;
	indexes[usedplaces] = CalcHash(name, hashtable, hashtablesize);
	hashtable[indexes[usedplaces]] = data;
	usedplaces++;
}

void Hash_Table::Insert(Key name, int age, std::string phonenumber)
{
	Value person;
	person.age = age;
	person.phonenumber = phonenumber;
	Insert(name, person);
}

bool Hash_Table::Erase(Key name)
{
	size_t index = Find(name);
	if (index == -1)
	{
		return false;
	}
	size_t hash = indexes[index];
	hashtable[hash].age = -1;
	hashtable[hash].phonenumber = "";
	size_t i = 0;
	while (keys[i] != name)
	{
		i++;
	}
	while (i < usedplaces)
	{
		keys[i] = keys[i + 1];
		indexes[i] = indexes[i + 1];
		i++;
	}
	usedplaces--;
	return true;
}

size_t Hash_Table::Find(Key name)
{
	for (size_t i = 0; i < usedplaces; i++)
	{
		if (keys[i] == name)
		{
			return i;
		}
	}
	return -1;
}

size_t Hash_Table::CalcHash(Key name, Value* usinghashtable, size_t sizeoftable)
{
	unsigned long long index = 0;
	int length = name.length();
	unsigned long long  multiplier = 1;
	for (int i = 0; i < length; i++)
	{
		index += (unsigned char)name[i] * multiplier;
		multiplier *= 37;
	}
	bool yes = false;
	while (true)
	{
		if (usinghashtable[index % hashtablesize].age == -1)
		{
			if (yes) collisioncount++;
			break;
		}
		index++;
		yes = true;
	}
	return index % sizeoftable;
}

void Hash_Table::ResizeKeys()
{
	std::string* keysarray = new std::string[hashtablesize];
	for (size_t i = 0; i < hashtablesize / 2; i++)
	{
		keysarray[i] = keys[i];
	}
	delete[] keys;
	keys = keysarray;
	ResizeHashtable();
}

void Hash_Table::ResizeHashtable()
{
	Value* newhashtable = new Value[hashtablesize];
	size_t* newindexes = new size_t[hashtablesize];
	for (size_t i = 0; i < usedplaces; i++)
	{
		newindexes[i] = CalcHash(keys[i], newhashtable, hashtablesize);
		newhashtable[newindexes[i]] = hashtable[indexes[i]];
	}
	delete[] hashtable;
	hashtable = newhashtable;
	delete[] indexes;
	indexes = newindexes;
}

void Hash_Table::DoubleHashTableSize()
{
	size_t size = hashtablesize;
	if (size == 0)
	{
		size = 1;
	}
	hashtablesize = size * 2;
	ResizeKeys();
	std::cout << "colissions: " << collisioncount << std::endl;
	collisioncount = 0;
}

Value Hash_Table::GetData(Key name)
{
	size_t position = Find(name);
	if (position == -1)
	{
		Person person;
		return person;
	}
	else
	{
		return hashtable[indexes[position]];
	}
}
