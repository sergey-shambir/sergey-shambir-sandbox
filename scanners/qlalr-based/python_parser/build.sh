#!/bin/sh

${FLEX-flex} -opy-lex.incl py-lex.l
${QLALR-qlalr} python.g

qmake
make
rm pyparser.o pygrammar.o