TEMPLATE = lib
TARGET = PyEditor

include(../../qtcreatorplugin.pri)

# dependencies
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/texteditor/texteditor.pri)
include(../../plugins/cpptools/cpptools.pri)
#include(../../plugins/projectexplorer/projectexplorer.pri)

DEFINES += \
    PYEDITOR_LIBRARY \
    QT_CREATOR

OTHER_FILES += pyeditor.pluginspec \
    pyeditor.mimetypes.xml

RESOURCES += \
    resource.qrc

HEADERS += \
    pyeditorplugin.h \
    pyeditorglobal.h \
    pyeditorfactory.h \
    pyeditor.h \
    pyeditorwidget.h \
    pyeditorconstants.h \
    pyeditorfeatures.h \
    wizard/filewizard.h \
    wizard/classwizarddialog.h \
    wizard/classwizard.h \
    wizard/classnamepage.h \
    tools/highlighter.h \
    tools/indenter.h \
    tools/lexical/formattoken.h \
    tools/lexical/scanner.h \
    tools/lexical/sourcecodestream.h

SOURCES += \
    pyeditorplugin.cpp \
    pyeditorfactory.cpp \
    pyeditor.cpp \
    pyeditorwidget.cpp \
    wizard/filewizard.cpp \
    wizard/classwizarddialog.cpp \
    wizard/classwizard.cpp \
    wizard/classnamepage.cpp \
    tools/highlighter.cpp \
    tools/indenter.cpp \
    tools/lexical/scanner.cpp

FORMS +=
