#!/usr/bin/env python
#-*- coding:utf-8 -*-

import face_recognition
import numpy as np
import time
from flask import Flask, jsonify, request, redirect

app = Flask(__name__)
def add_face(img_path,pick_path):
    img = face_recognition.load_image_file(img_path)
    encode = face_recognition.face_encodings(img)[0]
    np.savetxt(pick_path, encode, delimiter=",")
    # encode.dump(open(pick_path,"wb"))
    # print(encode)

@app.route('/', methods=['GET', 'POST'])
def upload_image():
    # Check if a valid image file was uploaded
    if 'res' not in request.values or 'im' not in request.values:
        print("need args")
    else:
        im = request.values['im']
        res = request.values['res']
        print("im:",im)
        print("res:",res)
        add_face(im,res)
    result = {
        "im": im,
        "res": res
    }
    #time.sleep(10)
    return jsonify(result)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
