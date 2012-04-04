QT = core
TARGET = python.bin

HEADERS += \
        pygrammar_p.h \
        pyparser_p.h

SOURCES += \
        pygrammar.cpp \
        pyparser.cpp

OTHER_FILES += \
    py-lex.l \
    python.g
