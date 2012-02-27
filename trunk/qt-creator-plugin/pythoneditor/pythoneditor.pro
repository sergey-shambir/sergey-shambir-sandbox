TEMPLATE = lib
TARGET = PythonEditor
#PROVIDER = Community

include(../../qtcreatorplugin.pri)

# dependencies
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/texteditor/texteditor.pri)
include(../../plugins/cpptools/cpptools.pri)
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
    features/Lexer.h \
    core/features.h \
    features/Indenter.h

OTHER_FILES += pythoneditor.pluginspec \
    pythoneditor.mimetypes.xml

SOURCES += \
    core/Plugin.cpp \
    core/EditorFactory.cpp \
    core/EditorWidget.cpp \
    core/Editor.cpp \
    features/FileWizard.cpp \
    features/Highlighter.cpp \
    features/Lexer.cpp \
    features/Indenter.cpp

RESOURCES += \
    resource.qrc

FORMS +=





























































































































