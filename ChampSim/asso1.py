import sys
import os

# Open the file in read mode and read all the lines into a list
with open('inc/champsim.h', 'r') as file:
    lines = file.readlines()

# Modify the particular line that needs to be changed
line_number = 21
lines[line_number] = f'# define fac 1\n'

# Open the file in write mode and write the modified list back to the file
with open('inc/champsim.h', 'w') as file:
    file.writelines(lines)


N_WARM = 10
N_SIM = 20

repl = "lfu"
pref = "bop"
inclusivity = ["exclusive", "inclusive", "non_inclusive"]

traces = []

for trace in os.listdir("../traces/"):
    traces.append(trace)

fac1 = ["0.25", "0.5", "1", "2"]

os.system("mkdir -p data_asso1")

for inclu in inclusivity:
    os.system(f"mkdir -p data_asso1/{inclu}")
    os.system("cp src/cache_" + inclu + ".cc.bak src/cache.cc")

    for f in fac1:

        os.system(f"mkdir -p data_asso1/{inclu}/{f}")

        # Open the file in read mode and read all the lines into a list
        with open('inc/cache.h', 'r') as file:
            lines = file.readlines()

        # Modify the particular line that needs to be changed
        line_number = 8
        lines[line_number] = f'# define fac1 {f}\n'

        # Open the file in write mode and write the modified list back to the file
        with open('inc/cache.h', 'w') as file:
            file.writelines(lines)

        os.system(f"./build_champsim.sh bimodal no no {pref} no {repl} 1")

        for t in traces:
            os.system(f"./run_champsim.sh bimodal-no-no-{pref}-no-{repl}-1core {N_WARM} {N_SIM} {t}")
        os.system(f"mv results_{N_SIM}M/* data_asso1/{inclu}/{f}/")

