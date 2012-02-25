/****************************************************************************
** Meta object code from reading C++ file 'EditorFactory.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../core/EditorFactory.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditorFactory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PythonEditor__Internal__CEditorFactory[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_PythonEditor__Internal__CEditorFactory[] = {
    "PythonEditor::Internal::CEditorFactory\0"
};

const QMetaObject PythonEditor::Internal::CEditorFactory::staticMetaObject = {
    { &Core::IEditorFactory::staticMetaObject, qt_meta_stringdata_PythonEditor__Internal__CEditorFactory,
      qt_meta_data_PythonEditor__Internal__CEditorFactory, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PythonEditor::Internal::CEditorFactory::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PythonEditor::Internal::CEditorFactory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PythonEditor::Internal::CEditorFactory::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PythonEditor__Internal__CEditorFactory))
        return static_cast<void*>(const_cast< CEditorFactory*>(this));
    typedef Core::IEditorFactory QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int PythonEditor::Internal::CEditorFactory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef Core::IEditorFactory QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
