# -*- coding: utf-8 -*-

## see example here:
## https://github.com/PySide/Examples/blob/master/examples/itemviews/simpletreemodel/simpletreemodel.py

class JsonItem:
    def __init__(self, data, parent = None):
        self.__data_ = data
        self.__parent_ = parent
        self.__children_ = []

    def appendChild(self, item):
        self.__children_.append(item)

    def child(self, row):
        return self.__children_[row]

    def childCount(self):
        return len(self.__children_)

    def columnCount(self):
        return len(self.__data_)

    def data(self, column):
        try:
            return self.__data_[column]
        except IndexError:
            return None

    def parent(self):
        return self.__parent_

    def row(self):
        if self.__parent_:
            return self.__parent_.__children_.index(self)
        return 0
