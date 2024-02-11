import tensorflow_decision_forests as tfdf

import os
import numpy as np
import pandas as pd
import tensorflow as tf
import math

(x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()

x_train = x_train.reshape(-1, 28 * 28) / 255.0
x_test = x_test.reshape(-1, 28 * 28) / 255.0

model = tfdf.keras.RandomForestModel(num_trees = 30)
model.compile(metrics = ["accuracy"])
model.fit(x_train, y_train, validation_data = (x_test, y_test))

model.evaluate(x_test, y_test)
