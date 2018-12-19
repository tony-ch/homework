#!/usr/bin/env python
#coding: utf-8

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import QListWidget,QListWidgetItem
from PyQt5.QtGui import QIcon,QPixmap

from PyQt5.QtCore import Qt
#
# class MyListWidget(QtWidgets.QListWidget):
# 	def itemDoubleClicked(self, item: QListWidgetItem):
# 		print("hello")

class HistoryView (QtWidgets.QDockWidget):

	def __init__(self, title, context, signals, Parent=None):

		super(HistoryView, self).__init__(title, Parent)

		self.context = context
		self.signals = signals
		self.parent = Parent
		self.setAllowedAreas(Qt.RightDockWidgetArea)
		self.setFeatures(QtWidgets.QDockWidget.NoDockWidgetFeatures)
		self.listWidget=QListWidget()
		self.listWidget.setObjectName("history view")
		self.listWidget.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Maximum)
		self.listWidget.setIconSize(QtCore.QSize(128,128))
		self.listWidget.setViewMode(QtWidgets.QListView.ListMode)
		self.listWidget.setBackgroundRole(QtGui.QPalette.WindowText)
		self.setWidget(self.listWidget)
		self.signals.updateCanvas.connect(self.update)
		self.signals.historyReset.connect(self.historyClear)
		self.listWidget.itemDoubleClicked.connect(self.historyDoubleClick)

	def historyDoubleClick(self, item):
		print("替换到第"+item.text())
		self.context.images[0].changeCurrentHistoryStep(item.data(0))

	def historyClear(self):
		self.listWidget.clear()
		for i in range(1,len(self.context.images[0].history)):
			item = QListWidgetItem(QIcon(
				QPixmap(self.context.images[0].history[i]).scaled(
					self.listWidget.width(), self.listWidget.width())), str(i) + "步")
			item.setTextAlignment(Qt.AlignHCenter|Qt.AlignVCenter)
			item.setData(0,i)
			self.listWidget.addItem(item)
		self.listWidget.scrollToBottom()

	def update(self):
		super(HistoryView, self).update()