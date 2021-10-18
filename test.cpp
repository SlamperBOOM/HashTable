#include "pch.h"
#include <string>
#include <fstream>
#include <exception>
#include "D:\Labs\Programs\Hash_table\Hash_table_class.h"
#include "D:\Labs\Programs\Hash_table\Hash_table_class.cpp"

class Hash_Table_Fixture:public::testing::Test
{
protected:

	Hash_Table table1; //empty
	Hash_Table table2; //filled

	void SetUp()
	{
		std::ifstream input;
		input.open("D:/Labs/Programs/Hash_table/input.txt");
		for (int i = 0; i < 275; i++)
		{
			Person chel;
			chel.age = i;
			chel.phonenumber = (char)(i + 1);
			chel.phonenumber += (char)(i + 1) * (i + 1);
			std::string name;
			getline(input, name);
			table2.Insert(name, chel);
		}
	}

	void TearDown()
	{

	}
};

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST_F(Hash_Table_Fixture, IsEmpty) {
	EXPECT_TRUE(table1.Empty());
	EXPECT_FALSE(table2.Empty());
}

TEST_F(Hash_Table_Fixture, IsErased) {
	EXPECT_TRUE(table2.Erase("Adam Hunter"));
	EXPECT_FALSE(table1.Erase("Adam Hunter"));
}

TEST_F(Hash_Table_Fixture, CorrectSize)
{
	EXPECT_EQ(table1.Size(), 0);
	EXPECT_EQ(table2.Size(), 275);
}

TEST_F(Hash_Table_Fixture, CorrectValue_operator)
{
	EXPECT_EQ(table2["Adam Hunter"].second.phonenumber, ",");
	EXPECT_EQ(table1["Adam Hunter"].second.phonenumber, "");
}

TEST_F(Hash_Table_Fixture, CorrectValue_At)
{
	EXPECT_EQ(table2.At("Adam Hunter").second.phonenumber, ",");
	EXPECT_THROW(table1.At("Adam Hunter").second.phonenumber, std::out_of_range);
}

TEST_F(Hash_Table_Fixture, Operator_equality)
{
	EXPECT_TRUE(table1 == table1);
	EXPECT_TRUE(table2 == table2);
	EXPECT_FALSE(table1 == table2);
	EXPECT_TRUE(table1 != table2);
}

TEST_F(Hash_Table_Fixture, IsContains)
{
	EXPECT_FALSE(table1.Contains("Adam Hunter"));
	EXPECT_TRUE(table2.Contains("Adam Hunter"));
}

TEST_F(Hash_Table_Fixture, Swap)
{
	Hash_Table newtable1 = table1;
	Hash_Table newtable2 = table2;
	newtable1.Swap(newtable2);
	EXPECT_TRUE(newtable1 == table2);
	EXPECT_TRUE(newtable2 == table1);
}