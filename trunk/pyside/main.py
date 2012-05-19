#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

try:
    from PyQt4 import QtCore
    from PyQt4 import QtGui
except:
    from PySide import QtCore
    from PySide import QtGui

from pyincludes.jsonview import JsonView

if __name__ == '__main__':
    WINDOW_X = 100
    WINDOW_Y = 100
    WINDOW_WIDTH = 600
    WINDOW_HEIGHT = 500
    BUTTON_HEIGHT = 25
    BUTTON_WIDTH = WINDOW_WIDTH // 3

    app = QtGui.QApplication(sys.argv)
    win = QtGui.QWidget()
    win.setWindowTitle(u'Информация о системе')
    win.setMaximumSize(WINDOW_WIDTH, WINDOW_HEIGHT)
    win.setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT)
    win.setGeometry(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT)

    view = JsonView(win)
    view.setGeometry(0, BUTTON_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - BUTTON_HEIGHT)

    button_os = QtGui.QPushButton(u"Показать сведения об ОС", win)
    button_os.setGeometry(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT)
    button_os.setShortcut(u"s")
    button_os.setToolTip(u"Хоткей - S")
    view.connect(button_os, QtCore.SIGNAL('clicked()'), view.load_os_info_model)

    button_cpu = QtGui.QPushButton(u"Показать сведения о ЦПУ", win)
    button_cpu.setGeometry(2 * BUTTON_WIDTH, 0, BUTTON_WIDTH, BUTTON_HEIGHT)
    button_cpu.setShortcut(u"c")
    button_cpu.setToolTip(u"Хоткей - C")
    view.connect(button_cpu, QtCore.SIGNAL('clicked()'), view.load_cpu_info_model)

    win.show()
    app.exec_()
