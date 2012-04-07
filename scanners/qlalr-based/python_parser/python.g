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

%parser PyGrammar
%decl pyparser_p.h
%impl pyparser.cpp

%token IDENTIFIER
%token ERROR

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

%token KW_WITH
%token KW_WHILE
%token KW_FOR
%token KW_IF
%token KW_ELIF
%token KW_ELSE
%token KW_BREAK
%token KW_CONTINUE
%token KW_RETURN
%token KW_YIELD
%token KW_RAISE

%token KW_TRY
%token KW_EXCEPT
%token KW_FINALLY
%token KW_PASS
%token KW_PRINT

%token KW_CLASS
%token KW_DEF
%token KW_LAMBDA

%token KW_AND
%token KW_OR
%token KW_XOR
%token KW_IS
%token KW_IN
%token KW_NOT

-- delimiters have DL_ prefix
-- string inside "" is token comment
%token DL_NEWLINE
%token DL_INDENT
%token DL_DEDENT
%token DL_LEFT_PAREN    "("
%token DL_RIGHT_PAREN   ")"
%token DL_LEFT_BRACKET  "["
%token DL_RIGHT_BRACKET "]"
%token DL_LEFT_BRACE    "{"
%token DL_RIGHT_BRACE   "}"
%token DL_SEMICOLON     ";"
%token DL_COMMA         ","
%token DL_COLON         ":"
%token DL_BACKQUOTE     "`"

%token OP_AUG_PLUS      "+="
%token OP_AUG_MINUS     "-="
%token OP_AUG_POWER     "**="
%token OP_AUG_MUL       "*="
%token OP_AUG_FLOORDIV  "//="
%token OP_AUG_DIV       "/="
%token OP_AUG_MOD       "%="
%token OP_AUG_AND       "&="
%token OP_AUG_OR        "|="
%token OP_AUG_XOR       "^="
%token OP_AUG_LSHIFT    "<<="
%token OP_AUG_RSHIFT    ">>="
%token OP_AND           "&"
%token OP_OR            "|"
%token OP_XOR           "^"
%token OP_LSHIFT        "<<"
%token OP_RSHIFT        ">>"

%token OP_PLUS "+"
%token OP_MINUS "-"
%token OP_DOT "."
%token OP_EQUAL "=="
%token OP_NOT_EQUAL "!= or <>"
%token OP_LT "<"
%token OP_LE "<="
%token OP_GT ">"
%token OP_GE ">="
%token OP_ASSIGN "="
%token OP_POWER "**"
%token OP_MOD "%"
%token OP_MUL "*"
%token OP_AT "@"
%token OP_COMPLEMENT "~"
%token OP_FLOORDIV "//"
%token OP_DIV "/"

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
    {
        return sym_stack [tos + index - 1];
    }

    int nextToken();

    bool isTypename(const QString *s) const
    {
        return types.contains(s);
    }

    inline const QString *intern(const QString &s)
    {
        return &*string_repository.insert(s);
    }

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
        int implicitJoiningLevel;
        QStack<int> indents;
        const QString *function_name;
        QString fileName;

        void init()
        {
            implicitJoiningLevel = 0;
            line = 1;
            indentCurrent = 0;
            function_name = 0;
            fileName.clear();
            indents.clear();
        }

        bool checkIndent()
        {
            if ((indentCurrent && indents.isEmpty()) || (indentCurrent > indents.top())) {
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

#include "pygrammar_p.h"
#include "pyparser_p.h"

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
            // printf ("*** reduce using rule %d %s ::=", r + 1, spell[rule_info [ridx]]);
            ++ridx;
            for (int i = ridx; i < ridx + rhs [r]; ++i)
            {
               // int symbol = rule_info [i];
                //// if (const char *name = spell [symbol])
               //     printf (" %s", name);
               //  else
                //    printf (" #%d", symbol);
            }
            //printf ("\n");

            tos -= rhs [r];
            act = state_stack [tos++];

            switch (r) {
./

translation_unit ::= statements_list ;

stringliteral ::= LT_STRING ;
stringliteral ::= stringliteral LT_STRING ;

atom ::= LT_NONE ;
atom ::= LT_BOOLEAN ;
atom ::= LT_INTEGER ;
atom ::= LT_FLOAT ;
atom ::= LT_COMPLEX ;
atom ::= stringliteral ;
atom ::= IDENTIFIER ;
atom ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
-- TODO: change to // testlist_comp: test ( comp_for | (',' test)* [','] )
atom ::= DL_LEFT_PAREN testlist DL_RIGHT_PAREN ;
atom ::= DL_LEFT_PAREN yield_expression DL_RIGHT_PAREN ;
atom ::= DL_LEFT_BRACKET DL_RIGHT_BRACKET ;
atom ::= DL_LEFT_BRACKET list_initializer DL_RIGHT_BRACKET ;
atom ::= DL_LEFT_BRACE DL_RIGHT_BRACE ;
atom ::= DL_LEFT_BRACE dict_or_set_initializer DL_RIGHT_BRACE ;
atom ::= DL_BACKQUOTE testlist_head DL_BACKQUOTE ;

trailer ::= OP_DOT IDENTIFIER ;
trailer ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
trailer ::= DL_LEFT_PAREN arguments_list DL_RIGHT_PAREN ;
trailer ::= DL_LEFT_BRACKET subscript_list DL_RIGHT_BRACKET ;

subscript_head ::= DL_COLON ;
subscript_head ::= test DL_COLON ;
subscript_head ::= DL_COLON test ;
subscript_head ::= test DL_COLON test ;
subscript_tail ::= DL_COLON ;
subscript_tail ::= DL_COLON test ;
subscript ::= OP_DOT OP_DOT OP_DOT ;
subscript ::= test ;
subscript ::= subscript_head ;
subscript ::= subscript_head subscript_tail ;
--! TODO: add rule // subscript ::= [test] ':' [test] [':' [test]]
subscript_list_head ::= subscript ;
subscript_list_head ::= subscript_list_head DL_COMMA subscript ;
subscript_list ::= subscript_list_head ;
subscript_list ::= subscript_list_head DL_COMMA ;



-- Control flow begin
yield_expression ::= KW_YIELD ;
yield_expression ::= KW_YIELD testlist ;

contol_flow_statement ::= KW_BREAK ;
contol_flow_statement ::= KW_CONTINUE ;
contol_flow_statement ::= KW_RETURN ;
contol_flow_statement ::= KW_RETURN testlist ;
contol_flow_statement ::= KW_RAISE ;
contol_flow_statement ::= KW_RAISE test ;
contol_flow_statement ::= KW_RAISE test DL_COMMA test ;
contol_flow_statement ::= KW_RAISE test DL_COMMA test DL_COMMA test ;
contol_flow_statement ::= yield_expression ;
-- Control flow end



-- Term begin
trailers_list ::= trailer ;
trailers_list ::= trailers_list trailer ;

term_power ::= atom ;
term_power ::= atom trailers_list ;
term_power ::= atom OP_POWER term_factor;
term_power ::= atom trailers_list OP_POWER term_factor ;

term_factor ::= term_power ;
term_factor ::= OP_PLUS term_factor ;
term_factor ::= OP_MINUS term_factor ;
term_factor ::= OP_COMPLEMENT term_factor ;

term ::= term_factor ;
term ::= term OP_MUL term_factor ;
term ::= term OP_MOD term_factor ;
term ::= term OP_DIV term_factor ;
term ::= term OP_FLOORDIV term_factor ;
-- Term end



-- Test begin
expression_or ::= expression_xor ;
expression_or ::= expression_or OP_OR expression_xor ;
expression_xor ::= expression_and ;
expression_xor ::= expression_xor OP_XOR expression_and ;
expression_and ::= expression_shift ;
expression_and ::= expression_and OP_AND expression_shift ;
expression_shift ::= expression_arifm ;
expression_shift ::= expression_shift OP_LSHIFT expression_arifm ;
expression_shift ::= expression_shift OP_RSHIFT expression_arifm ;

expression_arifm ::= term ;
expression_arifm ::= expression_arifm OP_PLUS term ;
expression_arifm ::= expression_arifm OP_MINUS term ;

comparison ::= expression_or ;
comparison ::= comparison comparison_operator expression_or ;
comparison_operator ::= OP_EQUAL ;
comparison_operator ::= OP_NOT_EQUAL ;
comparison_operator ::= OP_LT ;
comparison_operator ::= OP_GT ;
comparison_operator ::= OP_LE ;
comparison_operator ::= OP_GE ;
comparison_operator ::= KW_IN ;
comparison_operator ::= KW_NOT KW_IN ;
comparison_operator ::= KW_IS ;
comparison_operator ::= KW_IS KW_NOT ;
-- comparison operators: '<'|'>'|'=='|'>='|'<='|'<>'|'!='|'in'|'not' 'in'|'is'|'is' 'not'

-- Safe test is same with test, but can't contain "if ... else ..."
--testlist_safe_head ::= test_safe ;
--testlist_safe_head ::= testlist_safe DL_COMMA test_safe ;
--testlist_safe ::= testlist_safe_head ;
--testlist_safe ::= testlist_safe_head DL_COMMA ;

test_of_not ::= comparison ;
test_of_not ::= KW_NOT test_of_not ;

test_of_and ::= test_of_not ;
test_of_and ::= test_of_and KW_AND test_of_not ;

test_of_or ::= test_of_and ;
test_of_or ::= test_of_or KW_OR test_of_and ;

test ::= test_of_or ;
--test ::= test_of_or KW_IF test_of_or KW_ELSE test ;
test ::= lambda_definition ;

testlist_head ::= test ;
testlist_head ::= testlist_head DL_COMMA test ;
testlist ::= testlist_head ;
testlist ::= testlist_head DL_COMMA ;
-- Test end



-- Expressions begin
argument ::= test ;
argument ::= test dict_or_set_for ;
argument ::= test OP_ASSIGN test ;

arguments_list_head ::= argument ;
arguments_list_head ::= arguments_list_head DL_COMMA argument ;
arguments_list_tail ::= OP_POWER test ;
arguments_list_tail ::= OP_MUL test ;
arguments_list_tail ::= OP_MUL test DL_COMMA OP_POWER test ;
arguments_list_tail ::= OP_MUL test DL_COMMA arguments_list_head ;
arguments_list_tail ::= OP_MUL test DL_COMMA arguments_list_head DL_COMMA OP_POWER test ;
arguments_list ::= arguments_list_head ;
arguments_list ::= arguments_list_head DL_COMMA ;
arguments_list ::= arguments_list_head DL_COMMA arguments_list_tail ;
arguments_list ::= arguments_list_tail ;

augment_operator ::= OP_AUG_PLUS ;
augment_operator ::= OP_AUG_MINUS ;
augment_operator ::= OP_AUG_MUL ;
augment_operator ::= OP_AUG_DIV ;
augment_operator ::= OP_AUG_MOD ;
augment_operator ::= OP_AUG_AND ;
augment_operator ::= OP_AUG_OR ;
augment_operator ::= OP_AUG_XOR ;
augment_operator ::= OP_AUG_LSHIFT ;
augment_operator ::= OP_AUG_RSHIFT ;
augment_operator ::= OP_AUG_POWER ;
augment_operator ::= OP_AUG_FLOORDIV ;

expression_statement ::= testlist ;
expression_statement ::= expression_statement OP_ASSIGN testlist ;
expression_statement ::= expression_statement augment_operator testlist ;

expressions_list_head ::= expression_or ;
expressions_list_head ::= expressions_list_head DL_COMMA expression_or ;
expressions_list ::= expressions_list_head ;
expressions_list ::= expressions_list_head DL_COMMA ;

-- Expressions end

-- Lambda begin
lambda_definition ::= KW_LAMBDA DL_COLON test ;
lambda_definition ::= KW_LAMBDA var_arguments_list DL_COLON test ;
-- Lambda end

-- List and dict begin
--! TODO: replace testlist with testlist_safe at list_for, replace test with test_safe at list_if
--! See also rule for "old_test" here: http://docs.python.org/reference/grammar.html
list_if ::= KW_IF test ;
list_if ::= KW_IF test list_iteration ;
list_for_impl ::= KW_FOR expressions_list KW_IN testlist;
list_iteration ::= list_for ;
list_iteration ::= list_if ;
list_for ::= list_for_impl ;
list_for ::= list_for_impl list_iteration ;
list_initializer ::= test list_for ;
list_initializer ::= testlist ;

--! TODO: replace "test" with "test_safe". See also rule for "old_test" here: http://docs.python.org/reference/grammar.html
dict_or_set_if ::= KW_IF test ;
dict_or_set_if ::= KW_IF test dict_or_set_iteration ;
dict_or_set_iteration ::= dict_or_set_for ;
dict_or_set_iteration ::= dict_or_set_if ;
dict_or_set_for ::= KW_FOR expressions_list KW_IN test_of_or ;
dict_or_set_for ::= KW_FOR expressions_list KW_IN test_of_or dict_or_set_iteration ;

testpairs_list_head ::= test DL_COLON test ;
testpairs_list_head ::= testpairs_list_head DL_COMMA test DL_COLON test ;
testpairs_list ::= testpairs_list_head ;
testpairs_list ::= testpairs_list_head DL_COMMA ;
dict_or_set_initializer ::= test DL_COLON test dict_or_set_for ;
dict_or_set_initializer ::= testpairs_list ;
dict_or_set_initializer ::= testlist ;
dict_or_set_initializer ::= test dict_or_set_for ;

-- List and dict end


-- Import begin
dotted_name ::= dotted_name OP_DOT IDENTIFIER ;
dotted_name ::= IDENTIFIER ;

dotted_names_list_head ::= dotted_names_list_head dotted_name DL_COMMA ;
dotted_names_list_head ::= dotted_name DL_COMMA ;
dotted_names_list ::= dotted_name ;
dotted_names_list ::= dotted_names_list_head dotted_name ;

dotted_as_name ::= dotted_name ;
dotted_as_name ::= dotted_name KW_AS IDENTIFIER ;
dotted_as_names_list ::= dotted_as_name ;
dotted_as_names_list ::= dotted_as_names_list DL_COMMA dotted_as_name ;

import_as_name ::= IDENTIFIER ;
import_as_name ::= IDENTIFIER KW_AS IDENTIFIER ;
import_as_names_list_head ::= import_as_name ;
import_as_names_list_head ::= import_as_names_list_head DL_COMMA import_as_name ;
import_as_names_list ::= import_as_names_list_head ;
import_as_names_list ::= import_as_names_list_head DL_COMMA ;

-- import_statement = 'import' dotted_as_names_list |
--                ('from' ('.'* dotted_name | '.'+)
--                'import' ('*' | '(' import_as_names_list ')' | import_as_names_list))
import_statement ::= KW_IMPORT dotted_as_names_list ;
import_statement ::= KW_FROM dotted_name KW_IMPORT OP_MUL ;
import_statement ::= KW_FROM dotted_name KW_IMPORT import_as_names_list ;
import_statement ::= KW_FROM dotted_name KW_IMPORT DL_LEFT_PAREN import_as_names_list DL_RIGHT_PAREN ;
-- Import end


-- TODO: improve print statement
--print_stmt: 'print' ( [ test (',' test)* [','] ] | '>>' test [ (',' test)+ [','] ] )
print_statement ::= KW_PRINT ;
print_statement ::= KW_PRINT testlist ;

small_statement ::= import_statement ;
small_statement ::= expression_statement ;
small_statement ::= print_statement ;
small_statement ::= contol_flow_statement ;
small_statement ::= KW_PASS ;

small_statements_list ::= small_statements_list DL_SEMICOLON small_statement ;
small_statements_list ::= small_statement ;
simple_statement ::= small_statements_list DL_SEMICOLON DL_NEWLINE ;
simple_statement ::= small_statements_list DL_NEWLINE ;

-- Decorated begin
decorated ::= decorators class_definition ;
decorated ::= decorators function_definition ;
decorators ::= decorator ;
decorators ::= decorators decorator ;
decorator ::= OP_AT dotted_name DL_NEWLINE ;
decorator ::= OP_AT dotted_name DL_LEFT_PAREN arguments_list DL_RIGHT_PAREN DL_NEWLINE ;

-- Decorated end

-- Class begin
class_definition ::= KW_CLASS IDENTIFIER DL_COLON suite;
class_definition ::= KW_CLASS IDENTIFIER DL_LEFT_PAREN dotted_names_list DL_RIGHT_PAREN DL_COLON suite;
-- Class end

-- function definition begin
function_param_def ::= IDENTIFIER ;
function_param_def ::= DL_LEFT_PAREN function_param_list DL_RIGHT_PAREN ;
function_param_list_head ::= function_param_list_head DL_COMMA function_param_def ;
function_param_list ::= function_param_list_head ;
function_param_list ::= function_param_list_head DL_COMMA ;

function_param_full_list_head ::= function_param_def ;
function_param_full_list_head ::= function_param_def OP_ASSIGN test ;
function_param_full_list_head ::= function_param_full_list_head DL_COMMA function_param_def ;
function_param_full_list_head ::= function_param_full_list_head DL_COMMA function_param_def OP_ASSIGN test ;
function_param_full_list ::= function_param_full_list_head ;
function_param_full_list ::= function_param_full_list_head DL_COMMA ;

var_arguments_list_tail ::= OP_MUL IDENTIFIER ;
var_arguments_list_tail ::= OP_POWER IDENTIFIER ;
var_arguments_list_tail ::= OP_MUL IDENTIFIER DL_COMMA OP_POWER IDENTIFIER ;
var_arguments_list ::= function_param_full_list ;
var_arguments_list ::= var_arguments_list_tail;
var_arguments_list ::= function_param_full_list_head DL_COMMA var_arguments_list_tail;

function_parameters ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
function_parameters ::= DL_LEFT_PAREN var_arguments_list DL_RIGHT_PAREN ;
function_definition ::= KW_DEF IDENTIFIER function_parameters DL_COLON suite ;

-- function definition end

-- With begin
with_item ::= test ;
with_item ::= test KW_AS expression_or ; -- TODO: replace "expression_or" with "expression"
with_items_list ::= with_item ;
with_items_list ::= with_items_list DL_COMMA with_item ;
with_statement ::= KW_WITH with_items_list DL_COLON suite ;
-- With end

-- try statement begin
except_clause ::= KW_EXCEPT ;
except_clause ::= KW_EXCEPT test;
except_clause ::= KW_EXCEPT test KW_AS test;
except_clause ::= KW_EXCEPT test DL_COMMA test;
except_suite ::= except_clause DL_COLON suite ;
except_clauses_list ::= except_suite ;
except_clauses_list ::= except_clauses_list except_suite ;

except_block ::= except_clauses_list ;
except_block ::= except_clauses_list KW_ELSE DL_COLON suite ;
except_block ::= except_clauses_list KW_FINALLY DL_COLON suite ;
except_block ::= except_clauses_list KW_ELSE DL_COLON suite KW_FINALLY DL_COLON suite ;

try_statement ::= KW_TRY DL_COLON suite KW_FINALLY DL_COLON suite ;
try_statement ::= KW_TRY DL_COLON suite except_block ;
-- try statement end

-- For begin
for_block ::= KW_FOR expressions_list KW_IN testlist DL_COLON suite ;
for_statement ::= for_block ;
for_statement ::= for_block KW_ELSE DL_COLON suite ;
-- For end

-- While begin
while_statement ::= KW_WHILE test DL_COLON suite ;
while_statement ::= KW_WHILE test DL_COLON suite KW_ELSE DL_COLON suite ;
-- While end

-- if statement begin
-- Grammar // if_stmt: 'if' test ':' suite ('elif' test ':' suite)* ['else' ':' suite]

if_statement ::= KW_IF test DL_COLON suite ;
if_statement ::= KW_IF test DL_COLON suite elif_blocks_list ;
if_statement ::= KW_IF test DL_COLON suite KW_ELSE DL_COLON suite ;
if_statement ::= KW_IF test DL_COLON suite elif_blocks_list KW_ELSE DL_COLON suite ;

elif_block ::= KW_ELIF test DL_COLON suite ;
elif_blocks_list ::= elif_block ;
elif_blocks_list ::= elif_blocks_list elif_block ;

-- if statement end

suite ::= simple_statement ;
-- TODO: add INDENT/DEDENT tokens to this suite definition
newlines_list ::= DL_NEWLINE ;
newlines_list ::= newlines_list DL_NEWLINE ;
suite ::= newlines_list DL_INDENT statements_list DL_DEDENT;

compound_statement ::= if_statement ;
compound_statement ::= while_statement ;
compound_statement ::= for_statement ;
compound_statement ::= try_statement ;
compound_statement ::= with_statement ;
compound_statement ::= function_definition ;
compound_statement ::= class_definition ;
compound_statement ::= decorated ;

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
            context.implicitJoiningLevel = 0;
            fprintf (stderr, "%s:%d: Syntax Error\n", qPrintable(context.fileName), context.line);
            do {
                yytoken = nextToken();
                if (yytoken == EOF_SYMBOL)
                    return false;
            } while ((yytoken != DL_NEWLINE) && (yytoken != EOF_SYMBOL));
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
    for (int r = 0; r < PyGrammar::RULE_COUNT; ++r)
    {
        int ridx = PyGrammar::rule_index [r];
        int rhs = PyGrammar::rhs [r];
        printf ("%3d) %s ::=", r + 1, PyGrammar::spell[PyGrammar::rule_info [ridx]]);
        ++ridx;
        for (int i = ridx; i < ridx + rhs; ++i)
        {
            int symbol = PyGrammar::rule_info [i];
            if (const char *name = PyGrammar::spell [symbol])
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
