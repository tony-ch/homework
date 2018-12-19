#!/usr/bin/env python
#coding: utf-8

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import Qt
from audio_2_str import *

class VoiceControl (QtWidgets.QDockWidget):

	def __init__(self, title, context, signals, Parent=None):

		super(VoiceControl, self).__init__(title, Parent)

		self.context = context
		self.signals = signals
		self.parent = Parent
		self.setAllowedAreas(Qt.RightDockWidgetArea)
		self.setFeatures(QtWidgets.QDockWidget.NoDockWidgetFeatures)

		self.button=QtWidgets.QPushButton("录音")
		self.setWidget(self.button)
		self.update()
		self.button.clicked.connect(self.voiceRecord)
		self.signals.updateCanvas.connect(self.update)

	def voiceRecord(self,checked:bool):
		list=audio_2_str()
		print(list)
		if list[0]=="撤销":
			self.parent.undo()
		elif list[0]=="重做":
			self.parent.redo()
		return

	def update(self):
		super(VoiceControl, self).update()

