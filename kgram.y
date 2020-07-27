%{
# include <stdio.h>
# include "ansi_parse.h"
# include "lif.h"
	static	int     set_typedef = 0;
	//static  char    sccsid[] = "@(#)kgram.y	1.7  10/26/94";
	static	char	*parse_h_sccs_id = PARSE_H_SCCS_ID;
	static	char	*lif_h_sccs_id = LIF_H_SCCS_ID;
	
extern int yyerror(char *msg);

extern void connect_nodes (int from, int to);
extern void close_scope(void);
extern void open_scope(void);
extern void end_local_decl(void);
extern void start_local_decl(void);
extern void clear_type_flag(void);
extern void do_returns (int end_stmt);
extern void source_map (int n, token_ptr from, token_ptr to);
extern int yylex(void);
extern void merge_abstract_type (type_ptr old, type_ptr new);
extern void start_local_decls(void);
extern void end_locaal_decls(void);
extern void start_typedef(void);
extern void end_typedef(void);
extern void end_init(void);
extern void gen_require (int node_required, int from, int to);
extern void make_decl (token_ptr token, unsigned int flag);
void print_decl (token_ptr type_spec, token_ptr var_specs);
extern void insert_var_decl (token_ptr var);
extern void merge_abstract_type (type_ptr old, type_ptr new);
extern void decl (token_ptr type_desc, token_ptr var_list, int with_members);
extern void modify_type (token_ptr token, unsigned int flag, int style, token_ptr formals);

%}

%token <token> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token <token> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP
%token <token> EQ_OP NE_OP
%token <token> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <token> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <token> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token <token> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <token> CHAR SHORT INT LONG SIGNED UNSIGNED
%token <token> FLOAT DOUBLE CONST VOLATILE VOID
%token <token> STRUCT UNION ENUM ELIPSIS RANGE

%token <token> CASE DEFAULT IF ELSE SWITCH
%token <token> WHILE DO FOR GOTO CONTINUE BREAK RETURN
%type <tree> primary_expr
%type <tree> postfix_expr
%type <tree> argument_expr_list
%type <tree> unary_expr
%type <tree> unary_operator
%type <tree> cast_expr
%type <tree> multiplicative_expr
%type <tree> additive_expr
%type <tree> shift_expr
%type <tree> relational_expr
%type <tree> equality_expr
%type <tree> and_expr
%type <tree> exclusive_or_expr
%type <tree> inclusive_or_expr
%type <tree> logical_and_expr
%type <tree> logical_or_expr
%type <tree> conditional_expr
%type <tree> assignment_expr
%type <op> assignment_operator
%type <tree> oexpr expr
%type <tree> exprXX
%type <tree> constant_expr
%type <token> declaration declaration_specifiers init_declarator_list
%type <token> init_declarator storage_class_specifier type_specifier
%type <token> struct_or_union_specifier
%type <token> struct_or_union struct_declaration_list
%type <token> struct_declaration
%type <token> struct_declarator_list struct_declarator
%type <token> enum_specifier enumerator_list enumerator
%type <token> declarator declarator2 pointer
%type <token> parameter_identifier_list
%type <token> identifier_list parameter_type_list parameter_list
%type <token> parameter_declaration
%type <token> type_name abstract_declarator abstract_declarator2
%type <token> initializer initializer_list
%type <stmt> statement labeled_statement
%type <token> decl_end decl_start
%type <stmt> compound_statement
%type <token> declaration_list
%type <stmt> statement_list expression_statement selection_statement
%type <stmt> iteration_statement jump_statement function_body 
%type <token> program file external_definition
%type <token> function_definition
%type <stmt> function_start
%type <token> identifier
%token <token> ';' '(' ')' '{' '}' ':'
%token <token> '&' '*' '~' '+' '-' '!'
%type <token> string_literal_list
%type <token> specifier_qual_list type_qualifier type_qualifier_list

%start program
%%

primary_expr
	: identifier
		{$$ = make_leaf(ID_OP,$1);}
	| CONSTANT
		{$$ = make_leaf(CON_OP,$1);}
	| string_literal_list
		{$$ = make_leaf(CON_OP,$1);}
	| '(' exprXX ')'
		{
			tree_ptr	t;
			t = make_tree (LEFTP_OP,$2,(token_ptr)NULL);
			$$ = make_tree (RIGHTP_OP,t,(token_ptr)NULL);
			t->token = $1;
			$$->token = $3;}
	;
string_literal_list
	: STRING_LITERAL
	| string_literal_list STRING_LITERAL
	;

postfix_expr
	: primary_expr
	| postfix_expr '[' exprXX ']'
		{$$ = make_tree (ARRAY_OP,$1,$3);}
	| postfix_expr '(' ')'
		{$$ = make_tree (CALL_OP,$1,(token_ptr)NULL);}
	| postfix_expr '(' argument_expr_list ')'
		{$$ = make_tree (CALL_OP,$1,$3);}
	| postfix_expr '.' identifier
		{$$ = make_tree (DOT_OP,$1,
			make_leaf(ID_OP,$3));}
	| postfix_expr PTR_OP identifier
		{$$ = make_tree (POINTER_OP,$1,
			make_leaf(ID_OP,$3));}
	| postfix_expr INC_OP
		{$$ = make_tree (POST_OP,$1,(token_ptr)NULL); $$->token = $2;}
	| postfix_expr DEC_OP
		{$$ = make_tree (POST_OP,$1,(token_ptr)NULL); $$->token = $2;}
	;

argument_expr_list
	: assignment_expr
	| argument_expr_list comma_comming ',' assignment_expr
		{$$ = make_tree (ARG_OP,$1,$4);}
	;
comma_comming : /* empty */
			{if(!in_init)type_seen = 0;}
	;

unary_expr
	: postfix_expr
	| INC_OP unary_expr
		{$$ = make_tree (PRE_OP,$2,(token_ptr)NULL);$$->token = $1;}
	| DEC_OP unary_expr
		{$$ = make_tree (PRE_OP,$2,(token_ptr)NULL);$$->token = $1;}
	| unary_operator cast_expr
		{$$ = make_tree ($1->op_code,$2,(token_ptr)NULL);
		if ($1->op_code == DEREF_OP)
			$$->token = $1->token;}
	| SIZEOF unary_expr
		{$$ = make_tree (UN_OP,$2,(token_ptr)NULL);}
	| SIZEOF '(' type_name ')'
		{$$ = make_leaf(CON_OP,$1);}
	;

unary_operator
	: '&'
		{$$ = make_leaf(ADDR_OP,$1);}
	| '*'
		{$$ = make_leaf(DEREF_OP,$1);}
	| '+'
		{$$ = make_leaf(UN_OP,$1);}
	| '-'
		{$$ = make_leaf(UN_OP,$1);}
	| '~'
		{$$ = make_leaf(UN_OP,$1);}
	| '!'
		{$$ = make_leaf(UN_OP,$1);}
	;

cast_expr
	: unary_expr
	| '(' type_name ')' cast_expr
		{
			/*
			if ($2){if($2->desc.decl->decl_flags & FLAG_TYPE_USER_NAME)
					$$ = make_tree (CAST_OP,
						make_leaf(ID_OP,$2),$4);
				else 
					$$ = make_tree (CAST_OP,(token_ptr)NULL,$4);
			}
			else 
				$$ = make_tree (CAST_OP,(token_ptr)NULL,$4);
			*/
			$$ = make_tree (CAST_OP,
						make_leaf(TYPENAME_OP,$2),$4);
		}
	;

multiplicative_expr
	: cast_expr
	| multiplicative_expr '*' cast_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| multiplicative_expr '/' cast_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| multiplicative_expr '%' cast_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

additive_expr
	: multiplicative_expr
	| additive_expr '+' multiplicative_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| additive_expr '-' multiplicative_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

shift_expr
	: additive_expr
	| shift_expr LEFT_OP additive_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| shift_expr RIGHT_OP additive_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

relational_expr
	: shift_expr
	| relational_expr '<' shift_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| relational_expr '>' shift_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| relational_expr LE_OP shift_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| relational_expr GE_OP shift_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

equality_expr
	: relational_expr
	| equality_expr EQ_OP relational_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	| equality_expr NE_OP relational_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

and_expr
	: equality_expr
	| and_expr '&' equality_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

exclusive_or_expr
	: and_expr
	| exclusive_or_expr '^' and_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

inclusive_or_expr
	: exclusive_or_expr
	| inclusive_or_expr '|' exclusive_or_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

logical_and_expr
	: inclusive_or_expr
	| logical_and_expr AND_OP inclusive_or_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

logical_or_expr
	: logical_and_expr
	| logical_or_expr OR_OP logical_and_expr
		{$$ = make_tree (BIN_OP,$1,$3);}
	;

conditional_expr
	: logical_or_expr
	| logical_or_expr '?' expr ':' conditional_expr
		{$$ = make_tree (BIN_OP,$1,
			make_tree (BIN_OP,$3,$5));
		}
	;

assignment_expr
	: conditional_expr
	| unary_expr assignment_operator assignment_expr
		{$$ = make_tree (($2 == 0)?RELASGN_OP:ASGN_OP,$1,$3);}
	;

assignment_operator
	: '='
		{ $$ = 1;}
	| MUL_ASSIGN
		{ $$ = 0;}
	| DIV_ASSIGN
		{ $$ = 0;}
	| MOD_ASSIGN
		{ $$ = 0;}
	| ADD_ASSIGN
		{ $$ = 0;}
	| SUB_ASSIGN
		{ $$ = 0;}
	| LEFT_ASSIGN
		{ $$ = 0;}
	| RIGHT_ASSIGN
		{ $$ = 0;}
	| AND_ASSIGN
		{ $$ = 0;}
	| XOR_ASSIGN
		{ $$ = 0;}
	| OR_ASSIGN
		{ $$ = 0;}
	;

expr
	: exprXX
	;
exprXX
	: assignment_expr
	| expr ',' assignment_expr
		{$$ = make_tree (COMMA_OP,$1,$3);}
	;

constant_expr
	: conditional_expr
	;

declaration
	: declaration_specifiers ';'
		{
			decl ($1,(token_ptr)NULL,False);
			end_typedef();
		}
	| declaration_specifiers init_declarator_list ';'
		{
			decl ($1,$2,False);
			if(c_opt)print_decl ($1,$2);
			$1->desc.decl->vars = $2;
			end_typedef();
		}
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
		{$1->desc.decl->decl_flags |=
			$2->desc.decl->decl_flags ;
		$1->desc.decl->tag = $2->desc.decl->tag;
		$1->desc.decl->vars = $2->desc.decl->vars;
		$1->desc.decl->members = $2->desc.decl->members;
		$1->desc.decl->member_ste = $2->desc.decl->member_ste;
		if (!$1->text) $1->text = $2->text;
		}
	| type_qualifier
	| type_qualifier declaration_specifiers
		{if ($1 && $2)$2->desc.decl->decl_flags |=
				$1->desc.decl->decl_flags ;
			$$ = $2;
		}
	| type_specifier 
	| type_specifier declaration_specifiers 
		{$1->desc.decl->decl_flags |=
			$2->desc.decl->decl_flags ;
		if (!$1->text) $1->text = $2->text;
		}
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
		{
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of declarators                           *
*                                                        *
**********************************************************
*/

			if ($1){
				t = $1;
				while (t->next) t = t->next;
				t->next = $3;
			}
			else $$ = $3;
		}
	;

init_declarator
	: declarator
	| declarator '=' initializer
		{ end_init();}
	;

storage_class_specifier
	: TYPEDEF
		{make_decl ($1, FLAG_SC_TYPEDEF);
			start_typedef();
			set_typedef = True;}
	| EXTERN
		{make_decl ($1, FLAG_SC_EXTERN);}
	| STATIC
		{make_decl ($1, FLAG_SC_STATIC);}
	| AUTO
		{make_decl ($1, FLAG_SC_AUTO);}
	| REGISTER
		{make_decl ($1, FLAG_SC_REGISTER);}
	;

type_specifier
	: CHAR {make_decl ($1, FLAG_TYPE_CHAR);}
	| SHORT {make_decl ($1, FLAG_TYPE_SHORT);}
	| INT {make_decl ($1, FLAG_TYPE_INT);}
	| LONG {make_decl ($1, FLAG_TYPE_LONG);}
	| SIGNED {make_decl ($1, FLAG_TYPE_SIGNED);}
	| UNSIGNED {make_decl ($1, FLAG_TYPE_UNSIGNED);}
	| FLOAT {make_decl ($1, FLAG_TYPE_FLOAT);}
	| DOUBLE {make_decl ($1, FLAG_TYPE_DOUBLE);}
	| VOID {make_decl ($1, FLAG_TYPE_VOID);}
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME {make_decl ($1, FLAG_TYPE_USER_NAME);}
	;

struct_or_union_specifier
	: struct_or_union identifier
		{
			$1->desc.decl->tag = $2;
			decl ($1,(token_ptr)NULL,True);
		}
		'{' struct_declaration_list '}'
		{
			int		offset;

			$1->desc.decl->member_ste = struct_decl($5,&offset);
			$1->desc.decl->members = $5;
			$1->desc.decl->offset = offset;
		}
	| struct_or_union '{' struct_declaration_list '}'
		{
			int		offset;

			$1->desc.decl->members = $3;
			$1->desc.decl->member_ste = struct_decl($3,&offset);
			$1->desc.decl->offset = offset;
		}
	| struct_or_union identifier 
		{
			$1->desc.decl->tag = $2;
		}
	;

struct_or_union
	: STRUCT
		{make_decl ($1, FLAG_TYPE_US | FLAG_TYPE_STRUCT); }
	| UNION
		{make_decl ($1, FLAG_TYPE_US | FLAG_TYPE_UNION); }
	;
	

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
		{
			token_ptr	t;

			t = $1;
			if (t){
				while (t->next) t = t->next;
				t->next = $2;
			}
			else $$ = $2;
		}
	;

specifier_qual_list
	: type_specifier
	| type_qualifier
	| specifier_qual_list type_qualifier
		{
			$1->desc.decl->decl_flags |= $2->desc.decl->decl_flags;
			if ($1->desc.decl->tag == NULL)
				$1->desc.decl->tag = $2->desc.decl->tag;
			if ($1->desc.decl->vars == NULL)
				$1->desc.decl->vars = $2->desc.decl->vars;
			if ($1->desc.decl->members == NULL)
				$1->desc.decl->members = $2->desc.decl->members;
			if ($1->desc.decl->member_ste == NULL)
				$1->desc.decl->member_ste = $2->desc.decl->member_ste;
		}
	| specifier_qual_list type_specifier
		{
			$1->desc.decl->decl_flags |= $2->desc.decl->decl_flags;
			if ($1->desc.decl->tag == NULL)
				$1->desc.decl->tag = $2->desc.decl->tag;
			if ($1->desc.decl->vars == NULL)
				$1->desc.decl->vars = $2->desc.decl->vars;
			if ($1->desc.decl->members == NULL)
				$1->desc.decl->members = $2->desc.decl->members;
			if ($1->desc.decl->member_ste == NULL)
				$1->desc.decl->member_ste = $2->desc.decl->member_ste;
		}
	;

struct_declaration
	/* : type_specifier_list struct_declarator_list ';'
	| type_specifier_list ';' */
	: specifier_qual_list struct_declarator_list ';'
		{
			$1->desc.decl->vars = $2;
		}
	| specifier_qual_list ';'
		{
			$1->desc.decl->vars = NULL;
		}
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
		{
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of declarators                           *
*                                                        *
**********************************************************
*/

			if ($1){
				t = $1;
				while (t->next) t = t->next;
				t->next = $3;
			}
			else $$ = $3;
		}
	;

struct_declarator
	: declarator
	| ':' constant_expr
		{$$ = NULL;}
	| declarator ':' constant_expr
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
		{ make_decl ($1, FLAG_TYPE_ENUM);$1->desc.decl->members = $3;}
	| ENUM identifier '{' enumerator_list '}'
		{ make_decl ($1,FLAG_TYPE_ENUM);$1->desc.decl->members = $4;}
	| ENUM identifier
		{ make_decl ($1,FLAG_TYPE_ENUM); $1->desc.decl->members = NULL;}
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
		{
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of enumerators                           *
*                                                        *
**********************************************************
*/

			if ($1){
				t = $1;
				while (t->next) t = t->next;
				t->next = $3;
			}
			else $$ = $3;
		}
	| enumerator_list ','
	;

enumerator
	: identifier
	| identifier '=' constant_expr
	;

declarator
	: declarator2
	| pointer declarator2
		{	int i;
			for (i = 0; i < (int)$1; i++)
				modify_type ($2,IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
			$$ = $2;
		}
	;

parms_next : /* empty */
		{ /*printf ("PARM(%d)\n",lineno);*/in_parms = 1;}
	;
declarator2
	: identifier
	| '(' declarator ')'	{$$ = $2;}
	| declarator2 '[' ']'
		{modify_type ($1,IS_ARRAY_OF,NO_STYLE,(token_ptr)NULL);}
	| declarator2 '[' constant_expr ']'
		{modify_type ($1,IS_ARRAY_OF,NO_STYLE,(token_ptr)NULL);}
	| declarator2 parms_next '(' ')'
		{modify_type ($1,IS_FUNC_RET,OLD_STYLE_VOID,(token_ptr)NULL);
			in_parms = 0;}
	| declarator2 parms_next '(' parameter_type_list ')'
		{modify_type ($1,IS_FUNC_RET,NEW_STYLE,$4); in_parms = 0;}
	| declarator2 parms_next '(' parameter_identifier_list ')'
		{modify_type ($1,IS_FUNC_RET,OLD_STYLE,$4); in_parms = 0;}
	;

pointer
	: '*'
			{$$ = (token_ptr)1;}
	| '*' type_qualifier_list
			{$$ = (token_ptr)1;}
	| '*' pointer
			{$$ = (token_ptr)((int)$2 + 1);}
	| '*' type_qualifier_list pointer
			{$$ = (token_ptr)((int)$3 + 1);}
	;

type_qualifier
	: CONST {make_decl ($1, FLAG_TYPE_CONST);}
	| VOLATILE {make_decl ($1, FLAG_TYPE_VOLATILE);}
	;
type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;

/*
type_specifier_list
	: type_specifier
	| type_specifier_list type_specifier
		{
			$1->desc.decl->decl_flags |= $2->desc.decl->decl_flags;
			if ($1->desc.decl->tag == NULL)
				$1->desc.decl->tag = $2->desc.decl->tag;
			if ($1->desc.decl->vars == NULL)
				$1->desc.decl->vars = $2->desc.decl->vars;
			if ($1->desc.decl->members == NULL)
				$1->desc.decl->members = $2->desc.decl->members;
			if ($1->desc.decl->member_ste == NULL)
				$1->desc.decl->member_ste = $2->desc.decl->member_ste;
		}
	;
	*/

parameter_identifier_list
	: identifier_list
	/*
	| identifier_list ',' ELIPSIS
	*/
	;

identifier_list
	: identifier
	| identifier_list ',' identifier
		{
			token_ptr	t;

			if ($1){
				t = $1;
				while (t->next) t = t->next;
				t->next = $3;
			}
			else $$ = $3;
		}
	;

parameter_type_list
	: parameter_list
	| parameter_list ',' ELIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
		{
			token_ptr	t;

			if ($1){
				t = $1;
				while (t->next) t = t->next;
				t->next = $3;
			}
			else $$ = $3;
		}
	;

parameter_declaration
	: declaration_specifiers declarator
/*	: type_specifier_list declarator */
		{
	/* : declaration_specifiers declarator */
			$1->desc.decl->vars = $2;
		}
		/*
	| REGISTER type_specifier_list declarator
		{
			$2->desc.decl->vars = $3;
			$$ = $2;
		}
	| type_name
		*/
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

type_name
	/*
	: type_specifier_list
	| type_specifier_list abstract_declarator
	*/
	: specifier_qual_list
	| specifier_qual_list abstract_declarator
		{
			$1->desc.type_desc = (type_ptr) $2;
		}
	;

abstract_declarator
	: pointer
		{	int i;
			type_ptr	old = NULL,new;

			for (i = 0; i < (int)$1; i++){
				new = make_abstract_type (
					IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
				if (!old) old = new;
				else merge_abstract_type (old,new);
			}
			$$ = (token_ptr)old;
		}
	| abstract_declarator2
	| pointer abstract_declarator2
		{	int i;
			type_ptr	old = NULL,new;

			if ($2) old = (type_ptr)$2;

			for (i = 0; i < (int)$1; i++){
				new = make_abstract_type (
					IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
				if (!old) old = new;
				else merge_abstract_type (old,new);
			}
			$$ = (token_ptr)old;
		}
	;

abstract_declarator2
	: '(' abstract_declarator ')'
		{$$ = $2;}
	| '[' ']'
		{$$ = (token_ptr) make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);}
	| '[' constant_expr ']'
		{$$ = (token_ptr) make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);}
	| abstract_declarator2 '[' ']'
		{type_ptr	new;
			new = make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);
			if ($1){
				merge_abstract_type ($1,new);
			}
			else $$ = (token_ptr) new;
		}
	| abstract_declarator2 '[' constant_expr ']'
		{type_ptr	new;
			new = make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);
			if ($1){
				merge_abstract_type ($1,new);
			}
			else $$ = (token_ptr) new;
		}
	| '(' ')'
		{$$ = NULL;}
	| '(' parameter_type_list ')'
		{$$ = NULL;}
	| abstract_declarator2 parms_next '(' ')'
	| abstract_declarator2 parms_next '(' parameter_type_list ')'
	;

initializer
	: assignment_expr
		{ $$ = NULL;}
	| '{' initializer_list '}'
		{$$ = NULL;}
	| '{' initializer_list ',' '}'
		{$$ = NULL;}
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: identifier ':' statement
		{$$ = stmt_label($1,$3);}
	| CASE constant_expr ':' statement
		{$$ = case_stmt ($1,$3,$4,False);}
	| DEFAULT ':' statement
		{$$ = case_stmt ($1,$2,$3,True);}
	;

decl_end	
	: declaration_list
		{
			end_local_decl();
		}
	;

decl_start	
	: /* empty */
		{
			open_scope();
			start_local_decl();
		}
	;

compound_statement
	: '{' '}'
			{$$ = brace_stmt ($1,(token_ptr)NULL,$2);}
	| '{' statement_list '}'
			{$$ = brace_stmt ($1,$2,$3);}
	| '{' decl_start decl_end '}'
			{
				close_scope();
				$$ = brace_stmt ($1,(token_ptr)NULL,$4);
			}
	| '{' decl_start decl_end statement_list '}'
			{
				close_scope();
				$$ = brace_stmt ($1,$4,$5);
			}
	;

declaration_list
	: declaration
	| declaration_list declaration
		{
			if ($1){
				$1->next = $2;
			}
			else $$ = $2;
		}
	;

statement_list
	: statement
	| statement_list statement
		{ $$ = join_stmt ($1,$2);}
	;

expression_statement
	: ';'
		{
			int		node;

			node = get_stmt_no();
			source_map (node,$1,$1);
			$$ = make_stmt (node,node,node,node);
		}
	| expr ';'
		{int	from_node,to_node;
		int		node_id;

		node_id = xpr_gen ($1, &from_node,&to_node);
		$$ = make_stmt (node_id,stmtno,from_node,to_node);
		source_map (node_id,leftmost($1),$2);
		}
	;

selection_statement
	: IF '(' expr ')' statement
		{$$ = if_stmt ($1,$3,$4,$5,(token_ptr)NULL,(stmt_ptr)NULL);}
	| IF '(' expr ')' statement ELSE statement
		{$$ = if_stmt ($1,$3,$4,$5,$6,$7);}
	| SWITCH '(' expr ')' statement
		{$$ = switch_stmt ($1,$3,$4,$5);}
	;
oexpr : /* optional */
		{$$ = NULL;}
	| expr
	;
iteration_statement
	: WHILE '(' expr ')' statement
		{$$ = while_stmt ($1,$3,$4,$5);}
	| DO statement WHILE '(' expr ')' ';'
		{$$ = do_stmt($1,$2,$3,$5,$7);}
	| FOR '(' oexpr ';' oexpr ';' oexpr ')' statement
		{$$ = for_stmt ($1,$2,$3,$4,$5,$6,$7,$8,$9);}
	/*
	| FOR '(' ';' ';' ')' statement
	| FOR '(' ';' ';' expr ')' statement
	| FOR '(' ';' expr ';' ')' statement
	| FOR '(' ';' expr ';' expr ')' statement
	| FOR '(' expr ';' ';' ')' statement
	| FOR '(' expr ';' ';' expr ')' statement
	| FOR '(' expr ';' expr ';' ')' statement
	| FOR '(' expr ';' expr ';' expr ')' statement
	*/
	;

jump_statement
	: GOTO identifier ';'
		{
			$$ = goto_stmt ($1,$2,$3);
		}
	| CONTINUE ';'
		{$$ = continue_stmt ($1,$2);}
	| BREAK ';'
		{$$ = break_stmt ($1,$2);}
	| RETURN ';'
		{$$ = return_stmt ($1,(tree_ptr)NULL,$2);}
	| RETURN expr ';'
		{$$ = return_stmt ($1,$2,$3);}
	;

program : {$$ = NULL;}
		| file
		{
			close_scope();
		}
	;
file
	: external_definition
	| file external_definition
	;

external_definition
	: function_definition
	| declaration
	| ';'
	/*
	| IDENTIFIER "(" ")" ";"
	*/
	;

function_start	
	: /* empty */
		{
			token_ptr	tk,parm;
			type_ptr	t;
			
			$$ = fun_stmt ($<token>-1);
			open_scope();
			start_local_decls();
			tk = $<token>-1;
				/*printf ("Start Fundtion %s",tk->text); */
				t = tk->desc.type_desc;
				while (t){
					/*printf ("(mod %d style %d)",
						t->is,t->style); */
					if ((t->is == IS_FUNC_RET) &&
						(t->style == NEW_STYLE)){
						parm = t->formals;
						while (parm){

							decl (parm, parm->desc.decl->vars,False);
							parm = parm->next;
						}
					}
					t=t->next;
				}
				/*printf ("\n"); */
			clear_type_flag();
		}
	;

function_definition
	: declarator { } function_start ';'
	| declarator { } function_start function_body
			{
				int			end_stmt;

				end_stmt = $4->exit;
				do_returns (end_stmt);
				connect_nodes ($3->exit,$4->entry);
				gen_require ($4->entry,$3->exit,$3->exit);
				if (current_proc){
					current_proc->exit_node = $4->exit;
					current_proc->n_locals = current_local();
				}

				close_scope();
				fprintf (outfile,"%d(%d",LIF_PROC_END,$4->exit);
				if (current_proc->has_return_xpr)
					fprintf (outfile,",R");
				fprintf (outfile,")");
				if (z_opt)fprintf (outfile,"    end function %s",
					$1->text);
				fprintf (outfile,"\n");
			}
	| declaration_specifiers declarator
			{
				decl ($1,NULL,False);
			}
		function_start function_body
			{
				int	is_static = 0;

				do_returns ($5->exit);
				connect_nodes ($4->exit,$5->entry);
				gen_require ($5->entry,$4->exit,$4->exit);
				close_scope();
				fprintf (outfile,"%d(%d",LIF_PROC_END,$5->exit);
				if($1->desc.decl->decl_flags & FLAG_SC_STATIC){
					fprintf (outfile,",S");
					is_static = 1;
				}
				if (current_proc->has_return_xpr)
					fprintf (outfile,",R");
				fprintf (outfile,")");
				if (z_opt)fprintf (outfile,"    end %sfunction %s",
					is_static?"static ":"",
					$2->text);
				fprintf (outfile,"\n");
				if (current_proc){
					if (is_static) current_proc->static_or_extern = 'S';
					current_proc->type_decl = $1;
					current_proc->exit_node = $5->exit;
					current_proc->n_locals = current_local();
				}
			}
	;

function_body
	: compound_statement
		{$$ = $1;}
	| declaration_list
		{
			token_ptr	fun,parm;
			type_ptr	t;
			int			pn,level;
			var_ste_ptr	v;

			$<token>$ = NULL;
			fun = $<token>-2;
			/*
			printf ("\tOld Fundtion body %s: ",$<token>-1->text); 
			*/
				t = fun->desc.type_desc;
				while (t){
					/* printf ("(mod %d style %d)",
						t->is,t->style);  */
					if ((t->is == IS_FUNC_RET) &&
						(t->style == OLD_STYLE)){
						parm = t->formals;
						pn = 1;
						while (parm){
							/* printf ("%s(%d) ",parm->text,pn); */
							v = look_up_id (NULL,parm->text,&level);
							if (!v || (level != 2)){
								/* printf ("not a parm  "); */
								insert_var_decl (parm);
								v = look_up_id (NULL,parm->text,&level);
							}
							/* if(v)printf ("[%d]  ",v->id) */;
							if (v->id != pn) v->id = pn;
							pn++;
							parm = parm->next;
						}
					}
					t=t->next;
				}
			/* printf ("\n"); */
		}
	compound_statement
		{$$ = $3;}
	/* | declaration_list compound_statement */
	/*                ^        */
	/*                |        */
	/*                +--- insert check of decl_list vs declarator */
	;

identifier
	: IDENTIFIER
	;
%%

void
print_flags (int flags)
{
	static char	*flag_names[] = {
		"char", "short", "int", "long", "signed",
		"unsigned", "float", "double", "const",
		"volatile", "void", "u/s", "enum", "user type",
		"typedef", "extern", "static", "auto", "register",
		"struct", "union"};
	int		i = 0;

	while (flags){
		if (flags & 1) printf ("%s ",flag_names[i]);
		i++;
		flags = flags >> 1;
	}
}

void
print_decl (type_spec,var_specs)
	token_ptr	type_spec,var_specs;
{
	token_ptr 	s,t;
	type_ptr	ty;

	printf ("Declaration: ");
	if (type_spec) {
		printf ("%8x ",type_spec->desc.decl->decl_flags);
		print_flags (type_spec->desc.decl->decl_flags);
		if (type_spec->desc.decl->decl_flags & FLAG_TYPE_US){
			printf ("\n    Struct/union: ");
			if (type_spec){
				if (type_spec->desc.decl->tag){
					printf ("(%s) ",
						type_spec->desc.decl->tag->text);
				}
				s = type_spec->desc.decl->members;
				while (s){
					if (s->desc.decl)print_decl (s,s->desc.decl->vars);
					s = s->next;
				}
			}
			printf ("\n    End Struct/union: ");
		}
		t = var_specs;
		while (t) {
			if(t->text)printf (" %s",t->text);
			if (ty = t->desc.type_desc){
				printf (" (");
				while(ty){
					if (ty->is == IS_FUNC_RET) 
						printf (" function returning");
					else if (ty->is == IS_ARRAY_OF) 
						printf (" array of");
					else if (ty->is == IS_PTR_TO) 
						printf (" pointer to");
					ty = ty -> next;
				}
				printf (")");
			}
			t = t -> next;
			if(t)printf ("\n%21s","");
		}
	}
	printf ("\n");
}

void
print_token(token_ptr t)
{
	printf ("TOKEN:");
	if (t){
		printf (" addr %p",t);
		if (t->text) printf (" (%s)",t->text);
		printf (" at %d(%d)-%d(%d)",
			t->at.line_start,
			t->at.col_start,
			t->at.line_end,
			t->at.col_end);
	}
	else printf (" is null");
	printf ("\n");
}

