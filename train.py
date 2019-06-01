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

from models import *
from utils.loss_function import *
import time

batch_size = 16
batchnorm_momentum = 0.95
epochs = 20
lr_base = 0.01 * (float(batch_size) / 16)
lr_power = 0.9
resume_training = False
weight_decay = 1e-4    
target_size = (96, 96)
save_path = 'models/'
classes = 15
loss_fn = binary_crossentropy_with_logits
# loss_fn = mse

###########################################################
if target_size:
    input_shape = target_size + (1,)
else:
    input_shape = (None, None, 1)
batch_shape = (batch_size,) + input_shape
###########################################################
if not os.path.exists(save_path):
    os.mkdir(save_path)

# ###############learning rate scheduler####################
def lr_scheduler(epoch, mode='power_decay'):
    '''if lr_dict.has_key(epoch):
        lr = lr_dict[epoch]
        print 'lr: %f' % lr'''

    if mode is 'power_decay':
        # original lr scheduler
        lr = lr_base * ((1 - float(epoch)/epochs) ** lr_power)
    if mode is 'exp_decay':
        # exponential decay
        lr = (float(lr_base) ** float(lr_power)) ** float(epoch+1)
    # adam default lr
    if mode is 'adam':
        lr = 0.001

    if mode is 'progressive_drops':
        # drops as progression proceeds, good for sgd
        if epoch > 0.9 * epochs:
            lr = 0.0001
        elif epoch > 0.75 * epochs:
            lr = 0.001
        elif epoch > 0.5 * epochs:
            lr = 0.01
        else:
            lr = 0.1

    print('lr: %f' % lr)
    return lr
scheduler = LearningRateScheduler(lr_scheduler)

# ###################### make model ########################
checkpoint_path = os.path.join(save_path, 'checkpoint_weights.hdf5')

model = AtrousFCN_Vgg16_16s(weight_decay=weight_decay,
                                input_shape=input_shape,
                                batch_momentum=batchnorm_momentum,
                                classes=classes)

# ###################### optimizer ########################
optimizer = SGD(lr=lr_base, momentum=0.9)
# optimizer = Nadam(lr=lr_base, beta_1 = 0.825, beta_2 = 0.99685)

model.compile(loss=loss_fn,
                optimizer=optimizer)
if resume_training:
    model.load_weights(checkpoint_path, by_name=True)
model_path = os.path.join(save_path, "model.json")
# save model structure
f = open(model_path, 'w')
model_json = model.to_json()
f.write(model_json)
f.close
img_path = os.path.join(save_path, "model.png")
# #vis_util.plot(model, to_file=img_path, show_shapes=True)
model.summary()

# lr_reducer      = ReduceLROnPlateau(monitor=softmax_sparse_crossentropy_ignoring_last_label, factor=np.sqrt(0.1),
#                                     cooldown=0, patience=15, min_lr=0.5e-6)
# early_stopper   = EarlyStopping(monitor=sparse_accuracy_ignoring_last_label, min_delta=0.0001, patience=70)
# callbacks = [early_stopper, lr_reducer]
callbacks = [scheduler]

# ################### checkpoint saver#######################
checkpoint = ModelCheckpoint(filepath=os.path.join(save_path, 'checkpoint_weights.hdf5'), save_weights_only=True)#.{epoch:d}
callbacks.append(checkpoint)
# set data generator and train
with open('npz/label_rsp.npz','rb') as f:
    y_train = np.load(f)
with open('npz/train.npz','rb') as f:
    X_train = np.load(f)

history = model.fit(X_train,y_train, epochs=epochs, batch_size = batch_size, validation_split=0.1, callbacks=callbacks)

model.save_weights(save_path+'/model.hdf5')