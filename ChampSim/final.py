import matplotlib.pyplot as plt
import numpy as np

a = [0.190387, 0.210769]
b = [0.162177, 0.184651]
c = [0.30428, 0.331273]

old = [0.190387, 0.162177, 0.30428]
new = [0.210769, 0.184651, 0.331273]

bar_width = 0.2
x_values = np.arange(len(old))

fig, ax = plt.subplots()

# ax.bar(x_values - bar_width, a, bar_width, label='cadical-high-60K-134B.champsimtrace.xz')

# ax.bar(x_values, b, bar_width, label='cadical-high-60K-1227B.champsimtrace.xz')

# ax.bar(x_values + bar_width, c, bar_width, label='kissat-inc-high-30K-1802B.champsimtrace.xz')


ax.bar(x_values - 0.5*bar_width, old, bar_width, label='Baseline')

ax.bar(x_values + 0.5*bar_width, new, bar_width, label='Final')

ax.set_xlabel('Traces')
ax.set_ylabel('IPC values')
ax.set_ylim(0.16, 0.35)
ax.set_title(f'Final Speed-up')
ax.set_xticks(x_values)
ax.set_xticklabels(["cadical-high-60K-134B.champsimtrace.xz", "cadical-high-60K-1227B.champsimtrace.xz", "kissat-inc-high-30K-1802B.champsimtrace.xz"])
ax.legend()

plt.show()