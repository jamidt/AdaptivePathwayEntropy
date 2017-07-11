import numpy as np
import matplotlib.pyplot as plt

import glob
import re

from matplotlib import rc
rc('font', **{'family': 'serif', 'serif': ['Computer Modern']})
rc('text', usetex=True)

fig_path_entropy, ax_path_entropy = plt.subplots()
fig_endpoint_entropy, ax_endpoint_entropy = plt.subplots()
fig_conditional_entropy, ax_conditional_entropy = plt.subplots()

loci = [5, 10, 12, 13]
colors = ["g", "r", "b", "c", "b"]

fig = [plt.figure() for i in range(3)]
axes = [fig[i].add_subplot(111) for i in range(3)]

ylabels = [r"$<H(P)>$", r"$<H(\sigma)>$", r"$<H(P|\sigma)>$"]

pattern = re.compile(r"(^directed/data-\d*-)(.*)(.txt)")

for i, ax in enumerate(axes):
    for l, c in zip(loci, colors):
        s = []
        mean = []
        for f in glob.glob("directed/data-{}-*.txt".format(l)):
            search = pattern.search(f)
            s.append(float(search.group(2)))
            data = np.loadtxt(f, usecols=(i,))
            mean.append(np.mean(data))
#Sorting datapoints
        sarr = np.array(s)
        meanarr = np.array(mean)
        argsort = np.argsort(sarr)
        ax.plot(sarr[argsort], meanarr[argsort], "{}-".format(c), label="$L={}$".format(l))
    ax.set_xlabel(r"$x$")
    ax.set_ylabel(ylabels[i])
    ax.legend()

for f, name in zip(fig, ["path_entropy", "endpoint_entropy", "conditional_entropy"]):
    f.savefig("{}_RMF_directed.pdf".format(name))

