#include <string>
#include <exception>
#include <iostream>
#include "Hash_table_class.h"

bool operator==(const Person a, const Person b)
{
	if (a.age != b.age)
	{
		return false;
	}
	if (a.phonenumber != b.phonenumber)
	{
		return false;
	}
	return true;
}

bool operator!=(const Person a, const Person b)
{
	return !(a == b);
}

Hash_Table::Hash_Table()
{
	hashtablesize = 100;
	hashtable = new Pairs[hashtablesize];
	usedplaces = 0;
}

Hash_Table::Hash_Table(const Hash_Table& table)
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	this->hashtable = new Pairs[this->hashtablesize];
	for (size_t i = 0; i < this->hashtablesize; i++)
	{
		this->hashtable[i] = table.hashtable[i];
	}
}

Hash_Table::Hash_Table(Hash_Table&& table) noexcept
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	this->hashtable = table.hashtable;

	table.hashtablesize = 0;
	table.hashtable = nullptr;
}

Hash_Table& Hash_Table::operator=(const Hash_Table& table)
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	for (size_t i = 0; i < hashtablesize; i++)
	{
		this->hashtable[i].clear();
	}
	delete[] this->hashtable;
	this->hashtable = new Pairs[this->hashtablesize];
	for (size_t i = 0; i < this->hashtablesize; i++)
	{
		this->hashtable[i] = table.hashtable[i];
	}
	return *this;
}

Hash_Table& Hash_Table::operator=(Hash_Table&& table) noexcept
{
	this->hashtablesize = table.hashtablesize;
	this->usedplaces = table.usedplaces;
	delete[] this->hashtable;
	this->hashtable = table.hashtable;

	table.hashtable = nullptr;
	return *this;
}

Hash_Table::~Hash_Table()
{
	for (size_t i = 0; i < hashtablesize; i++)
	{
		hashtable[i].clear();
	}
	delete[] hashtable;
}

void Hash_Table::Swap(Hash_Table& table)
{
	std::swap(this->hashtablesize, table.hashtablesize);
	std::swap(this->usedplaces, table.usedplaces);
	std::swap(this->hashtable, table.hashtable);
}

void Hash_Table::Clear()
{
	for (size_t i = 0; i < hashtablesize; i++)
	{
		hashtable[i].clear();
	}
	delete[] hashtable;
	hashtablesize = 100;
	hashtable = new Pairs[hashtablesize];
	usedplaces = 0;
}

bool Hash_Table::Insert(const Key& name, Value& data)
{
	if (usedplaces >= hashtablesize * 0.75)
	{
		DoubleHashTableSize();
	}
	std::pair<size_t, Pairs::iterator> index = Find(name);
	if (hashtable[index.first].end() != index.second)
	{
		return false;
	}
	DataPair pair = std::make_pair(name, data);
	hashtable[index.first].push_back(pair);
	usedplaces++;
	return true;
}

bool Hash_Table::Erase(const Key& name)
{
	std::pair<size_t, Pairs::iterator> index = Find(name);
	Pairs& list = hashtable[index.first];
	if (hashtable[index.first].end() == index.second)
	{
		return false;
	}
	list.erase(index.second);
	if (list.size() == 0)
	{
		list.push_back(DataPair());
	}
	return true;
}

bool Hash_Table::Contains(const Key& name)
{
	std::pair<size_t, Pairs::iterator> index = Find(name);
	if (hashtable[index.first].end() == index.second)
	{
		return false;
	}
	return true;
}

DataPair& Hash_Table::operator[](const Key& name)
{
	std::pair<size_t, Pairs::iterator> index = Find(name); 
	if (hashtable[index.first].end() == index.second)
	{
		hashtable[index.first].push_back(DataPair());
		return *(--hashtable[index.first].end());
	}
	DataPair& pair = *(index.second);
	return pair;
}

DataPair& Hash_Table::At(Key& name)
{
	std::pair<size_t, Pairs::iterator> index = Find(name);
	if (hashtable[index.first].end() == index.second)
	{
		throw std::out_of_range("Out of range");
	}
	DataPair& data = *(index.second);
	return data;
}

const DataPair& Hash_Table::At(const Key& name) const
{
	std::pair<size_t, Pairs::iterator> index = Find(name);
	if (hashtable[index.first].end() == index.second)
	{
		throw std::out_of_range("Out of range");
	}
	const DataPair& data = *(index.second);
	return data;
}

std::pair<size_t, Pairs::iterator> Hash_Table::Find(const Key& name) const
{
	long index = CalcHash(name);
	size_t size = hashtable[index].size();
	auto it = hashtable[index].begin();
	if (size > 0)
	{
		DataPair pair = *(it);
		auto end = hashtable[index].end();
		while (it != end)
		{
			pair = *(it);
			if (pair.first == name)
			{
				break;
			}
			it++;
		}
	}
	std::pair<size_t, Pairs::iterator> indexpair = std::make_pair(index, it);
	return indexpair;
}

size_t Hash_Table::CalcHash(const Key& name) const
{
	unsigned long long index = 0;
	int length = name.length();
	unsigned long long  multiplier = 1;
	for (int i = 0; i < length; i++)
	{
		index += (unsigned char)name[i] * multiplier;
		multiplier *= 37;
	}
	return index % hashtablesize;
}

void Hash_Table::ResizeHashtable()
{
	Pairs* newhashtable = new Pairs[hashtablesize];
	for (size_t i = 0; i < hashtablesize / 2; i++)
	{
		if (hashtable[i].size() > 0)
		{
			auto end = hashtable[i].end();
			for (auto it = hashtable[i].begin(); it != end; it++)
			{
				DataPair pair = *(it);
				size_t index = CalcHash(pair.first);
				if (newhashtable[index].size() == 1)
				{
					*(newhashtable[index].begin()) = pair;
				}
				else
				{
					newhashtable[index].push_back(pair);
				}
			}
		}/*
		else
		{
			DataPair pair = *(hashtable[i].begin());
			size_t index = CalcHash(pair.first);
			if (newhashtable[index].size() == 1)
			{
				*(newhashtable[index].begin()) = pair;
			}
			else
			{
				newhashtable[index].push_back(pair);
			}
		}*/
	}
	for (size_t i = 0; i < hashtablesize / 2; i++)
	{
		hashtable[i].clear();
	}
	delete[] hashtable;
	hashtable = newhashtable;
}

void Hash_Table::DoubleHashTableSize()
{
	size_t size = hashtablesize;
	if (size == 0)
	{
		size = 1;
	}
	hashtablesize = size * 2;
	ResizeHashtable();
}

bool operator==(const Hash_Table& a, const Hash_Table& b)
{
	if (a.hashtablesize != b.hashtablesize)
	{
		return false;
	}
	if (a.usedplaces != b.usedplaces)
	{
		return false;
	}
	for (size_t i = 0; i < a.hashtablesize; i++)
	{
		if (a.hashtable[i].size() != b.hashtable[i].size())
		{
			return false;
		}
		auto it_a = a.hashtable[i].begin();
		auto it_b = b.hashtable[i].begin();
		for (size_t j = 0; j < a.hashtable[i].size(); j++)
		{
			if (*(it_a) != *(it_b))
			{
				return false;
			}
		}
	}
	return true;
}

bool operator!=(const Hash_Table& a, const Hash_Table& b)
{
	return !(a == b);
}
