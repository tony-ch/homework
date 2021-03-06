#!/usr/bin/env python
#coding: utf-8

from configparser import ConfigParser, NoSectionError, NoOptionError

from window.image import Image
from tool.translation import TDatabase
from tool.brushes import *
from tool import names as Pixeler


class Context:

	def __init__(self, signals):

		self.signals = signals

		self.palette = []
		# self.defaultPalette = [
		# 	[9, 125, 3], [5, 244, 193], [89, 14, 15], [32, 160, 191]
		# ]
		self.defaultPalette = [ [247, 176, 114], [14, 53, 75], [30, 85, 55], [184, 37, 53] ]
		self.defaultPalette = [ [180,120,120],      [6,230,230],      [4,200,3] ,[80,50,50], [254,85,0], [0,0,85]]
		

		self.paletteText = ['1','2','3','4','5','6']
		# self.defaultPalette = [[14, 53, 75], [0, 76, 115], [18, 121, 174], [49, 162, 238], [136, 199, 234], [27, 52, 43],
		# 					  [30, 85, 55], [69, 145, 26], [121, 191, 29], [190, 222, 44], [69, 18, 18], [113, 31, 31],
		# 					  [184, 37, 53], [220, 81, 115], [255, 159, 182], [39, 20, 67], [105, 28, 99], [173, 81, 185],
		# 					  [184, 152, 208], [53, 48, 36], [89, 66, 40], [140, 92, 77], [208, 128, 112], [229, 145, 49],
		# 					  [247, 176, 114], [252, 215, 142], [0, 0, 0], [33, 33, 33], [79, 79, 79], [179, 179, 179],
		# 					  [255, 255, 255], [37, 42, 46], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0],
		# 					  [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0],
		# 					  [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0],
		# 					  [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0],
		# 					  [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]]

		self.imagePos = -1
		self.images = []

		self.theme = "algae"
		self.currentTool = 0

		self.clipboard = QtWidgets.QApplication.clipboard()

		self.pencilCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/penicon.png"), 2, 17)
		self.colorPickerCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/droppericon.png"), 2, 17)
		self.eraserCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/erasericon.png"), 2, 17)
		self.fillCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/fillicon.png"), 1, 14)
		self.lassoCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/lasso.png"), 3, 17)
		self.handCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/hand.png"), 10, 10)
		self.handgrabCur = QtGui.QCursor(QtGui.QPixmap("images/cursors/handgrab.png"), 10, 10)

		self.circles, self.brushes = createBrushes()

		self.loadDefaults()

	def newImage(self, width, height, bg):
		self.images.append(Image.newImage(width, height, bg, self))
		self.imagePos = len(self.images) - 1

		print("newImage")

		self.signals.newImage.emit()

	def loadImage(self, fileName):

		self.images.append(Image.fromFile(fileName, self))
		self.imagePos = len(self.images) - 1

		self.signals.newImage.emit()

	def setCurrentImagePos(self, index):

		self.imagePos = index
		self.signals.imageChanged.emit(index)

	def getCurrentImagePos(self):

		return self.imagePos

	def currentImage(self):
		if len(self.images) > 0:
			return self.images[self.imagePos]
		return None

	def currentQImage(self):

		if self.imagePos != -1:
			return self.images[self.imagePos].image
		return None

	def getImagePos(self, index):

		return self.images[index]

	def removeImagePos(self, index):

		del self.images[index]
		#self.images = self.images[:index] + self.images[index+1:]
		if self.imagePos > index:
			self.imagePos -= 1
			self.signals.imageChanged.emit(index-1)
		self.signals.imageRemoved.emit(index)

	def setTransparentSelection(self, x):

		self.transparentSelection = x
		self.signals.transparentSelection.emit(x)
		self.signals.updateCanvas.emit()

	def setPencilSize(self, size):

		if size < 31 and size > 0:
			self.pencilSize = size
			self.signals.updatePencilSize.emit(self.pencilSize)

	def setEraserSize(self, size):

		if size < 31 and size > 0:
			self.eraserSize = size
			self.signals.updateEraserSize.emit(self.eraserSize)

	def changePrimaryColor(self, c):

		self.primaryColor = c
		self.signals.updateColor.emit()

	def changeSecondaryColor(self, c):

		self.secondaryColor = c
		self.signals.updateColor.emit()

	def changeCurrentTool(self, index):

		print("Emitting updateTool")
		self.currentTool = index
		self.signals.updateTool.emit(index)
		if index==Pixeler.Tools.Eraser:
			self.signals.showBG.emit(True)
			self.signals.showFG.emit(True)

	def getText(self, sect, ident): # Get some text in the current language

		return self.tdatabase.getText(self.lang, sect, ident).replace("\\n", "\n")

	def getTextInLang(self, lang, sect, ident): # Get some text in a specific language

		return self.tdatabase.getText(lang, sect, ident)

	def setDefault(self, sect, ident, value):

		try:
			self.cp.set(sect, ident, value)
		except NoSectionError:
			print("Trying to set \"" + ident + "\" to \"" + str(value) + "\" on section \"" + sect + "\", but given section does not exist. Creating section.")
			self.cp.add_section(sect)
			self.cp.set(sect, ident, value)

		f = open("defaults.cfg", "w")
		self.cp.write(f)
		f.close()

	def getDefault(self, sect, ident, default):

		try:
			return self.cp.get(sect, ident)
		except NoSectionError:
			print("Trying to get value from option \"" + ident + "\" on section \"" + sect + "\", but no section with that name exists. Returning default value.")
			return default
		except NoOptionError:
			print("Trying to get value from option \"" + ident + "\" on section \"" + sect + "\", but specified option does not exist within that section. Returning default value.")
			return default

	def getBoolDefault(self, sect, ident, default):

		try:
			return self.cp.getboolean(sect, ident)
		except ValueError:
			print("Trying to get boolean value from option \"" + ident + "\" on section \"" + sect + "\", but given option value is not boolean.")
		except NoSectionError:
			print("Trying to get boolean value from option \"" + ident + "\" on section \"" + sect + "\", but no section with that name exists. Returning default value.")
			return default
		except NoOptionError:
			print("Trying to get boolean value from option \"" + ident + "\" on section \"" + sect + "\", but specified option does not exist within that section. Returning default value.")
			return default

	def getIntDefault(self, sect, ident, default):

		try:
			return self.cp.getint(sect, ident)
		except ValueError:
			print("Trying to get integer value from option \"" + ident + "\" on section \"" + sect + "\", but given option value is not an integer.")
		except NoSectionError:
			print("Trying to get integer value from option \"" + ident + "\" on section \"" + sect + "\", but no section with that name exists. Returning default value.")
			return default
		except NoOptionError:
			print("Trying to get integer value from option \"" + ident + "\" on section \"" + sect + "\", but specified option does not exist within that section. Returning default value.")
			return default

	def getFloatDefault(self, sect, ident, default):

		try:
			return self.cp.getfloat(sect, ident)
		except ValueError:
			print("Trying to get float value from option \"" + ident + "\" on section \"" + sect + "\", but given option value is not a floating point number.")
		except NoSectionError:
			print("Trying to get float value from option \"" + ident + "\" on section \"" + sect + "\", but no section with that name exists. Returning default value.")
			return default
		except NoOptionError:
			print("Trying to get float value from option \"" + ident + "\" on section \"" + sect + "\", but specified option does not exist within that section. Returning default value.")
			return default

	def loadDefaults(self):

		self.cp = ConfigParser()
		self.cp.read("defaults.cfg")

		self.loadDefaultsPalette()
		self.loadDefaultsLanguage()
		self.loadDefaultsGrid()
		self.loadDefaultsColor()
		self.loadDefaultsTheme()
		self.loadDefaultsSelection()
		self.loadDefaultsPencil()
		self.loadDefaultsEraser()
		self.showFg = True
		self.showBg = True
		self.showOri = False
		self.para_mode = 'cg_mg'
		self.para_beta = 130
		self.para_alpha = 0.8
		self.para_usegray = False

	def loadDefaultsPalette(self):

		try:
			f = open("palette.cfg", 'r')
		except IOError:
			print("Cannot open palette.cfg, falling back to default palette.")
			self.palette = self.defaultPalette
			return

		l = f.readlines()
		for i in l:
			colors = i[:-1].split(' ')
			red = int(colors[0])
			green = int(colors[1])
			blue = int(colors[2])
			self.palette.append([red, green, blue])
		f.close()

	def loadDefaultsLanguage(self):

		self.tdatabase = TDatabase()
		lang = self.getDefault("language", "lang", "en")
		if lang in self.tdatabase.langAvailable:
			self.lang = lang
		else:
			self.lang = "en"

	def loadDefaultsGrid(self):

		self.grid = self.getBoolDefault("grid", "grid", False)
		self.matrixGrid = self.getBoolDefault("grid", "matrix_grid", False)
		self.matrixGridWidth = self.getIntDefault("grid", "matrix_grid_width", 16)
		self.matrixGridHeight = self.getIntDefault("grid", "matrix_grid_height", 16)

	def loadDefaultsColor(self):

		color1 = self.defaultPalette[0]
		color0 = self.defaultPalette[-1]
		self.primaryColor = QtGui.QColor(self.getIntDefault("color", "primary_color", QtGui.QColor(color1[0], color1[1], color1[2])))
		self.secondaryColor = QtGui.QColor(self.getIntDefault("color", "secondary_color",QtGui.QColor(color0[0], color0[1], color0[2])))

	def loadDefaultsTheme(self):

		self.theme = self.getDefault("theme", "theme", "algae")

	def loadDefaultsSelection(self):

		self.transparentSelection = self.getBoolDefault("selection", "transparent", True)

	def loadDefaultsPencil(self):

		self.pencilSize = self.getIntDefault("pencil", "size", 5)
		self.secondaryColorEraser = self.getBoolDefault("pencil", "secondary_color_eraser", False)

	def loadDefaultsEraser(self):

		self.eraserSize = self.getIntDefault("eraser", "size", 5)

	def saveDefaults(self):
		return

		# self.setDefault("color", "primary_color", self.primaryColor.rgb())
		# self.setDefault("color", "secondary_color", self.secondaryColor.rgb())
		#
		# self.setDefault("selection", "transparent", self.transparentSelection)
		# self.setDefault("pencil", "size", self.pencilSize)
		# self.setDefault("pencil", "secondary_color_eraser", self.secondaryColorEraser)
		# self.setDefault("eraser", "size", self.eraserSize)
		#
		# self.savePalette()

	def savePalette(self):

		f = open("palette.cfg", 'w')
		for i in self.palette:
			f.write(str(i[0]) + " " + str(i[1]) + " " + str(i[2]) + "\n")
		f.close()
