#ifndef CONSTANTS_H_b2b9e454_9802_4bdf_a851_baeee115ac09
#define CONSTANTS_H_b2b9e454_9802_4bdf_a851_baeee115ac09

namespace PythonEditor {
namespace Constants {

const char C_PYTHONEDITOR_ID[] = "PythonEditor.PythonEditor";

// File/Project creation wizard
const char C_PY_WIZARD_CATEGORY[] = "U.Python";
const char C_PY_EXTENSION[] = ".py";
const char C_PY_DISPLAY_CATEGORY[] = "Python";

const char C_PY_SOURCE_WIZARD_ID[] = "F.PySource";
const char C_PY_SOURCE_CONTENT[] =
        "#!/usr/bin/env python\n"
        "# -*- coding: utf-8 -*-\n";
const char EN_PY_SOURCE_DISPLAY_NAME[] = "Python source file";
const char EN_PY_SOURCE_DESCRIPTION[] =
        "Creates an empty python script with utf-8 charset";

const char C_PY_CLASS_WIZARD_ID[] = "F.PyClass";
const char EN_PY_CLASS_DISPLAY_NAME[] = "Python class";
const char EN_PY_CLASS_DESCRIPTION[] =
        "Creates new Python class";

// MIME type
const char C_PY_MIMETYPE[] = "text/x-python";
const char RC_PY_MIME_XML[] = ":/pythoneditor/pythoneditor.mimetypes.xml";
const char RC_PY_MIME_ICON[] = ":/pythoneditor/images/text-x-python.png";

/**
  List of Python keywords
  */
const char* const LIST_OF_PYTHON_KEYWORDS[] = {
    "and",
    "as",
    "assert",
    "break",
    "class",
    "continue",
    "def",
    "del",
    "elif",
    "else",
    "except",
    "exec",
    "finally",
    "for",
    "from",
    "global",
    "if",
    "import",
    "in",
    "is",
    "lambda",
    "not",
    "or",
    "pass",
    "print",
    "raise",
    "return",
    "try",
    "while",
    "with",
    "yield"
};

/**
  List of Python magic methods
  */
const char* const LIST_OF_PYTHON_MAGICS[] = {
    // ctor & dtor
    "__init__",
    "__del__",
    // string conversion functions
    "__str__",
    "__repr__",
    "__unicode__",
    // attribute access functions
    "__setattr__",
    "__getattr__",
    "__delattr__",
    // binary operators
    "__add__",
    "__sub__",
    "__mul__",
    "__truediv__",
    "__floordiv__",
    "__mod__",
    "__pow__",
    "__and__",
    "__or__",
    "__xor__",
    "__eq__",
    "__ne__",
    "__gt__",
    "__lt__",
    "__ge__",
    "__le__",
    "__lshift__",
    "__rshift__",
    "__contains__",
    // unary operators
    "__pos__",
    "__neg__",
    "__inv__",
    "__abs__",
    "__len__",
    // item operators like []
    "__getitem__",
    "__setitem__",
    "__delitem__",
    "__getslice__",
    "__setslice__",
    "__delslice__",
    // other functions
    "__cmp__",
    "__hash__",
    "__nonzero__",
    "__call__",
    "__iter__",
    "__reversed__",
    "__divmod__",
    "__int__",
    "__long__",
    "__float__",
    "__complex__",
    "__hex__",
    "__oct__",
    "__index__",
    "__copy__",
    "__deepcopy__",
    "__sizeof__",
    "__trunc__",
    "__format__"
};

/**
  List of python built-in functions
  */
const char* const LIST_OF_PYTHON_BUILTINS[] = {
    "range",
    "xrange",
    "int",
    "float",
    "long",
    "hex",
    "oct"
    "chr",
    "ord",
    "len",
    "abs",
    "None",
    "True",
    "False"
};

/**
  List of default Python modules (only standard Python implementation)
  */
const char* const LIST_OF_PYTHON_MODULES[] = {
    "__builtin__",
    "gettext",
    "sys"
};

} // namespace Constants
} // namespace GLSLEditor

#endif // CONSTANTS_H_b2b9e454_9802_4bdf_a851_baeee115ac09
