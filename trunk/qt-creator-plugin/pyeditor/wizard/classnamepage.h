#ifndef PYEDITOR_CLASSNAMEPAGE_H
#define PYEDITOR_CLASSNAMEPAGE_H

/**
 * Страница должна иметь:
 *
 * 1. поле ввода строки документации
 * 2. поле ввода базовых классов
 * 3. поле выбора базового класса Qt!!!!
 *
 * Генерироваться должны:
 * 1. метод __init__
 * 2. строка документации, если указана
 * 3. базовые классы, если указаны
 *
 */

#include <QtGui/QWizardPage>

namespace Utils { class NewClassWidget; }

namespace PyEditor {

class ClassNamePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ClassNamePage(QWidget *parent = 0);
    virtual ~ClassNamePage();

    bool isComplete() const { return _isValid; }
    Utils::NewClassWidget *newClassWidget() const { return _newClassWidget; }

private slots:
    void slotValidChanged();

private:
    void initParameters();

    Utils::NewClassWidget *_newClassWidget;
    bool _isValid;
};

} // namespace PyEditor

#endif // PYEDITOR_CLASSNAMEPAGE_H
