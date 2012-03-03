#ifndef PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c
#define PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c

/**
  �����-�������� (singleton), ����������� ��������� �������
  */

#include <extensionsystem/iplugin.h>
#include <texteditor/texteditoractionhandler.h>

namespace PythonEditor {
class EditorFactory;
class EditorWidget;
}

namespace PythonEditor {

class Plugin
  : public ExtensionSystem::IPlugin
{
    Q_OBJECT

public:
    /**
      ����������� ������� Python Editor. ������ ������ ������������ �������
      �� ������ �����, ����� ������������� ������ ������. �������� ������
      ����������� �����, � ������� initialize() � extensionsInitialized()
      */
    Plugin();

    /**
      ������� �������� �� �������� �������� �� ���� � ����������� �� �����������
      � ��������� ��������
      */
    virtual ~Plugin();

    /**
      �������������� ������, ���������� true � ������ ������.
      ���� ������ ����� ���������������� ������� � ��������� ��������, �� ������
      ������� ��� �����.

        \a errorMessage ������������ ��� �������� ��������� �� ������ ������� ��������
      */
    virtual bool initialize(const QStringList &arguments,
                            QString *errorMessage);

    /**
      ����������� � ���, ��� ��� ��������� ������� �������������������.
      ���������� ����������� � ����� .pluginspec

    Normally this method is used for things that rely on other plugins to have
    added objects to the plugin manager, that implement interfaces that we're
    interested in. These objects can now be requested through the
    PluginManagerInterface.
      */
    virtual void extensionsInitialized();

    static Plugin *instance() { return m_instance; }

    static void initializeEditor(EditorWidget *widget);

private:
    static Plugin *m_instance;
    EditorFactory* m_factory;
    TextEditor::TextEditorActionHandler *m_actionHandler;
};

} // namespace PythonEditor

#endif // PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c
