
from matplotlib import pyplot as plt
import numpy as np
from sklearn.model_selection import train_test_split
from collections import OrderedDict
from keras.models import load_model
from keras.layers import Dense,Reshape

from data import load2d, load, FlippedImageDataGenerator, SPECIALIST_SETTINGS, FTRAIN
from model import fc_model,CNN
from util import plot_loss, plot_sample,prepare_submission,predict_specialist,predict_single,flatten_except_1dim

EPOCH = 300

def demo_fc():
    X, y, w = load()
    y = flatten_except_1dim(y,ndim=3)
    w = flatten_except_1dim(w,ndim=2)
    print("X.shape == {}; X.min == {:.3f}; X.max == {:.3f}".format(
        X.shape, X.min(), X.max()))
    print("y.shape == {}; y.min == {:.3f}; y.max == {:.3f}".format(
        y.shape, y.min(), y.max()))
    print("w.shape == {}; w.min == {:.3f}; w.max == {:.3f}".format(
        w.shape, w.min(), w.max()))
    model = fc_model(input_dim = X.shape[1])
    hist = model.fit(X, y, sample_weight=w, epochs=EPOCH, batch_size=128, validation_split=0.2)
    # plot_loss(hist.history,"model 1",plt)
    # plt.legend()
    # plt.grid()
    # plt.yscale("log")
    # plt.xlabel("epoch")
    # plt.ylabel("log loss")
    # plt.show()
    model.save('./models/fc_weighted_model.h5')

    X_test,_, _ = load(test=True)
    # y_test = model.predict(X_test)
    # fig = plt.figure(figsize=(10, 7))
    # fig.subplots_adjust(
    #     left=0, right=1, bottom=0, top=1, hspace=0.05, wspace=0.05)

    # for i in range(16):
    #     axis = fig.add_subplot(4, 4, i+1, xticks=[], yticks=[])
    #     plot_sample(X_test[i], y_test[i], axis)
    # plt.show()
    df_y_pred = predict_single(model, X_test)
    prepare_submission(df_y_pred,"fc_weighted")

def demo_cnn():
    X,y,w = load2d()
    y = flatten_except_1dim(y,ndim=3)
    w = flatten_except_1dim(w,ndim=2)
    print(X.shape)
    print(y.shape)
    print(w.shape)
    model = CNN()
    hist = model.fit(X, y, sample_weight=w, epochs=EPOCH,batch_size=128, validation_split=0.2)
    # plot_loss(hist.history,"CNN model",plt)
    # plt.legend()
    # plt.grid()
    # plt.yscale("log")
    # plt.xlabel("epoch")
    # plt.ylabel("loss")
    # plt.show()
    model.save('./models/cnn_weighted_model.h5')

    X_test,_,_ = load2d(test=True)
    # y_test = model.predict(X_test)
    # fig = plt.figure(figsize=(10, 7))
    # fig.subplots_adjust(
    #     left=0, right=1, bottom=0, top=1, hspace=0.05, wspace=0.05)

    # for i in range(16):
    #     axis = fig.add_subplot(4, 4, i+1, xticks=[], yticks=[])
    #     plot_sample(X_test[i], y_test[i], axis)
    # plt.show()
    df_y_pred = predict_single(model, X_test)
    prepare_submission(df_y_pred,"cnn_weighted")

def demo_cnn_aug():
    X, y, w = load2d()
    y = flatten_except_1dim(y,ndim=3)
    w = flatten_except_1dim(w,ndim=2)
    X_train, X_val, y_train, y_val, w_train, w_val = train_test_split(X, y, w, test_size=0.2, random_state=42)

    model = CNN()
    flip_indices = [
            (0, 2), (1, 3),
            (4, 8), (5, 9), (6, 10), (7, 11),
            (12, 16), (13, 17), (14, 18), (15, 19),
            (22, 24), (23, 25),
            ]
    hist = model.fit_generator(FlippedImageDataGenerator(X_train, y_train, w_train, 32, flip_indices=flip_indices),
                                # steps_per_epoch=X_train.shape[0],
                                epochs=EPOCH,
                                validation_data=(X_val, y_val, w_val)
                                )
    model.save('./models/cnn_aug_weighted_model.h5')
    X_test,_,_ = load2d(test=True)
    df_y_pred = predict_single(model, X_test)
    prepare_submission(df_y_pred,"cnn_aug_weighted")

    # plot_loss(hist.history,"model",plt)
    # plt.legend()
    # plt.grid()
    # plt.yscale("log")
    # plt.xlabel("epoch")
    # plt.ylabel("loss")
    # plt.show()


def fit_specialists(freeze=True,
                    print_every=10,
                    epochs=100,
                    prop=0.1,
                    name_transfer_model="model.h5"):
    specialists = OrderedDict()
    for setting in SPECIALIST_SETTINGS:
        
        cols = setting['columns']
        
        X, y, w = load2d(cols=cols)
        y = flatten_except_1dim(y,ndim=3)
        w = flatten_except_1dim(w,ndim=2)
        X_train, X_val, y_train, y_val, w_train, w_val = train_test_split(X, y, w,
                                                          test_size=0.2, 
                                                          random_state=42)
        model = load_model(name_transfer_model) 
        if freeze:
            for layer in model.layers:
                layer.trainable = False
            
        model._layers.pop() # get rid of output layer
        # model.summary()
        model._layers.pop()
        # model.summary()
        model.outputs = [model.layers[-1].output]
        # model.summary()
        model.layers[-1].outbound_nodes = []
        model.add(Dense(len(cols),name='dense_new')) # add new output layer
        model.add(Reshape((len(cols),1)))
        # model.summary()

        model.compile(loss='mean_squared_error', optimizer="adam", sample_weight_mode = 'temporal')
        
        print(X_train.shape)
        print(y_train.shape)
        print(w_train.shape)
        print(X_val.shape)
        print(y_val.shape)
        print(w_val.shape)
        hist_final = model.fit_generator(FlippedImageDataGenerator(X_train, y_train, w_train, 32, setting['flip_indices']),
                                     epochs=epochs,
                                     validation_data=(X_val, y_val,w_val))
        
        ## print(model.summary()) 
        specialists[cols] = model
    return(specialists)

def demo_cnn_aug_special():
    specialists1 = fit_specialists(freeze=True,
                    print_every=10,
                    epochs=100,
                    name_transfer_model="./models/cnn_aug_weighted_model.h5")
    X_test,_,_ = load2d(test=True)
    df_y_pred_s = predict_specialist(specialists1,X_test)
    prepare_submission(df_y_pred_s,"cnn_aug_special_weighted")

def demo_cnn_special():
    specialists2 = fit_specialists(freeze=True,
                        print_every=10,
                        epochs=100,
                        name_transfer_model="./models/cnn_weighted_model.h5")
    X_test,_,_ = load2d(test=True)
    df_y_pred_s = predict_specialist(specialists2,X_test)
    prepare_submission(df_y_pred_s,"cnn_special_weighted")


if __name__ == "__main__":
    demo_fc()
    demo_cnn()
    demo_cnn_aug()
    demo_cnn_special()
    demo_cnn_aug_special()
    
