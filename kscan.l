D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{

# include "tokens.h"
# include "ansi_parse.h"
# include "lif.h"
# define Q want_id=1; /* printf ("WID on\n"); */
# define T type_seen=0;
# define R in_enum=1;
# define S in_init=1;

//static  char    sccsid[] = "@(#)kscan.l	1.4  7/20/94";

void eat_string();
void count();
void comment();
int reserved_word(int tok);
int check_type(char *t);
void reset_line(void);
int is_type_name (char *t, int *i);

/*static*/	
int	in_struct = 0,recent_comma = 0;
int	in_typedef = 0, braces = 0,
		in_init = 0, need_brace = 0,
		type_seen = 0, in_local = 0, in_enum = 0;
int	want_id = 0;
int	last_was_dot = 0;

%}

%%
"/*"			{ comment(); }

"auto"			{T return reserved_word (AUTO); }
"break"			{ return reserved_word (BREAK); }
"case"			{ return reserved_word (CASE); }
"char"			{Q return reserved_word (CHAR); }
"const"			{ return reserved_word (CONST); }
"continue"		{ return reserved_word (CONTINUE); }
"default"		{ return reserved_word (DEFAULT); }
"do"			{ return reserved_word (DO); }
"double"		{Q return reserved_word (DOUBLE); }
"else"			{ return reserved_word (ELSE); }
"enum"			{Q R return reserved_word (ENUM); }
"extern"		{T return reserved_word (EXTERN); }
"float"			{Q return reserved_word (FLOAT); }
"for"			{ return reserved_word (FOR); }
"goto"			{ return reserved_word (GOTO); }
"if"			{ return reserved_word (IF); }
"int"			{Q return reserved_word (INT); }
"long"			{Q return reserved_word (LONG); }
"register"		{T ;return reserved_word (REGISTER); }
"return"		{ return reserved_word (RETURN); }
"short"			{Q return reserved_word (SHORT); }
"signed"		{Q return reserved_word (SIGNED); }
"sizeof"		{ return reserved_word (SIZEOF); }
"static"		{T want_id = 0; return reserved_word (STATIC); }
"struct"		{Q in_struct++;
					need_brace = 2;
					return reserved_word (STRUCT); }
"switch"		{ return reserved_word (SWITCH); }
"typedef"		{
				in_typedef = 1;
				type_seen = 0;
				return reserved_word (TYPEDEF); }
"union"			{Q in_struct++;
					need_brace = 2;
					return reserved_word (UNION); }
"unsigned"		{Q return reserved_word (UNSIGNED); }
"void"			{Q return reserved_word (VOID); }
"volatile"		{ return reserved_word (VOLATILE); }
"while"			{ return reserved_word (WHILE); }
__extension__	{count(); /* eat gcc extension keyword */}
^#.*\n	                { reset_line(); }

{L}({L}|{D})*		{ int	type_code;
					if (need_brace > 0) need_brace--;
					type_code = check_type(yytext);
					if (type_code) return type_code;
					}

0[xX]{H}+{IS}?		{ count(); return(CONSTANT); }

0{D}+{IS}?		{ count(); return(CONSTANT); }

{D}+{IS}?		{ count(); return(CONSTANT); }

L?'(\\.|[^\\'])+'		{ count(); return(CONSTANT); }

{D}+{E}{FS}?		{ count(); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(); return(CONSTANT); }
L?\"			{eat_string(); return(STRING_LITERAL);
	/*
	L?\"([^\n"\\]|(\\[^\n]))*\"	{
				 count(); return(STRING_LITERAL);
	\"([^"\n]|\\["\n])*\"	{ count(); return(STRING_LITERAL); }
	*/}

">>="		{ count(); return(RIGHT_ASSIGN); }
"<<="		{ count(); return(LEFT_ASSIGN); }
"+="		{ count(); return(ADD_ASSIGN); }
"-="		{ count(); return(SUB_ASSIGN); }
"*="		{ count(); return(MUL_ASSIGN); }
"/="		{ count(); return(DIV_ASSIGN); }
"%="		{ count(); return(MOD_ASSIGN); }
"&="		{ count(); return(AND_ASSIGN); }
"^="		{ count(); return(XOR_ASSIGN); }
"|="		{ count(); return(OR_ASSIGN); }
">>"		{ count(); return(RIGHT_OP); }
"<<"		{ count(); return(LEFT_OP); }
"++"		{ yylval.token = create_token(NULL);
				count(); return(INC_OP); }
"--"		{ yylval.token = create_token(NULL);
				count(); return(DEC_OP); }
"->"		{ count(); last_was_dot = 1; return(PTR_OP); }
"&&"		{ count(); return(AND_OP); }
"||"		{ count(); return(OR_OP); }
"<="		{ count(); return(LE_OP); }
">="		{ count(); return(GE_OP); }
"=="		{ count(); return(EQ_OP); }
"!="		{ count(); return(NE_OP); }
"..."		{ count(); return ELIPSIS; }
","			{ count(); recent_comma = 1;
				if (in_parms) want_id = 0;
				if (k_opt) printf ("SEE COMMA NOW ip %d wi %d\n",
					in_parms,want_id);
				if (need_brace) in_struct--; return(','); }
"="			{S count(); return('='); }
"["			{ count(); return('['); }
"]"			{ count(); return(']'); }
"."			{ count(); last_was_dot = 1; return('.'); }
"&"			{ count(); return('&'); }
"!"			{ count(); return('!'); }
"~"			{ count(); return('~'); }
"-"			{ count(); return('-'); }
"+"			{ count(); return('+'); }
"*"			{ 
				yylval.token = create_token(NULL);
				count(); return('*'); }
"/"			{ count(); return('/'); }
"%"			{ count(); return('%'); }
"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }
"^"			{ count(); return('^'); }
"|"			{ count(); return('|'); }
"?"			{ count(); return('?'); }
"}"		{	
				braces--;
				/*
				if (in_struct){Q in_struct--;}
				*/
				yylval.token = create_token(NULL);
				count();
				return yytext[0];
			}
[;{]		{
				want_id = 0;
				if (k_opt) printf (" want id = %d :%s: \n",want_id,
					yytext);
				if (yytext[0] == '{') braces++;
				if (yytext[0] == '{') need_brace = 0;
				if ((!in_struct) && (in_typedef)) in_typedef = 0;
				if(yytext[0] == ';'){
					type_seen = 0;
					in_enum = 0;
					in_init = 0;
					if (!braces) in_struct = 0;
				}
				else if (in_struct) type_seen = 0;
				yylval.token = create_token(NULL);
				count();
				return yytext[0];
			}
[():]		{ yylval.token = create_token(NULL);
				if (yytext[0] == ')') want_id = 0;
				if (yytext[0] == '(') want_id = 0;
				if (k_opt) printf (" want id = %d :%s: \n",want_id,
					yytext);
				count();
				return yytext[0];
			}

[ \t\v\n\f]		{int save;
					save = last_was_dot;
					count();
					last_was_dot = save;}
.			{ count(); /* ignore bad characters */ }

%%

int yywrap()
{
	return(1);
}

void comment()
{
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		/*putchar(c) */;

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
	/*	putchar(c1) */;
}

void count()
{
	int i;

	recent_comma = 0;
	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n'){
			current_column = 1;
			lineno++;
		}
		else
			current_column++;
/*
	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			current_column = 0;
		else if (yytext[i] == '\t')
			current_column += 8 - (current_column % 8);
		else
			current_column++;

	ECHO;
printf ("count token (%s) dot(%d)\n",yytext,last_was_dot);
*/
last_was_dot = 0;
}

void eat_string()
{
	char	c;
	int		more = 1,
			in_esc = 0;

	count();
	while (more) {
		c = input();
		current_column++;
		if (in_esc) in_esc = 0;
		else if (c == '"') more = 0;
		else if (c == '\\') in_esc = 1;
	}
}

/*
**************************************************************
*                                                            *
* keep a global string table in <strings>                    *
*                                                            *
* find_string saves the text of an unseen string in the      *
* string table and returns a pointer to a copy of the        *
* string, or returns a pointer to the copy already saved.    *
*                                                            *
**************************************************************
*/

char	*find_string (s)
	char	*s;
{
	static string_ptr	strings = NULL;
	string_ptr	at;

	at = strings;
	while (at){		/* return pointer to string if found */
		if (strcmp(at->text,s) == 0) return at->text;
		at = at -> next;
	}
/*
**************************************************************
*  alloc space for string if not seen before                 *
**************************************************************
*/
	strings = alloc_string(s,strings);
	return strings->text;
}

/*
**************************************************************
*                                                            *
*  Create a token record for tokens that are used to         *
*  anchor statements (nodes) to places in the source         *
*                                                            *
**************************************************************
*/
token_ptr create_token(s)
	char	*s;
{
	token_ptr	new;

	new = alloc_token();
	new -> next = NULL;
	new -> desc.any = NULL;
	new -> at.line_start = lineno;
	new -> at.line_end = lineno;
	new -> at.col_start = current_column;
	new -> at.col_end = current_column + strlen(yytext) - 1;
	if (s) new->text = find_string(s);
	else new -> text = NULL;
	if (k_opt){
		printf ("create token (%s) at line %d(%d,%d)",
			yytext,lineno,current_column,new->at.col_end);
		if (in_struct) printf (" struct");
		if (in_typedef) printf (" typedef");
		printf ("scope %d in %d type %d",top_scope(),
			in_local,type_seen);
		printf ("\n");
	}
	return new;
}

int check_type(t)
   char *t;
{
   int		is_type,is_local;

	if (is_type_name(t,&is_local)) {
		/*
		if ((!is_local) && want_id)is_type = IDENTIFIER;
		*/
		if (k_opt) printf (" wID %d ",want_id);
		if (want_id || last_was_dot){
			want_id = 0; is_type = IDENTIFIER;}
		else { Q is_type = TYPE_NAME;}
		if (k_opt)
			printf ("in %d is %d T %d I %d S %d want_id %d ip %d ",
				in_local,is_local,type_seen,in_init,in_struct,want_id,
				in_parms);
	}
	else is_type = IDENTIFIER;
	if (k_opt) printf (" %s (%d,%d[.,->])\n",is_type == TYPE_NAME?"Type":"Id",
		want_id,last_was_dot);
	yylval.token = create_token(t);
	count(); 
	return is_type ;
}

int reserved_word(t)
   int	t;
{
   yylval.token = create_token(NULL);
   count(); 
   return t ;
}

void start_local_decl() {
	in_local = 1;
}

void end_init() {
	in_init = 0;
}
	
void clear_type_flag() {
	type_seen = 0;
}

void end_local_decl() {
	in_init = 0;
	in_local = 0;
}

void reset_line (void)	/* adjust line and column after # include */
{
	char	*at;

	at = yytext;
	/*
	sscanf(at,"#%d \"%s\"",&lineno,the_current_file);
	*/
	sscanf(at,"#%d \"%[^\"]",&lineno,the_current_file);
	if(h_file)fprintf(h_file,"@ %s\n",the_current_file);
	current_column = 1;
	at[strlen(at) - 2] = '\0';
	if(k_opt)printf ("reset to line %d in [%s] (%s)\n",lineno,
		the_current_file,at);
}


