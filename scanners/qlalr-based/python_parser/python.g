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

%token TK_IDENTIFIER
%token TK_ERROR

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

%token KW_OR
%token KW_IS
%token KW_IN
%token KW_NOT

-- delimiters have DL_ prefix
%token DL_NEWLINE
%token DL_COLON
%token DL_INDENT
%token DL_DEDENT
%token DL_RIGHT_PAREN
%token DL_LEFT_PAREN
%token DL_LEFT_BRACKET
%token DL_RIGHT_BRACKET
%token DL_SEMICOLON
%token DL_COMMA

%token OP_PLUS
%token OP_MINUS
%token OP_DOT
%token OP_EQUAL
%token OP_NOT_EQUAL
%token OP_LT
%token OP_LE
%token OP_GT
%token OP_GE
%token OP_ASSIGN
%token OP_POWER
%token OP_MOD
%token OP_PLUSHKA

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
                int symbol = rule_info [i];
                // if (const char *name = spell [symbol])
                    //printf (" %s", name);
                // else
                    //printf (" #%d", symbol);
            }
            //printf ("\n");

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
atom ::= TK_IDENTIFIER ;
atom ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
-- TODO: change to // testlist_comp: test ( comp_for | (',' test)* [','] )
atom ::= DL_LEFT_PAREN testlist DL_RIGHT_PAREN ;
atom ::= DL_LEFT_BRACKET DL_RIGHT_BRACKET ;
atom ::= DL_LEFT_BRACKET list_initializer DL_RIGHT_BRACKET ;

trailer ::= OP_DOT TK_IDENTIFIER ;
trailer ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
trailer ::= DL_LEFT_PAREN arguments_list DL_RIGHT_PAREN ;
trailer ::= DL_LEFT_BRACKET subscript_list DL_RIGHT_BRACKET ;

subscript ::= OP_DOT OP_DOT OP_DOT ;
subscript ::= test ;
--! TODO: add rule // subscript ::= [test] ':' [test] [sliceop]
subscript_list_head ::= subscript ;
subscript_list_head ::= subscript_list_head DL_COMMA subscript ;
subscript_list ::= subscript_list_head ;
subscript_list ::= subscript_list_head DL_COMMA ;

trailers_list ::= trailer ;
trailers_list ::= trailers_list trailer ;

primary_expression ::= atom ;
primary_expression ::= atom trailers_list ;
--primary_expression ::= LEFT_PAREN expression RIGHT_PAREN ;
--postfix_expression ::= primary_expression ;
--postfix_expression ::= function_call ;
--postfix_expression ::= postfix_expression DOT IDENTIFIER ;

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

-- Expressions begin

-- TODO: implement normal test_of_binary

term ::= primary_expression ;

test_of_binary ::= term ;
test_of_binary ::= test_of_binary OP_PLUS term ;
test_of_binary ::= test_of_binary OP_MINUS term ;
test_of_binary ::= test_of_binary OP_MOD term ;
comparison ::= test_of_binary ;
comparison ::= comparison comparison_operator test_of_binary ;
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

test_of_not ::= comparison ;
test_of_not ::= KW_NOT test_of_not ;

test_of_or ::= test_of_not ;
test_of_or ::= test_of_not KW_OR test_of_not ;

test ::= test_of_or ;
--test ::= KW_IF or_test KW_ELSE test
--test ::= lambda_definition ;

testlist_head ::= test ;
testlist_head ::= testlist_head DL_COMMA test ;
testlist ::= testlist_head ;
testlist ::= testlist_head DL_COMMA ;

-- TODO: extend this definition // argument: test [comp_for] | test '=' test
argument ::= test ;
argument ::= test OP_ASSIGN test ;

arguments_list_head ::= argument ;
arguments_list_head ::= arguments_list_head DL_COMMA argument ;
-- TODO: add this ending variant // '*' test (',' argument)* [',' '**' test] 
arguments_list ::= arguments_list_head ;
arguments_list ::= arguments_list_head DL_COMMA ;
arguments_list ::= arguments_list_head DL_COMMA OP_POWER test ;

expression_statement ::= testlist ;
expression_statement ::= expression_statement OP_ASSIGN testlist ;

expressions_list_head ::= test_of_binary ;
expressions_list_head ::= expressions_list_head DL_COMMA test_of_binary ;
expressions_list ::= expressions_list_head ;
expressions_list ::= expressions_list_head DL_COMMA ;

-- Expressions end

-- List and dict begin

--test_safe ::= test_of_or ;
--! TODO: add test_safe ::= lambda_definition_safe ;

--testlist_safe_head ::= test_safe ;
--testlist_safe_head ::= testlist_safe DL_COMMA test_safe ;
--testlist_safe ::= testlist_safe_head ;
--testlist_safe ::= testlist_safe_head DL_COMMA ;

--! TODO: replace testlist with testlist_safe at list_for, replace test with test_safe at list_if
list_if ::= KW_IF test ;
list_if ::= KW_IF test list_iteration ;
list_for_impl ::= KW_FOR expressions_list KW_IN testlist;
list_iteration ::= list_for ;
list_iteration ::= list_if ;
list_for ::= list_for_impl ;
list_for ::= list_for_impl list_iteration ;
list_initializer ::= test list_for ;
list_initializer ::= testlist ;
-- List and dict end

qualified_name ::= qualified_name OP_DOT TK_IDENTIFIER ;
qualified_name ::= TK_IDENTIFIER ;

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
decorator ::= OP_PLUSHKA qualified_name DL_NEWLINE ;
decorator ::= OP_PLUSHKA qualified_name DL_LEFT_PAREN arguments_list DL_RIGHT_PAREN DL_NEWLINE ;

-- Decorated end

-- Class begin
class_definition ::= KW_CLASS TK_IDENTIFIER DL_COLON suite;
class_definition ::= KW_CLASS TK_IDENTIFIER DL_LEFT_PAREN qualified_names_list DL_RIGHT_PAREN DL_COLON suite;
-- Class end

-- function definition begin
function_param_def ::= TK_IDENTIFIER ;
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

-- varargslist: ((fpdef ['=' test] ',')*
--              ('*' NAME [',' '**' NAME] | '**' NAME) |
--              fpdef ['=' test] (',' fpdef ['=' test])* [','])
var_arguments_list ::= function_param_full_list ;
var_arguments_list ::= function_param_full_list_head DL_COMMA OP_POWER TK_IDENTIFIER ;

function_parameters ::= DL_LEFT_PAREN DL_RIGHT_PAREN ;
function_parameters ::= DL_LEFT_PAREN var_arguments_list DL_RIGHT_PAREN ;
function_definition ::= KW_DEF TK_IDENTIFIER function_parameters DL_COLON suite ;

-- function definition end

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
--compound_statement ::= with_statement ;
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
