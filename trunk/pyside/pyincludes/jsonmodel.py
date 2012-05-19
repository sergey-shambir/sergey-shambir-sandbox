# -*- coding: utf-8 -*-

import sys

try:
    from PyQt4 import QtCore
except:
    from PySide import QtCore

from jsonitem import JsonItem
from parser import Parser

class JsonModel(QtCore.QAbstractItemModel):
    def __init__(self, data, parent = None):
        super(JsonModel, self).__init__(parent)
        self.__root_ = JsonItem([u"Параметр", u"Полученное значение"])
        self.setupModelData(data, self.__root_)

    def columnCount(self, parent):
        if parent.isValid():
            return parent.internalPointer().columnCount()
        else:
            return self.__root_.columnCount()

    def data(self, index, role):
        if not index.isValid():
            return None

        if role != QtCore.Qt.DisplayRole:
            return None

        item = index.internalPointer()

        return item.data(index.column())

    def flags(self, index):
        if not index.isValid():
            return QtCore.Qt.NoItemFlags

        return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable

    def headerData(self, section, orientation, role):
        if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
            return self.__root_.data(section)

        return None

    def index(self, row, column, parent):
        if not self.hasIndex(row, column, parent):
            return QtCore.QModelIndex()

        if not parent.isValid():
            parentItem = self.__root_
        else:
            parentItem = parent.internalPointer()

        childItem = parentItem.child(row)
        if childItem:
            return self.createIndex(row, column, childItem)
        else:
            return QtCore.QModelIndex()

    def parent(self, index):
        if not index.isValid():
            return QtCore.QModelIndex()

        childItem = index.internalPointer()
        parentItem = childItem.parent()

        if parentItem == self.__root_:
            return QtCore.QModelIndex()

        return self.createIndex(parentItem.row(), 0, parentItem)

    def rowCount(self, parent):
        if parent.column() > 0:
            return 0
        if not parent.isValid():
            parentItem = self.__root_
        else:
            parentItem = parent.internalPointer()
        return parentItem.childCount()

    def setupModelData(self, objects, parent):
        if isinstance(objects, list):
            if (len(objects) == 1):
                self.setupModelData(objects[0], parent)
                return
            for index in range(len(objects)):
                obj = objects[index]
                if isinstance(obj, list) or isinstance(obj, dict):
                    label = "#%d" % (index + 1)
                    if isinstance(obj, dict) and obj.has_key(u"User"):
                        label = obj["User"]
                    item = JsonItem([label, u""], parent)
                    parent.appendChild(item)
                    self.setupModelData(obj, item)
                else:
                    parent.appendChild(JsonItem([obj, u""], parent))
        else:
            for key in objects:
                obj = objects[key]
                if isinstance(obj, list) or isinstance(obj, dict):
                    item = JsonItem([key, u""], parent)
                    parent.appendChild(item)
                    self.setupModelData(obj, item)
                else:
                    parent.appendChild(JsonItem([key, obj], parent))
