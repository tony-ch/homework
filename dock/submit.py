#!/usr/bin/env python
#coding: utf-8

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import Qt

class ViewOpinion (QtWidgets.QDockWidget):

	def __init__(self, title, context, signals, Parent=None):

		super(ViewOpinion, self).__init__(title, Parent)

		self.context = context
		self.signals = signals
		self.parent = Parent
		self.setAllowedAreas(Qt.RightDockWidgetArea)
		self.setFeatures(QtWidgets.QDockWidget.NoDockWidgetFeatures)

		w = QtWidgets.QWidget()
		w.setObjectName("ToolProperties")
		w.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
		vbox = QtWidgets.QVBoxLayout()

		self.checkFg = QtWidgets.QCheckBox("显示前景", self)
		self.checkFg.setChecked(self.context.showFg)
		self.checkFg.toggled.connect(self.updateFg)

		self.checkBg = QtWidgets.QCheckBox("显示背景", self)
		self.checkBg.setChecked(self.context.showBg)
		self.checkBg.toggled.connect(self.updateBg)

		# self.context.signals.enterCanvas.connect(self.showFg)
		# self.context.signals.submitCavas.connect(self.hideFg)
		self.context.signals.updateViewOpition.connect(self.updateOpition)

		vbox.setAlignment(QtCore.Qt.AlignTop)
		vbox.addWidget(self.checkBg)
		vbox.addWidget(self.checkFg)
		w.setLayout(vbox)
		self.setWidget(w)

	def updateOpition(self, option):
		self.checkFg.setChecked(option)
		self.updateFg()

	def updateFg(self):

		super(ViewOpinion, self).update()
		if self.context.currentImage() != None:
			if(self.checkFg.isChecked()):
				self.context.showFg=True
			else:
				self.context.showFg=False
			self.signals.updateCanvas.emit()

	def updateBg(self):

		super(ViewOpinion, self).update()
		if self.context.currentImage() != None:
			if(self.checkBg.isChecked()):
				self.context.showBg=True
			else:
				self.context.showBg=False
			self.signals.updateCanvas.emit()


class Submit (QtWidgets.QDockWidget):

	def __init__(self, title, context, signals, Parent=None):

		super(Submit, self).__init__(title, Parent)

		self.context = context
		self.signals = signals
		self.parent = Parent
		self.setAllowedAreas(Qt.RightDockWidgetArea)
		self.setFeatures(QtWidgets.QDockWidget.NoDockWidgetFeatures)

		w = QtWidgets.QWidget()
		w.setObjectName("ToolProperties")
		w.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
		hbox = QtWidgets.QHBoxLayout()

		self.reset = QtWidgets.QPushButton("重置", self)
		self.reset.clicked.connect(self.reset_image)
		self.reset.setToolTip("重置前景背景和操作历史")
		self.submit = QtWidgets.QPushButton("提交", self)
		self.submit.setToolTip("提交纹理更改，查看结果")
		self.submit.clicked.connect(self.submit_image)

		hbox.setAlignment(QtCore.Qt.AlignTop)
		hbox.addWidget(self.reset)
		hbox.addWidget(self.submit)
		w.setLayout(hbox)
		self.setWidget(w)


	def reset_image(self):
		self.signals.resetCavas.emit()

	def submit_image(self):
		# self.parent.viewOpinion.checkFg.setChecked(False)
		self.signals.updateViewOpition.emit(False)
		self.signals.submitCavas.emit()