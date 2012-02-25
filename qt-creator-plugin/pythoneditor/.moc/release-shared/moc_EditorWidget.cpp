/****************************************************************************
** Meta object code from reading C++ file 'EditorWidget.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../core/EditorWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditorWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PythonEditor__Internal__CEditorWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      39,   38,   38,   38, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PythonEditor__Internal__CEditorWidget[] = {
    "PythonEditor::Internal::CEditorWidget\0"
    "\0setFontSettings(TextEditor::FontSettings)\0"
};

const QMetaObject PythonEditor::Internal::CEditorWidget::staticMetaObject = {
    { &TextEditor::BaseTextEditorWidget::staticMetaObject, qt_meta_stringdata_PythonEditor__Internal__CEditorWidget,
      qt_meta_data_PythonEditor__Internal__CEditorWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PythonEditor::Internal::CEditorWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PythonEditor::Internal::CEditorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PythonEditor::Internal::CEditorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PythonEditor__Internal__CEditorWidget))
        return static_cast<void*>(const_cast< CEditorWidget*>(this));
    typedef TextEditor::BaseTextEditorWidget QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int PythonEditor::Internal::CEditorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef TextEditor::BaseTextEditorWidget QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFontSettings((*reinterpret_cast< const TextEditor::FontSettings(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
