# -*- coding: utf-8 -*-


try:
    from PyQt4.QtCore import pyqtSlot
    from PyQt4 import QtCore
    from PyQt4 import QtGui
except:
    from PySide.QtCore import Slot as pyqtSlot
    from PySide import QtCore
    from PySide import QtGui

from parser import Parser
from jsonmodel import JsonModel

class JsonView(QtGui.QTreeView):
    @pyqtSlot()
    def load_cpu_info_model(self):
        s = self.__parser_.parse_cpu_info()
        self.__model_ = JsonModel(s)
        self.setModel(self.__model_)

    @pyqtSlot()
    def load_os_info_model(self):
        s = self.__parser_.parse_os_info()
        self.__model_ = JsonModel(s)
        self.setModel(self.__model_)

    def __init__(self, parent = None):
        QtGui.QTreeView.__init__(self, parent)
        self.header().setResizeMode(QtGui.QHeaderView.ResizeToContents)
        self.__parser_ = Parser()
        self.__model_ = None
        self.load_os_info_model()
