
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
%x STMT

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

newline \n\r|\r|\n

escapednewline \\\n[ \t]*

%%

<*>{escapednewline}                       { /* skip */ }
<*>[\r]+                                { /* skip */ }

<*>{newline}    {
        if (context.implicitJoiningLevel == 0) {
            ++context.line;
            context.indentCurrent = 0;
            BEGIN(INITIAL);
            return DL_NEWLINE;
        }
    }

<*>#.* {
  /* skip */
}

<INITIAL>[ ]         { context.indentCurrent += 1; }
<INITIAL>[\t]        { context.indentCurrent += 4; }
<INITIAL>.           {
        if (context.checkIndent()) {
            yyunput(*yytext, yytext);
            BEGIN(STMT);
            return DL_INDENT;
        }
        if (context.checkDedent()) {
            yyunput(*yytext, yytext);
            /* we can't return to INITIAL state here, becouse we can meet several DL_DEDENT at single line */
            return DL_DEDENT;
        }
        yyunput(*yytext, yytext);
        BEGIN(STMT);
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

<STMT>\"\"\" {
        //! TODO: May be refactor it? Anyway, it works.
        QString qtext;
        qtext.reserve(100);
        int matching[3];
        matching[1] = yyinput();
        matching[2] = yyinput();
        for (;;) {
            matching[0] = matching[1];
            matching[1] = matching[2];
            matching[2] = yyinput();
            if ((matching[2] == 0) || (matching[2] == EOF))
                return ERROR;
            if (matching[0] == '\\') {
                qtext += matching[0];
                qtext += matching[1];
                matching[0] = matching[1];
                matching[1] = matching[2];
                matching[2] = yyinput();
                continue;
            }
            if ((matching[0] == '\"') && (matching[1] == '\"') && (matching[2] == '\"')) {
                yylval.s = intern(qtext);
                return LT_STRING;
            }
            if (qtext.size() == qtext.capacity())
                qtext.reserve(qtext.size() + 100);
            qtext += matching[0];
        }
    }

<STMT>\'\'\' {
        //! TODO: May be refactor it? Anyway, it works.
        QString qtext;
        qtext.reserve(100);
        int matching[3];
        matching[1] = yyinput();
        matching[2] = yyinput();
        for (;;) {
            matching[0] = matching[1];
            matching[1] = matching[2];
            matching[2] = yyinput();
            if ((matching[2] == 0) || (matching[2] == EOF))
                return ERROR;
            if (matching[0] == '\\') {
                qtext += matching[0];
                qtext += matching[1];
                matching[0] = matching[1];
                matching[1] = matching[2];
                matching[2] = yyinput();
                continue;
            }
            if ((matching[0] == '\'') && (matching[1] == '\'') && (matching[2] == '\'')) {
                yylval.s = intern(qtext);
                return LT_STRING;
            }
            if (qtext.size() == qtext.capacity())
                qtext.reserve(qtext.size() + 100);
            qtext += matching[0];
        }
    }

<STMT>[ \t]+ { /* skip */ }

<STMT>{simplestring} {
        QString qtext(yytext);
        yylval.s = intern (qtext.mid(1, qtext.size() - 2));
        return LT_STRING;
    }

<STMT>"import" { return KW_IMPORT; }
<STMT>"from" { return KW_FROM; }
<STMT>"as" { return KW_AS; }
<STMT>"with" { return KW_WITH; }
<STMT>"while" { return KW_WHILE; }
<STMT>"for" { return KW_FOR; }
<STMT>"if" { return KW_IF; }
<STMT>"elif" { return KW_ELIF; }
<STMT>"else" { return KW_ELSE; }
<STMT>"break" { return KW_BREAK; }
<STMT>"continue" { return KW_CONTINUE; }
<STMT>"return" { return KW_RETURN; }
<STMT>"yield" { return KW_YIELD; }
<STMT>"raise" { return KW_RAISE; }

<STMT>"try" { return KW_TRY; }
<STMT>"except" { return KW_EXCEPT; }
<STMT>"finally" { return KW_FINALLY; }
<STMT>"pass" { return KW_PASS; }
<STMT>"print" { return KW_PRINT; }
<STMT>"class" { return KW_CLASS; }
<STMT>"def" { return KW_DEF; }
<STMT>"lambda" { return KW_LAMBDA; }

<STMT>"and" { return KW_AND; }
<STMT>"or" { return KW_OR; }
<STMT>"xor" { return KW_XOR; }
<STMT>"is" { return KW_IS; }
<STMT>"in" { return KW_IN; }
<STMT>"not" { return KW_NOT; }

<STMT>"None" { return LT_NONE; }
<STMT>"True" { return LT_BOOLEAN; }
<STMT>"False" { return LT_BOOLEAN; }

<STMT>"(" {
        ++context.implicitJoiningLevel;
        return DL_LEFT_PAREN;
    }
<STMT>")" {
        --context.implicitJoiningLevel;
        return DL_RIGHT_PAREN;
    }
<STMT>"[" {
        ++context.implicitJoiningLevel;
        return DL_LEFT_BRACKET;
    }
<STMT>"]" {
        --context.implicitJoiningLevel;
        return DL_RIGHT_BRACKET;
    }
<STMT>"{" {
        ++context.implicitJoiningLevel;
        return DL_LEFT_BRACE;
    }
<STMT>"}" {
        --context.implicitJoiningLevel;
        return DL_RIGHT_BRACE;
    }
<STMT>":" { return DL_COLON; }
<STMT>"," { return DL_COMMA; }
<STMT>";" { return DL_SEMICOLON; }
<STMT>"`" { return DL_BACKQUOTE; }

<STMT>"+=" { return OP_AUG_PLUS; }
<STMT>"-=" { return OP_AUG_MINUS; }
<STMT>"**=" { return OP_AUG_POWER; }
<STMT>"*=" { return OP_AUG_MUL; }
<STMT>"//=" { return OP_AUG_FLOORDIV; }
<STMT>"/=" { return OP_AUG_DIV; }
<STMT>"%=" { return OP_AUG_MOD; }
<STMT>"&=" { return OP_AUG_AND; }
<STMT>"|=" { return OP_AUG_OR; }
<STMT>"^=" { return OP_AUG_XOR; }
<STMT>"<<=" { return OP_AUG_LSHIFT; }
<STMT>">>=" { return OP_AUG_RSHIFT; }

<STMT>"+" { return OP_PLUS; }
<STMT>"-" { return OP_MINUS; }
<STMT>"==" { return OP_EQUAL; }
<STMT>"!=" { return OP_NOT_EQUAL; }
<STMT>"<>" { return OP_NOT_EQUAL; }
<STMT>"=" { return OP_ASSIGN; }
<STMT>">=" { return OP_GE; }
<STMT>"<=" { return OP_LE; }
<STMT>">" { return OP_GT; }
<STMT>"<" { return OP_LT; }
<STMT>"." { return OP_DOT; }
<STMT>"**" { return OP_POWER; }
<STMT>"%" { return OP_MOD; }
<STMT>"*" { return OP_MUL; }
<STMT>"//" { return OP_FLOORDIV; }
<STMT>"/" { return OP_DIV; }
<STMT>"@" { return OP_AT; }
<STMT>"~" { return OP_COMPLEMENT; }

<STMT>[_a-zA-Z][_a-zA-Z0-9]* {
  yylval.s = intern (yytext);
  return IDENTIFIER;
}

<STMT>{icst} {
  yylval.i = (int) strtol (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}[uU] {
  yylval.u = (unsigned) strtoul (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}[uU][lL] {
  yylval.ul = strtoul (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}[lL][uU] {
  yylval.ul = strtoul (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}[lL] {
  yylval.l = strtol (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}[uU](ll|LL) {
  yylval.l = strtoull (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}(ll|LL) {
  yylval.l = strtoll (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{icst}(ll|LL)[uU] {
  yylval.l = strtoull (yytext, 0, 0);
  return LT_INTEGER;
}

<STMT>{fract}{exp}?[jJ] {
  yylval.f = strtof (yytext, 0);
  return LT_COMPLEX;
}

<STMT>{digit}+{exp}[jJ] {
  yylval.f = strtof (yytext, 0);
  return LT_COMPLEX;
}

<STMT>{fract}{exp}? {
  yylval.f = strtof (yytext, 0);
  return LT_FLOAT;
}

<STMT>{digit}+{exp} {
  yylval.f = strtof (yytext, 0);
  return LT_FLOAT;
}

<STMT>. {
  fprintf (stderr, "invalid char: %d, or %c\n", (int) yytext [0], (char)(yytext[0]));
  return ERROR;
}


%%

