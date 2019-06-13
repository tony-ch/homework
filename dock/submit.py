#!/usr/bin/env python
#coding: utf-8

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import Qt

class ParaLabel (QtWidgets.QLabel):
	def setValue(self, value):
		self.setText(str(value))

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

class ParaOpinion (QtWidgets.QDockWidget):

	def __init__(self, title, context, signals, Parent=None):

		super(ParaOpinion, self).__init__(title, Parent)

		self.context = context
		self.signals = signals
		self.parent = Parent
		self.setAllowedAreas(Qt.RightDockWidgetArea)
		self.setFeatures(QtWidgets.QDockWidget.NoDockWidgetFeatures)

		w = QtWidgets.QWidget()
		w.setObjectName("ParaProperties")
		w.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
		vbox = QtWidgets.QVBoxLayout()

		self.checkGray = QtWidgets.QCheckBox("使用灰度图", self)
		self.checkGray.setChecked(self.context.para_usegray)
		self.checkGray.toggled.connect(self.update_para)

		self.checkMode_cgmg = QtWidgets.QRadioButton("cg_mg", self)
		self.checkMode_cgmg.setChecked(True)
		self.checkMode_cgmg.toggled.connect(self.update_para)

		self.checkMode_cg = QtWidgets.QRadioButton("cg", self)
		self.checkMode_cg.setChecked(False)
		self.checkMode_cg.toggled.connect(self.update_para)

		self.checkMode_bf = QtWidgets.QRadioButton("bf", self)
		self.checkMode_bf.setChecked(False)
		self.checkMode_bf.toggled.connect(self.update_para)

		btnGroup = QtWidgets.QButtonGroup()
		btnGroup.addButton(self.checkMode_cgmg,0)
		btnGroup.addButton(self.checkMode_cg,1)
		btnGroup.addButton(self.checkMode_bf,2)
		btnGroup.setExclusive(True)

		self.slider = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
		self.slider.setMaximum(1000)
		self.slider.setMinimum(1)
		self.slider.setPageStep(1)
		self.slider.setValue(self.context.para_beta)
		self.slider.valueChanged.connect(self.update_para)
		self.paraBetaLabel = ParaLabel()
		self.paraBetaLabel.setValue(self.context.para_beta)
		hbox = QtWidgets.QHBoxLayout()
		hbox.addWidget(QtWidgets.QLabel("beta:"))
		hbox.addWidget(self.slider)
		hbox.addWidget(self.paraBetaLabel)

		self.slider_alpha = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
		self.slider_alpha.setMaximum(100)
		self.slider_alpha.setMinimum(0)
		self.slider_alpha.setPageStep(5)
		self.slider_alpha.setValue(self.context.para_alpha * 100)
		self.slider_alpha.valueChanged.connect(self.update_para)
		self.paraAlphaLabel = ParaLabel()
		self.paraAlphaLabel.setValue(self.context.para_alpha)
		hbox_alpha = QtWidgets.QHBoxLayout()
		hbox_alpha.addWidget(QtWidgets.QLabel("透明度:"))
		hbox_alpha.addWidget(self.slider_alpha)
		hbox_alpha.addWidget(self.paraAlphaLabel)

		vbox.setAlignment(QtCore.Qt.AlignTop)
		vbox.addLayout(hbox)
		vbox.addLayout(hbox_alpha)
		vbox.addSpacing(5)
		vbox.addWidget(self.checkGray)
		vbox.addSpacing(5)
		vbox.addWidget(self.checkMode_cgmg)
		vbox.addWidget(self.checkMode_cg)
		vbox.addWidget(self.checkMode_bf)
		w.setLayout(vbox)
		self.setWidget(w)

	def update_para(self):
		super(ParaOpinion, self).update()
		self.context.para_usegray = self.checkGray.isChecked()
		
		if self.checkMode_cgmg.isChecked():
			self.context.para_mode = 'cg_mg'
		elif self.checkMode_cg.isChecked():
			self.context.para_mode = 'cg'
		else:
			self.context.para_mode = 'bf'
		
		self.context.para_beta = self.slider.value()
		self.paraBetaLabel.setValue(self.slider.value())
		self.context.para_alpha = self.slider_alpha.value()/100
		self.paraAlphaLabel.setValue(self.slider_alpha.value()/100)


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