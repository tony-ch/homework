from keras.models import Sequential
from keras.layers import Dense, Activation, Reshape
from keras.optimizers import SGD
from keras.layers import Dropout
from keras.layers import MaxPooling2D, Conv2D , Flatten, Dropout
from keras.layers.normalization import BatchNormalization

def fc_model(input_dim):
    model = Sequential()
    model.add(Dense(128,input_dim=input_dim))
    model.add(Activation('relu'))
    model.add(Dropout(0.1))
    model.add(Dense(64))
    model.add(Activation('relu'))
    model.add(Dense(30))
    model.add(Reshape((30,1)))

    model.summary()
    sgd = SGD(lr=0.01, momentum=0.9, nesterov=True)
    model.compile(loss='mean_squared_error', optimizer=sgd, sample_weight_mode='temporal')
    return model

def CNN():
    model = Sequential()

    model.add(Conv2D(filters=16,kernel_size=2,padding="same",activation="relu",input_shape=(96,96,1)))
    model.add(Dropout(0.1))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2), border_mode="valid"))
    model.add(BatchNormalization())

    model.add(Conv2D(32, 5, 5,activation="relu"))
    # model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2), border_mode="valid"))
    model.add(Dropout(0.2))
    model.add(BatchNormalization())

    model.add(Conv2D(64, 5, 5,activation="relu"))
    # model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2), border_mode="valid"))
    model.add(BatchNormalization())

    model.add(Conv2D(128, 3, 3,activation="relu"))
    # model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2), border_mode="valid"))
    model.add(Dropout(0.4))
    model.add(BatchNormalization())

    model.add(Flatten())

    model.add(Dense(500, activation="relu"))
    model.add(Dropout(0.1))

    model.add(Dense(128, activation="relu"))
    model.add(Dropout(0.1))

    model.add(Dense(30))
    model.add(Reshape((30,1)))

    model.summary()
    model.compile(optimizer='adam', 
              loss='mse',
              metrics=['mae','accuracy'], sample_weight_mode='temporal')
    return model