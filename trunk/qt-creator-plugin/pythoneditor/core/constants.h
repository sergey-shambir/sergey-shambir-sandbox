#ifndef CONSTANTS_H_b2b9e454_9802_4bdf_a851_baeee115ac09
#define CONSTANTS_H_b2b9e454_9802_4bdf_a851_baeee115ac09

namespace PythonEditor {
namespace Constants {

const char C_PYTHONEDITOR_ID[] = "PythonEditor.PythonEditor";
const char C_NEW_FILE_CONTENT[] =
        "#!/usr/bin/env python\n"
        "# -*- coding: utf-8 -*-\n";

const char C_WIZARD_CATEGORY_PYTHON[] = "U.Python";
const char C_DISPLAY_CATEGORY_PYTHON[] = "Python";
const char C_WIZARD_ID_PYTHON[] = "F.Python";
const char C_PYTHON_MIMETYPE[] = "text/x-python";
const char RC_PYTHON_MIME_XML[] = ":/pythoneditor/pythoneditor.mimetypes.xml";
const char RC_PYTHON_MIME_ICON[] = ":/pythoneditor/images/text-x-python.png";

const char* const C_PYTHON_KEYWORDS[] = {
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
    "tru",
    "while",
    "with",
    "yield",
    "None"
};

} // namespace Constants
} // namespace GLSLEditor

#endif // CONSTANTS_H_b2b9e454_9802_4bdf_a851_baeee115ac09
