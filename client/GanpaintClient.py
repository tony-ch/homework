#!/usr/bin/env python
#-*- coding:utf-8 -*-
import json
import requests
import base64
import numpy as np
import cv2 as cv
from PyQt5.QtCore import QBuffer, QIODevice
from matplotlib import pyplot as plt
from PIL import Image
import io
from PIL.ImageQt import ImageQt
from PyQt5.QtGui import QImage

class GANPaint(object):
    def __init__(self, preset_ablation='client/preset_ops.json', url = "http://gandissect.res.ibm.com/api/generate", defaultPalette=None):
        with open(preset_ablation,'r') as f:
            self.preset_ablation = json.load(f)
        self.url = url
        self.mask_shape=(32,32,4)
        if(defaultPalette is not None):
            defaultOp = ['add_tree', 'add_grass', 'add_door', 'add_sky', 'add_cloud', 'add_brick', 'add_dome']
            self.defaultPalette = defaultPalette
            self.defaultOp = defaultOp

    def upload(self, id, operations, project="churchoutdoor", format = 'QImage', qImageOp=None):
        """
        operations: list of operation, [ op1, op2, ...]
        operation: {['mask']: np_img data 32x32, ['op']: op_name}
        op_name: 'add_tree','remove tree', ...

        if operations is None, qimage mask should be provided
        """
        assert operations is not None or qImageOp is not None
        if(operations is None):
            operations=self._interpreteOp(qImageOp)
        interventions = self._intervent(operations)
        final_json = self._final_json(id, interventions,project)
        print(final_json)
        pil_img = self._send_request(self.url, final_json, format)
        if format == 'NP':
            return self.PILtoNP(pil_img)
        else:
            pil_img.convert('RGBA')
            return self.PILtoQImage(pil_img)


    def _send_request(self, url, json_data,format):
        response = requests.post(url,json=json_data)
        print(response)
        res_base64 = response.json()['res'][0]['d']
        img_base64 = res_base64.split(',')[1]
        # fp =open('1.jpg','wb')
        # fp.write(res)
        pil_img = self._base64toPIL(img_base64)
        return pil_img

    def _final_json(self,id_num,interventions,project="churchoutdoor"):
        """
        project: str, scene name
        id: int, image id
        interventions: see intervent()
        """
        return {
            "project": project,
            "ids": [
                id_num
            ],
            "wantz": 0,
            "interventions": interventions,
            "return_urls": 0
        }

    def _intervent(self, operations):
        """
        operations: list of operation, [ op1, op2, ...]
        operation: {['mask']: np_img data 32x32, ['op']: op_name}
        op_name: 'add_tree','remove tree', ...
        """
        return [ {
            "ablations": self.preset_ablation[op['op']], 
            "mask": {
                "bitbounds": [],
                "bitstring": self._PILtobase64(op['mask'],format='PNG'),
                "shape": []
            }
        } for op in operations ]

    def _interpreteOp(self, qimagOp):
        operations = [{'op': 'add_dome', 'mask': Image.open("client/mask.png")}]
        pil_img = self.QImagetoPIL(qimagOp)
        np_img = self.PILtoNP(pil_img)
        assert self.defaultPalette is not None
        operations_from_qimage = []
        for color,action in zip(self.defaultPalette,self.defaultOp):
            color = [color[0],color[1],color[2],255]
            print(color)
            op = np.zeros(np_img.shape,dtype=np.uint8)
            idx = (np_img==color).all(axis=-1)
            op[idx]=color
            # op = np.zeros(np_img.shape,dtype=np.uint8)
            #
            # for i in range(np_img.shape[0]):
            #     for j in range(np_img.shape[1]):
            #         if((np_img[i][j][:-1]==color).all()):
            #             op[i][j] = np_img[i][j]
            # op = op + np_img * (np_img == (color.append(255)))
            if(op[:,:,:-1].max()>0):
                op = self.NPtoPIL(op)
                # plt.figure()
                # plt.imshow(op)
                # plt.show()
                # op = op.resize((self.mask_shape[0],self.mask_shape[1]),Image.ANTIALIAS)
                op = op.resize((self.mask_shape[0],self.mask_shape[1]))
                print(action)
                # plt.figure()
                # plt.imshow(op)
                # plt.show()
                operations_from_qimage.append({"op":action,"mask":op})
        return operations_from_qimage

    def _base64toPIL(self, base64str):
        binary_data = base64.b64decode(base64str)
        image=io.BytesIO(binary_data)
        res = Image.open(image)
        # plt.imshow(res)
        # plt.show()
        return res
    
    # def base64toCV(self, base64str):
    #     binary_data = base64.b64decode(base64str)
    #     nparr = np.fromstring(binary_data, np.uint8)  
    #     cv_img = cv.imdecode(nparr,cv.IMREAD_COLOR)
    #     # cv.imshow("test",cv_img)
    #     # cv.waitKey(0)
    #     return cv_img
    
    def _PILtobase64(self, pil_img, format='JPEG'):
        output_buffer = io.BytesIO()
        pil_img.save(output_buffer, format)
        byte_data = output_buffer.getvalue()
        base64_bytes = base64.b64encode(byte_data)
        base64_str = base64_bytes.decode("ascii")
        return "data:image/"+format.lower()+";base64,"+base64_str
    
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

if __name__ == "__main__":
    ganpaint = GANPaint('preset_ops.json')
    operations = [{'op':'add_dome','mask':Image.open("mask.png")}]
    np_img = ganpaint.upload(700,operations,format='NP')

    plt.imshow(np_img)
    plt.show()
    q_img = ganpaint.upload(700,operations,format='QImage')
    plt.imshow(ganpaint.QImagetoPIL(q_img))
    plt.show()
    print("success")