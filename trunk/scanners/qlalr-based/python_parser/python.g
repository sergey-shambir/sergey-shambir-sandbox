----------------------------------------------------------------------------
--
-- Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
-- Contact: http://www.qt-project.org/
--
-- This file is part of the QtCore module of the Qt Toolkit.
--
-- $QT_BEGIN_LICENSE:LGPL$
-- GNU Lesser General Public License Usage
-- This file may be used under the terms of the GNU Lesser General Public
-- License version 2.1 as published by the Free Software Foundation and
-- appearing in the file LICENSE.LGPL included in the packaging of this
-- file. Please review the following information to ensure the GNU Lesser
-- General Public License version 2.1 requirements will be met:
-- http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
--
-- In addition, as a special exception, Nokia gives you certain additional
-- rights. These rights are described in the Nokia Qt LGPL Exception
-- version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
--
-- GNU General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU General
-- Public License version 3.0 as published by the Free Software Foundation
-- and appearing in the file LICENSE.GPL included in the packaging of this
-- file. Please review the following information to ensure the GNU General
-- Public License version 3.0 requirements will be met:
-- http://www.gnu.org/copyleft/gpl.html.
--
-- Other Usage
-- Alternatively, this file may be used in accordance with the terms and
-- conditions contained in a signed written agreement between you and Nokia.
--
--
--
--
--
--
-- $QT_END_LICENSE$
--
----------------------------------------------------------------------------

%parser PyParserTable
%merged_output python.cpp

%token IDENTIFIER

-- literals have LT_ prefix
%token LT_NONE
%token LT_BOOLEAN
%token LT_INTEGER
%token LT_FLOAT
%token LT_COMPLEX
%token LT_STRING

-- keywords have KW_ prefix
%token KW_IMPORT
%token KW_FROM
%token KW_AS

%token KW_IF
%token KW_BREAK
%token KW_CONTINUE
%token KW_PASS
%token KW_PRINT

%token KW_CLASS

%token KW_OR

-- delimiters have DL_ prefix
%token DL_NEWLINE
%token DL_COLON
%token DL_INDENT
%token DL_DEDENT
%token DL_RIGHT_PAREN
%token DL_LEFT_PAREN
%token DL_SEMICOLON
%token DL_COMMA

%token OP_PLUS
%token OP_MINUS
%token OP_DOT
%token OP_ASSIGN

%token ERROR
%start translation_unit


/:

#include <QtCore>

class PyParser: protected $table
{
public:
    union Value {
      int i;
      unsigned u;
      unsigned long ul;
      unsigned long long ull;
      long l;
      double d;
      float f;
      const QString *s;
      // ### more...
    };

public:
    PyParser();
    ~PyParser();

    bool parse();

protected:
    inline void reallocateStack();

    inline Value &sym(int index)
    { return sym_stack [tos + index - 1]; }

    int nextToken();

    bool isTypename(const QString *s) const
    {
      return types.contains(s);
    }

    inline const QString *intern(const QString &s)
    { return &*string_repository.insert(s); }

protected:
    int tos;
    int stack_size;
    Value *sym_stack;
    int *state_stack;
    Value yylval;
    QSet<QString> string_repository;
    QSet<const QString*> types;

    struct /*Context*/ {
      int line;
      /* Python semantics depends on indentation level, so we should keep it */
      int indentCurrent;
      QStack<int> indents;
      const QString *function_name;
      QString fileName;

      void init()
      {
        line = 1;
        indentCurrent = 0;
        function_name = 0;
        fileName.clear();
        indents.clear();
      }

      bool checkIndent()
      {
        if ((indentCurrent && indents.isEmpty())
            || (indentCurrent > indents.top())) {
          indents.push(indentCurrent);
          return true;
        }
        return false;
      }

      bool checkDedent()
      {
        if (indents.isEmpty())
          return false;
        if (indentCurrent < indents.top()) {
          indents.pop();
          return true;
        }
        return false;
      }
    } context;
};

inline void PyParser::reallocateStack()
{
    if (! stack_size)
        stack_size = 128;
    else
        stack_size <<= 1;

    sym_stack = reinterpret_cast<Value*> (realloc(sym_stack, stack_size * sizeof(Value)));
    state_stack = reinterpret_cast<int*> (realloc(state_stack, stack_size * sizeof(int)));
}

:/


/.

PyParser::PyParser():
    tos(0),
    stack_size(0),
    sym_stack(0),
    state_stack(0)
{
}

PyParser::~PyParser()
{
    if (stack_size) {
        free(sym_stack);
        free(state_stack);
    }
}

bool PyParser::parse()
{
  const int INITIAL_STATE = 0;

  int yytoken = -1;

  reallocateStack();

  context.init();
  tos = 0;
  state_stack[++tos] = INITIAL_STATE;

  while (true)
    {
      if (yytoken == -1 && - TERMINAL_COUNT != action_index [state_stack [tos]])
        yytoken = nextToken();

      int act = t_action (state_stack [tos], yytoken);

      if (act == ACCEPT_STATE) {
        return true;
      }

      else if (act > 0)
        {
          if (++tos == stack_size)
            reallocateStack();

          sym_stack [tos] = yylval;
          state_stack [tos] = act;
          yytoken = -1;
        }

      else if (act < 0)
        {
          int r = - act - 1;

          int ridx = rule_index [r];
          printf ("*** reduce using rule %d %s ::=", r + 1, spell[rule_info [ridx]]);
          ++ridx;
          for (int i = ridx; i < ridx + rhs [r]; ++i)
            {
              int symbol = rule_info [i];
              if (const char *name = spell [symbol])
                printf (" %s", name);
              else
                printf (" #%d", symbol);
            }
          printf ("\n");

          tos -= rhs [r];
          act = state_stack [tos++];

          switch (r) {
./

translation_unit ::= statements_list ;

atom ::= LT_NONE ;
atom ::= LT_BOOLEAN ;
atom ::= LT_INTEGER ;
atom ::= LT_FLOAT ;
atom ::= LT_COMPLEX ;
atom ::= LT_STRING ;
atom ::= IDENTIFIER ;
atom ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
-- TODO: change to // testlist_comp: test ( comp_for | (',' test)* [','] )
atom ::= DL_LEFT_PAREN testlist DL_RIGHT_PAREN ;

trailer ::= OP_DOT IDENTIFIER ;
trailer ::= DL_LEFT_PAREN arguments_list DL_RIGHT_PAREN ;
-- TODO: "func(args)" expressions
-- TODO: "array[index]" expressions

trailers_list ::= trailer ;
trailers_list ::= trailers_list trailer ;

primary_expression ::= atom ;
primary_expression ::= atom trailers_list ;
--primary_expression ::= LEFT_PAREN expression RIGHT_PAREN ;
--postfix_expression ::= primary_expression ;
--postfix_expression ::= function_call ;
--postfix_expression ::= postfix_expression DOT IDENTIFIER ;

-- Expressions begin

-- TODO: implement normal test_of_and

term ::= primary_expression ;

test_of_and ::= term ;
test_of_and ::= test_of_and OP_PLUS term;
test_of_and ::= test_of_and OP_MINUS term;

test_of_or ::= test_of_and ;
test_of_or ::= test_of_and KW_OR test_of_and ;

test ::= test_of_or ;
--test ::= KW_IF or_test KW_ELSE test
--test ::= lambda_definition ;

testlist_head ::= test ;
testlist_head ::= testlist_head DL_COMMA test ;
testlist ::= testlist_head ;
testlist ::= testlist_head DL_COMMA ;

argument ::= test ;
-- TODO: extend this definition // argument: test [comp_for] | test '=' test

arguments_list_head ::= argument ;
arguments_list_head ::= arguments_list_head DL_COMMA argument ;
arguments_list ::= arguments_list_head ;
arguments_list ::= arguments_list_head DL_COMMA ;

expression_statement ::= testlist ;
expression_statement ::= expression_statement OP_ASSIGN testlist ;

-- Expressions end

qualified_name ::= qualified_name OP_DOT IDENTIFIER ;
qualified_name ::= IDENTIFIER ;

qualified_names_list_head ::= qualified_names_list_head qualified_name DL_COMMA ;
qualified_names_list_head ::= qualified_name DL_COMMA ;
qualified_names_list ::= qualified_name ;
qualified_names_list ::= qualified_names_list_head qualified_name ;

import_statement ::= KW_IMPORT qualified_name ;
import_statement ::= KW_FROM qualified_name KW_IMPORT qualified_name ;

--print_stmt: 'print' ( [ test (',' test)* [','] ] | '>>' test [ (',' test)+ [','] ] )
print_statement ::= KW_PRINT ;
print_statement ::= KW_PRINT testlist ;

small_statement ::= import_statement ;
small_statement ::= expression_statement ;
small_statement ::= print_statement ;
small_statement ::= KW_PASS ;

small_statements_list ::= small_statements_list DL_SEMICOLON small_statement ;
small_statements_list ::= small_statement ;
simple_statement ::= small_statements_list DL_SEMICOLON DL_NEWLINE ;
simple_statement ::= small_statements_list DL_NEWLINE ;

-- TODO: add class testlist (need for base classes support)
class_definition ::= KW_CLASS IDENTIFIER DL_COLON suite;
class_definition ::= KW_CLASS IDENTIFIER DL_LEFT_PAREN qualified_names_list DL_RIGHT_PAREN DL_COLON suite;

suite ::= simple_statement ;
-- TODO: add INDENT/DEDENT tokens to this suite definition
suite ::= DL_NEWLINE DL_INDENT statements_list DL_DEDENT;

--compound_statement ::= if_statement ;
--compound_statement ::= while_statement ;
--compound_statement ::= for_statement ;
compound_statement ::= class_definition ;

common_statement ::= simple_statement ;
common_statement ::= compound_statement ;
common_statement ::= DL_NEWLINE ;

statements_list ::= common_statement ;
statements_list ::= statements_list common_statement ;

/.
          } // switch

          state_stack [tos] = nt_action (act, lhs [r] - TERMINAL_COUNT);
        }

      else
        {
          // ### ERROR RECOVERY HERE
          break;
        }
    }

    fprintf (stderr, "%s:%d: Syntax Error\n", qPrintable(context.fileName), context.line);

    return false;
}

#include "py-lex.incl"


/////////////////////////////
// entry point
/////////////////////////////
int main()
{
#if 0 // dump the Python grammar
    for (int r = 0; r < PyParserTable::RULE_COUNT; ++r)
      {
        int ridx = PyParserTable::rule_index [r];
        int rhs = PyParserTable::rhs [r];
        printf ("%3d) %s ::=", r + 1, PyParserTable::spell[PyParserTable::rule_info [ridx]]);
        ++ridx;
        for (int i = ridx; i < ridx + rhs; ++i)
          {
            int symbol = PyParserTable::rule_info [i];
            if (const char *name = PyParserTable::spell [symbol])
              printf (" %s", name);
            else
              printf (" #%d", symbol);
          }
        printf ("\n");
      }
#endif

    PyParser parser;

    if (parser.parse())
        qDebug() << "OK";
    else
        qDebug() << "KO";
}

./
