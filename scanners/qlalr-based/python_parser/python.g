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

-- literals have LT_ prefix
%token LT_NONE
%token LT_BOOLEAN
%token LT_INTEGER
%token LT_FLOAT
%token LT_COMPLEX

-- keywords have KW_ prefix
%token KW_IMPORT
%token KW_FROM
%token KW_AS
%token KW_IF
%token KW_BREAK
%token KW_CONTINUE
%token KW_PASS
%token KW_CLASS

-- delimiters have DL_ prefix
%token DL_NEWLINE
%token DL_COLON
%token DL_INDENT
%token DL_DEDENT
%token DL_RIGHT_PAREN
%token DL_LEFT_PAREN
%token DL_SEMICOLON
%token DL_COMMA

%token ADD_ASSIGN
%token AMPERSAND
%token AND_ASSIGN
%token AND_OP
%token BANG
%token CARET
%token DASH
%token DEC_OP
%token DIV_ASSIGN
%token DO
%token DOT
%token ELSE
%token EQUAL
%token EQ_OP
%token FOR
%token GE_OP
%token IDENTIFIER
%token IN
%token INC_OP
%token LEFT_ANGLE
%token LEFT_ASSIGN
%token LEFT_BRACE
%token LEFT_BRACKET
%token LEFT_OP
%token LE_OP
%token MAT2
%token MAT3
%token MAT4
%token MOD_ASSIGN
%token MUL_ASSIGN
%token NE_OP
%token OR_ASSIGN
%token OR_OP
%token OUT
%token PERCENT
%token PLUS
%token QUESTION
%token RETURN
%token RIGHT_ANGLE
%token RIGHT_ASSIGN
%token RIGHT_BRACE
%token RIGHT_BRACKET
%token RIGHT_OP
%token SLASH
%token STAR
%token SUB_ASSIGN
%token TILDE
%token VERTICAL_BAR
%token WHILE
%token XOR_ASSIGN
%token XOR_OP
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

variable_identifier ::= IDENTIFIER ;

primary_expression ::= variable_identifier ;
primary_expression ::= LT_NONE ;
primary_expression ::= LT_BOOLEAN ;
primary_expression ::= LT_INTEGER ;
primary_expression ::= LT_FLOAT ;
primary_expression ::= LT_COMPLEX ;
--primary_expression ::= LEFT_PAREN expression RIGHT_PAREN ;


postfix_expression ::= primary_expression ;
--postfix_expression ::= function_call ;
postfix_expression ::= postfix_expression DOT IDENTIFIER ;

-- pythonized
qualified_name ::= qualified_name DOT IDENTIFIER ;
qualified_name ::= IDENTIFIER ;

import_statement ::= KW_IMPORT qualified_name ;
import_statement ::= KW_FROM qualified_name KW_IMPORT qualified_name ;

small_statement ::= import_statement ;
small_statement ::= postfix_expression ;
small_statement ::= KW_PASS ;

small_statements_list ::= small_statements_list DL_SEMICOLON small_statement ;
small_statements_list ::= small_statement ;
simple_statement ::= small_statements_list DL_SEMICOLON DL_NEWLINE ;
simple_statement ::= small_statements_list DL_NEWLINE ;

-- TODO: add class testlist (need for base classes support)
class_definition ::= KW_CLASS IDENTIFIER DL_COLON suite;
class_definition ::= KW_CLASS IDENTIFIER DL_LEFT_PAREN base_classes_list DL_RIGHT_PAREN DL_COLON suite;

base_classes_list ::= qualified_name ;
base_classes_list ::= qualified_names_list qualified_name ;
qualified_names_list ::= qualified_names_list qualified_name DL_COMMA ;
qualified_names_list ::= qualified_name DL_COMMA ;

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
