import os
import sys
import numpy as np
import matplotlib.pyplot as plt

repl = "lru"
pref = "bop"
inclusivity = ["exclusive", "inclusive", "non_inclusive"]

traces = []

for trace in os.listdir("../traces/"):
    traces.append(trace)

factors = ["8", "4", "2", "1", "0.5", "0.25", "0.125"]
l = [str(int(64/float(i))) for i in factors]

blocksize = [str(int(64./float(i))) for i in factors]
r = list(range(1, len(blocksize) + 1))
cache = ["l1d", "l2c", "llc"]

for t in traces:
    data = {}
    ipc = {}

    for i in inclusivity:
        data[i] = {}
        ipc[i] = []
        for c in cache:
            data[i][c] = []
    
    for i in inclusivity:
        file = f"{t}-bimodal-no-no-{pref}-no-{repl}-1core.txt"
        for f in factors:
            with open(f'data_line/{i}/{f}/{file}', 'r') as f:
                stripped_lines = [line.strip() for line in f]
                
                index = 0
                while (stripped_lines[index] != "Region of Interest Statistics"):
                    index += 1
                index += 2
                ipc[i].append(float(stripped_lines[index].split()[4]))
                index += 1
                data[i]["l1d"].append(100 * float(stripped_lines[index].split()[5])/float(stripped_lines[index].split()[3]))
                index += 14
                data[i]["l2c"].append(100 * float(stripped_lines[index].split()[5])/float(stripped_lines[index].split()[3]))
                index += 7
                data[i]["llc"].append(100 * float(stripped_lines[index].split()[5])/float(stripped_lines[index].split()[3]))

    
    x_values = np.arange(len(blocksize))

    for k in range(3):
        plt.plot(r, ipc[inclusivity[k]], label=inclusivity[k])
    plt.xlabel("Block Size")
    plt.ylabel("IPC value")
    plt.title(f"IPC for {t}")
    plt.xticks(r, l)
    plt.legend()
    plt.show()


    fig, ax = plt.subplots(1, 3)
    for k in range(3):
        # plt.subplot(1, 3, k+1)
        for i in inclusivity:
            # ax[0, k].plot(r, data[i][cache[k]], label=i)
            ax[k].plot(r, data[i][cache[k]], label=i)
        
        ax[k].set_xlabel('Block Size')
        ax[k].set_ylabel('Hit Rate')
        ax[k].set_title(f'{str.upper(cache[k])}')
        ax[k].set_xticks(r, l)
        ax[k].legend()

    fig.suptitle(f'Hit Rate for {t}')
    # ax.set_xticklabels(blocksize)

    plt.show()
