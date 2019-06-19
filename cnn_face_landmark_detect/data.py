import os
import numpy as np
from pandas.io.parsers import read_csv
from sklearn.utils import shuffle
from matplotlib import pyplot as plt
from keras.preprocessing.image import Iterator

FTRAIN = './input/training/training.csv'
FTEST = './input/test/test.csv'
FIdLookup = './input/IdLookupTable.csv'


def load(test=False, cols=None):
    """Loads data from FTEST if *test* is True, otherwise from FTRAIN.
    Pass a list of *cols* if you're only interested in a subset of the
    target columns.
    """
    fname = FTEST if test else FTRAIN
    df = read_csv(os.path.expanduser(fname))  # load pandas dataframe

    # The Image column has pixel values separated by space; convert
    # the values to numpy arrays:
    df['Image'] = df['Image'].apply(lambda im: np.fromstring(im, sep=' '))

    if cols:  # get a subset of columns
        df = df[list(cols) + ['Image']]

    print(df.count())  # prints the number of values for each column
    # df = df.dropna()  # drop all rows that have missing values in them
    df = df.fillna(-1)

    X = np.vstack(df['Image'].values) / 255.  # scale pixel values to [0, 1]
    X = X.astype(np.float32)

    if not test:  # only FTRAIN has any target columns
        y = df[df.columns[:-1]].values
        w = np.ones_like(y)
        w[y==-1] = 0
        y = (y - 48) / 48  # scale target coordinates to [-1, 1]
        X, y, w = shuffle(X, y, w, random_state=42)  # shuffle train data
        y = y.astype(np.float32)
        w = w.astype(np.float32)
    else:
        y = None
        w = None

    return X, y, w

def load2d(test=False,cols=None):

    re = load(test, cols)
    
    X = re[0].reshape(-1,96,96,1)
    y = re[1]
    w = re[2]

    return X, y, w

# X, y = load()
# print("X.shape == {}; X.min == {:.3f}; X.max == {:.3f}".format(
#     X.shape, X.min(), X.max()))
# print("y.shape == {}; y.min == {:.3f}; y.max == {:.3f}".format(
#     y.shape, y.min(), y.max()))

class FlippedImageDataGenerator(Iterator):
    def __init__(self, x_train, y_train, w_train, batch_size, flip_indices = None, shuffle=None, seed = None):
        self.x_train = x_train
        self.y_train = y_train
        self.w_train = w_train
        self.flip_indices = flip_indices
        self.num_sample = x_train.shape[0]
        super().__init__(self.num_sample, batch_size, shuffle, seed)
    def _get_batches_of_transformed_samples(self, index_array):
        X_batch, y_batch, w_batch = self.x_train[index_array],self.y_train[index_array], self.w_train[index_array]
        batch_size = X_batch.shape[0]
        indices = np.random.choice(batch_size, batch_size//2, replace=False)
        X_batch[indices] = X_batch[indices, :, ::-1, :]

        if y_batch is not None:
            
            y_batch[indices, ::2] = y_batch[indices, ::2] * -1

            # left_eye_center_x -> right_eye_center_x のようにフリップ
            for a, b in self.flip_indices:
                y_batch[indices, a], y_batch[indices, b] = (
                    y_batch[indices, b], y_batch[indices, a]
                )
                w_batch[indices, a], w_batch[indices, b] = (
                    w_batch[indices, b], w_batch[indices, a]
                )

        return X_batch, y_batch, w_batch

SPECIALIST_SETTINGS = [
    dict(
        columns=(
            'left_eye_center_x', 'left_eye_center_y',
            'right_eye_center_x', 'right_eye_center_y',
            ),
        flip_indices=((0, 2), (1, 3)),
        ),

    dict(
        columns=(
            'nose_tip_x', 'nose_tip_y',
            ),
        flip_indices=(),
        ),

    dict(
        columns=(
            'mouth_left_corner_x', 'mouth_left_corner_y',
            'mouth_right_corner_x', 'mouth_right_corner_y',
            'mouth_center_top_lip_x', 'mouth_center_top_lip_y',
            ),
        flip_indices=((0, 2), (1, 3)),
        ),

    dict(
        columns=(
            'mouth_center_bottom_lip_x',
            'mouth_center_bottom_lip_y',
            ),
        flip_indices=(),
        ),

    dict(
        columns=(
            'left_eye_inner_corner_x', 'left_eye_inner_corner_y',
            'right_eye_inner_corner_x', 'right_eye_inner_corner_y',
            'left_eye_outer_corner_x', 'left_eye_outer_corner_y',
            'right_eye_outer_corner_x', 'right_eye_outer_corner_y',
            ),
        flip_indices=((0, 2), (1, 3), (4, 6), (5, 7)),
        ),

    dict(
        columns=(
            'left_eyebrow_inner_end_x', 'left_eyebrow_inner_end_y',
            'right_eyebrow_inner_end_x', 'right_eyebrow_inner_end_y',
            'left_eyebrow_outer_end_x', 'left_eyebrow_outer_end_y',
            'right_eyebrow_outer_end_x', 'right_eyebrow_outer_end_y',
            ),
        flip_indices=((0, 2), (1, 3), (4, 6), (5, 7)),
        ),
    ]

