#!/usr/bin/env python
#coding: utf-8

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import Qt
from solver.random_walker import RandomWalker

class Image:

	def __init__(self, fileName, image: QtGui.QImage, bg, context, bg_image=None):

		self.fileName = fileName
		self.image = image
		if bg_image is None:
			self.bg_image = QtGui.QImage(image.width(), image.height(), QtGui.QImage.Format_ARGB32_Premultiplied)
		else:
			self.bg_image = bg_image
		self.ori_bg_img = self.bg_image
		self.bgColor = bg
		self.context = context
		self.zoom = 1
		self.selection = None
		self.history = [QtGui.QImage(self.image)]
		self.posHistory = 0
		self.modified = False
		self.random_walker = RandomWalker(palette=self.context.defaultPalette)

	def reset(self):
		print("reset")
		w = self.image.width()
		h = self.image.height()
		self.image = QtGui.QImage(w, h, QtGui.QImage.Format_ARGB32_Premultiplied)
		self.image.fill(self.bgColor)
		self.history.clear()
		self.history.append(QtGui.QImage(self.image))
		self.posHistory = 0
		self.modified=False
		# self.selection=None
		self.bg_image=self.ori_bg_img
		self.context.signals.historyReset.emit()
		

	def submit(self):
		print("submit")
		use_gray = self.context.para_usegray
		beta = self.context.para_beta
		mode = self.context.para_mode
		alpha = self.context.para_alpha
		print("mode:",mode,"beta:",beta,"use gray:",use_gray,"alpha:",alpha)
		res = self.random_walker.solve(self.ori_bg_img, self.image, 'QImage', use_gray, mode, beta, alpha)
		if res is not None:
			self.bg_image=res

	@classmethod
	def fromFile(cls, fileName, context):
		bg_image = QtGui.QImage(fileName[0]).convertToFormat(QtGui.QImage.Format_ARGB32_Premultiplied)
		w = bg_image.width()
		h = bg_image.height()

		if bg_image.hasAlphaChannel():
			bgColor = QtGui.QColor(0,0,0,0)
		else:
			bgColor = QtGui.QColor(255,255,255)

		image = QtGui.QImage(w, h, QtGui.QImage.Format_ARGB32_Premultiplied)
		image.fill(bgColor)
		
		return cls(fileName, image, bgColor, context, bg_image)

	@classmethod
	def newImage(cls, w, h, bg, context):

		image = QtGui.QImage(w, h, QtGui.QImage.Format_ARGB32_Premultiplied)
		image.fill(bg)

		return cls("", image, bg, context)

	def save(self):
		self.bg_image.save(self.fileName)
		self.modified = False
	def addHistoryStep(self):

		if self.posHistory != len(self.history)-1:
			self.history = self.history[:self.posHistory+1]
		self.history.append(QtGui.QImage(self.image))
		
		self.posHistory += 1
		self.modified = True
		#add by lyc
		self.context.signals.historyReset.emit()

	#add by lyc
	def changeCurrentHistoryStep(self,target:int):
		self.posHistory = target
		self.image=QtGui.QImage(self.history[self.posHistory])
		self.context.signals.updateCanvas.emit()
		self.context.signals.resizeCanvas.emit()
	def paintPoint(self, x, y, color, size):

		painter = QtGui.QPainter(self.image)
		painter.setPen(color)
		painter.setCompositionMode(QtGui.QPainter.CompositionMode_Source)
		painter.drawPixmap(QtCore.QPoint(x-size+1, y-size+1), self.context.brushes[size-1])