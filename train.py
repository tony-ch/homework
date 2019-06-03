#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import pickle
import os
from keras.optimizers import SGD, Adam, Nadam
from keras.callbacks import *
from keras.objectives import *
from keras.metrics import binary_accuracy,categorical_accuracy
from keras.models import load_model
import keras.backend as K
from keras.preprocessing.image import *

from models import *
from utils.loss_function import *
import time
from data import transform_imgs, load2d, find_weight,flatten_except_1dim


resume_training = False
target_size = (96, 96)
save_path = 'models/'
classes = 15
prop_train = 0.9

batch_size = 32
epochs = 100

batchnorm_momentum = 0.95
# lr_base = 0.01 * (float(batch_size) / 16)
weight_decay = 1e-4

loss_fn = binary_crossentropy_with_logits
# loss_fn = "mse"
aug_data = True

const = 10 if loss_fn=="mse" else 1

#########################################################
print("create session..")
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.95
config.gpu_options.visible_device_list = "0"
K.set_session(tf.Session(config=config))
###########################################################
if target_size:
    input_shape = target_size + (1,)
else:
    input_shape = (None, None, 1)
batch_shape = (batch_size,) + input_shape
###########################################################
if not os.path.exists(save_path):
    os.mkdir(save_path)

# ###################### make model ########################
print("create model ...")
model = FCN_Vgg16_16s(input_shape=input_shape, classes=classes)

# ###################### optimizer ########################
# optimizer = SGD(lr=lr_base, momentum=0.9)
# optimizer = Nadam(lr=lr_base, beta_1 = 0.825, beta_2 = 0.99685)

model.compile(loss=loss_fn,
                optimizer='rmsprop', sample_weight_mode='temporal')
model_path = os.path.join(save_path, "model.json")
# save model structure
f = open(model_path, 'w')
model_json = model.to_json()
f.write(model_json)
f.close
model.summary()

# prepare data and label for training
print("prepare data ...")
X_train, y_train, y_train0, nm_landmarks = load2d(test=False)
print(X_train.shape,y_train.shape, y_train0.shape)
# print(y_train0)
landmark_order = {"orig" : [0,1,2,3,4,5,6,7,8,9,11,12],
                  "new"  : [1,0,4,5,2,3,8,9,6,7,12,11]}

print("split data ...")
Ntrain = int(X_train.shape[0]*prop_train)
X_tra, y_tra, X_val,y_val = X_train[:Ntrain],y_train[:Ntrain],X_train[Ntrain:],y_train[Ntrain:]
del X_train, y_train

w_tra = find_weight(y_tra)

weight_val = find_weight(y_val)

class generator(Iterator):
    '''
    data generator for saving memory
    '''
    def __init__(self, X_tra,y_tra,w_tra,landmark_order,batch_size, shuffle=None, seed=None):
        self.x_tra = X_tra
        self.y_tra = y_tra
        self.w_tra = w_tra
        self.landmark_order = landmark_order
        self.batch_size = batch_size
        self.num_sample = X_tra.shape[0]
        super().__init__(self.num_sample,batch_size,shuffle,seed)
    def _get_batches_of_transformed_samples(self, index_array):
        """Gets a batch of transformed samples.
        # Arguments
            index_array: array of sample indices to include in batch.
        # Returns
            A batch of transformed samples.
        """
        # print("\nindex_array", index_array)
        # rdx_idx = choices(range(self.num_sample),k=self.batch_size)
        # print('rdx idx', rdx_idx)
        if aug_data:
            x_batch, y_batch, w_batch = transform_imgs([self.x_tra[index_array],self.y_tra[index_array], self.w_tra[index_array]],self.landmark_order)
        else:
            x_batch, y_batch, w_batch = self.x_tra[index_array],self.y_tra[index_array], self.w_tra[index_array]
        w_batch_fla = flatten_except_1dim(w_batch,ndim=2)
        y_batch_fla = flatten_except_1dim(y_batch,ndim=3)
        return x_batch, y_batch_fla*const, w_batch_fla


##################### starting training ######################
print("start train ...")
weight_val_fla = flatten_except_1dim(weight_val,ndim=2)
y_val_fla = flatten_except_1dim(y_val,ndim=3)
steps_per_epoch = int(np.ceil(X_tra.shape[0] / float(batch_size)))

# for i in range(y_tra.shape[0]):
#         for j in range(15):
#             plt.imshow(y_tra[i,:,:,j])
#             plt.show()
hist = model.fit_generator(generator(X_tra,y_tra,w_tra,landmark_order,batch_size, shuffle=True),
                validation_data=(X_val, y_val_fla*const, weight_val_fla),
                steps_per_epoch = steps_per_epoch,
                epochs=epochs)

##################### saving model ######################
model.save_weights(save_path+'/model.hdf5')

for label in ["val_loss","loss"]:
    plt.plot(hist.history[label],label=label)
plt.legend()
plt.savefig("result/loss.png")
plt.pause(1)
plt.close()
