#!/usr/bin/env python
#-*- coding:utf-8 -*-
import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
from PIL import Image
import io
from PIL.ImageQt import ImageQt
from PyQt5.QtGui import QImage
from PyQt5.QtCore import QBuffer, QIODevice

from skimage import data
import skimage.segmentation as seg
import skimage.filters as filters
import skimage.draw as draw
import skimage.color as color

class RandomWalker(object):
    def __init__(self, palette=None):
        self.palette = palette
        palette = [ [ color[0], color[1], color[2], 255] for color in palette]
        self.colormap = np.array(palette).astype(np.uint8)

    def solve(self, image, imageLabel, format = 'QImage', use_gray=False, mode='cg_mg', beta =130, alpha = 0.75):
        assert format == 'NP' or format == 'QImage'
        if format == 'QImage':
            image = self.PILtoNP(self.QImagetoPIL(image))
            imageLabel = self._interpreteLabel(imageLabel)
        ori_img = image
        if use_gray:
            image = color.rgb2gray(image)
        image_segmented = seg.random_walker(image, imageLabel, beta=beta, multichannel=not use_gray, mode=mode)
        colored_segmented = self.colormap[image_segmented-1]
        composited = ori_img * alpha + colored_segmented * (1-alpha)
        return self.PILtoQImage(self.NPtoPIL(composited.astype(np.uint8)))

    def _interpreteLabel(self, qImageLabel):
        pil_img = self.QImagetoPIL(qImageLabel)
        np_img = self.PILtoNP(pil_img)
        assert self.palette is not None
        label = np.zeros(np_img.shape[:2],dtype=np.uint8)
        for i, color in enumerate(self.palette):
            color = [color[0],color[1],color[2],255]
            print(color)
            idx = (np_img==color).all(axis=-1)
            label[idx]=(i+1)
        return label
    
    def CVtoPIL(self, cv_img):
        return Image.fromarray(cv.cvtColor(cv_img,cv.COLOR_BGR2RGB)) 
    
    def PILtoCV(self, pil_img):
        return cv.cvtColor(np.asarray(pil_img),cv.COLOR_RGB2BGR)

    def PILtoNP(self, pil_img):
        return np.array(pil_img)

    def NPtoPIL(self, np_img):
        return Image.fromarray(np_img)

    def PILtoQImage(self, pil_img):
        qimage = ImageQt(pil_img)
        return qimage

    def QImagetoPIL(self,qimage):
        buffer = QBuffer()
        buffer.open(QIODevice.ReadWrite)
        qimage.save(buffer, "PNG")

        byteio = io.BytesIO()
        byteio.write(buffer.data())
        buffer.close()
        byteio.seek(0)
        pil_im = Image.open(byteio)
        return pil_im