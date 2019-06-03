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
import pandas as pd

from models import *
from data import load2d,transfer_target

weight_file = 'model.hdf5'
save_path = 'models/'
FIdLookup = 'raw/IdLookupTable.csv'
FTRAIN = "raw/training.csv"
n_points_final = 25
image_size = (96, 96)
using_cross = True

# batch_shape = (1, ) + image_size + (1, )
checkpoint_path = os.path.join(save_path, weight_file)
classes = 15
# model_path = os.path.join(current_dir, 'model_weights/fcn_atrous/model_change.hdf5')
# model = FCN_Resnet50_32s((480,480,3))

config = tf.ConfigProto(gpu_options=tf.GPUOptions(allow_growth=True))
session = tf.Session(config=config)
K.set_session(session)

model = FCN_Vgg16_16s(input_shape=(96, 96, 1), classes=classes)
model.load_weights(checkpoint_path, by_name=True)
model.summary()

df = pd.read_csv(os.path.expanduser(FTRAIN)) 
nm_landmarks = df.columns[:-1]
X_test,  y_test, _, _ = load2d(test=True)

print(X_test.shape,y_test)
y_pred_test = model.predict(X_test)  ## estimated heatmap
y_pred_test = y_pred_test.reshape(-1,96,96,15)

# for i in range(X_test.shape[0]):
#     for j in range(15):
#         plt.imshow(y_pred_test[i,:,:,j])
#         plt.show()

y_pred_test_xy = transfer_target(y_pred_test,thresh=0, n_points=n_points_final, sigmoid = using_cross) ## estimated xy coord
if not os.path.exists('result/figs/'):
    os.mkdir("result/figs/")
for i in range(X_test.shape[0]):
    name = '{:06d}.png'.format(i+1)
    plt.imshow((np.squeeze(X_test[i])*255).astype(np.uint8), cmap='gray')
    fig = plt.gcf()
    ax = fig.gca()
    for p in range(0,15*2, 2):
        ax.add_artist(plt.Circle((int(y_pred_test_xy[i][p]),int(y_pred_test_xy[i][p+1])),1,color='r'))
    plt.savefig("result/figs/"+name)
    # plt.pause(0)
    plt.close()
y_pred_test_xy = pd.DataFrame(y_pred_test_xy,columns=nm_landmarks)
IdLookup = pd.read_csv(os.path.expanduser(FIdLookup))

def prepare_submission(y_pred4,loc):
    '''
    loc : the path to the submission file
    save a .csv file that can be submitted to kaggle
    '''
    ImageId = IdLookup["ImageId"]
    FeatureName = IdLookup["FeatureName"]
    RowId = IdLookup["RowId"]
    
    submit = []
    for rowId,irow,landmark in zip(RowId,ImageId,FeatureName):
        submit.append([rowId,y_pred4[landmark].iloc[irow-1]])
    
    submit = pd.DataFrame(submit,columns=["RowId","Location"])
    ## adjust the scale 
    submit["Location"] = submit["Location"]

    submit.to_csv(loc,index=False)
    print("File is saved at:" +  loc)

prepare_submission(y_pred_test_xy,loc = "result/submission.csv")