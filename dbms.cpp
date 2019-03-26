#include "dbms.h"

//Function to delete the database from list of databases
void drop_database(string name,database_list* temp_list){
	for(int i=0;i<temp_list->list.size();i++){
		if(temp_list->list[i]->database_name==name){
			temp_list->list.erase(temp_list->list.begin()+i);
			break;
		}
	}
}


//Function to delete the table from the current database that is being used
void drop_table(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//if no of databases>0
		if(temp_list->list[i]->database_name==curr_database){	//if curr_database is created
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//if no of tables in database > 0
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//curr_table present in database
					temp_list->list[i]->table_list.erase(temp_list->list[i]->table_list.begin()+j);
				}
			}
		}
	}
}

//Function to check if the table is present in the database that is being used
bool check_table(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//if no of databases>0
		if(temp_list->list[i]->database_name==curr_database){	//if curr_database is created
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//if no of tables in database > 0
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//curr_table present in database
					return true;
				}
			}
		}
	}
	return false;
}

//
void delete_tuple(table* curr_table,condition* temp_cond){
	for(int i=0;i<curr_table->table_st.size();i++){
		if(curr_table->table_st[i]->attr_name==temp_cond->tab_attr1){
			for(int j=0;j<curr_table->tup.size();j++){
				if(temp_cond->oper=="="){
					if(curr_table->tup[j][i]->attr_val==temp_cond->nor_val){
						curr_table->tup.erase(curr_table->tup.begin()+j);
						j--;
					}
				}
				else if(temp_cond->oper=="<="){
					if(curr_table->tup[j][i]->attr_val<=temp_cond->nor_val){
						curr_table->tup.erase(curr_table->tup.begin()+j);
						j--;
					}
				}
				else if(temp_cond->oper==">="){
					if(curr_table->tup[j][i]->attr_val>=temp_cond->nor_val){
						curr_table->tup.erase(curr_table->tup.begin()+j);
						j--;
					}
				}
				else if(temp_cond->oper=="!="){
					if(curr_table->tup[j][i]->attr_val!=temp_cond->nor_val){
						curr_table->tup.erase(curr_table->tup.begin()+j);
						j--;
					}
				}
			}
		}
	}
}

//Function to retreive the table from the database that is being used
table* get_table(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//if no of databases > 0
		if(temp_list->list[i]->database_name==curr_database){	//if curr_database is created
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//if no of tables in database > 0
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//curr_table present in database
					return temp_list->list[i]->table_list[j];		//return pointer to the table
				}
			}
		}
	}
}

//Function to retreive the database from the list of databases
database* get_database(string curr_database,database_list* temp_list){
	for(int i=0;i<temp_list->list.size();i++){
		if(temp_list->list[i]->database_name==curr_database){
			return temp_list->list[i];
		}
	}
}

//Function to check if the database has been created or not
bool check_database(string temp, database_list* temp_list){
	for(int i=0;i<temp_list->list.size();i++){		//if no of databases > 0
		if(temp_list->list[i]->database_name==temp){	//if curr_database is created
			return true;
		}
	}
	return false;
}

//Function to check if the row to be insereted has the structure same as the table
bool check_insertion(vector<attr*>tup, table* curr_table){	
	if(tup.size()==curr_table->table_st.size()){	//if no of attributes in tuple and table structure are equal
		for(int j=0;j<tup.size();j++){		//for all attributes in the tuple to be inserted
			tup[j]->attr_name = curr_table->table_st[j]->attr_name;
			if(tup[j]->data_type!=curr_table->table_st[j]->data_type){	//checks for data_type
				return false;
			}
			else{
				if(tup[j]->attr_val=="" && curr_table->table_st[j]->constraint=="NOTNULL"){//checks for constraints
					return false;
				}
			}
		}
	}
	else{
		return false;
	}
	return true;
}

//
bool check_primary(vector<attr*>tup, table* curr_table){
	for(int i=0;i<tup.size();i++){
		if(curr_table->table_st[i]->primary_key){
			for(int j=0;j<curr_table->tup.size();j++){
				for(int k=0;k<curr_table->tup[j].size();k++){
					if(curr_table->tup[j][k]->attr_name==curr_table->table_st[i]->attr_name){
						if(tup[i]->attr_val==curr_table->tup[j][k]->attr_val){
							return false;
						}
					}
				}
				
			}
		}
	}
	return true;
}

//Function to check if the table that is created meets the primary key constraints
bool check_creation(table* curr_table){
	for(int i=0;i<curr_table->table_st.size();i++){		//for all attributes of the table to be created
		if(curr_table->table_st[i]->primary_key){	//if the attribute is a primary key
			if(curr_table->table_st[i]->constraint=="NULL"){//if constraint is NULL then return false
				return false;
			}
		}
	}
	return true;
}

//Function to print the meta data of the table
void print_tuple_st(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//for all databases
		if(temp_list->list[i]->database_name==curr_database){	//In the current database
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//for all tables
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//In the current table
					print_sequence(60);
					print_format(60,curr_table+" meta data");
					cout<<endl;
					print_sequence(60);
					print_format(15,"Attribute");
					print_format(15,"Data Type");
					print_format(15,"Constraint");
					print_format(15,"Key");
					cout<<endl;
					print_sequence(60);
					for(int l=0;l<temp_list->list[i]->table_list[j]->table_st.size();l++){ //print structure
						print_format(15,temp_list->list[i]->table_list[j]->table_st[l]->attr_name);
						print_format(15,temp_list->list[i]->table_list[j]->table_st[l]->data_type);
						print_format(15,temp_list->list[i]->table_list[j]->table_st[l]->constraint);
						print_format(15,to_string(temp_list->list[i]->table_list[j]->table_st[l]->primary_key));
						cout<<endl;
					}
					print_sequence(60);
				}
			}
		}
	}
	cout<<endl;
}

//Function to print the rows present in the table of currently used database
void print_tuples(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//for all databases
		if(temp_list->list[i]->database_name==curr_database){	//In current database
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//for all tables
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//For the current table
					int k=temp_list->list[i]->table_list[j]->table_st.size();
					print_sequence(15*k);
					print_format(15*k,curr_table);
					cout<<endl;
					print_sequence(15*k);
					for(int l=0;l<temp_list->list[i]->table_list[j]->table_st.size();l++){	//print all attribute names
						print_format(15,temp_list->list[i]->table_list[j]->table_st[l]->attr_name);
					}
					cout<<endl;
					print_sequence(15*k);
					for(int l=0;l<temp_list->list[i]->table_list[j]->tup.size();l++){//Print the tuples
						for(int k=0;k<temp_list->list[i]->table_list[j]->tup[l].size();k++){
							if(temp_list->list[i]->table_list[j]->tup[l][k]->attr_val!="")
							print_format(15,temp_list->list[i]->table_list[j]->tup[l][k]->attr_val);
							else
							print_format(15,"NULL");
						}
						cout<<endl;
					}
					print_sequence(15*k);
				}
			}
		}
	}
	cout<<endl;
}

//Function to print the databases present 
void print_databases(database_list* temp_list){
	cout<<"+--------------------------------+"<<endl;
	cout<<"|        List of databases       |"<<endl;
	cout<<"+--------------------------------+"<<endl;
	for(int i=0;i<temp_list->list.size();i++){	//Print all the databases
		print_format(34,temp_list->list[i]->database_name);
		cout<<endl;
	}
	cout<<"+--------------------------------+"<<endl;
	cout<<endl;
}

//Function to print the tables present in the currently used database
void print_tables(database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//For all the databases
		if(temp_list->list[i]->database_name==curr_database){	//In the current database
			cout<<"+--------------------------------+"<<endl;
			print_format(34,"Tables in "+curr_database);
			cout<<endl;
			cout<<"+--------------------------------+"<<endl;
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){ //print all tables
				print_format(34,temp_list->list[i]->table_list[j]->table_name);
				cout<<endl;
			}
			cout<<"+--------------------------------+"<<endl;
		}
	}
	cout<<endl;
}

//Function to print the detais neatly
void print_format(int count,string s){
	int l = count-s.length()-2;
	cout<<"|";
	if(l%2==0){
		for(int j=0;j<l/2;j++) cout<<" ";
	}
	else{
		for(int j=0;j<l/2+1;j++) cout<<" ";
	}
	cout<<s;
	for(int j=0;j<l/2;j++) cout<<" ";
	cout<<"|";
}

//Function to print the detais neatly
void print_sequence(int count){
	cout<<"+";
	for(int i=0;i<count-2;i++) cout<<"-";
	cout<<"+"<<endl;
}

//Function to print all the attributes of the selected tables by using join
join_table* select_all(vector<table*> sel_tables,database_list* temp_list){
	int total = 1;
	join_table *temp_join = new join_table;
	for(int i=0;i<sel_tables.size();i++){	//For all selected tables
		total = total * sel_tables[i]->tup.size();
		for(int j=0;j<sel_tables[i]->table_st.size();j++){
			temp_join->table_st.push_back(sel_tables[i]->table_st[j]);
		}
	}
	int total1 = total;	//Total rows in the table created by join
	vector<vector<vector<vector<attr*> > > >temp(sel_tables.size(),vector<vector<vector<attr*> > >(total));
	int r = 1;
	for(int i=0;i<sel_tables.size();i++){	//For all selected tables
		total = total/sel_tables[i]->tup.size();
		int j=0;
		for(int k=0;k<total;k++){	//Push the attributes to the table as per the join rule
			for(int l=0;l<sel_tables[i]->tup.size();l++){
				for(int m=0;m<r;m++){
					temp[i][j].push_back(sel_tables[i]->tup[l]);
					j++;
				}
			}
		}
		r=r*sel_tables[i]->tup.size();
	}
	temp_join->join_sel_tables = temp;
	return temp_join;
}

//
join_table* select_ids(join_table* temp_join,vector<string> sel_attributes){
	int total = 0;
	if(temp_join->join_sel_tables.size()>0)
		total = temp_join->join_sel_tables[0].size();		//calculate no of tuples in newly created table
	join_table* temp_id_join = new join_table;
	vector<vector<vector<vector<attr*> > > >temp(1,vector<vector<vector<attr*> > >(total,vector<vector<attr*> > (sel_attributes.size())));
	for(int i=0;i<sel_attributes.size();i++){
		for(int j=0;j<temp_join->table_st.size();j++){
			if(sel_attributes[i]==temp_join->table_st[j]->attr_name){
				temp_id_join->table_st.push_back(temp_join->table_st[j]);
				for(int k=0;k<temp_join->join_sel_tables.size();k++){
					for(int l=0;l<temp_join->join_sel_tables[k].size();l++){
						for(int m=0;m<temp_join->join_sel_tables[k][l].size();m++){
							for(int n=0;n<temp_join->join_sel_tables[k][l][m].size();n++){
								if(temp_join->join_sel_tables[k][l][m][n]->attr_name==sel_attributes[i]){
									temp[0][l][i].push_back(temp_join->join_sel_tables[k][l][m][n]);
								}
							}
						}
					}
				}
			}
		}
	}
	temp_id_join->join_sel_tables = temp;
	return temp_id_join;
}

//
join_table* select_cond(join_table* temp_join,condition* temp_cond){
	join_table* temp_cond_join = new join_table;
	temp_cond_join->table_st = temp_join->table_st;
	vector<vector<vector<vector<attr*> > > >temp(temp_join->join_sel_tables.size());
	vector<vector<vector<vector<attr*> > > >temp1 = temp_join->join_sel_tables;
	if(temp_cond->tab_attr2!=""){
		for(int i=0;i<temp_join->table_st.size();i++){
			if(temp_join->table_st[i]->attr_name==temp_cond->tab_attr1){
				for(int j=0;j<temp_join->table_st.size();j++){
					if(temp_join->table_st[j]->attr_name==temp_cond->tab_attr2){
						//cout<<"ok"<<endl;
						int k = get_index(temp1,temp_cond->tab_attr1);
						int l = get_index(temp1,temp_cond->tab_attr2);
						for(int m=0;m<temp1[k].size();m++){
							for(int n=0;n<temp1[k][m].size();n++){
								for(int p=0;p<temp1[k][m][n].size();p++){
									if(temp1[k][m][n][p]->attr_name==temp_cond->tab_attr1){
									for(int q=0;q<temp1[l][m][n].size();q++){
									if(temp1[l][m][n][q]->attr_name==temp_cond->tab_attr2){
									if(temp_cond->oper=="="){
									if(temp1[l][m][n][q]->attr_val==temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper=="<="){
									if(temp1[l][m][n][q]->attr_val<=temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper==">="){
									if(temp1[l][m][n][q]->attr_val>=temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper=="!="){
									if(temp1[l][m][n][q]->attr_val!=temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper==">"){
									if(temp1[l][m][n][q]->attr_val>temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper=="<"){
									if(temp1[l][m][n][q]->attr_val<temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									}
									}
									}
									else if(temp1[k][m][n][p]->attr_name==temp_cond->tab_attr2){
									for(int q=0;q<temp1[l][m][n].size();q++){
									if(temp1[l][m][n][q]->attr_name==temp_cond->tab_attr1){
									if(temp_cond->oper=="="){
									if(temp1[l][m][n][q]->attr_val==temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper=="<="){
									if(temp1[l][m][n][q]->attr_val<=temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper==">="){
									if(temp1[l][m][n][q]->attr_val>=temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper=="!="){
									if(temp1[l][m][n][q]->attr_val!=temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper==">"){
									if(temp1[l][m][n][q]->attr_val>temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									else if(temp_cond->oper=="<"){
									if(temp1[l][m][n][q]->attr_val<temp1[k][m][n][p]->attr_val){
											for(int r=0;r<temp.size();r++){
												temp[r].push_back(temp1[r][m]);
											}
									}
									}
									}
									}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	temp_cond_join->join_sel_tables = temp;
	return temp_cond_join;
}

//
int get_index(vector<vector<vector<vector<attr*> > > > temp,string attr_name){
	for(int k=0;k<temp.size();k++){
		for(int l=0;l<temp[k].size();l++){
			for(int m=0;m<temp[k][l].size();m++){
				for(int n=0;n<temp[k][l][m].size();n++){
					if(temp[k][l][m][n]->attr_name==attr_name){
						return k;
					}
				}
			}
		}
	}
	return -1;
}

//Function to find the the number of attributes that are present in the selected attributes
int no_select_ids(join_table* temp_join,vector<string> sel_attributes){
	int l=0;
	for(int i=0;i<sel_attributes.size();i++){
		for(int j=0;j<temp_join->table_st.size();j++){
			if(sel_attributes[i]==temp_join->table_st[j]->attr_name){
				l++;
			}
		}
	}
	return l;
}

//Function to print all the selected attributes in the select statement
void print_all(join_table* temp_join){
	int total = 0;
	int l = temp_join->table_st.size();
	vector<vector<vector<vector<attr*> > > >temp = temp_join->join_sel_tables;
	if(temp.size()>0)
		total = temp[0].size();		//calculate no of tuples in newly created table
	l = 15*l;
	print_sequence(l);
	for(int i=0;i<temp_join->table_st.size();i++){	//For all selected tables
		print_format(15,temp_join->table_st[i]->attr_name);
	}
	cout<<endl;
	print_sequence(l);
	for(int m=0;m<total;m++){	//print the tuples in newly created table
		for(int i=0;i<temp.size();i++){
			for(int k=0;k<temp[i][m].size();k++){
				for(int l=0;l<temp[i][m][k].size();l++){
					print_format(15,temp[i][m][k][l]->attr_val);
				}
			}
		}
		cout<<endl;
	}
	print_sequence(l);
	cout<<endl;
}

