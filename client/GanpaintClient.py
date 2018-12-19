#!/usr/bin/env python
#-*- coding:utf-8 -*-
import json
import requests
import base64
import numpy as np
from cv2 import cv2 as cv
from matplotlib import pyplot as plt
from PIL import Image
import io


class GANPaint(object):
    def __init__(self, preset_ablation='preset_ops.json', url = "http://gandissect.res.ibm.com/api/generate"):
        with open(preset_ablation,'r') as f:
            self.preset_ablation = json.load(f)
        self.url = url
    
    def upload(self, id, operations, project="churchoutdoor"):
        """
        operations: list of operation, [ op1, op2, ...]
        operation: {['mask']: np_img data 32x32, ['op']: op_name}
        op_name: 'add_tree','remove tree', ...
        """
        interventions = self._intervent(operations)
        final_json = self._final_json(id, interventions,project)
        res = self._send_request(self.url, final_json)
        return res


    def _send_request(self, url, json_data):
        response = requests.post(url,json=json_data)
        res_base64 = response.json()['res'][0]['d']
        img_base64 = res_base64.split(',')[1]
        # fp =open('1.jpg','wb')
        # fp.write(res)
        np_img = np.array(self._base64toPIL(img_base64))
        return np_img

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
                "bitstring": self._PILtobase64(self.NPtoPIL(op['mask']),format='PNG'),
                "shape": []
            }
        } for op in operations ]
    
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

if __name__ == "__main__":
    ganpaint = GANPaint()
    openrations = [{'op':'add_dome','mask':ganpaint.PILtoNP(Image.open("mask.png"))}]
    np_img = ganpaint.upload(700,openrations)

    plt.imshow(np_img)
    plt.show()
    print("success")