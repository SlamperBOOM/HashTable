#pragma once
#include <string>

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
	int age;
	std::string phonenumber;
}Person;

typedef std::string Key;
typedef Person Value;

class Hash_Table
{
public:
	class iterator 
	{
	public:

		iterator()
		{
			index = 0;
		}
		iterator(int a)
		{
			index = a;
		}
		iterator(const iterator& it)
		{
			index = it.index;
		}
		iterator(iterator&& it)
		{
			index = it.index;
		}

		void operator++();
		void operator--();

		void operator+(int a);
		void operator-(int a);

		void operator+=(int a);
		void operator-=(int a);

		Value& operator*();

	private:
		size_t index;

		Value& GetDataFromClass(int a);
	};

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

	Value& operator[](const Key& name)
	{
		Value& data = GetData(name);
		return data;
	}

	Value& At(Key& name);
	const Value& At(const Key& name) const;

	friend bool operator==(const Hash_Table& a, const Hash_Table& b);
	friend bool operator!=(const Hash_Table& a, const Hash_Table& b);


	iterator Begin()
	{
		return iterator(0);
	}

	iterator End()
	{
		return iterator(usedplaces);
	}

private:
	size_t hashtablesize;
	size_t usedplaces;
	std::string* keys;
	Value *hashtable;
	size_t* indexes;
	Value defaultvalue;
	const Value defaultconstvalue;
	int collisioncount = 0;

	size_t Find(Key& name);
	size_t Find(const Key& name) const;
	
	size_t CalcHash(Key& name, Value* hashtable, size_t sizeofhash);

	void ResizeKeys();

	void ResizeHashtable();

	void DoubleHashTableSize();

	Value& GetData(const Key& name);

	Value& GetData(const size_t& position);
};

