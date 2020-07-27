//extern char *malloc(), *realloc();

# line 2 "kgram.y"
# include <stdlib.h>
# include <stdio.h>
# include "ansi_parse.h"
# include "lif.h"
# include "sym_tab.h"
# include "stmt.h"

extern void clear_type_flag(void);
extern void end_init(void);

void start_local_decl(void);
int yyerror(char *msg);
int yylex(void);

static	int set_typedef = 0;
// static  char    sccsid[] = "@(#)kgram.y	1.7  10/26/94";
static	char	*parse_h_sccs_id = PARSE_H_SCCS_ID;
static	char	*lif_h_sccs_id = LIF_H_SCCS_ID;

# define IDENTIFIER 257
# define CONSTANT 258
# define STRING_LITERAL 259
# define SIZEOF 260
# define PTR_OP 261
# define INC_OP 262
# define DEC_OP 263
# define LEFT_OP 264
# define RIGHT_OP 265
# define LE_OP 266
# define GE_OP 267
# define EQ_OP 268
# define NE_OP 269
# define AND_OP 270
# define OR_OP 271
# define MUL_ASSIGN 272
# define DIV_ASSIGN 273
# define MOD_ASSIGN 274
# define ADD_ASSIGN 275
# define SUB_ASSIGN 276
# define LEFT_ASSIGN 277
# define RIGHT_ASSIGN 278
# define AND_ASSIGN 279
# define XOR_ASSIGN 280
# define OR_ASSIGN 281
# define TYPE_NAME 282
# define TYPEDEF 283
# define EXTERN 284
# define STATIC 285
# define AUTO 286
# define REGISTER 287
# define CHAR 288
# define SHORT 289
# define INT 290
# define LONG 291
# define SIGNED 292
# define UNSIGNED 293
# define FLOAT 294
# define DOUBLE 295
# define CONST 296
# define VOLATILE 297
# define VOID 298
# define STRUCT 299
# define UNION 300
# define ENUM 301
# define ELIPSIS 302
# define RANGE 303
# define CASE 304
# define DEFAULT 305
# define IF 306
# define ELSE 307
# define SWITCH 308
# define WHILE 309
# define DO 310
# define FOR 311
# define GOTO 312
# define CONTINUE 313
# define BREAK 314
# define RETURN 315
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 1069 "kgram.y"

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
print_token(t)
	token_ptr	t;
{
	printf ("TOKEN:");
	if (t){
		printf (" addr %d",t);
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
int yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 7,
	59, 224,
	-2, 226,
-1, 9,
	40, 137,
	-2, 135,
-1, 46,
	59, 89,
	44, 89,
	-2, 228,
-1, 50,
	40, 137,
	-2, 136,
-1, 58,
	123, 108,
	-2, 111,
-1, 255,
	40, 137,
	-2, 166,
-1, 306,
	40, 137,
	-2, 167,
	};
# define YYNPROD 234
# define YYLAST 1381
int yyact[]={

    96,    14,   311,    14,   194,   205,   333,   332,   364,    14,
   151,    14,    49,   132,   123,   131,    14,   192,   106,   138,
    16,   139,   166,   119,     5,   182,     5,   136,   137,   142,
   143,   120,     8,   219,     8,   255,    69,   272,    58,    61,
    35,    15,   172,    51,    52,    53,    24,    25,    26,    27,
    28,    29,    30,    31,    22,    23,    32,    39,    40,    38,
    60,   112,     9,    15,    36,    16,    14,    15,    59,    16,
   196,    77,   117,    50,   301,    76,   150,   124,   186,   105,
   111,   184,    80,    73,    75,   133,    44,   186,   257,   152,
   107,   272,   118,    16,   175,   354,    74,    15,   113,    16,
   134,   173,    72,   168,   345,   115,   309,   314,    14,   129,
   307,   256,    48,    95,   112,    66,   187,    47,    90,   201,
    83,    14,    91,    92,   177,    93,   368,    79,   252,    12,
    81,   366,   174,   252,   116,   275,   124,   232,    65,   204,
   217,   189,   257,   336,   353,   155,    57,    62,   289,   335,
   274,   156,   297,    64,   296,   300,   252,   262,   263,   280,
   152,    10,   257,   114,   327,   288,   261,   152,   185,   286,
   303,   152,    95,   276,   153,    14,   252,    90,    56,    83,
   346,    91,    92,   121,    93,   103,   144,   112,   145,   109,
   148,    63,   110,   201,    36,   146,   154,   283,   204,   116,
   147,   365,    36,   118,   252,   347,    94,    78,   237,   238,
   201,   236,   295,   109,   293,   128,   233,   102,   163,   235,
   299,   277,   268,   130,   279,   140,   141,   245,   246,   302,
   234,    95,   267,   231,   230,   109,    90,   178,    83,   287,
    91,    92,   264,    93,   220,   221,   222,   223,   224,   225,
   226,   227,   228,   229,    36,   273,   153,   204,    36,   370,
   253,   281,   118,   305,   324,    94,   285,   278,   308,   320,
   243,   244,    14,    14,    22,    23,    14,   247,   248,   249,
    36,   270,   363,   178,    36,   201,   344,   201,   174,   201,
   306,   326,   177,   328,   313,   109,   329,   330,   331,   174,
   334,   322,   325,   343,   174,   109,   188,   306,    36,   189,
   319,     7,   124,   323,    36,   254,   338,   317,   315,   308,
    46,   118,   215,   282,    94,   266,   201,    54,   201,   265,
   283,   251,   348,   250,    50,   101,   271,    36,    97,    99,
    88,   352,    85,    86,   341,   356,   239,   240,   241,   242,
   201,   201,   201,   342,   358,   359,   360,   357,   361,   334,
   362,   294,   272,   310,    16,   201,   157,   158,   159,   367,
   292,   201,   334,   369,   174,   371,   351,   122,   350,   252,
   349,   252,   304,   252,   202,   203,   206,   127,   207,   208,
   209,   210,   211,   212,   213,   214,    36,    97,    99,    88,
    95,    85,    86,   109,   291,    90,   149,    83,   290,    91,
    92,   100,    93,   257,   135,   190,    67,     3,   254,   183,
    41,    43,    42,   104,   316,    55,   204,    98,     4,     2,
     1,   200,   122,   199,   271,   198,    82,   197,   193,   284,
   195,   216,   170,   202,   203,   206,   171,   207,   208,   209,
   210,   211,   212,   213,   214,    36,    97,    99,    88,    95,
    85,    86,   169,    34,    90,   180,    83,    37,    91,    92,
    33,    93,    11,   160,   162,    82,   164,   258,    45,   218,
    71,    87,   260,    84,   149,   204,   269,    89,   149,     0,
   118,     0,   191,    94,     0,     0,     0,     0,     0,     0,
     0,   167,   202,   203,   206,     0,   207,   208,   209,   210,
   211,   212,   213,   214,     0,     0,     0,     0,    82,     0,
    82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
    82,    82,    82,    82,    82,    82,    82,     0,    95,     0,
     0,     0,     0,    90,     0,    83,     0,    91,    92,   118,
    93,     0,    94,     0,     0,     0,    95,     0,     0,     0,
     0,    90,     0,    83,     0,    91,    92,     0,    93,     0,
     0,     0,    82,     0,     0,    82,     0,     0,     0,    36,
     0,   256,   310,    16,    54,     0,     0,   183,     0,     0,
    82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    35,    17,    18,    19,    20,    21,
    24,    25,    26,    27,    28,    29,    30,    31,    22,    23,
    32,    39,    40,    38,    36,    97,    99,    88,     0,    85,
    86,    94,   257,    15,     0,    16,     0,     0,     0,    82,
     0,     0,     0,     0,     0,    82,   125,     0,   337,    94,
     0,     0,     6,     0,   355,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
     0,   202,   203,   206,     0,   207,   208,   209,   210,   211,
   212,   213,   214,    36,    97,    99,    88,     0,    85,    86,
     0,    82,   118,     0,     0,    82,    35,    17,    18,    19,
    20,    21,    24,    25,    26,    27,    28,    29,    30,    31,
    22,    23,    32,    39,    40,    38,   318,    36,   256,     0,
    16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   202,   203,   206,     0,   207,   208,   209,   210,   211,   212,
   213,   214,    35,    17,    18,    19,    20,    21,    24,    25,
    26,    27,    28,    29,    30,    31,    22,    23,    32,    39,
    40,    38,    36,    97,    99,    88,     0,    85,    86,   257,
    15,     0,    16,     0,     0,     0,     0,     0,   321,     0,
    36,    97,    99,    88,     0,    85,    86,    35,   184,   181,
     0,     0,     0,    24,    25,    26,    27,    28,    29,    30,
    31,    22,    23,    32,    39,    40,    38,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    35,    17,    18,    19,    20,    21,    24,
    25,    26,    27,    28,    29,    30,    31,    22,    23,    32,
    39,    40,    38,     0,     0,     0,     0,     0,     0,     0,
    36,    35,    17,    18,    19,    20,    21,    24,    25,    26,
    27,    28,    29,    30,    31,    22,    23,    32,    39,    40,
    38,     0,     0,     0,     0,    35,    17,    18,    19,    20,
    21,    24,    25,    26,    27,    28,    29,    30,    31,    22,
    23,    32,    39,    40,    38,    35,    17,    18,    19,    20,
    21,    24,    25,    26,    27,    28,    29,    30,    31,    22,
    23,    32,    39,    40,    38,    35,    17,    18,    19,    20,
    21,    24,    25,    26,    27,    28,    29,    30,    31,    22,
    23,    32,    39,    40,    38,    35,   176,   126,     0,     0,
     0,    24,    25,    26,    27,    28,    29,    30,    31,    22,
    23,    32,    39,    40,    38,     0,     0,     0,     0,     0,
    35,     0,     0,     0,     0,     0,    24,    25,    26,    27,
    28,    29,    30,    31,    22,    23,    32,    39,    40,    38,
    13,     0,     0,    95,     0,     0,    70,    36,    90,     0,
    83,    95,    91,    92,     0,    93,    90,     0,    83,    95,
    91,    92,     0,    93,    90,     0,    83,   259,    91,    92,
     0,    93,    35,     0,     0,     0,     0,     0,    24,    25,
    26,    27,    28,    29,    30,    31,    22,    23,    32,    39,
    40,    38,     0,     0,     0,     0,     0,    95,     0,     0,
   108,     0,    90,   340,    83,    95,    91,    92,     0,    93,
    90,   312,    83,     0,    91,    92,     0,    93,     0,     0,
    95,     0,     0,   298,   108,    90,     0,    83,    95,    91,
    92,     0,    93,    90,     0,    83,    94,    91,    92,     0,
    93,     0,     0,     0,    94,     0,   108,     0,   179,     0,
     0,     0,    94,    35,     0,     0,     0,     0,     0,    24,
    25,    26,    27,    28,    29,    30,    31,    22,    23,    32,
    39,    40,    38,     0,     0,     0,     0,     0,     0,     0,
    68,     0,    70,     0,     0,    70,     0,     0,     0,     0,
    94,     0,     0,     0,   179,   125,     0,     0,    94,     0,
    70,     0,    95,     0,     0,     0,   108,    90,     0,   165,
    95,    91,    92,    94,    93,    90,   108,   161,     0,    91,
    92,    94,    93,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    70,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    36,    97,    99,
    88,     0,    85,    86,    70,    36,    97,    99,    88,     0,
    85,    86,     0,    36,    97,    99,    88,     0,    85,    86,
     0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
     0,   339,     0,    94,     0,    70,     0,     0,     0,     0,
     0,     0,     0,     0,   108,     0,     0,     0,     0,     0,
     0,    36,    97,    99,    88,     0,    85,    86,     0,    36,
    97,    99,    88,     0,    85,    86,     0,     0,     0,     0,
     0,     0,     0,     0,    36,    97,    99,    88,     0,    85,
    86,     0,    36,    97,    99,    88,     0,    85,    86,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    36,    97,    99,    88,
     0,    85,    86,     0,    36,    97,    99,    88,     0,    85,
    86 };
int yypact[]={

   593, -1000,   593, -1000, -1000, -1000, -1000, -1000,    27,    21,
     1,   633,   633,   633, -1000,    57,   -22, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000,   -55,   -63, -1000,
 -1000, -1000, -1000, -1000, -1000,    94,    54, -1000,  1027,   371,
    21, -1000, -1000, -1000,   294,   -22, -1000, -1000, -1000,  -242,
  -193,   -25,   104,   569, -1000,    57,  1012, -1000, -1000,    16,
 -1000,   -48,  -257,   -39,     6,   376,  -241,   -41,  -235,   143,
   153, -1000, -1000,   505,   105,  1117,  1117,  1035,  1109, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,  -237, -1000,
   460, -1000, -1000, -1000,   -29,   811, -1000,   730, -1000, -1000,
    43, -1000,    55,  -193, -1000, -1000, -1000,   633,   367, -1000,
    27, -1000,    54, -1000, -1000,  1012, -1000,   -28,   569, -1000,
  1035,  1035,  1035,  1035,  1035,  1035,  1035,  1035,  1035,  1035,
  1035,  1035,  1035,  1035,  1035,  1035,  1035,  1035,  1035,   292,
   290, -1000,   132,   678,  1035,   966,  -193,  -193, -1000, -1000,
 -1000,  1035, -1000, -1000, -1000,   505, -1000, -1000,   288,   284,
   188,   178, -1000, -1000,    51,  -242, -1000, -1000, -1000, -1000,
    91, -1000, -1000,   115,  1035, -1000,  -193,  1035,    34, -1000,
   -31, -1000,   198,   633, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000,   111,  1035,   107, -1000,    89,   368,   364,   330,   426,
   321,  -193,    95,    93,  1004, -1000,    30, -1000,  1035, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000,  -257,   112,   -39,     6,   376,  -241,   -41,   -41,  -235,
  -235,  -235,  -235,   143,   143,   153,   153, -1000, -1000, -1000,
 -1000,  1035,  1035, -1000,    71,    19,   541,   958,    14, -1000,
   277, -1000, -1000, -1000,   276, -1000, -1000,   414,  -193, -1000,
 -1000,    -3,   322,   653, -1000,    23,  1035, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,   139,   633,   426,   106,   426, -1000,
  1035,  1035,  1035,  -302,  1035,    90, -1000, -1000, -1000,    84,
 -1000,   523, -1000,  1035, -1000, -1000,    19,   950,   313,   262,
 -1000,   245, -1000,    11, -1000, -1000,   136, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000,    80, -1000,   426, -1000,   339,
   337,   335,   301,    85,   132, -1000, -1000, -1000, -1000, -1000,
 -1000,     2,   613, -1000, -1000, -1000,  1035, -1000, -1000,   426,
   426,   426,  1035,  1035, -1000, -1000,   241, -1000,  -299, -1000,
 -1000,   160,    72, -1000,   426,    67,  1035, -1000, -1000,   218,
   426, -1000 };
int yypgo[]={

     0,   487,   483,   482,   387,   481,   130,    82,   127,   207,
    71,    75,    84,    96,    83,   102,   480,   937,    10,   479,
     6,     5,   322,    36,    23,    31,   478,   117,   472,   980,
   470,   467,    79,    18,   465,    25,   463,   192,    80,   311,
    62,   161,   462,   446,     2,   442,    42,    76,   106,    35,
    14,   441,     4,   440,   439,   438,    70,    72,    17,   437,
   435,   433,   431,   105,   430,   429,   417,   428,   147,     0,
   427,    90,   129,   425,   424,   423,    12,   422,   421,   416,
   415 };
int yyr1[]={

     0,     1,     1,     1,     1,    70,    70,     2,     2,     2,
     2,     2,     2,     2,     2,     3,     3,    74,     4,     4,
     4,     4,     4,     4,     5,     5,     5,     5,     5,     5,
     6,     6,     7,     7,     7,     7,     8,     8,     8,     9,
     9,     9,    10,    10,    10,    10,    10,    11,    11,    11,
    12,    12,    13,    13,    14,    14,    15,    15,    16,    16,
    17,    17,    18,    18,    19,    19,    19,    19,    19,    19,
    19,    19,    19,    19,    19,    21,    22,    22,    23,    24,
    24,    25,    25,    25,    25,    25,    25,    26,    26,    27,
    27,    28,    28,    28,    28,    28,    29,    29,    29,    29,
    29,    29,    29,    29,    29,    29,    29,    29,    75,    30,
    30,    30,    31,    31,    32,    32,    71,    71,    71,    71,
    33,    33,    34,    34,    35,    35,    35,    36,    36,    36,
    37,    37,    37,    38,    38,    39,    39,    76,    40,    40,
    40,    40,    40,    40,    40,    41,    41,    41,    41,    72,
    72,    73,    73,    42,    43,    43,    44,    44,    45,    45,
    46,    46,    46,    47,    47,    48,    48,    48,    49,    49,
    49,    49,    49,    49,    49,    49,    49,    50,    50,    50,
    51,    51,    52,    52,    52,    52,    52,    52,    53,    53,
    53,    54,    55,    56,    56,    56,    56,    57,    57,    58,
    58,    59,    59,    60,    60,    60,    20,    20,    61,    61,
    61,    62,    62,    62,    62,    62,    64,    64,    65,    65,
    66,    66,    66,    68,    77,    67,    78,    67,    79,    67,
    63,    80,    63,    69 };
int yyr2[]={

     0,     3,     3,     3,     7,     2,     4,     2,     9,     7,
     9,     7,     7,     5,     5,     2,     9,     1,     2,     5,
     5,     5,     5,     9,     3,     3,     3,     3,     3,     3,
     2,     9,     2,     7,     7,     7,     2,     7,     7,     2,
     7,     7,     2,     7,     7,     7,     7,     2,     7,     7,
     2,     7,     2,     7,     2,     7,     2,     7,     2,     7,
     2,    11,     2,     7,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     2,     2,     7,     2,     5,
     7,     2,     5,     2,     5,     2,     5,     2,     7,     2,
     7,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     2,     2,     3,     1,    13,
     9,     5,     3,     3,     2,     5,     2,     2,     5,     5,
     7,     5,     2,     7,     2,     5,     6,     9,    11,     5,
     2,     7,     4,     2,     6,     2,     5,     1,     2,     7,
     7,     9,     9,    11,    11,     3,     5,     5,     7,     3,
     3,     2,     4,     2,     2,     7,     2,     6,     2,     7,
     5,     4,     2,     2,     5,     3,     2,     5,     7,     5,
     7,     7,     9,     5,     7,     8,    10,     3,     7,     9,
     2,     6,     2,     2,     2,     2,     2,     2,     7,     9,
     7,     3,     1,     5,     7,     9,    11,     2,     5,     2,
     5,     3,     5,    11,    15,    11,     1,     2,    11,    15,
    19,     7,     5,     5,     5,     7,     1,     3,     2,     4,
     2,     2,     2,     1,     1,     8,     1,     9,     1,    11,
     3,     1,     7,     2 };
int yychk[]={

 -1000,   -64,   -65,   -66,   -67,   -24,    59,   -39,   -25,   -40,
   -41,   -28,   -72,   -29,   -69,    40,    42,   283,   284,   285,
   286,   287,   296,   297,   288,   289,   290,   291,   292,   293,
   294,   295,   298,   -30,   -36,   282,   257,   -31,   301,   299,
   300,   -66,   -77,   -78,    59,   -26,   -39,   -27,    91,   -76,
   -40,   -25,   -25,   -25,   -39,   -73,   -41,   -72,   -69,   123,
   123,   -69,   -68,   -68,    59,    44,    61,   -79,    93,   -23,
   -17,   -16,   -15,   -14,   -13,   -12,   -11,   -10,    -9,    -8,
    -7,    -6,    -4,    40,    -2,   262,   263,    -5,   260,    -1,
    38,    42,    43,    45,   126,    33,   -69,   258,   -70,   259,
    40,    41,   -41,   -72,   -75,   -32,   -33,   -71,   -29,   -72,
   -37,   -38,   -69,   123,    59,   -63,   -56,   -57,   123,   -24,
   -25,   -27,   -39,   -50,   -18,   123,   -17,    -4,   -68,    93,
   271,    63,   270,   124,    94,    38,   268,   269,    60,    62,
   266,   267,   264,   265,    43,    45,    42,    47,    37,   -22,
   -47,   -18,   -21,   -71,    91,    40,    46,   261,   262,   263,
    -4,    40,    -4,    -6,    -4,    40,   259,    41,   -44,   -42,
   -45,   -43,   -46,   -69,   -25,   123,   125,   -33,   -72,   -29,
   -34,    59,   -35,   -39,    58,   125,    44,    61,   -37,   -24,
   -80,   125,   -58,   -55,   -52,   -53,   -56,   -59,   -60,   -61,
   -62,   -69,   304,   305,    59,   -21,   306,   308,   309,   310,
   311,   312,   313,   314,   315,   -22,   -51,   -50,   -19,    61,
   272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
   -63,   -15,   -21,   -14,   -13,   -12,   -11,   -10,   -10,    -9,
    -9,    -9,    -9,    -8,    -8,    -7,    -7,    -6,    -6,    -6,
    41,    41,    44,   -48,   -41,   -49,    40,    91,   -22,    41,
    -3,   -18,   -69,   -69,   -47,    41,    41,    44,    44,   -39,
   -48,   -41,    40,   -32,    59,    44,    58,   -23,   -38,   -23,
   125,   -56,   125,   -52,   -54,   -57,    58,   -23,    58,    59,
    40,    40,    40,   -52,    40,   -69,    59,    59,    59,   -21,
   125,    44,   -18,    58,    -6,   -18,   -49,    91,   -76,   -48,
    41,   -44,    93,   -23,    93,    41,   -74,    41,   302,   -46,
   -69,   125,   -35,   -23,   125,   -58,   -52,    58,   -52,   -21,
   -21,   -21,   309,   -20,   -21,    59,    59,   125,   -50,   -17,
    93,   -23,    40,    41,    41,    93,    44,   125,   -52,    41,
    41,    41,    40,    59,    93,    41,   -44,   -18,   -52,   -52,
   -52,   -21,   -20,    41,   307,    41,    59,   -52,    59,   -20,
    41,   -52 };
int yydef[]={

   216,    -2,   217,   218,   220,   221,   222,    -2,     0,    -2,
     0,    81,    83,    85,   138,     0,   145,    91,    92,    93,
    94,    95,   149,   150,    96,    97,    98,    99,   100,   101,
   102,   103,   104,   105,   106,   107,   233,     0,     0,   112,
   113,   219,   223,   223,    79,     0,    -2,    87,     0,     0,
    -2,    82,    84,    86,     0,   146,   147,   151,    -2,     0,
     0,   129,     0,     0,    80,     0,     0,   223,   140,     0,
    78,    60,    58,    56,    54,    52,    50,    47,    42,    39,
    36,    32,    30,     0,    18,     0,     0,     0,     0,     7,
    24,    25,    26,    27,    28,    29,     1,     2,     3,     5,
     0,   139,   148,   152,     0,     0,   114,     0,   116,   117,
     0,   130,   133,     0,   225,   227,   230,   231,   192,   197,
     0,    88,    89,    90,   177,     0,    62,    30,     0,   141,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
     0,    76,     0,   163,     0,     0,     0,     0,    13,    14,
    19,     0,    20,    21,    22,     0,     6,   142,     0,     0,
   156,   153,   158,   154,   162,     0,   110,   115,   118,   119,
     0,   121,   122,   124,     0,   127,   132,     0,     0,   198,
     0,   193,     0,     0,   199,   182,   183,   184,   185,   186,
   187,     1,     0,     0,   201,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    75,     0,   180,     0,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
   229,    59,     0,    57,    55,    53,    51,    48,    49,    43,
    44,    45,    46,    40,    41,    37,    38,    33,    34,    35,
     4,     0,     0,   164,   165,    -2,     0,     0,    75,     9,
    17,    15,    11,    12,     0,   143,   144,     0,     0,   160,
   161,   165,     0,     0,   120,     0,     0,   125,   131,   134,
   128,   232,   194,   200,     0,   191,     0,     0,     0,   202,
     0,     0,     0,     0,   206,     0,   212,   213,   214,     0,
   178,     0,    63,     0,    31,    77,    -2,     0,     0,     0,
   173,     0,   169,     0,     8,    10,     0,    23,   157,   159,
   155,   109,   123,   126,   195,     0,   188,     0,   190,     0,
     0,     0,     0,     0,   207,   211,   215,   179,   181,    61,
   171,     0,     0,   168,   174,   170,     0,   196,   189,     0,
     0,     0,     0,   206,   172,   175,     0,    16,   203,   205,
   208,     0,     0,   176,     0,     0,   206,   204,   209,     0,
     0,   210 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	1	/* allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"IDENTIFIER",	257,
	"CONSTANT",	258,
	"STRING_LITERAL",	259,
	"SIZEOF",	260,
	"PTR_OP",	261,
	"INC_OP",	262,
	"DEC_OP",	263,
	"LEFT_OP",	264,
	"RIGHT_OP",	265,
	"LE_OP",	266,
	"GE_OP",	267,
	"EQ_OP",	268,
	"NE_OP",	269,
	"AND_OP",	270,
	"OR_OP",	271,
	"MUL_ASSIGN",	272,
	"DIV_ASSIGN",	273,
	"MOD_ASSIGN",	274,
	"ADD_ASSIGN",	275,
	"SUB_ASSIGN",	276,
	"LEFT_ASSIGN",	277,
	"RIGHT_ASSIGN",	278,
	"AND_ASSIGN",	279,
	"XOR_ASSIGN",	280,
	"OR_ASSIGN",	281,
	"TYPE_NAME",	282,
	"TYPEDEF",	283,
	"EXTERN",	284,
	"STATIC",	285,
	"AUTO",	286,
	"REGISTER",	287,
	"CHAR",	288,
	"SHORT",	289,
	"INT",	290,
	"LONG",	291,
	"SIGNED",	292,
	"UNSIGNED",	293,
	"FLOAT",	294,
	"DOUBLE",	295,
	"CONST",	296,
	"VOLATILE",	297,
	"VOID",	298,
	"STRUCT",	299,
	"UNION",	300,
	"ENUM",	301,
	"ELIPSIS",	302,
	"RANGE",	303,
	"CASE",	304,
	"DEFAULT",	305,
	"IF",	306,
	"ELSE",	307,
	"SWITCH",	308,
	"WHILE",	309,
	"DO",	310,
	"FOR",	311,
	"GOTO",	312,
	"CONTINUE",	313,
	"BREAK",	314,
	"RETURN",	315,
	";",	59,
	"(",	40,
	")",	41,
	"{",	123,
	"}",	125,
	":",	58,
	"&",	38,
	"*",	42,
	"~",	126,
	"+",	43,
	"-",	45,
	"!",	33,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"primary_expr : identifier",
	"primary_expr : CONSTANT",
	"primary_expr : string_literal_list",
	"primary_expr : '(' exprXX ')'",
	"string_literal_list : STRING_LITERAL",
	"string_literal_list : string_literal_list STRING_LITERAL",
	"postfix_expr : primary_expr",
	"postfix_expr : postfix_expr '[' exprXX ']'",
	"postfix_expr : postfix_expr '(' ')'",
	"postfix_expr : postfix_expr '(' argument_expr_list ')'",
	"postfix_expr : postfix_expr '.' identifier",
	"postfix_expr : postfix_expr PTR_OP identifier",
	"postfix_expr : postfix_expr INC_OP",
	"postfix_expr : postfix_expr DEC_OP",
	"argument_expr_list : assignment_expr",
	"argument_expr_list : argument_expr_list comma_comming ',' assignment_expr",
	"comma_comming : /* empty */",
	"unary_expr : postfix_expr",
	"unary_expr : INC_OP unary_expr",
	"unary_expr : DEC_OP unary_expr",
	"unary_expr : unary_operator cast_expr",
	"unary_expr : SIZEOF unary_expr",
	"unary_expr : SIZEOF '(' type_name ')'",
	"unary_operator : '&'",
	"unary_operator : '*'",
	"unary_operator : '+'",
	"unary_operator : '-'",
	"unary_operator : '~'",
	"unary_operator : '!'",
	"cast_expr : unary_expr",
	"cast_expr : '(' type_name ')' cast_expr",
	"multiplicative_expr : cast_expr",
	"multiplicative_expr : multiplicative_expr '*' cast_expr",
	"multiplicative_expr : multiplicative_expr '/' cast_expr",
	"multiplicative_expr : multiplicative_expr '%' cast_expr",
	"additive_expr : multiplicative_expr",
	"additive_expr : additive_expr '+' multiplicative_expr",
	"additive_expr : additive_expr '-' multiplicative_expr",
	"shift_expr : additive_expr",
	"shift_expr : shift_expr LEFT_OP additive_expr",
	"shift_expr : shift_expr RIGHT_OP additive_expr",
	"relational_expr : shift_expr",
	"relational_expr : relational_expr '<' shift_expr",
	"relational_expr : relational_expr '>' shift_expr",
	"relational_expr : relational_expr LE_OP shift_expr",
	"relational_expr : relational_expr GE_OP shift_expr",
	"equality_expr : relational_expr",
	"equality_expr : equality_expr EQ_OP relational_expr",
	"equality_expr : equality_expr NE_OP relational_expr",
	"and_expr : equality_expr",
	"and_expr : and_expr '&' equality_expr",
	"exclusive_or_expr : and_expr",
	"exclusive_or_expr : exclusive_or_expr '^' and_expr",
	"inclusive_or_expr : exclusive_or_expr",
	"inclusive_or_expr : inclusive_or_expr '|' exclusive_or_expr",
	"logical_and_expr : inclusive_or_expr",
	"logical_and_expr : logical_and_expr AND_OP inclusive_or_expr",
	"logical_or_expr : logical_and_expr",
	"logical_or_expr : logical_or_expr OR_OP logical_and_expr",
	"conditional_expr : logical_or_expr",
	"conditional_expr : logical_or_expr '?' expr ':' conditional_expr",
	"assignment_expr : conditional_expr",
	"assignment_expr : unary_expr assignment_operator assignment_expr",
	"assignment_operator : '='",
	"assignment_operator : MUL_ASSIGN",
	"assignment_operator : DIV_ASSIGN",
	"assignment_operator : MOD_ASSIGN",
	"assignment_operator : ADD_ASSIGN",
	"assignment_operator : SUB_ASSIGN",
	"assignment_operator : LEFT_ASSIGN",
	"assignment_operator : RIGHT_ASSIGN",
	"assignment_operator : AND_ASSIGN",
	"assignment_operator : XOR_ASSIGN",
	"assignment_operator : OR_ASSIGN",
	"expr : exprXX",
	"exprXX : assignment_expr",
	"exprXX : expr ',' assignment_expr",
	"constant_expr : conditional_expr",
	"declaration : declaration_specifiers ';'",
	"declaration : declaration_specifiers init_declarator_list ';'",
	"declaration_specifiers : storage_class_specifier",
	"declaration_specifiers : storage_class_specifier declaration_specifiers",
	"declaration_specifiers : type_qualifier",
	"declaration_specifiers : type_qualifier declaration_specifiers",
	"declaration_specifiers : type_specifier",
	"declaration_specifiers : type_specifier declaration_specifiers",
	"init_declarator_list : init_declarator",
	"init_declarator_list : init_declarator_list ',' init_declarator",
	"init_declarator : declarator",
	"init_declarator : declarator '=' initializer",
	"storage_class_specifier : TYPEDEF",
	"storage_class_specifier : EXTERN",
	"storage_class_specifier : STATIC",
	"storage_class_specifier : AUTO",
	"storage_class_specifier : REGISTER",
	"type_specifier : CHAR",
	"type_specifier : SHORT",
	"type_specifier : INT",
	"type_specifier : LONG",
	"type_specifier : SIGNED",
	"type_specifier : UNSIGNED",
	"type_specifier : FLOAT",
	"type_specifier : DOUBLE",
	"type_specifier : VOID",
	"type_specifier : struct_or_union_specifier",
	"type_specifier : enum_specifier",
	"type_specifier : TYPE_NAME",
	"struct_or_union_specifier : struct_or_union identifier",
	"struct_or_union_specifier : struct_or_union identifier '{' struct_declaration_list '}'",
	"struct_or_union_specifier : struct_or_union '{' struct_declaration_list '}'",
	"struct_or_union_specifier : struct_or_union identifier",
	"struct_or_union : STRUCT",
	"struct_or_union : UNION",
	"struct_declaration_list : struct_declaration",
	"struct_declaration_list : struct_declaration_list struct_declaration",
	"specifier_qual_list : type_specifier",
	"specifier_qual_list : type_qualifier",
	"specifier_qual_list : specifier_qual_list type_qualifier",
	"specifier_qual_list : specifier_qual_list type_specifier",
	"struct_declaration : specifier_qual_list struct_declarator_list ';'",
	"struct_declaration : specifier_qual_list ';'",
	"struct_declarator_list : struct_declarator",
	"struct_declarator_list : struct_declarator_list ',' struct_declarator",
	"struct_declarator : declarator",
	"struct_declarator : ':' constant_expr",
	"struct_declarator : declarator ':' constant_expr",
	"enum_specifier : ENUM '{' enumerator_list '}'",
	"enum_specifier : ENUM identifier '{' enumerator_list '}'",
	"enum_specifier : ENUM identifier",
	"enumerator_list : enumerator",
	"enumerator_list : enumerator_list ',' enumerator",
	"enumerator_list : enumerator_list ','",
	"enumerator : identifier",
	"enumerator : identifier '=' constant_expr",
	"declarator : declarator2",
	"declarator : pointer declarator2",
	"parms_next : /* empty */",
	"declarator2 : identifier",
	"declarator2 : '(' declarator ')'",
	"declarator2 : declarator2 '[' ']'",
	"declarator2 : declarator2 '[' constant_expr ']'",
	"declarator2 : declarator2 parms_next '(' ')'",
	"declarator2 : declarator2 parms_next '(' parameter_type_list ')'",
	"declarator2 : declarator2 parms_next '(' parameter_identifier_list ')'",
	"pointer : '*'",
	"pointer : '*' type_qualifier_list",
	"pointer : '*' pointer",
	"pointer : '*' type_qualifier_list pointer",
	"type_qualifier : CONST",
	"type_qualifier : VOLATILE",
	"type_qualifier_list : type_qualifier",
	"type_qualifier_list : type_qualifier_list type_qualifier",
	"parameter_identifier_list : identifier_list",
	"identifier_list : identifier",
	"identifier_list : identifier_list ',' identifier",
	"parameter_type_list : parameter_list",
	"parameter_type_list : parameter_list ',' ELIPSIS",
	"parameter_list : parameter_declaration",
	"parameter_list : parameter_list ',' parameter_declaration",
	"parameter_declaration : declaration_specifiers declarator",
	"parameter_declaration : declaration_specifiers abstract_declarator",
	"parameter_declaration : declaration_specifiers",
	"type_name : specifier_qual_list",
	"type_name : specifier_qual_list abstract_declarator",
	"abstract_declarator : pointer",
	"abstract_declarator : abstract_declarator2",
	"abstract_declarator : pointer abstract_declarator2",
	"abstract_declarator2 : '(' abstract_declarator ')'",
	"abstract_declarator2 : '[' ']'",
	"abstract_declarator2 : '[' constant_expr ']'",
	"abstract_declarator2 : abstract_declarator2 '[' ']'",
	"abstract_declarator2 : abstract_declarator2 '[' constant_expr ']'",
	"abstract_declarator2 : '(' ')'",
	"abstract_declarator2 : '(' parameter_type_list ')'",
	"abstract_declarator2 : abstract_declarator2 parms_next '(' ')'",
	"abstract_declarator2 : abstract_declarator2 parms_next '(' parameter_type_list ')'",
	"initializer : assignment_expr",
	"initializer : '{' initializer_list '}'",
	"initializer : '{' initializer_list ',' '}'",
	"initializer_list : initializer",
	"initializer_list : initializer_list ',' initializer",
	"statement : labeled_statement",
	"statement : compound_statement",
	"statement : expression_statement",
	"statement : selection_statement",
	"statement : iteration_statement",
	"statement : jump_statement",
	"labeled_statement : identifier ':' statement",
	"labeled_statement : CASE constant_expr ':' statement",
	"labeled_statement : DEFAULT ':' statement",
	"decl_end : declaration_list",
	"decl_start : /* empty */",
	"compound_statement : '{' '}'",
	"compound_statement : '{' statement_list '}'",
	"compound_statement : '{' decl_start decl_end '}'",
	"compound_statement : '{' decl_start decl_end statement_list '}'",
	"declaration_list : declaration",
	"declaration_list : declaration_list declaration",
	"statement_list : statement",
	"statement_list : statement_list statement",
	"expression_statement : ';'",
	"expression_statement : expr ';'",
	"selection_statement : IF '(' expr ')' statement",
	"selection_statement : IF '(' expr ')' statement ELSE statement",
	"selection_statement : SWITCH '(' expr ')' statement",
	"oexpr : /* empty */",
	"oexpr : expr",
	"iteration_statement : WHILE '(' expr ')' statement",
	"iteration_statement : DO statement WHILE '(' expr ')' ';'",
	"iteration_statement : FOR '(' oexpr ';' oexpr ';' oexpr ')' statement",
	"jump_statement : GOTO identifier ';'",
	"jump_statement : CONTINUE ';'",
	"jump_statement : BREAK ';'",
	"jump_statement : RETURN ';'",
	"jump_statement : RETURN expr ';'",
	"program : /* empty */",
	"program : file",
	"file : external_definition",
	"file : file external_definition",
	"external_definition : function_definition",
	"external_definition : declaration",
	"external_definition : ';'",
	"function_start : /* empty */",
	"function_definition : declarator",
	"function_definition : declarator function_start ';'",
	"function_definition : declarator",
	"function_definition : declarator function_start function_body",
	"function_definition : declaration_specifiers declarator",
	"function_definition : declaration_specifiers declarator function_start function_body",
	"function_body : compound_statement",
	"function_body : declaration_list",
	"function_body : declaration_list compound_statement",
	"identifier : IDENTIFIER",
};
#endif /* YYDEBUG */
#line 1 "/usr/lib/yaccpar"
/*	@(#)yaccpar 1.10 89/04/04 SMI; from S5R3 1.10	*/

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	{ free(yys); free(yyv); return(0); }
#define YYABORT		{ free(yys); free(yyv); return(1); }
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** static variables used by the parser
*/
static YYSTYPE *yyv;			/* value stack */
static int *yys;			/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;			/* top of state stack */

static int yystate;			/* current state */
static int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */

int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */
	unsigned yymaxdepth = YYMAXDEPTH;

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yyv = (YYSTYPE*)malloc(yymaxdepth*sizeof(YYSTYPE));
	yys = (int*)malloc(yymaxdepth*sizeof(int));
	if (!yyv || !yys)
	{
		yyerror( "out of memory" );
		return(1);
	}
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			(void)printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			yymaxdepth += YYMAXDEPTH;
			yyv = (YYSTYPE*)realloc((char*)yyv,
				yymaxdepth * sizeof(YYSTYPE));
			yys = (int*)realloc((char*)yys,
				yymaxdepth * sizeof(int));
			if (!yyv || !yys)
			{
				yyerror( "yacc stack overflow" );
				return(1);
			}
			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			(void)printf( "Received token " );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				(void)printf( "Received token " );
				if ( yychar == 0 )
					(void)printf( "end-of-file\n" );
				else if ( yychar < 0 )
					(void)printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					(void)printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						(void)printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					(void)printf( "Error recovery discards " );
					if ( yychar == 0 )
						(void)printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						(void)printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						(void)printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			(void)printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:
# line 80 "kgram.y"
{yyval.tree = make_leaf(ID_OP,yypvt[-0].token);} break;
case 2:
# line 82 "kgram.y"
{yyval.tree = make_leaf(CON_OP,yypvt[-0].token);} break;
case 3:
# line 84 "kgram.y"
{yyval.tree = make_leaf(CON_OP,yypvt[-0].token);} break;
case 4:
# line 86 "kgram.y"
{
			tree_ptr	t;
			t = make_tree (LEFTP_OP,yypvt[-1].tree,(token_ptr)NULL);
			yyval.tree = make_tree (RIGHTP_OP,t,(token_ptr)NULL);
			t->token = yypvt[-2].token;
			yyval.tree->token = yypvt[-0].token;} break;
case 8:
# line 101 "kgram.y"
{yyval.tree = make_tree (ARRAY_OP,yypvt[-3].tree,yypvt[-1].tree);} break;
case 9:
# line 103 "kgram.y"
{yyval.tree = make_tree (CALL_OP,yypvt[-2].tree,(token_ptr)NULL);} break;
case 10:
# line 105 "kgram.y"
{yyval.tree = make_tree (CALL_OP,yypvt[-3].tree,yypvt[-1].tree);} break;
case 11:
# line 107 "kgram.y"
{yyval.tree = make_tree (DOT_OP,yypvt[-2].tree,
			make_leaf(ID_OP,yypvt[-0].token));} break;
case 12:
# line 110 "kgram.y"
{yyval.tree = make_tree (POINTER_OP,yypvt[-2].tree,
			make_leaf(ID_OP,yypvt[-0].token));} break;
case 13:
# line 113 "kgram.y"
{yyval.tree = make_tree (POST_OP,yypvt[-1].tree,(token_ptr)NULL); yyval.tree->token = yypvt[-0].token;} break;
case 14:
# line 115 "kgram.y"
{yyval.tree = make_tree (POST_OP,yypvt[-1].tree,(token_ptr)NULL); yyval.tree->token = yypvt[-0].token;} break;
case 16:
# line 121 "kgram.y"
{yyval.tree = make_tree (ARG_OP,yypvt[-3].tree,yypvt[-0].tree);} break;
case 17:
# line 124 "kgram.y"
{if(!in_init)type_seen = 0;} break;
case 19:
# line 130 "kgram.y"
{yyval.tree = make_tree (PRE_OP,yypvt[-0].tree,(token_ptr)NULL);yyval.tree->token = yypvt[-1].token;} break;
case 20:
# line 132 "kgram.y"
{yyval.tree = make_tree (PRE_OP,yypvt[-0].tree,(token_ptr)NULL);yyval.tree->token = yypvt[-1].token;} break;
case 21:
# line 134 "kgram.y"
{yyval.tree = make_tree (yypvt[-1].tree->op_code,yypvt[-0].tree,(token_ptr)NULL);
		if (yypvt[-1].tree->op_code == DEREF_OP)
			yyval.tree->token = yypvt[-1].tree->token;} break;
case 22:
# line 138 "kgram.y"
{yyval.tree = make_tree (UN_OP,yypvt[-0].tree,(token_ptr)NULL);} break;
case 23:
# line 140 "kgram.y"
{yyval.tree = make_leaf(CON_OP,yypvt[-3].token);} break;
case 24:
# line 145 "kgram.y"
{yyval.tree = make_leaf(ADDR_OP,yypvt[-0].token);} break;
case 25:
# line 147 "kgram.y"
{yyval.tree = make_leaf(DEREF_OP,yypvt[-0].token);} break;
case 26:
# line 149 "kgram.y"
{yyval.tree = make_leaf(UN_OP,yypvt[-0].token);} break;
case 27:
# line 151 "kgram.y"
{yyval.tree = make_leaf(UN_OP,yypvt[-0].token);} break;
case 28:
# line 153 "kgram.y"
{yyval.tree = make_leaf(UN_OP,yypvt[-0].token);} break;
case 29:
# line 155 "kgram.y"
{yyval.tree = make_leaf(UN_OP,yypvt[-0].token);} break;
case 31:
# line 161 "kgram.y"
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
			yyval.tree = make_tree (CAST_OP,
						make_leaf(TYPENAME_OP,yypvt[-2].token),yypvt[-0].tree);
		} break;
case 33:
# line 180 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 34:
# line 182 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 35:
# line 184 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 37:
# line 190 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 38:
# line 192 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 40:
# line 198 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 41:
# line 200 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 43:
# line 206 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 44:
# line 208 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 45:
# line 210 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 46:
# line 212 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 48:
# line 218 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 49:
# line 220 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 51:
# line 226 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 53:
# line 232 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 55:
# line 238 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 57:
# line 244 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 59:
# line 250 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 61:
# line 256 "kgram.y"
{yyval.tree = make_tree (BIN_OP,yypvt[-4].tree,
			make_tree (BIN_OP,yypvt[-2].tree,yypvt[-0].tree));
		} break;
case 63:
# line 264 "kgram.y"
{yyval.tree = make_tree ((yypvt[-1].op == 0)?RELASGN_OP:ASGN_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 64:
# line 269 "kgram.y"
{ yyval.op = 1;} break;
case 65:
# line 271 "kgram.y"
{ yyval.op = NULL;} break;
case 66:
# line 273 "kgram.y"
{ yyval.op = NULL;} break;
case 67:
# line 275 "kgram.y"
{ yyval.op = NULL;} break;
case 68:
# line 277 "kgram.y"
{ yyval.op = NULL;} break;
case 69:
# line 279 "kgram.y"
{ yyval.op = NULL;} break;
case 70:
# line 281 "kgram.y"
{ yyval.op = NULL;} break;
case 71:
# line 283 "kgram.y"
{ yyval.op = NULL;} break;
case 72:
# line 285 "kgram.y"
{ yyval.op = NULL;} break;
case 73:
# line 287 "kgram.y"
{ yyval.op = NULL;} break;
case 74:
# line 289 "kgram.y"
{ yyval.op = NULL;} break;
case 77:
# line 298 "kgram.y"
{yyval.tree = make_tree (COMMA_OP,yypvt[-2].tree,yypvt[-0].tree);} break;
case 79:
# line 307 "kgram.y"
{
			decl (yypvt[-1].token,(token_ptr)NULL,False);
			end_typedef();
		} break;
case 80:
# line 312 "kgram.y"
{
			decl (yypvt[-2].token,yypvt[-1].token,False);
			if(c_opt)print_decl (yypvt[-2].token,yypvt[-1].token);
			yypvt[-2].token->desc.decl->vars = yypvt[-1].token;
			end_typedef();
		} break;
case 82:
# line 323 "kgram.y"
{yypvt[-1].token->desc.decl->decl_flags |=
			yypvt[-0].token->desc.decl->decl_flags ;
		yypvt[-1].token->desc.decl->tag = yypvt[-0].token->desc.decl->tag;
		yypvt[-1].token->desc.decl->vars = yypvt[-0].token->desc.decl->vars;
		yypvt[-1].token->desc.decl->members = yypvt[-0].token->desc.decl->members;
		yypvt[-1].token->desc.decl->member_ste = yypvt[-0].token->desc.decl->member_ste;
		if (!yypvt[-1].token->text) yypvt[-1].token->text = yypvt[-0].token->text;
		} break;
case 84:
# line 333 "kgram.y"
{if (yypvt[-1].token && yypvt[-0].token)yypvt[-0].token->desc.decl->decl_flags |=
				yypvt[-1].token->desc.decl->decl_flags ;
			yyval.token = yypvt[-0].token;
		} break;
case 86:
# line 339 "kgram.y"
{yypvt[-1].token->desc.decl->decl_flags |=
			yypvt[-0].token->desc.decl->decl_flags ;
		if (!yypvt[-1].token->text) yypvt[-1].token->text = yypvt[-0].token->text;
		} break;
case 88:
# line 348 "kgram.y"
{
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of declarators                           *
*                                                        *
**********************************************************
*/

			if (yypvt[-2].token){
				t = yypvt[-2].token;
				while (t->next) t = t->next;
				t->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 90:
# line 370 "kgram.y"
{ end_init();} break;
case 91:
# line 375 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_SC_TYPEDEF);
			start_typedef();
			set_typedef = True;} break;
case 92:
# line 379 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_SC_EXTERN);} break;
case 93:
# line 381 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_SC_STATIC);} break;
case 94:
# line 383 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_SC_AUTO);} break;
case 95:
# line 385 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_SC_REGISTER);} break;
case 96:
# line 389 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_CHAR);} break;
case 97:
# line 390 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_SHORT);} break;
case 98:
# line 391 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_INT);} break;
case 99:
# line 392 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_LONG);} break;
case 100:
# line 393 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_SIGNED);} break;
case 101:
# line 394 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_UNSIGNED);} break;
case 102:
# line 395 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_FLOAT);} break;
case 103:
# line 396 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_DOUBLE);} break;
case 104:
# line 397 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_VOID);} break;
case 107:
# line 400 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_USER_NAME);} break;
case 108:
# line 405 "kgram.y"
{
			yypvt[-1].token->desc.decl->tag = yypvt[-0].token;
			decl (yypvt[-1].token,(token_ptr)NULL,True);
		} break;
case 109:
# line 410 "kgram.y"
{
			int		offset;

			yypvt[-5].token->desc.decl->member_ste = struct_decl(yypvt[-1].token,&offset);
			yypvt[-5].token->desc.decl->members = yypvt[-1].token;
			yypvt[-5].token->desc.decl->offset = offset;
		} break;
case 110:
# line 418 "kgram.y"
{
			int		offset;

			yypvt[-3].token->desc.decl->members = yypvt[-1].token;
			yypvt[-3].token->desc.decl->member_ste = struct_decl(yypvt[-1].token,&offset);
			yypvt[-3].token->desc.decl->offset = offset;
		} break;
case 111:
# line 426 "kgram.y"
{
			yypvt[-1].token->desc.decl->tag = yypvt[-0].token;
		} break;
case 112:
# line 433 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_US | FLAG_TYPE_STRUCT); } break;
case 113:
# line 435 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_US | FLAG_TYPE_UNION); } break;
case 115:
# line 442 "kgram.y"
{
			token_ptr	t;

			t = yypvt[-1].token;
			if (t){
				while (t->next) t = t->next;
				t->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 118:
# line 458 "kgram.y"
{
			yypvt[-1].token->desc.decl->decl_flags |= yypvt[-0].token->desc.decl->decl_flags;
			if (yypvt[-1].token->desc.decl->tag == NULL)
				yypvt[-1].token->desc.decl->tag = yypvt[-0].token->desc.decl->tag;
			if (yypvt[-1].token->desc.decl->vars == NULL)
				yypvt[-1].token->desc.decl->vars = yypvt[-0].token->desc.decl->vars;
			if (yypvt[-1].token->desc.decl->members == NULL)
				yypvt[-1].token->desc.decl->members = yypvt[-0].token->desc.decl->members;
			if (yypvt[-1].token->desc.decl->member_ste == NULL)
				yypvt[-1].token->desc.decl->member_ste = yypvt[-0].token->desc.decl->member_ste;
		} break;
case 119:
# line 470 "kgram.y"
{
			yypvt[-1].token->desc.decl->decl_flags |= yypvt[-0].token->desc.decl->decl_flags;
			if (yypvt[-1].token->desc.decl->tag == NULL)
				yypvt[-1].token->desc.decl->tag = yypvt[-0].token->desc.decl->tag;
			if (yypvt[-1].token->desc.decl->vars == NULL)
				yypvt[-1].token->desc.decl->vars = yypvt[-0].token->desc.decl->vars;
			if (yypvt[-1].token->desc.decl->members == NULL)
				yypvt[-1].token->desc.decl->members = yypvt[-0].token->desc.decl->members;
			if (yypvt[-1].token->desc.decl->member_ste == NULL)
				yypvt[-1].token->desc.decl->member_ste = yypvt[-0].token->desc.decl->member_ste;
		} break;
case 120:
# line 487 "kgram.y"
{
			yypvt[-2].token->desc.decl->vars = yypvt[-1].token;
		} break;
case 121:
# line 491 "kgram.y"
{
			yypvt[-1].token->desc.decl->vars = NULL;
		} break;
case 123:
# line 499 "kgram.y"
{
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of declarators                           *
*                                                        *
**********************************************************
*/

			if (yypvt[-2].token){
				t = yypvt[-2].token;
				while (t->next) t = t->next;
				t->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 125:
# line 521 "kgram.y"
{yyval.token = NULL;} break;
case 127:
# line 527 "kgram.y"
{ make_decl (yypvt[-3].token, FLAG_TYPE_ENUM);yypvt[-3].token->desc.decl->members = yypvt[-1].token;} break;
case 128:
# line 529 "kgram.y"
{ make_decl (yypvt[-4].token,FLAG_TYPE_ENUM);yypvt[-4].token->desc.decl->members = yypvt[-1].token;} break;
case 129:
# line 531 "kgram.y"
{ make_decl (yypvt[-1].token,FLAG_TYPE_ENUM); yypvt[-1].token->desc.decl->members = NULL;} break;
case 131:
# line 537 "kgram.y"
{
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of enumerators                           *
*                                                        *
**********************************************************
*/

			if (yypvt[-2].token){
				t = yypvt[-2].token;
				while (t->next) t = t->next;
				t->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 136:
# line 565 "kgram.y"
{	int i;
			for (i = 0; i < (int)yypvt[-1].token; i++)
				modify_type (yypvt[-0].token,IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
			yyval.token = yypvt[-0].token;
		} break;
case 137:
# line 573 "kgram.y"
{ /*printf ("PARM(%d)\n",lineno);*/in_parms = 1;} break;
case 139:
# line 577 "kgram.y"
{yyval.token = yypvt[-1].token;} break;
case 140:
# line 579 "kgram.y"
{modify_type (yypvt[-2].token,IS_ARRAY_OF,NO_STYLE,(token_ptr)NULL);} break;
case 141:
# line 581 "kgram.y"
{modify_type (yypvt[-3].token,IS_ARRAY_OF,NO_STYLE,(token_ptr)NULL);} break;
case 142:
# line 583 "kgram.y"
{modify_type (yypvt[-3].token,IS_FUNC_RET,OLD_STYLE_VOID,(token_ptr)NULL);
			in_parms = 0;} break;
case 143:
# line 586 "kgram.y"
{modify_type (yypvt[-4].token,IS_FUNC_RET,NEW_STYLE,yypvt[-1].token); in_parms = 0;} break;
case 144:
# line 588 "kgram.y"
{modify_type (yypvt[-4].token,IS_FUNC_RET,OLD_STYLE,yypvt[-1].token); in_parms = 0;} break;
case 145:
# line 593 "kgram.y"
{yyval.token = (token_ptr)1;} break;
case 146:
# line 595 "kgram.y"
{yyval.token = (token_ptr)1;} break;
case 147:
# line 597 "kgram.y"
{yyval.token = (token_ptr)((int)yypvt[-0].token + 1);} break;
case 148:
# line 599 "kgram.y"
{yyval.token = (token_ptr)((int)yypvt[-0].token + 1);} break;
case 149:
# line 603 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_CONST);} break;
case 150:
# line 604 "kgram.y"
{make_decl (yypvt[-0].token, FLAG_TYPE_VOLATILE);} break;
case 155:
# line 639 "kgram.y"
{
			token_ptr	t;

			if (yypvt[-2].token){
				t = yypvt[-2].token;
				while (t->next) t = t->next;
				t->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 159:
# line 659 "kgram.y"
{
			token_ptr	t;

			if (yypvt[-2].token){
				t = yypvt[-2].token;
				while (t->next) t = t->next;
				t->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 160:
# line 674 "kgram.y"
{
	/* : declaration_specifiers declarator */
			yypvt[-1].token->desc.decl->vars = yypvt[-0].token;
		} break;
case 164:
# line 697 "kgram.y"
{
			yypvt[-1].token->desc.type_desc = (type_ptr) yypvt[-0].token;
		} break;
case 165:
# line 704 "kgram.y"
{	int i;
			type_ptr	old = NULL,new;

			for (i = 0; i < (int)yypvt[-0].token; i++){
				new = make_abstract_type (
					IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
				if (!old) old = new;
				else merge_abstract_type (old,new);
			}
			yyval.token = (token_ptr)old;
		} break;
case 167:
# line 717 "kgram.y"
{	int i;
			type_ptr	old = NULL,new;

			if (yypvt[-0].token) old = (type_ptr)yypvt[-0].token;

			for (i = 0; i < (int)yypvt[-1].token; i++){
				new = make_abstract_type (
					IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
				if (!old) old = new;
				else merge_abstract_type (old,new);
			}
			yyval.token = (token_ptr)old;
		} break;
case 168:
# line 734 "kgram.y"
{yyval.token = yypvt[-1].token;} break;
case 169:
# line 736 "kgram.y"
{yyval.token = (token_ptr) make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);} break;
case 170:
# line 738 "kgram.y"
{yyval.token = (token_ptr) make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);} break;
case 171:
# line 740 "kgram.y"
{type_ptr	new;
			new = make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);
			if (yypvt[-2].token){
				merge_abstract_type (yypvt[-2].token,new);
			}
			else yyval.token = (token_ptr) new;
		} break;
case 172:
# line 748 "kgram.y"
{type_ptr	new;
			new = make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);
			if (yypvt[-3].token){
				merge_abstract_type (yypvt[-3].token,new);
			}
			else yyval.token = (token_ptr) new;
		} break;
case 173:
# line 756 "kgram.y"
{yyval.token = NULL;} break;
case 174:
# line 758 "kgram.y"
{yyval.token = NULL;} break;
case 177:
# line 765 "kgram.y"
{ yyval.token = NULL;} break;
case 178:
# line 767 "kgram.y"
{yyval.token = NULL;} break;
case 179:
# line 769 "kgram.y"
{yyval.token = NULL;} break;
case 188:
# line 788 "kgram.y"
{yyval.stmt = stmt_label(yypvt[-2].token,yypvt[-0].stmt);} break;
case 189:
# line 790 "kgram.y"
{yyval.stmt = case_stmt (yypvt[-3].token,yypvt[-1].token,yypvt[-0].stmt,False);} break;
case 190:
# line 792 "kgram.y"
{yyval.stmt = case_stmt (yypvt[-2].token,yypvt[-1].token,yypvt[-0].stmt,True);} break;
case 191:
# line 797 "kgram.y"
{
			end_local_decl();
		} break;
case 192:
# line 804 "kgram.y"
{
			open_scope();
			start_local_decl();
		} break;
case 193:
# line 812 "kgram.y"
{yyval.stmt = brace_stmt (yypvt[-1].token,(token_ptr)NULL,yypvt[-0].token);} break;
case 194:
# line 814 "kgram.y"
{yyval.stmt = brace_stmt (yypvt[-2].token,yypvt[-1].stmt,yypvt[-0].token);} break;
case 195:
# line 816 "kgram.y"
{
				close_scope();
				yyval.stmt = brace_stmt (yypvt[-3].token,(token_ptr)NULL,yypvt[-0].token);
			} break;
case 196:
# line 821 "kgram.y"
{
				close_scope();
				yyval.stmt = brace_stmt (yypvt[-4].token,yypvt[-1].stmt,yypvt[-0].token);
			} break;
case 198:
# line 830 "kgram.y"
{
			if (yypvt[-1].token){
				yypvt[-1].token->next = yypvt[-0].token;
			}
			else yyval.token = yypvt[-0].token;
		} break;
case 200:
# line 841 "kgram.y"
{ yyval.stmt = join_stmt (yypvt[-1].stmt,yypvt[-0].stmt);} break;
case 201:
# line 846 "kgram.y"
{
			int		node;

			node = get_stmt_no();
			source_map (node,yypvt[-0].token,yypvt[-0].token);
			yyval.stmt = make_stmt (node,node,node,node);
		} break;
case 202:
# line 854 "kgram.y"
{int	from_node,to_node;
		int		node_id;

		node_id = xpr_gen (yypvt[-1].tree, &from_node,&to_node);
		yyval.stmt = make_stmt (node_id,stmtno,from_node,to_node);
		source_map (node_id,leftmost(yypvt[-1].tree),yypvt[-0].token);
		} break;
case 203:
# line 865 "kgram.y"
{yyval.stmt = if_stmt (yypvt[-4].token,yypvt[-2].tree,yypvt[-1].token,yypvt[-0].stmt,(token_ptr)NULL,(stmt_ptr)NULL);} break;
case 204:
# line 867 "kgram.y"
{yyval.stmt = if_stmt (yypvt[-6].token,yypvt[-4].tree,yypvt[-3].token,yypvt[-2].stmt,yypvt[-1].token,yypvt[-0].stmt);} break;
case 205:
# line 869 "kgram.y"
{yyval.stmt = switch_stmt (yypvt[-4].token,yypvt[-2].tree,yypvt[-1].token,yypvt[-0].stmt);} break;
case 206:
# line 872 "kgram.y"
{yyval.tree = NULL;} break;
case 208:
# line 877 "kgram.y"
{yyval.stmt = while_stmt (yypvt[-4].token,yypvt[-2].tree,yypvt[-1].token,yypvt[-0].stmt);} break;
case 209:
# line 879 "kgram.y"
{yyval.stmt = do_stmt(yypvt[-6].token,yypvt[-5].stmt,yypvt[-4].token,yypvt[-2].tree,yypvt[-0].token);} break;
case 210:
# line 881 "kgram.y"
{yyval.stmt = for_stmt (yypvt[-8].token,yypvt[-7].token,yypvt[-6].tree,yypvt[-5].token,yypvt[-4].tree,yypvt[-3].token,yypvt[-2].tree,yypvt[-1].token,yypvt[-0].stmt);} break;
case 211:
# line 896 "kgram.y"
{
			yyval.stmt = goto_stmt (yypvt[-2].token,yypvt[-1].token,yypvt[-0].token);
		} break;
case 212:
# line 900 "kgram.y"
{yyval.stmt = continue_stmt (yypvt[-1].token,yypvt[-0].token);} break;
case 213:
# line 902 "kgram.y"
{yyval.stmt = break_stmt (yypvt[-1].token,yypvt[-0].token);} break;
case 214:
# line 904 "kgram.y"
{yyval.stmt = return_stmt (yypvt[-1].token,(tree_ptr)NULL,yypvt[-0].token);} break;
case 215:
# line 906 "kgram.y"
{yyval.stmt = return_stmt (yypvt[-2].token,yypvt[-1].tree,yypvt[-0].token);} break;
case 216:
# line 909 "kgram.y"
{yyval.token = NULL;} break;
case 217:
# line 911 "kgram.y"
{
			close_scope();
		} break;
case 223:
# line 931 "kgram.y"
{
			token_ptr	tk,parm;
			type_ptr	t;
			
			yyval.stmt = fun_stmt (yypvt[-1].token);
			open_scope();
			start_local_decls();
			tk = yypvt[-1].token;
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
		} break;
case 224:
# line 961 "kgram.y"
{ } break;
case 226:
# line 962 "kgram.y"
{ } break;
case 227:
# line 963 "kgram.y"
{
				int			end_stmt;

				end_stmt = yypvt[-0].stmt->exit;
				do_returns (end_stmt);
				connect_nodes (yypvt[-1].stmt->exit,yypvt[-0].stmt->entry);
				gen_require (yypvt[-0].stmt->entry,yypvt[-1].stmt->exit,yypvt[-1].stmt->exit);
				if (current_proc){
					current_proc->exit_node = yypvt[-0].stmt->exit;
					current_proc->n_locals = current_local();
				}

				close_scope();
				fprintf (outfile,"%d(%d",LIF_PROC_END,yypvt[-0].stmt->exit);
				if (current_proc->has_return_xpr)
					fprintf (outfile,",R");
				fprintf (outfile,")");
				if (z_opt)fprintf (outfile,"    end function %s",
					yypvt[-3].token->text);
				fprintf (outfile,"\n");
			} break;
case 228:
# line 985 "kgram.y"
{
				decl (yypvt[-1].token,NULL,False);
			} break;
case 229:
# line 989 "kgram.y"
{
				int	is_static = 0;

				do_returns (yypvt[-0].stmt->exit);
				connect_nodes (yypvt[-1].stmt->exit,yypvt[-0].stmt->entry);
				gen_require (yypvt[-0].stmt->entry,yypvt[-1].stmt->exit,yypvt[-1].stmt->exit);
				close_scope();
				fprintf (outfile,"%d(%d",LIF_PROC_END,yypvt[-0].stmt->exit);
				if(yypvt[-4].token->desc.decl->decl_flags & FLAG_SC_STATIC){
					fprintf (outfile,",S");
					is_static = 1;
				}
				if (current_proc->has_return_xpr)
					fprintf (outfile,",R");
				fprintf (outfile,")");
				if (z_opt)fprintf (outfile,"    end %sfunction %s",
					is_static?"static ":"",
					yypvt[-3].token->text);
				fprintf (outfile,"\n");
				if (current_proc){
					if (is_static) current_proc->static_or_extern = 'S';
					current_proc->type_decl = yypvt[-4].token;
					current_proc->exit_node = yypvt[-0].stmt->exit;
					current_proc->n_locals = current_local();
				}
			} break;
case 230:
# line 1019 "kgram.y"
{yyval.stmt = yypvt[-0].stmt;} break;
case 231:
# line 1021 "kgram.y"
{
			token_ptr	fun,parm;
			type_ptr	t;
			int			pn,level;
			var_ste_ptr	v;

			yyval.stmt = NULL;
			fun = yypvt[-3].token;
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
		} break;
case 232:
# line 1059 "kgram.y"
{yyval.stmt = yypvt[-0].stmt;} break;
	}
	goto yystack;		/* reset registers in driver code */
}

