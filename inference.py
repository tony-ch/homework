import numpy as np
import matplotlib.pyplot as plt
from pylab import *
import os
import sys
import cv2
from PIL import Image
from keras.preprocessing.image import *
from keras.models import load_model
import keras.backend as K
from keras_applications.imagenet_utils import preprocess_input

from models import *

weight_file = 'model.hdf5'
save_path = 'models/'
image_size = (96, 96)
batch_shape = (1, ) + image_size + (1, )
checkpoint_path = os.path.join(save_path, weight_file)
classes = 15
# model_path = os.path.join(current_dir, 'model_weights/fcn_atrous/model_change.hdf5')
# model = FCN_Resnet50_32s((480,480,3))

config = tf.ConfigProto(gpu_options=tf.GPUOptions(allow_growth=True))
session = tf.Session(config=config)
K.set_session(session)

model = AtrousFCN_Vgg16_16s(batch_shape=batch_shape, input_shape=(96, 96, 1), classes=classes)
model.load_weights(checkpoint_path, by_name=True)
model.summary()

with open('npz/test.npz','rb') as f:
    X_test = np.load(f)

results = []
for idx in range(len(X_test)):
    image = X_test[idx]
    img_h, img_w = image.shape[0:2]

    # long_side = max(img_h, img_w, image_size[0], image_size[1])
    
    '''img = array_to_img(image, 'channels_last', scale=False)
    img.show()
    exit()'''
    plt.imshow(np.squeeze(image))
    fig = plt.gcf()
    ax = fig.gca()
    # plt.show()
    # image = cv2.resize(image, image_size)

    image = np.expand_dims(image, axis=0)
    # image = preprocess_input(image)

    result = model.predict(image, batch_size=1)
    result = np.squeeze(result)
    pos = np.zeros(result.shape[-1]*2)
    for i in range(result.shape[-1]):
        position = np.argmax(result[:,:,i])
        x, y = divmod(position, img_w)
        print(x, y)
        
        # plt.imshow(result[:,:,i])
        
        ax.add_artist(plt.Circle((y,x),1,color='r'))
    plt.show()
    
    # result = np.argmax(, axis=-1).astype(np.uint8)

    
    # result_img = result_img.resize(label_size, resample=Image.BILINEAR)
    # result_img.show(title='result')
    # if save_dir:
    #     result_img = Image.fromarray(result, mode='P')
    #     result_img.save(os.path.join(save_dir, img_num + '.png'))