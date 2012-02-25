TEMPLATE = lib
TARGET = PythonEditor
include(../../qtcreatorplugin.pri)

# dependencies
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/texteditor/texteditor.pri)
#include(../../plugins/projectexplorer/projectexplorer.pri)

DEFINES += \
    GLSLEDITOR_LIBRARY \
    QT_CREATOR

HEADERS += \
    core/constants.h \
    core/Plugin.h \
    core/EditorFactory.h \
    core/EditorWidget.h \
    core/Editor.h \
    features/FileWizard.h \
    features/Highlighter.h \
    features/Lexer.h

OTHER_FILES +=

SOURCES += \
    core/Plugin.cpp \
    core/EditorFactory.cpp \
    core/EditorWidget.cpp \
    core/Editor.cpp \
    features/FileWizard.cpp \
    features/Highlighter.cpp \
    features/Lexer.cpp

RESOURCES += \
    resource.qrc















































































































