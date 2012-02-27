#ifndef PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c
#define PLUGIN_H_ec8aa375_5969_4bfa_b9fb_93383a3a186c

/**
  Класс-одиночка (singleton), реализующий интерфейс плагина
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
      Конструктор плагина Python Editor. Обычно внутри конструктора плагины
      не заняты ничем, кроме инициализации членов класса. Основная работа
      выполняется позже, в методах initialize() и extensionsInitialized()
      */
    CPlugin();

    /**
      Плагины отвечают за удаление объектов их кучи и прекращение их регистрации
      в менеджере плагинов
      */
    virtual ~CPlugin();

    /**
      Инициализирует плагин, возвращает true в случае успеха.
      Если плагин хочет зарегистрировать объекты в менеджере плагинов, он должен
      сделать это здесь.

        \a errorMessage используется для отправки сообщения об ошибке системе плагинов
      */
    virtual bool initialize(const QStringList &arguments,
                            QString *errorMessage);

    /**
      Уведомление о том, что все требуемые плагины проинициализированы.
      Требования описываются в файле .pluginspec

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
