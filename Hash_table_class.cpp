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
		DoubleSizeHashTable();
	}
	keys[usedplaces] = name;
	indexes[usedplaces] = CalcHash(name, hashtablesize);
	std::cout << indexes[usedplaces] << std::endl;
	hashtable[indexes[usedplaces]] = data;
	usedplaces++;
}

void Hash_Table::Insert(Key name, int age, std::string phonenumber)
{
	if (usedplaces >= hashtablesize * 0.75)
	{
		DoubleSizeHashTable();
	}
	keys[usedplaces] = name;
	indexes[usedplaces] = CalcHash(name, hashtablesize);
	Value person;
	person.age = age;
	person.phonenumber = phonenumber;
	hashtable[indexes[usedplaces]] = person;
	usedplaces++;
}

bool Hash_Table::Erase(Key name)
{
	size_t index = Find(name);
	if (index == -1)
	{
		return false;
	}
	size_t hash = indexes[Find(name)];
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

size_t Hash_Table::CalcHash(Key name, size_t sizeoftable)
{
	size_t index = 0;
	int length = name.length();
	size_t  multiplier = 1;
	for (int i = 0; i < length; i++)
	{
		index += (unsigned char)name[i] * multiplier;
		multiplier *= 117;
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
	for (size_t i = 0; i < hashtablesize / 2; i++)
	{
		newhashtable[i] = hashtable[i];
	}
	for (size_t i = 0; i < usedplaces; i++)
	{
		newindexes[i] = CalcHash(keys[i], hashtablesize);
		newhashtable[newindexes[i]] = hashtable[indexes[i]];
	}
	delete[] hashtable;
	hashtable = newhashtable;
	delete[] indexes;
	indexes = newindexes;
}

void Hash_Table::DoubleSizeHashTable()
{
	size_t size = hashtablesize;
	if (size == 0)
	{
		size = 1;
	}
	hashtablesize = size * 2;
	ResizeKeys();
}

Value Hash_Table::GetData(Key name)
{
	size_t hashedkey = -1;
	for (size_t i = 0; i < usedplaces; i++)
	{
		if (keys[i] == name)
		{
			hashedkey = indexes[i];
			break;
		}
	}
	if (hashedkey == -1)
	{
		Person person;
		return person;
	}
	return hashtable[hashedkey];
}
