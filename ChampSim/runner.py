import sys
import os

# repl = ["drrip", "ship", "lru", "lfu", "fifo", "mru", "prob_mru"]
repl = ["prob_lfu"]
pref = ["no", "next_line", "bop", "bop_opt"]
# pref = ["bop_opt"]
inclusivity = ["exclusive", "inclusive", "non_inclusive"]


traces = []

for trace in os.listdir("../traces/"):
    traces.append(trace)

# os.system("mkdir data")

for inclu in inclusivity:
    # os.system("mkdir data/" + inclu)
    os.system("cp src/cache_" + inclu + ".cc.bak src/cache.cc")
    
    for r in repl:
        os.system(f"mkdir data/{inclu}/{r}")
        for p in pref:
            os.system("mkdir data/" + inclu + "/" + r + "/" + p)

            os.system("./build_champsim.sh bimodal no no " + p + " no " + r + " 1")

            for t in traces:
                os.system("./run_champsim.sh bimodal-no-no-" + p + "-no-" + r + "-1core 20 30 " + t)
            os.system("mv results_30M/* data/" + inclu + "/" + r + "/" + p + "/")

