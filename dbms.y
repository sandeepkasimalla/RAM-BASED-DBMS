%{
	#include "dbms.h"
	using namespace std;
	database *temp_database = new database;
	table *curr_table ;
	table *temp_table = new table;
	database_list *temp_list = new database_list;
	vector< table* > sel_tables;
	vector<string> sel_attributes;
	string curr_database="";
	condition* temp_cond;
	int yylex(void);
	int yyerror(string);
%}

%union {
	char* p;
	node* con;
	attr* a;
}

%token <p> 	CREATE TABLE DATABASE SHOW INSERT DESCRIBE DROP DATA_TYPE INTO CONSTRAINT DELETE
%token <p> 	IDENTIFIER VALUES NUM USE DATABASES TABLES SELECT FROM WHERE REL_OP PRIMARY_KEY
%type  <con> 	OP_CON CR_ARG CREATE_ST
%type  <a>	INS_ARG INSERT_ST 

%%
PROGRAM:		ST 
			|ST PROGRAM
			;
ST:			CREATE_ST ';' 
			| INSERT_ST ';'
			| USE_ST ';'
			| SHOW_ST ';'
			| SELECT_ST ';'
			| DESCRIBE_ST ';'
			| DROP_ST ';'
			| DELETE_ST ';'
			| error ';' {yyerror("Invalid Query");}
			;
CREATE_ST:		CREATE DATABASE IDENTIFIER {
				if(!check_database($3,temp_list)){
					temp_database->database_name = $3;
					temp_list->list.push_back(temp_database);
					temp_database = new database;
				}
				else {
					cout<<"Database already present"<<endl;
				}
			}
			| CREATE TABLE IDENTIFIER {curr_table = new table;}'(' CR_ARG ')' {
				if(curr_database==""){
					cout<<"No database is being used"<<endl;
				}
				else{
					if(check_database(curr_database,temp_list)){
						curr_table->table_name = $3;
						if(check_creation(curr_table)){
							database* temp = new database;
							temp = get_database(curr_database,temp_list);
							temp->table_list.push_back(curr_table);
						}
						else{
							curr_table = new table;
							cout<<"Primary key constraints is not satisfying"<<endl;
						}
					}
					else{
						cout<<"No database is being used"<<endl;
					}
				}
			}
			;
CR_ARG:			IDENTIFIER OP_CON {
				st *temp_st = new st;
				temp_st->attr_name = $1;
				temp_st->data_type = $<con>2->data_type;
				temp_st->constraint = $<con>2->constraint;
				temp_st->primary_key = $<con>2->primary_key;
				curr_table->table_st.insert(curr_table->table_st.begin(),temp_st);
			}
			| IDENTIFIER OP_CON ',' CR_ARG {
				st *temp_st = new st;
				temp_st->attr_name = $1;
				temp_st->data_type = $<con>2->data_type;
				temp_st->constraint = $<con>2->constraint;
				temp_st->primary_key = $<con>2->primary_key;
				curr_table->table_st.insert(curr_table->table_st.begin(),temp_st);
			}
			;
OP_CON:			DATA_TYPE {
				node *temp = new node;
				temp->data_type = $1;
				temp->constraint = "NULL";
				temp->primary_key = false;
				$<con>$ = temp;
			}
			| CONSTRAINT {
				node *temp = new node;
				temp->data_type = "string";
				temp->constraint = $1;
				temp->primary_key = false;
				$<con>$ = temp;
			}
			| PRIMARY_KEY {
				node *temp = new node;
				temp->data_type = "string";
				temp->constraint = "NULL";
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| DATA_TYPE CONSTRAINT {
				node *temp = new node;
				temp->data_type = $1;
				temp->constraint = $2;
				temp->primary_key = false;
				$<con>$ = temp;
			}
			| CONSTRAINT DATA_TYPE {
				node* temp = new node;
				temp->constraint = $1;
				temp->data_type = $2;
				temp->primary_key = false;
				$<con>$ = temp;
			}
			| DATA_TYPE PRIMARY_KEY {
				node* temp = new node;
				temp->constraint = "NOTNULL";
				temp->data_type = $1;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| PRIMARY_KEY DATA_TYPE {
				node* temp = new node;
				temp->constraint = "NOTNULL";
				temp->data_type = $2;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| CONSTRAINT PRIMARY_KEY {
				node* temp = new node;
				temp->constraint = $1;
				temp->data_type = "string";
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| PRIMARY_KEY CONSTRAINT {
				node* temp = new node;
				temp->constraint = $2;
				temp->data_type = "string";
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| PRIMARY_KEY DATA_TYPE CONSTRAINT {
				node *temp = new node;
				temp->data_type = $2;
				temp->constraint = $3;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| PRIMARY_KEY CONSTRAINT DATA_TYPE {
				node *temp = new node;
				temp->data_type = $3;
				temp->constraint = $1;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| DATA_TYPE PRIMARY_KEY CONSTRAINT {
				node *temp = new node;
				temp->data_type = $1;
				temp->constraint = $3;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| DATA_TYPE CONSTRAINT PRIMARY_KEY {
				node *temp = new node;
				temp->data_type = $1;
				temp->constraint = $2;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			| CONSTRAINT DATA_TYPE PRIMARY_KEY {
				node *temp = new node;
				temp->data_type = $2;
				temp->constraint = $1;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			|CONSTRAINT PRIMARY_KEY DATA_TYPE {
				node *temp = new node;
				temp->data_type = $3;
				temp->constraint = $1;
				temp->primary_key = true;
				$<con>$ = temp;
			}
			|%empty{
				node* temp = new node;
				temp -> data_type = "string";
				temp -> constraint = "NULL";
				temp->primary_key = false;
				$<con>$ = temp;
			}
			;
INSERT_ST:		INSERT INTO IDENTIFIER VALUES '(' {
								if(check_table($3,temp_list,curr_database)){
									curr_table = get_table($3,temp_list,curr_database);
								}
							  } INS_ARG ')' {
				if(check_insertion(curr_table->temp_tup,curr_table)){
					if(check_primary(curr_table->temp_tup,curr_table)){
						curr_table->tup.push_back(curr_table->temp_tup);
						vector<attr* >().swap(curr_table->temp_tup);
						print_tuples($3,temp_list,curr_database);
						cout<<"VALID INSERTION"<<endl;
					}
					else{
						cout<<"Tuple to be inserted is conflicting with primary key constraint of uniqueness"<<endl;
					}
				}
				else{
					cout<<"INVALID INSERTION"<<endl;
				}
			}
			;
INS_ARG:		'"' IDENTIFIER '"' {
				attr* temp_attr = new attr;
				temp_attr->attr_val = $2;
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|'"' IDENTIFIER '"' ',' INS_ARG{
				attr* temp_attr = new attr;
				temp_attr->attr_val = $2;
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			| '"' '"' {
				attr* temp_attr = new attr;
				temp_attr->attr_val = "";
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			| '"' '"' ',' INS_ARG {
				attr* temp_attr = new attr;
				temp_attr->attr_val = "";
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|NUM {
				attr* temp_attr = new attr;
				temp_attr->attr_val = $1;
				temp_attr->data_type = "int";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|NUM ',' INS_ARG{
				attr* temp_attr = new attr;
				temp_attr->attr_val = $1;
				temp_attr->data_type = "int";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|'"' NUM '"' {
				attr* temp_attr = new attr;
				temp_attr->attr_val = $2;
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|'"' NUM '"' ',' INS_ARG{
				attr* temp_attr = new attr;
				temp_attr->attr_val = $2;
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			;
USE_ST:			USE IDENTIFIER {
				if(check_database($2,temp_list)){
					curr_database = $2;
				}
				else {
					cout<<"No database is present with the name"<<$2<<endl;
				}
			}
			;
SHOW_ST:		SHOW DATABASES {
				print_databases(temp_list);
			}
			|SHOW TABLES {
				print_tables(temp_list,curr_database);
			}
			;
DESCRIBE_ST:		DESCRIBE IDENTIFIER {
				if(check_table($2,temp_list,curr_database)){
					print_tuple_st($2,temp_list,curr_database);
				}
				else{
					cout<<"No such table is there in the current database "<<curr_database<<endl;
				}
			}
			;
SELECT_ST:		SELECT '*' FROM SEL_TABLES {
				join_table* temp_join = select_all(sel_tables,temp_list);
				print_all(temp_join);
				vector<table*>().swap(sel_tables);
			}
			| SELECT '*' FROM SEL_TABLES WHERE CONDITION_ST {
				join_table* temp_join = select_all(sel_tables,temp_list);
				join_table* temp_id_join = select_cond(temp_join,temp_cond);
				print_all(temp_id_join);
				vector<table*>().swap(sel_tables);
			}
			| SELECT IDS FROM SEL_TABLES {
				join_table* temp_join = select_all(sel_tables,temp_list);
				int l = no_select_ids(temp_join,sel_attributes);
				if(l == sel_attributes.size()){
					join_table* temp_id_join = select_ids(temp_join,sel_attributes);
					print_all(temp_id_join);
				}
				else if(l<sel_attributes.size()){
					cout<<"Some attributes are not found in the selected tables"<<endl;
					cout<<endl;
				}
				else{
					cout<<"Some ambiguous attributes found in the selected tables"<<endl;
					cout<<endl;
				}
				vector<string>().swap(sel_attributes);
				vector<table*>().swap(sel_tables);
			}
			| SELECT IDS FROM SEL_TABLES WHERE CONDITION_ST {
				join_table* temp_join = select_all(sel_tables,temp_list);
				int l = no_select_ids(temp_join,sel_attributes);
				if(l == sel_attributes.size()){
					join_table* temp_cond_join = select_cond(temp_join,temp_cond);
					join_table* temp_id_join = select_ids(temp_cond_join,sel_attributes);
					print_all(temp_id_join);
				}
				else if(l<sel_attributes.size()){
					cout<<"Some attributes are not found in the selected tables"<<endl;
					cout<<endl;
				}
				else{
					cout<<"Some ambiguous attributes found in the selected tables"<<endl;
					cout<<endl;
				}
				vector<string>().swap(sel_attributes);
				vector<table*>().swap(sel_tables);
			}
			;
SEL_TABLES:		IDENTIFIER ',' SEL_TABLES {
				table* temp = new table;
				string t = $1;
				if(check_table($1,temp_list,curr_database)){
					temp = get_table($1,temp_list,curr_database);
					sel_tables.insert(sel_tables.begin(),temp);
				}
				else{
					yyerror(t+" table not found");
					vector<table*>().swap(sel_tables);
				}	
			}
			| IDENTIFIER {
				table* temp = new table;
				string t = $1;
				if(check_table($1,temp_list,curr_database)){
					temp = get_table($1,temp_list,curr_database);
					sel_tables.insert(sel_tables.begin(),temp);
				}
				else{
					yyerror(t+" table not found");
					vector<table*>().swap(sel_tables);
				}
			}
			;
IDS:			IDENTIFIER {
				sel_attributes.insert(sel_attributes.begin(),$1);
			}
			| IDENTIFIER ',' IDS {
				sel_attributes.insert(sel_attributes.begin(),$1);
			}
			;
DROP_ST:		DROP DATABASE IDENTIFIER {
				if(check_database($3,temp_list)){
					drop_database($3,temp_list);
				}
				else{
					cout<<"No database with "<<$3<<" is present"<<endl;
				}
			}
			| DROP TABLE IDENTIFIER {
				if(check_database(curr_database,temp_list)){
					if(check_table($3,temp_list,curr_database)){
						drop_table($3,temp_list,curr_database);
					}
					else{
						cout<<"No table with name "<<$3<<" is present in database "<<curr_database<<endl;
					}
				}
				
			}
			;
DELETE_ST:		DELETE FROM IDENTIFIER WHERE {temp_cond = new condition;} CONDITION_ST {
				if(check_table($3,temp_list,curr_database)){
					table* temp = get_table($3,temp_list,curr_database);
					delete_tuple(temp,temp_cond);
					print_tuples($3,temp_list,curr_database);
				}
				else{
					cout<<"No table found with the name "<<$3<<endl;
				}
			}
			;
CONDITION_ST:		IDENTIFIER REL_OP {temp_cond = new condition;}EXP {
				temp_cond->tab_attr1 = $1;
				temp_cond->oper = $2;
			}
			;
EXP:			'(' EXP ')'
			| '"' IDENTIFIER '"' {
				temp_cond->nor_val = $2;
				temp_cond->data_type = "string";
			}
			| IDENTIFIER {
				temp_cond->tab_attr2 = $1;
				temp_cond->data_type = "";
			}
			| '"' NUM '"' {
				temp_cond->nor_val = $2;
				temp_cond->data_type="string";
			}
			| NUM {
				temp_cond->nor_val = $1;
				temp_cond->data_type="int";
			}
			;
			

%%

int main(int argc, char *argv[]){
	yyparse();
}

int yyerror(string s){
  	cout<<s<<endl;
}

