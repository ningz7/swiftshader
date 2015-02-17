
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INVARIANT = 258,
     HIGH_PRECISION = 259,
     MEDIUM_PRECISION = 260,
     LOW_PRECISION = 261,
     PRECISION = 262,
     ATTRIBUTE = 263,
     CONST_QUAL = 264,
     BOOL_TYPE = 265,
     FLOAT_TYPE = 266,
     INT_TYPE = 267,
     UINT_TYPE = 268,
     BREAK = 269,
     CONTINUE = 270,
     DO = 271,
     ELSE = 272,
     FOR = 273,
     IF = 274,
     DISCARD = 275,
     RETURN = 276,
     SWITCH = 277,
     CASE = 278,
     DEFAULT = 279,
     BVEC2 = 280,
     BVEC3 = 281,
     BVEC4 = 282,
     IVEC2 = 283,
     IVEC3 = 284,
     IVEC4 = 285,
     VEC2 = 286,
     VEC3 = 287,
     VEC4 = 288,
     MATRIX2 = 289,
     MATRIX3 = 290,
     MATRIX4 = 291,
     IN_QUAL = 292,
     OUT_QUAL = 293,
     INOUT_QUAL = 294,
     UNIFORM = 295,
     VARYING = 296,
     CENTROID = 297,
     FLAT = 298,
     SMOOTH = 299,
     STRUCT = 300,
     VOID_TYPE = 301,
     WHILE = 302,
     SAMPLER2D = 303,
     SAMPLERCUBE = 304,
     SAMPLER_EXTERNAL_OES = 305,
     SAMPLER2DRECT = 306,
     SAMPLER3D = 307,
     SAMPLER3DRECT = 308,
     SAMPLER2DSHADOW = 309,
     IDENTIFIER = 310,
     TYPE_NAME = 311,
     FLOATCONSTANT = 312,
     INTCONSTANT = 313,
     BOOLCONSTANT = 314,
     FIELD_SELECTION = 315,
     LEFT_OP = 316,
     RIGHT_OP = 317,
     INC_OP = 318,
     DEC_OP = 319,
     LE_OP = 320,
     GE_OP = 321,
     EQ_OP = 322,
     NE_OP = 323,
     AND_OP = 324,
     OR_OP = 325,
     XOR_OP = 326,
     MUL_ASSIGN = 327,
     DIV_ASSIGN = 328,
     ADD_ASSIGN = 329,
     MOD_ASSIGN = 330,
     LEFT_ASSIGN = 331,
     RIGHT_ASSIGN = 332,
     AND_ASSIGN = 333,
     XOR_ASSIGN = 334,
     OR_ASSIGN = 335,
     SUB_ASSIGN = 336,
     LEFT_PAREN = 337,
     RIGHT_PAREN = 338,
     LEFT_BRACKET = 339,
     RIGHT_BRACKET = 340,
     LEFT_BRACE = 341,
     RIGHT_BRACE = 342,
     DOT = 343,
     COMMA = 344,
     COLON = 345,
     EQUAL = 346,
     SEMICOLON = 347,
     BANG = 348,
     DASH = 349,
     TILDE = 350,
     PLUS = 351,
     STAR = 352,
     SLASH = 353,
     PERCENT = 354,
     LEFT_ANGLE = 355,
     RIGHT_ANGLE = 356,
     VERTICAL_BAR = 357,
     CARET = 358,
     AMPERSAND = 359,
     QUESTION = 360
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    struct {
        TSourceLoc line;
        union {
            TString *string;
            float f;
            int i;
            bool b;
        };
        TSymbol* symbol;
    } lex;
    struct {
        TSourceLoc line;
        TOperator op;
        union {
            TIntermNode* intermNode;
            TIntermNodePair nodePair;
            TIntermTyped* intermTypedNode;
            TIntermAggregate* intermAggregate;
        };
        union {
            TPublicType type;
            TPrecision precision;
            TQualifier qualifier;
            TFunction* function;
            TParameter param;
            TTypeLine typeLine;
            TTypeList* typeList;
        };
    } interm;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




