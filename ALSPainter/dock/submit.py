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

		self.checkFg = QtWidgets.QCheckBox("显示笔画", self)
		self.checkFg.setChecked(self.context.showFg)
		self.checkFg.toggled.connect(self.updateFg)

		self.checkBg = QtWidgets.QRadioButton("显示结果", self)
		self.checkBg.setChecked(self.context.showBg)
		self.checkBg.toggled.connect(self.updateBg)

		self.checkOri = QtWidgets.QRadioButton("显示原图", self)
		self.checkOri.setChecked(self.context.showBg)
		self.checkOri.toggled.connect(self.updateOri)

		self.btnGroup = QtWidgets.QButtonGroup()
		self.btnGroup.addButton(self.checkOri,0)
		self.btnGroup.addButton(self.checkBg,1)
		self.btnGroup.setExclusive(True)

		# self.context.signals.enterCanvas.connect(self.showFg)
		# self.context.signals.submitCavas.connect(self.hideFg)
		self.context.signals.showFG.connect(self.showFG)
		self.context.signals.showBG.connect(self.showBG)

		vbox.setAlignment(QtCore.Qt.AlignTop)
		vbox.addWidget(self.checkOri)
		vbox.addWidget(self.checkBg)
		vbox.addSpacing(5)
		vbox.addWidget(self.checkFg)
		w.setLayout(vbox)
		self.setWidget(w)

	def showFG(self, option):
		self.checkFg.setChecked(option)
		self.updateFg()

	def showBG(self,option):
		self.checkBg.setChecked(option)
		self.updateBg()

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

	def updateOri(self):

		super(ViewOpinion, self).update()
		if self.context.currentImage() != None:
			if(self.checkOri.isChecked()):
				self.context.showOri=True
			else:
				self.context.showOri=False
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
		self.signals.showBG.emit(True)
		self.signals.showFG.emit(False)
		self.signals.submitCavas.emit()