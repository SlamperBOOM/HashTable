#pragma once
#include <string>

typedef struct Person
{
	int age = -1;
	std::string phonenumber;
}Person;

typedef std::string Key;
typedef Person Value;

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

	void Insert(Key name, Value data);
	void Insert(Key name, int age, std::string phonenumber);

	bool Erase(Key name);

	bool Empty()
	{
		return (usedplaces == 0);
	}

	size_t Size()
	{
		return usedplaces;
	}

	bool Contains(Key name)
	{
		int index = Find(name);
		return !(index == -1);
	}

	Value& operator[](Key name)
	{
		Value data = GetData(name);
		return data;
	}

	Value& At(const Key name);
	const Value& At(const Key name) const;


	friend bool operator==(const Hash_Table& a, const Hash_Table& b);
	friend bool operator!=(const Hash_Table& a, const Hash_Table& b);

private:
	size_t hashtablesize;
	size_t usedplaces;
	std::string* keys;
	Value *hashtable;
	size_t* indexes;
	int collisioncount = 0;

	size_t Find(Key name);
	
	size_t CalcHash(Key name, Value* hashtable, size_t sizeofhash);

	void ResizeKeys();

	void ResizeHashtable();

	void DoubleHashTableSize();

	Value& GetData(Key name);
};

