from clustimage import Clustimage

cl = Clustimage(method = "pca",
                embedding = "tsne",
                grayscale = False,
                dim = (128, 128),
                params_pca = {"n_components" : 0.95})

X = cl.import_example(data = "mnist")

results = cl.fit_transform(X,
                           cluster = "agglomerative",
                           evaluate = "silhouette",
                           metric = "euclidean",
                           linkage = "ward",
                           min_clust = 10,
                           max_clust = 30,
                           cluster_space = "high")

cl.results.keys()

cl.clusteval.plot()
cl.clusteval.scatter(cl.results["xycoord"])

cl.pca.plot()

#cl.dendrogram()

cl.plot_unique(img_mean = False)

cl.scatter(zoom = 3, img_mean = False)
cl.scatter(zoom = None, img_mean = False)

#cl.plot(cmap = "binary", labels = [1, 2])
#cl.plot(cmap = "binary")
