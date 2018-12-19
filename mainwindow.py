#!/usr/bin/env python
#coding: utf-8

import os

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import Qt

import names as Pixeler
from mainwidget import MainWidget
from palette import Palette
from toolproperties import ToolProperties
#from preview import Preview
from dialogs import NewFileDialog, ResizeImageDialog, ResizeCanvasDialog, Preferences
from submit import ViewOpinion,Submit
from historyView import HistoryView
from voiceControl import VoiceControl

class MainWindow(QtWidgets.QMainWindow):

	def __init__(self, context, signals):

		super(MainWindow, self).__init__()

		self.signals = signals
		self.context = context

		self.resize(1000,600)
		self.setWindowTitle(self.context.getText("pyqx", "title"))

		self.statusBar = self.statusBar()
		# self.menuBar = self.createMenuBar()
		self.toolBar = self.createToolBar()
		self.createDockWidgets()

		self.ctrlPressed = False

		self.mainWidget = MainWidget(context, signals, self)
		self.setCentralWidget(self.mainWidget)

		self.imagePosLabel = QtWidgets.QLabel()
		self.imagePosLabel.setObjectName("ImagePosLabel")

		self.signals.autoUpdateTool.connect(self.setCurrentTool)
		self.signals.enterCanvas.connect(self.showImagePosition)
		self.signals.leaveCanvas.connect(self.hideImagePosition)
		self.signals.overCanvas.connect(self.setImagePosition)

		self.show()
		self.context.newImage(256, 256, QtGui.QColor(0, 0, 0, 0))
		self.context.changeCurrentTool(Pixeler.Tools.Pencil)
		self.context.currentImage().loadDemoFromFile(['demoimg/700.jpg'])
		self.signals.updateCanvas.emit()

	def createToolBarActions(self):

		l = []

		self.tools = QtWidgets.QActionGroup(self)

		a = QtWidgets.QAction(QtGui.QIcon(os.path.join("themes", self.context.theme, "open.png")),
							  '打开', self.tools)
		a.setShortcut("Ctrl+O")
		a.triggered.connect(self.openDemoImg)
		l.append(a)

		a = QtWidgets.QAction(QtGui.QIcon(os.path.join("themes", self.context.theme, "save.png")),
							  '保存', self.tools)
		a.setShortcut("Ctrl+S")
		a.triggered.connect(self.saveResultImg)
		l.append(a)

		tools = ["selection", "magicwand", "pencil", "eraser", "colorpicker", "fill", "gradient", "exchange", "lasso", "hand"]
		connects = [lambda: self.context.changeCurrentTool(Pixeler.Tools.Selection),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.MagicWand),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Pencil),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Eraser),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.ColorPicker),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Fill),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Gradient),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Exchange),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Lasso),
					lambda: self.context.changeCurrentTool(Pixeler.Tools.Hand),
					]
		shortcuts = ['Z', '', 'X', 'C', 'A', 'S', 'D', '', 'L', 'H']

		for i in range(len(tools)):
			a = QtWidgets.QAction(QtGui.QIcon( os.path.join("themes", self.context.theme, tools[i] + ".png") ), self.context.getText("tools", tools[i]) + " (" + shortcuts[i] + ")", self.tools)
			a.setCheckable(True)
			if tools[i] == 'pencil':
				a.setChecked(True)
			a.setShortcut(shortcuts[i])
			if connects[i] != 0: a.toggled.connect(connects[i])
			l.append(a)

		a = QtWidgets.QAction(QtGui.QIcon( os.path.join("themes", self.context.theme, "zoomin.png") ), self.context.getText("tools", "zoomin"), self.tools)
		a.setShortcut("Ctrl++")
		a.triggered.connect(self.zoomIn)
		l.append(a)

		a = QtWidgets.QAction(QtGui.QIcon( os.path.join("themes", self.context.theme, "zoomout.png") ), self.context.getText("tools", "zoomout"), self.tools)
		a.setShortcut("Ctrl+-")
		a.triggered.connect(self.zoomOut)
		l.append(a)

		l[self.context.currentTool].setChecked(True)

		return l

	def createToolBar(self):

		toolBar = QtWidgets.QToolBar()
		l = self.createToolBarActions()

		j = 0
		for i in l:
			if j in [2, 3, 6, 7, 8, 9, 10]:
				j += 1
				continue
			toolBar.addAction(i)
			if j==1 or j == 11:
				toolBar.addSeparator()
			j += 1

		toolBar.setMovable(False)
		toolBar.setOrientation(Qt.Vertical)
		self.addToolBar(Qt.LeftToolBarArea, toolBar)

		return toolBar

	def createDockWidgets(self):

		# Palette widget
		self.palette = QtWidgets.QDockWidget(self.context.getText("dock_widgets", "palette"), self)
		self.palette.setAllowedAreas(Qt.RightDockWidgetArea)
		self.palette.setFeatures(QtWidgets.QDockWidget.NoDockWidgetFeatures)

		paletteWidget = Palette(self.context, self.signals)

		self.palette.setWidget(paletteWidget)
		self.palette.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)

		self.addDockWidget(Qt.RightDockWidgetArea, self.palette)

		# Tool Properties widget
		self.toolProperties = ToolProperties(self.context.getText("dock_widgets", "tool_properties"), self.context, self.signals)
		self.toolProperties.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
		self.addDockWidget(Qt.RightDockWidgetArea, self.toolProperties)

		# View Opinion Widget
		self.viewOpinion = ViewOpinion("查看选项",self.context,self.signals)
		self.viewOpinion.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
		self.addDockWidget(Qt.RightDockWidgetArea,self.viewOpinion)

		# Submit
		self.submit = Submit("", self.context, self.signals,self)
		self.addDockWidget(Qt.RightDockWidgetArea, self.submit)
		self.submit.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)

		#comment by lyc
		# Preview
		#self.preview = Preview(self.context.getText("dock_widgets", "preview"), self.context, self.signals, self)
		#self.addDockWidget(Qt.RightDockWidgetArea, self.preview)

		#add by lyc
		#HisotryView
		self.historyView=HistoryView("操作历史", self.context, self.signals, self)
		#self.historyView.setStyleSheet("border: 2px solid black;")
		self.addDockWidget(Qt.RightDockWidgetArea,self.historyView)
		self.historyView.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)

		# add by lyc
		# HisotryView
		self.voiceControl = VoiceControl("请点击按钮进行语音控制", self.context, self.signals, self)
		self.addDockWidget(Qt.RightDockWidgetArea, self.voiceControl)
		self.voiceControl.setSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)

	def restoreFocus(self):

		print("Restoring Focus")
		self.ctrlPressed = False
		self.releaseMouse()
		self.releaseKeyboard()
		QtCore.QCoreApplication.instance().restoreOverrideCursor()

	def setCurrentTool(self, index):
		self.tools.actions()[0].setChecked(True)
		self.signals.updateTool.emit(0)

	def zoomIn(self):

		if self.context.currentImage().zoom < 25:
			self.context.currentImage().zoom += 1
			self.signals.zoom.emit()

	def zoomOut(self):

		if self.context.currentImage().zoom > 1:
			self.context.currentImage().zoom -= 1
			self.signals.zoom.emit()

	def saveFile(self):
		self.saveFileAs()

	def saveFileAs(self):

		d = QtWidgets.QFileDialog()
		fileName, filterName = d.getSaveFileName(self,
					self.context.getText("dialog_save", "title"),
					"",
					"*.bmp;;*.gif;;*.png;;*.xpm;;*.jpg")

		if fileName.split(".")[-1] in ["bmp", "gif", "png", "xpm", "jpg"]:
			self.context.currentImage().fileName = fileName
			self.signals.fileNameChanged.emit(self.context.getCurrentImagePos(), os.path.basename(str(fileName)))
		else:
			self.context.currentImage().fileName = fileName + filterName[1:]
			self.signals.fileNameChanged.emit(self.context.getCurrentImagePos(), os.path.basename(str(fileName + filterName[1:])))
		self.context.currentImage().save()


	def openDemoImg(self):
		filename = QtWidgets.QFileDialog.getOpenFileName(self,
					'选择一张图片',
					"./demoimg",
					self.context.getText("dialog_open", "images") + u" (*.bmp *.gif *.png *.xpm *.jpg);;" + self.context.getText("dialog_open", "all_files") + u" (*)")
		if(filename):
			self.context.currentImage().loadDemoFromFile(filename)
		self.signals.updateCanvas.emit()

	def saveResultImg(self):
		self.saveFile()


	def close(self):

		pass

	def undo(self):
		# modified by lyc
		# return
		if self.context.currentImage().posHistory > 0:
			print(self.context.currentImage().posHistory)
			self.context.currentImage().posHistory -= 1
			self.context.currentImage().image = QtGui.QImage(self.context.currentImage().history[self.context.currentImage().posHistory])
			self.signals.updateCanvas.emit()
			self.signals.resizeCanvas.emit()

	def redo(self):
		# modified by lyc
		# return
		if self.context.currentImage().posHistory < len(self.context.currentImage().history)-1:
			print(self.context.currentImage().posHistory)
			self.context.currentImage().posHistory += 1
			self.context.currentImage().image = QtGui.QImage(self.context.currentImage().history[self.context.currentImage().posHistory])
			self.signals.updateCanvas.emit()
			self.signals.resizeCanvas.emit()

	def setPixelGrid(self):

		self.context.grid = not self.context.grid
		self.signals.updateCanvas.emit()
		self.context.setDefault("grid", "grid", self.context.grid)

	def setMatrixGrid(self):

		self.context.matrixGrid = not self.context.matrixGrid
		self.signals.updateCanvas.emit()
		self.context.setDefault("grid", "matrix_grid", self.context.matrixGrid)

	def showImagePosition(self):

		if self.imagePosLabel.isHidden():
			self.statusBar.insertWidget(0, self.imagePosLabel, 0)
			self.imagePosLabel.show()

	def hideImagePosition(self):

		self.statusBar.removeWidget(self.imagePosLabel)

	def setImagePosition(self, x, y):

		self.imagePosLabel.setText("  Pos: (" + str(x) + ", " + str(y) + ")")

	def keyPressEvent(self, event):

		super(MainWindow, self).keyPressEvent(event)

		if event.key() == Qt.Key_Control:
			print("Control Pressed")
			self.ctrlPressed = True
			QtCore.QCoreApplication.instance().setOverrideCursor(self.context.colorPickerCur)
			self.signals.ctrlPressed.emit()
			# self.grabMouse()
			self.grabKeyboard()

		elif event.key() == Qt.Key_Plus:
			if self.context.currentTool == 1:
				self.context.setPencilSize(self.context.pencilSize+1)
			elif self.context.currentTool == 2:
				self.context.setEraserSize(self.context.eraserSize+1)

		elif event.key() == Qt.Key_Minus:
			if self.context.currentTool == 1:
				self.context.setPencilSize(self.context.pencilSize-1)
			elif self.context.currentTool == 2:
				self.context.setEraserSize(self.context.eraserSize-1)

		else:
			QtCore.QCoreApplication.instance().restoreOverrideCursor()
			self.releaseMouse()
			self.releaseKeyboard()

	def keyReleaseEvent(self, event):

		super(MainWindow, self).keyReleaseEvent(event)

		if event.key() == Qt.Key_Control:
			self.ctrlPressed = False
			QtCore.QCoreApplication.instance().restoreOverrideCursor()
			self.releaseMouse()
			self.releaseKeyboard()

	def wheelEvent(self, event):

		if self.ctrlPressed:
			if event.angleDelta().y() > 0:
				self.zoomIn()
			else:
				self.zoomOut()

		super(MainWindow, self).wheelEvent(event)

	def closeEvent(self, event):

		l = []
		for i in range(len(self.context.images)):
			if self.context.images[i].modified: l.append(i)

		if len(l) > 0:
			# Mostrar diálogo
			reply = QtWidgets.QMessageBox.warning(self, self.context.getText("dialog_exit", "title"),
				self.context.getText("dialog_exit", "message"),
				QtWidgets.QMessageBox.SaveAll | QtWidgets.QMessageBox.Discard | QtWidgets.QMessageBox.Cancel,
				QtWidgets.QMessageBox.Cancel)
			if reply == QtWidgets.QMessageBox.Discard:
				event.accept()
			elif reply == QtWidgets.QMessageBox.Cancel:
				event.ignore()
				return
			elif reply == QtWidgets.QMessageBox.SaveAll:
				for i in l:
					self.mainWidget.setCurrentIndex(i)
					self.context.setCurrentImagePos(i)
					self.saveFile()
				event.accept()
				return



		self.context.saveDefaults()

		super(MainWindow, self).closeEvent(event)
