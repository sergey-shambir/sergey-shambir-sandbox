#ifndef TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
#define TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a

/**
  QtCreator ��������� � ������������ ��� Core ���������� IFile � IEditor
  ����� ��������� ���������� ������� ������������� ��������� ��� ������,
  ���������� ����������� ���������� IFile � IEditor.
  ��� ���������� �� ��������� � �������� ���������� ����, �
  ������ �������������� ������ ����� ���� �����, ��������, ��� � Firebug.

********************************************************************************
  �������� ����������� �� BaseTextEditorWidget.
  ������� ����� �������� ����������, � �� ��������������� ����������,
  �������� �� ��� ���� ����������� ����� ����, �� ��������� ��������� ����
  ������� ��������� (Do/Undo).
  */

#include <texteditor/basetexteditor.h>
#include <utils/uncommentselection.h>

namespace PythonEditor {

class CEditorWidget : public TextEditor::BaseTextEditorWidget
{
    Q_OBJECT

public:
    CEditorWidget(QWidget *parent = 0);
    virtual ~CEditorWidget();

    virtual void unCommentSelection();

public slots:
    /**
      ��������� ����� ��� IDE �������� ��������� ����, ������, ��������������
      */
    virtual void setFontSettings(const TextEditor::FontSettings &);

protected:
    TextEditor::BaseTextEditor *createEditor();

private:
    Utils::CommentDefinition m_commentDefinition;
};

} // namespace PythonEditor

#endif // TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
