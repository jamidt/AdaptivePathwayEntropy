import numpy as np
import matplotlib.pyplot as plt
import os.path

from matplotlib import rc
rc('font', **{'family': 'serif', 'serif': ['Computer Modern']})
rc('text', usetex=True)

fig_path_entropy, ax_path_entropy = plt.subplots()
fig_endpoint_entropy, ax_endpoint_entropy = plt.subplots()
fig_conditional_entropy, ax_conditional_entropy = plt.subplots()

loci = [10, 20, 30]
start = [0, 0.2, 0.4, 0.6, 0.8]

filename = "data-{0}-{1}.txt"

for l in loci:
    s = []
    mean = []
    var = []
    for x in start:
        if os.path.isfile(filename.format(l, x)):
            s.append(x)
            data = np.loadtxt(filename.format(l, x), usecols=(0,))
            mean.append(np.mean(data))
            var.append(np.var(data))
    ax_path_entropy.plot(s, mean, label=r"$L={}$".format(l))

ax_path_entropy.set_xlabel(r"$x$")
ax_path_entropy.set_ylabel(r"$<H(P)>$")
ax_path_entropy.legend()

for l in loci:
    s = []
    mean = []
    var = []
    for x in start:
        if os.path.isfile(filename.format(l, x)):
            s.append(x)
            data = np.loadtxt(filename.format(l, x), usecols=(1,))
            mean.append(np.mean(data))
            var.append(np.var(data))
    ax_endpoint_entropy.plot(s, mean, label=r"$L={}$".format(l))

ax_endpoint_entropy.set_xlabel(r"$x$")
ax_endpoint_entropy.set_ylabel(r"$<H(\sigma)>$")
ax_endpoint_entropy.legend()

for l in loci:
    s = []
    mean = []
    var = []
    for x in start:
        if os.path.isfile(filename.format(l, x)):
            s.append(x)
            data = np.loadtxt(filename.format(l, x), usecols=(2,))
            mean.append(np.mean(data))
            var.append(np.var(data))
    ax_conditional_entropy.plot(s, mean, label=r"$L={}$".format(l))

ax_conditional_entropy.set_xlabel(r"$x$")
ax_conditional_entropy.set_ylabel(r"$<H(P|\sigma)>$")
ax_conditional_entropy.legend()

fig_path_entropy.savefig("path_entropy_HoC_back.pdf")
fig_endpoint_entropy.savefig("endpoint_entropy_HoC_back.pdf")
fig_conditional_entropy.savefig("conditional_entropy_HoC_back.pdf")
