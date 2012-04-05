#ifndef PYEDITOR_PLUGIN_H
#define PYEDITOR_PLUGIN_H

#include "pyeditorglobal.h"

#include <extensionsystem/iplugin.h>
#include <texteditor/texteditoractionhandler.h>

namespace PyEditor {

class EditorFactory;
class EditorWidget;

/**
  \class PyEditor::Plugin implements ExtensionSystem::IPlugin
  Singletone object - PyEditor plugin
  */
class Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT

public:
    Plugin();

    virtual ~Plugin();

    virtual bool initialize(const QStringList &arguments,
                            QString *errorMessage);

    virtual void extensionsInitialized();

    static Plugin *instance() { return _instance; }

    static void initializeEditor(EditorWidget *widget);

private:
    static Plugin *_instance;
    EditorFactory* _factory;
    TextEditor::TextEditorActionHandler *_actionHandler;
};

} // namespace PyEditor

#endif // PYEDITOR_PLUGIN_H
