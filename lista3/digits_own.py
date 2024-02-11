from IPython.display import display
import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import os
import glob
import cv2
from PIL import Image

def get_test_data():
    x = []
    y = np.repeat([i for i in range(10)], 3)
    for f in sorted(glob.glob("./images/*.jpg")):
#        print(f)
        img = cv2.imread(f, 0)
        img_arr = np.asarray(img)
        img_arr = img_arr / 255.0
#        print(np.asarray(img_arr))
        x.append(img_arr)
    return np.array(x), y

mnist = tf.keras.datasets.mnist

(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train, x_test = x_train / 255.0, x_test / 255.0

x_test, y_test = get_test_data()

model = tf.keras.models.Sequential([
    tf.keras.layers.Flatten(input_shape = (28, 28)),
    tf.keras.layers.Dense(128, activation = "relu"),
    tf.keras.layers.Dropout(0.2),
    tf.keras.layers.Dense(10),
])

loss_fn = tf.keras.losses.SparseCategoricalCrossentropy(from_logits = True)

model.compile(
        optimizer = "adam",
        loss = loss_fn,
        metrics = ["accuracy"]
)

model.summary()

model.fit(x_train, y_train, epochs = 5)

model.evaluate(x_test, y_test, verbose = 2)


