#!/bin/sh

${FLEX-flex} -opy-lex.incl py-lex.l
${QLALR-qlalr} python.g

qmake
make
rm pyparser.o pygrammar.o Makefile py-lex.incl pygrammar.cpp pygrammar_p.h pyparser.cpp pyparser_p.h
mv python.bin bin/python.bin