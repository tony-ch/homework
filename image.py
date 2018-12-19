#!/usr/bin/env python
#coding: utf-8

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import Qt
from client import GANPaint

class Image:

	def __init__(self, fileName, image: QtGui.QImage, bg, context):

		self.fileName = fileName
		self.image = image
		self.bg_image = QtGui.QImage(image.width(), image.height(), QtGui.QImage.Format_ARGB32_Premultiplied)
		self.ori_bg_img = self.bg_image
		self.id=None
		self.bgColor = bg
		self.context = context

		self.zoom = 1
		self.selection = None
		self.history = [QtGui.QImage(self.image)]
		self.posHistory = 0
		self.modified = False
		self.ganPaintClient=GANPaint(defaultPalette=self.context.defaultPalette)

	def reset(self):
		print("reset")
		self.image.fill(self.bgColor)
		self.history.clear()
		self.history.append(QtGui.QImage(self.image))
		self.posHistory = 0
		self.modified=False
		self.selection=None
		self.bg_image=self.ori_bg_img

	def submit(self):
		if self.id is not None:
			print("submit")
			res = self.ganPaintClient.upload(self.id,operations=None,qImageOp=self.image)
			self.bg_image=res

	@classmethod
	def fromFile(cls, fileName, context):
		image = QtGui.QImage(fileName[0]).convertToFormat(QtGui.QImage.Format_ARGB32_Premultiplied)

		if image.hasAlphaChannel():
			bgColor = QtGui.QColor(0,0,0,0)
		else:
			bgColor = QtGui.QColor(255,255,255)

		return cls(fileName, image, bgColor, context)

	@classmethod
	def newImage(cls, w, h, bg, context):

		image = QtGui.QImage(w, h, QtGui.QImage.Format_ARGB32_Premultiplied)
		image.fill(bg)

		return cls("", image, bg, context)

	def loadDemoFromFile(self, fileName):
		# self.image.fill(QtGui.QColor(0, 0, 0, 0))
		# self.bg_image =
		print("filename",fileName)
		if(fileName is None or (fileName[0])!=0):
			self.id = int(fileName[0].split("/")[-1].split(".")[0])
			print("open: image ", self.id)
			self.ori_bg_img = QtGui.QImage(fileName[0]).convertToFormat(QtGui.QImage.Format_ARGB32_Premultiplied)
			# self.bg_image = self.ori_bg_img
			self.reset()
			# self.context.signals.bgImageChanged.emit()

	def save(self):
		self.bg_image.save(self.fileName)
		self.modified = False

	def addHistoryStep(self):

		if self.posHistory != len(self.history)-1:
			self.history = self.history[:self.posHistory+1]
		self.history.append(QtGui.QImage(self.image))
		
		self.posHistory += 1
		self.modified = True

	def paintPoint(self, x, y, color, size):

		painter = QtGui.QPainter(self.image)
		painter.setPen(color)
		painter.setCompositionMode(QtGui.QPainter.CompositionMode_Source)
		painter.drawPixmap(QtCore.QPoint(x-size+1, y-size+1), self.context.brushes[size-1])

	def createMaskFromArea(self, point, threshold=0):

		mask = self.recursiveFill(QtGui.QImage(self.image), point, self.image.pixel(point))
		return mask

	"""
	def recursiveFill(self, mask, point, threshold=0, color=None):

		x, y = point.x(), point.y()

		# The recursive algorithm. Starting at x and y, changes any adjacent
		# characters that match oldChar to newChar.
		width = mask.width()
		height = mask.height()

		if color == None:
			color = mask.pixel(x,y)

		if mask.pixel(x,y) != color:
			# Base case. If the current x, y character is not the oldChar,
			# then do nothing.
			return

		# Change the character at world[x][y] to newChar
		mask.setPixel(x, y, QtCore.Qt.color0)

		# Recursive calls. Make a recursive call as long as we are not on the
		# boundary (which would cause an Index Error.)
		if x > 0: # left
			self.recursiveFill(mask, QtCore.QPoint(x-1, y))

		if y > 0: # up
			self.recursiveFill(mask, QtCore.QPoint(x, y-1))

		if x < width-1: # right
			self.recursiveFill(mask, QtCore.QPoint(x+1, y))

		if y < height-1: # down
			self.recursiveFill(mask, QtCore.QPoint(x, y+1))
	"""

	def recursiveFill(self, mask:QtGui.QImage, point, color, threshold=0):

		x, y = point.x(), point.y()

		if x < 0 or y < 0 or x > mask.width()-1 or y > mask.height()-1:
			return

		elif mask.pixel(x,y) == color:
			mask.setPixel(x, y, QtCore.Qt.color0)
			self.recursiveFill( mask, QtCore.QPoint(x+1, y), color)
			self.recursiveFill( mask, QtCore.QPoint(x, y+1), color)
			self.recursiveFill( mask, QtCore.QPoint(x-1, y), color)
			self.recursiveFill( mask, QtCore.QPoint(x, y-1), color)
