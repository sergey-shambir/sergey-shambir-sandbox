
%{
/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QLALR tool of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <cassert>
#define YY_DECL int PyParser::nextToken()
%}

%option noyywrap
%x ONSTATEMENT

hexditit [0-9a-fA-F]
octdigit [0-7]
bindigit [01]
digit [0-9]

fract {digit}*\.{digit}+|{digit}+\.
exp [eE][+-]?{digit}+

octal 0[oO]?{octdigit}+
binary 0[bB]{bindigit}+
decimal [1-9][0-9]*|0

icst ({decimal}|{octal}|0[xX]{hexditit}+)

simplestring_squote \'([^'\\\n]*(\\(.|\n)[^'\\\n]*)*)\'
simplestring_dquote \"([^"\\\n]*(\\(.|\n)[^"\\\n]*)*)\"

simplestring ({simplestring_squote}|{simplestring_dquote})

escapednewline \\[\n]

%%

<*>escapednewline                       { /* skip */ }
<*>[\r]+                                { /* skip */ }

<*>[\n]         {
        ++context.line;
        context.indentCurrent = 0;
        BEGIN(INITIAL);
        return DL_NEWLINE;
    }

<INITIAL>[ ]         { context.indentCurrent += 1; }
<INITIAL>[\t]        { context.indentCurrent += 4; }
<INITIAL>.           {
        if (context.checkIndent()) {
            yyunput(*yytext, yytext);
            BEGIN(ONSTATEMENT);
            return DL_INDENT;
        }
        if (context.checkDedent()) {
            yyunput(*yytext, yytext);
            /* we can't return to INITIAL state here */
            return DL_DEDENT;
        }
        yyunput(*yytext, yytext);
        BEGIN(ONSTATEMENT);
    }

<INITIAL><<EOF>>     {
        if (context.checkIndent()) {
            return DL_INDENT;
        }
        if (context.checkDedent()) {
            return DL_DEDENT;
        }
        yyterminate();
    }

<ONSTATEMENT>[ \t]+ { /* skip */ }

<ONSTATEMENT>{simplestring} {
        QString qtext(yytext);
        yylval.s = intern (qtext.mid(1, qtext.size()));
        return LT_STRING;
    }

<ONSTATEMENT>"import" { return KW_IMPORT; }
<ONSTATEMENT>"from" { return KW_FROM; }
<ONSTATEMENT>"as" { return KW_AS; }
<ONSTATEMENT>"while" { return KW_WHILE; }
<ONSTATEMENT>"if" { return KW_IF; }
<ONSTATEMENT>"elif" { return KW_ELIF; }
<ONSTATEMENT>"else" { return KW_ELSE; }
<ONSTATEMENT>"break" { return KW_BREAK; }
<ONSTATEMENT>"continue" { return KW_CONTINUE; }
<ONSTATEMENT>"try" { return KW_TRY; }
<ONSTATEMENT>"except" { return KW_EXCEPT; }
<ONSTATEMENT>"finally" { return KW_FINALLY; }
<ONSTATEMENT>"pass" { return KW_PASS; }
<ONSTATEMENT>"print" { return KW_PRINT; }
<ONSTATEMENT>"class" { return KW_CLASS; }
<ONSTATEMENT>"def" { return KW_DEF; }

<ONSTATEMENT>"or" { return KW_OR; }
<ONSTATEMENT>"is" { return KW_IS; }
<ONSTATEMENT>"in" { return KW_IN; }
<ONSTATEMENT>"not" { return KW_NOT; }

<ONSTATEMENT>"None" { return LT_NONE; }
<ONSTATEMENT>"True" { return LT_BOOLEAN; }
<ONSTATEMENT>"False" { return LT_BOOLEAN; }

<ONSTATEMENT>":" { return DL_COLON; }
<ONSTATEMENT>"," { return DL_COMMA; }
<ONSTATEMENT>"(" { return DL_LEFT_PAREN; }
<ONSTATEMENT>")" { return DL_RIGHT_PAREN; }
<ONSTATEMENT>";" { return DL_SEMICOLON; }

<ONSTATEMENT>"+" { return OP_PLUS; }
<ONSTATEMENT>"-" { return OP_MINUS; }
<ONSTATEMENT>"==" { return OP_EQUAL; }
<ONSTATEMENT>"!=" { return OP_NOT_EQUAL; }
<ONSTATEMENT>"<>" { return OP_NOT_EQUAL; }
<ONSTATEMENT>"=" { return OP_ASSIGN; }
<ONSTATEMENT>">=" { return OP_GE; }
<ONSTATEMENT>"<=" { return OP_LE; }
<ONSTATEMENT>">" { return OP_GT; }
<ONSTATEMENT>"<" { return OP_LT; }
<ONSTATEMENT>"." { return OP_DOT; }
<ONSTATEMENT>"**" { return OP_POWER; }
<ONSTATEMENT>"%" { return OP_MOD; }

<ONSTATEMENT>#.* {
  /* skip */
}

<ONSTATEMENT>[_a-zA-Z][_a-zA-Z0-9]* {
  yylval.s = intern (yytext);
  return IDENTIFIER;
}

<ONSTATEMENT>{icst} {
  yylval.i = (int) strtol (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}[uU] {
  yylval.u = (unsigned) strtoul (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}[uU][lL] {
  yylval.ul = strtoul (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}[lL][uU] {
  yylval.ul = strtoul (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}[lL] {
  yylval.l = strtol (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}[uU](ll|LL) {
  yylval.l = strtoull (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}(ll|LL) {
  yylval.l = strtoll (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{icst}(ll|LL)[uU] {
  yylval.l = strtoull (yytext, 0, 0);
  return LT_INTEGER;
}

<ONSTATEMENT>{fract}{exp}?[jJ] {
  yylval.f = strtof (yytext, 0);
  return LT_COMPLEX;
}

<ONSTATEMENT>{digit}+{exp}[jJ] {
  yylval.f = strtof (yytext, 0);
  return LT_COMPLEX;
}

<ONSTATEMENT>{fract}{exp}? {
  yylval.f = strtof (yytext, 0);
  return LT_FLOAT;
}

<ONSTATEMENT>{digit}+{exp} {
  yylval.f = strtof (yytext, 0);
  return LT_FLOAT;
}

<ONSTATEMENT>. {
  fprintf (stderr, "invalid char: %d\n", yytext [0]);
  return ERROR;
}


%%

