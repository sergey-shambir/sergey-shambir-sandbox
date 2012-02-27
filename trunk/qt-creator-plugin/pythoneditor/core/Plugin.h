#ifndef PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c
#define PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c

/**
  �����-�������� (singleton), ����������� ��������� �������
  */

#include <extensionsystem/iplugin.h>
#include <texteditor/texteditoractionhandler.h>

namespace PythonEditor {
class CEditorFactory;
class CEditorWidget;
}

namespace PythonEditor {

class CPlugin
  : public ExtensionSystem::IPlugin
{
    Q_OBJECT

public:
    /**
      ����������� ������� Python Editor. ������ ������ ������������ �������
      �� ������ �����, ����� ������������� ������ ������. �������� ������
      ����������� �����, � ������� initialize() � extensionsInitialized()
      */
    CPlugin();

    /**
      ������� �������� �� �������� �������� �� ���� � ����������� �� �����������
      � ��������� ��������
      */
    virtual ~CPlugin();

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

    static CPlugin *instance() { return m_instance; }

    static void initializeEditor(CEditorWidget *widget);

private:
    static CPlugin *m_instance;
    CEditorFactory* m_factory;
    TextEditor::TextEditorActionHandler *m_actionHandler;
};

} // namespace PythonEditor

#endif // PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c
