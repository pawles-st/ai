from time import time
import numpy as np
import matplotlib.pyplot as plt
from keras.datasets import mnist
import tensorflow as tf


(x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()

x_train = x_train.reshape(-1, 28 * 28) / 255.0
x_test = x_test.reshape(-1, 28 * 28) / 255.0

colors = ["#476A2A","#7851B8",'#BD3430','#4A2D4E','#875525',
          "#F76A2A","#78BBB8",'#9D94CC','#102D4E','#870025',
          "#47FF2A","#785100",'#BDFF30','#FFFF00','#8755FF',
          '#A83683','#4E655E','#853541','#3A3120','#535D8E', 'black']


#feature scaling
# from sklearn.preprocessing import StandardScaler
# scaler = StandardScaler()
# x_train = scaler.fit_transform(x_train)
# x_test = scaler.fit_transform(x_test)

#umap
# import umap.umap_ as umap
# reducer = umap.UMAP(random_state=42)
# x_train = reducer.fit_transform(x_train)
# x_test = reducer.fit_transform(x_test)
import umap.umap_ as umap
from sklearn.datasets import fetch_openml
import matplotlib.pyplot as plt
import seaborn as sns
# mnist = fetch_openml("mnist_784", version=1)

reducer = umap.UMAP(random_state=42)
x_train = reducer.fit_transform(x_train)
x_test = reducer.fit_transform(x_test)

#LDA
# from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
# lda = LDA(n_components=2)
# x_train = lda.fit_transform(x_train, y_train)
# x_test = lda.fit_transform(x_test, y_test)

#PCA
# t=time()
# from sklearn.decomposition import PCA
# pca = PCA(n_components=2, random_state=0)
# train_pca = pca.fit_transform(x_train, y_train)
# test_pca = pca.fit_transform(x_test, y_test)
#
# plt.figure(figsize=(10,10))
# plt.xlim(train_pca[:,0].min(), train_pca[:,0].max())
# plt.ylim(train_pca[:,1].min(), train_pca[:,1].max())
# for i in range(len(x_train)):
#     plt.text(train_pca[i,0], train_pca[i,1], str(y_train[i]),
#              color = colors[y_train[i]],
#              fontdict={'weight':'bold', 'size':9})
# plt.title('PCA')
# plt.xlabel("first PC")
# plt.ylabel("second PC")
# plt.text(15, 25, str("PCA time: {}".format(time()-t)),
#          bbox = dict(boxstyle='round', facecolor='wheat', alpha=0.5),
#          fontsize=9)
# plt.show()


sample_group_size = 23
distance_to_consider = 0.21

#PCA -> DBSCAN
# t=time()
from sklearn.cluster import DBSCAN
db = DBSCAN(eps=distance_to_consider, min_samples=sample_group_size)
db = db.fit(x_train)
labels_db = db.labels_
clusters_db = db.fit_predict(x_test)
n_clusters_ = len(set(clusters_db)) - (1 if -1 in clusters_db else 0)
print("number of clusters in pca-DBSCAN: ", n_clusters_)

#DBSCAN visualization
plt.figure(figsize=(10,10))
plt.xlim(x_test[:,0].min(), x_test[:,0].max())
plt.ylim(x_test[:,1].min(), x_test[:,1].max())
for i in range(len(y_test)):
    plt.text(x_test[i,0], x_test[i,1], str(y_test[i]),
             color = colors[clusters_db[i]],
             fontdict={'weight':'bold', 'size':9})
plt.title('PCA -> DBSCAN')
plt.xlabel("first PC")
plt.ylabel("second PC")
plt.text(15, 25, str("DBSCAN time: CZAS"),
         bbox = dict(boxstyle='round', facecolor='wheat', alpha=0.5),
         fontsize=9)
plt.show()
# print("DBSCAN time: ", time()-t)
plt.scatter(x_test[:,0], x_test[:,1], c=clusters_db, s=50, edgecolors='b')




# sns.set(context="paper", style="white")

# fig, ax = plt.subplots(figsize=(12, 10))
# # color = mnist.target.astype(int)
# plt.scatter(x_test[:, 0], x_test[:, 1], s=0.1)
# plt.setp(ax, xticks=[], yticks=[])
# plt.title("MNIST data embedded into two dimensions by UMAP", fontsize=18)
#
# plt.show()

# sns.set(context="paper", style="white")
#
# fig, ax = plt.subplots(figsize=(12, 10))
# color = mnist.target.astype(int)
# plt.scatter(embedding[:, 0], embedding[:, 1], c=color, cmap="Spectral", s=0.1)
# plt.setp(ax, xticks=[], yticks=[])
# plt.title("MNIST data embedded into two dimensions by UMAP", fontsize=18)
#
# plt.show()
