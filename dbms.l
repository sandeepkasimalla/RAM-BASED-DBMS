%{
	#include "dbms.h"
	#include "y.tab.h"
	using namespace std;
	char* p;
%}

alp 		[A-Za-z0-9_]
data_type 	"string"|"int"
sp 		" "|"\t"
nwln 		"\n"
constraint 	"NULL"|"NOTNULL"
rel_op		"="|"<="|">="|"!="|">"|"<"

%%

"create" 	{return CREATE;}
"table" 	{return TABLE;}
"tables"	{return TABLES;}
"database" 	{return DATABASE;}
"databases"	{return DATABASES;}
"show" 		{return SHOW;}
"delete"	{return DELETE;}
"insert" 	{return INSERT;}
"describe" 	{return DESCRIBE;}
"drop" 		{return DROP;}
"into" 		{return INTO;}
"values"	{return VALUES;}
"use"		{return USE;}
"select"	{return SELECT;}
"from"		{return FROM;}
"where"		{return WHERE;}
"primary key"	{return PRIMARY_KEY;}
{rel_op}	{yylval.p=strdup(yytext);return REL_OP;}
{constraint} 	{yylval.p=strdup(yytext);return CONSTRAINT;}
{data_type} 	{yylval.p=strdup(yytext);return DATA_TYPE;}
[A-Za-z_]{alp}* {yylval.p=strdup(yytext);return IDENTIFIER;}
[0-9]+		{yylval.p=strdup(yytext);return NUM;}
{sp}+ 		;
{nwln}* 	;
. 		{return yytext[0];}

%%

