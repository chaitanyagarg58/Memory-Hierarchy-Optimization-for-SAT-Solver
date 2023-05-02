import os
import sys
import numpy as np
import matplotlib.pyplot as plt

# repl = ["drrip", "ship", "lru", "lfu", "fifo"]
repl = ["drrip", "ship", "lru", "lfu", "fifo", "mru", "prob_mru", "prob_lfu"]
pref = ["no", "next_line", "bop", "bop_opt"]
# pref = ["bop"]
inclusivity = ["exclusive", "inclusive", "non_inclusive"]
# inclusivity = ["non_inclusive"]

traces = []

for trace in os.listdir("../traces/"):
    traces.append(trace)
# traces = ["cadical-high-60K-134B.champsimtrace.xz"/]

for t in traces:
    data = {}
    lab = []

    for i in inclusivity:
        data[i] = []
    
    for i in inclusivity:
        for r in repl:
            for p in pref:
                file = t + "-bimodal-no-no-" + p + "-no-" + r + "-1core.txt"
                with open(f'data/{i}/{r}/{p}/{file}', 'r') as f:
                    stripped_lines = [line.strip() for line in f]
                    
                    print (f"{i}/{r}/{p}/{file}")
                    index = 0
                    while (stripped_lines[index] != "Region of Interest Statistics"):
                        index += 1
                    index += 2
                    data[i].append(float(stripped_lines[index].split()[4]))
    for r in repl:
        for p in pref:
            lab.append(f"({r}, {p})")          
    
    print (data["non_inclusive"])
    mi = 999
    ma =0
    for i in inclusivity:
        for a in data[i]:
            mi = min(mi, a)
            ma = max(ma, a)


    bar_width = 0.2
    x_values = np.arange(len(data['non_inclusive']))

    fig, ax = plt.subplots()

    ax.bar(x_values - bar_width, data['exclusive'], bar_width, label='exclusive')

    ax.bar(x_values, data['inclusive'], bar_width, label='inclusive')

    ax.bar(x_values + bar_width, data['non_inclusive'], bar_width, label='non-inclusive')

    ax.set_xlabel('Combinations')
    ax.set_ylabel('IPC values')
    ax.set_ylim(mi*0.999, ma*1.001)
    ax.set_title(f'{t}')
    ax.set_xticks(x_values)
    ax.set_xticklabels(lab, rotation=30)
    ax.legend()

    plt.show()
