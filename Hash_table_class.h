#pragma once
#include <string>
#include <list>

typedef struct Person
{
	Person()
	{
		age = -1;
	}
	Person(int a, std::string b)
	{
		age = a;
		phonenumber = b;
	}
	friend bool operator==(const Person a, const Person b);
	friend bool operator!=(const Person a, const Person b);
	int age;
	std::string phonenumber;
}Person;

typedef std::string Key;
typedef Person Value;
typedef std::pair<Key, Value> DataPair;
typedef std::list<DataPair> Pairs;


class Hash_Table
{
public:
	Hash_Table();
	Hash_Table(const Hash_Table& table);
	Hash_Table(Hash_Table&& table) noexcept;

	Hash_Table& operator=(const Hash_Table& table);
	Hash_Table& operator=(Hash_Table&& table) noexcept;

	~Hash_Table();

	void Swap(Hash_Table& b);

	void Clear();

	void Insert(const Key& name, Value& data);

	bool Erase(const Key& name);

	bool Empty()
	{
		return (usedplaces == 0);
	}

	size_t Size()
	{
		return usedplaces;
	}

	bool Contains(const Key& name);

	DataPair& operator[](const Key& name);

	DataPair& At(Key& name);
	const DataPair& At(const Key& name) const;

	friend bool operator==(const Hash_Table& a, const Hash_Table& b);
	friend bool operator!=(const Hash_Table& a, const Hash_Table& b);

private:
	size_t hashtablesize;
	size_t usedplaces;
	Pairs* hashtable;

	std::pair<size_t, Pairs::iterator> Find(const Key& name) const;
	
	size_t CalcHash(const Key& name) const;

	void ResizeHashtable();

	void DoubleHashTableSize();
};
