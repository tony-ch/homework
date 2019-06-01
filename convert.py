#!/usr/bin/env python
# -*- coding:utf-8 -*-

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import cv2

train_csv_data = pd.read_csv('raw/training.csv')
print(train_csv_data.shape)
count = train_csv_data.shape[0]
fields = train_csv_data.shape[1]
IM_SZ = 96
P_NUM = 15

imag = []
for i in range(count):
    img = train_csv_data['Image'][i].split(' ')
    img = ['0' if x == '' else x for x in img]
    imag.append(img)
image_list = np.array(imag, dtype=np.uint8)
X_train = image_list.reshape(-1,IM_SZ,IM_SZ,1)
with open('npz/train.npz','wb') as f:
    np.save(f,X_train)

training = train_csv_data.drop('Image',axis = 1)
y_train = []
# print(training.isnull().any().value_counts())
training = training.fillna(value=training.mean(axis=0))
# print(training.isnull().any().value_counts())

for i in range(count):
    y = training.iloc[i,:]
    y_train.append(y)
y_train = np.array(y_train,dtype = 'float')

y_train_respone = np.zeros((count, IM_SZ, IM_SZ, P_NUM), dtype = np.uint8)
for i in range(count):
    print(i)
    name = '{:06d}.png'.format(i+1)
    # plt.imshow(X_train[i])
    # fig = plt.gcf()
    # ax = fig.gca()
    # new_arr = np.zeros((IM_SZ,IM_SZ),dtype=np.uint8)
    # all_channel = np.zeros((P_NUM, IM_SZ, IM_SZ), dtype = np.uint8)
    for p in range(0,P_NUM*2, 2):
        # ax.add_artist(plt.Circle((y_train[i,p],y_train[i,p+1]),1,color='r'))
        x = np.round(y_train[i,p]).astype(np.uint8)
        y = np.round(y_train[i,p+1]).astype(np.uint8)
        l,r,t,b = max(y-2,0),min(IM_SZ-1,y+3),max(0,x-2),min(IM_SZ-1,x+3)
        # new_arr[l:r,t:b] = np.ones((r-l,b-t))*(p//2+1)
        # all_channel[p//2, l:r,t:b] = np.ones((r-l,b-t))
        y_train_respone[i, l:r,t:b, p//2] = np.ones((r-l,b-t))*1.0
    # y_train_respone.append(all_channel)
    # cv2.imwrite('label/'+name,new_arr)
    cv2.imwrite('raw/train/'+name,X_train[i])
    # new_arr_r = cv2.imread('test.png', cv2.IMREAD_UNCHANGED)
with open('npz/label.npz','wb') as f:
    np.save(f,y_train)
with open('npz/label_rsp.npz','wb') as f:
    np.save(f,y_train_respone)



test_csv_data = pd.read_csv('raw/test.csv')
print(test_csv_data.shape)
test_count = test_csv_data.shape[0]
imag = []
for i in range(test_count):
    img = test_csv_data['Image'][i].split(' ')
    img = ['0' if x == '' else x for x in img]
    imag.append(img)
image_list = np.array(imag, dtype=np.uint8)
X_test = image_list.reshape(-1,IM_SZ,IM_SZ, 1)
for i in range(test_count):
    name = '{:06d}.png'.format(i+1)
    cv2.imwrite('raw/test/'+name,X_test[i])
with open('npz/test.npz','wb') as f:
    np.save(f,X_test)