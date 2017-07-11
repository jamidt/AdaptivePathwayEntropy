import numpy as np
import glob

for d in ["backstep", "directed"]:
    for l in [5, 10, 15, 20, 25, 30, 35]:
        datafiles = glob.glob("{0}/data-{1}-*.txt".format(d, l))
        for filename in datafiles:
            for i in range(3):
                print("Testing {} column {}".format(filename, i))
                testing = np.loadtxt(filename, usecols=(i,))
