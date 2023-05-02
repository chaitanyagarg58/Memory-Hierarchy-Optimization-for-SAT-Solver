import os
import sys
import numpy as np
import matplotlib.pyplot as plt

n = int(sys.argv[1])

if n not in [1, 2, 3]:
    print("invalid argument")
    exit()

repl = "lfu"
pref = "bop_opt"
inclusivity = ["exclusive", "inclusive", "non_inclusive"]

traces = []

for trace in os.listdir("../traces/"):
    traces.append(trace)

factors = ["0.5", "1", "2"]

if n == 1:
    l = [str(int(64*float(i))) for i in factors]
elif n == 2:
    l = [str(int(1024*float(i))) for i in factors]
else:
    l = [str(int(2048*float(i))) for i in factors]

# blocksize = [str(int(64./float(i))) for i in factors]
r = list(range(1, len(factors) + 1))
cache = ["L1D", "L2C", "LLC"]

for t in traces:
    data = {}
    ipc = {}

    for i in inclusivity:
        data[i] = []
        ipc[i] = []
    
    for i in inclusivity:
        file = f"{t}-bimodal-no-no-{pref}-no-{repl}-1core.txt"
        for f in factors:
            with open(f'data_set{n}/{i}/{f}/{file}', 'r') as f:
                stripped_lines = [line.strip() for line in f]
                
                index = 0
                while (stripped_lines[index] != "Region of Interest Statistics"):
                    index += 1
                index += 2
                ipc[i].append(float(stripped_lines[index].split()[4]))
                
                if n == 1:
                    index += 1
                elif n == 2:
                    index += 15
                else:
                    index += 22
                
                data[i].append(100 * float(stripped_lines[index].split()[7])/float(stripped_lines[index].split()[3]))
    
    x_values = np.arange(len(r))

    for i in inclusivity:
        plt.plot(r, data[i], label=i)

    plt.xlabel('Cache Size (SET)')
    plt.ylabel(f'Miss Rate')
    plt.title(f'Trace {t} for {cache[n-1]}')
    plt.xticks(r, l)
    plt.legend()

    plt.show()

    for i in inclusivity:
        plt.plot(r, ipc[i], label=i)

    plt.xlabel('Cache Size (SET)')
    plt.ylabel(f'IPC')
    plt.title(f'Trace {t} for {cache[n-1]}')
    plt.xticks(r, l)
    plt.legend()

    plt.show()

