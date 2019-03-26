#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

// Structure of the attributes that are to be inserted into the table
struct attr{
	string attr_name;		//Attribute name of the attr
	string attr_val;		//Attribute value
	string data_type;		//Datatype of attribute to be insereted
};

// Structure of the attribute when the table is created
struct st{
	string attr_name;		//Name of the attribute
	string data_type;		//Datatype of the attribute
	string constraint;		//Constraint(NULL/NOTNULL)
	bool primary_key;		//check if primary key or not
};

// Structure of each table
struct  table{
	string table_name;		//Table name
	vector< st* > table_st;		//Table structure at the time of creation of table
	vector<vector< attr* > > tup;	//Structure of each row
	vector< attr* > temp_tup;	//Temporary row to validate the inserting row before insertion
};

// Structure of each database
struct database{
	string database_name;		//Database name
	vector< table* > table_list;	//List of table that the database consists
};

// Structure of the list of databases 
struct database_list{			
	vector < database* > list;	//List of databases that are created
};

//Structure to verify the constraints of each value to be insereted
struct node{
	string data_type;		//Datatype of the value or attribute to be inserted into the table
	string constraint;		//Constraint to check if it allows NULL values or not
	bool primary_key;
};

//Structure of table formed by join of selected tables in select statement
struct join_table{
	vector< st* > table_st;
	vector<vector<vector<vector<attr*> > > >  join_sel_tables;
};

//structure of each condition statement
struct condition{
	string tab_attr1;	//if condition involes tab_attribute
	string tab_attr2;	//if condition involes tab_attribute
	string nor_val;		//if condition involes normal value
	string oper;		//operator involving in the condition
	string data_type;	//data_type of the nor_val
};

//Function to check if the row to be insereted has the structure same as the table
bool check_insertion(vector<attr*> tup, table* curr_table);

//Function to check if the database has been created or not
bool check_database(string temp,database_list* temp_list);

//Function to check if the table is present in the database that is being used
bool check_table(string curr_table,database_list* temp_list,string curr_database);

//Function to check if the table that is created meets the primary key constraints
bool check_creation(table* curr_table);

//Function to retreive the table from the database that is being used
table* get_table(string curr_table,database_list* temp_list,string curr_database);

//Function to retreive the database from the list of databases
database* get_database(string curr_database,database_list* temp_list);

//Function to print the databases present 
void print_databases(database_list* temp_list);

//Function to print the tables present in the currently used database
void print_tables(database_list* temp_list,string curr_database);

//Function to print the meta data of the table
void print_tuple_st(string curr_table,database_list* temp_list,string curr_database);

//Function to print the rows present in the table of currently used database
void print_tuples(string curr_table,database_list* temp_list,string curr_database);

//Function to print the detais neatly
void print_format(int count,string s);

//Function to print the detais neatly
void print_sequence(int count);

//Function to create a table from all selected tables by using join and return pointer to that newly created table
join_table* select_all(vector<table*> sel_tables,database_list* temp_list);

//Function to print all the selected attributes
void print_all(join_table* temp_join);

//Function to check the number of sel_attributes that are present in the join table
int no_select_ids(join_table* temp_join,vector<string> sel_attributes);

//
join_table* select_ids(join_table* temp_join,vector<string> sel_attributes);

//Function to delete the database from the list of databases
void drop_database(string name,database_list* temp_list);

//Function to delete the table from the current database
void drop_table(string curr_table,database_list* temp_list,string curr_database);

//checks if the attr is a primary_key or not
bool check_primary(vector<attr*>tup, table* curr_table);

//returns a table that satisfies the condition in select statement from the table formed by join of selected tables
join_table* select_cond(join_table* temp_join,condition* temp_cond);

//deletes the row that satisffies the condition
void delete_tuple(table* curr_table,condition* temp_cond);

//gets the index of the table in which the attribute is present
int get_index(vector<vector<vector<vector<attr*> > > > temp,string attr_name);
