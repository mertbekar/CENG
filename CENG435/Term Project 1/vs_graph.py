# experiment 1
#     5.68, 95% CI [5.48, 5.88]
#     Margin of Error: 0.202
#     Sample Size: 2000
#     Sample Mean: 5.68
#     Standard Deviation: 4.61
#     Confidence Level: 95%

# experiment 2
#     41.1, 95% CI [40.8, 41.4]
#     Margin of Error: 0.321
#     Sample Size: 2000
#     Sample Mean: 41.1
#     Standard Deviation: 7.32
#     Confidence Level: 95%

# experiment 3
#     121, 95% CI [121, 121]
#     Margin of Error: 0.306
#     Sample Size: 2000
#     Sample Mean: 121
#     Standard Deviation: 6.99
#     Confidence Level: 95%



%matplotlib inline
# libraries
import numpy as np
import matplotlib.pyplot as plt

# width of the bars
barWidth = 0.3

# Choose the height of the blue bars
bars1 = [2, 40, 120]

# Choose the height of the cyan bars
bars2 = [5.68, 41.1, 121]

# Choose the height of the error bars (bars1)
yer1 = [15, 15, 15]

# Choose the height of the error bars (bars2)
yer2 = [0.202, 0.321, 0.306]

# The x position of bars
r1 = np.arange(len(bars1))
r2 = [x + barWidth for x in r1]
plt.figure(figsize=(25,15))

# Create blue bars
plt.bar(r1, bars1, width=barWidth, color='blue', edgecolor='black', yerr=yer1, capsize=7, label='Network Emulation Delay (ms)')

# Create cyan bars
plt.bar(r2, bars2, width=barWidth, color='cyan', edgecolor='black', yerr=yer2, capsize=7, label='End-to-End Delay (ms)')

# general layout
plt.xticks([r + barWidth for r in range(len(bars1))], ['Experiment 1', 'Experiment 2', 'Experiment 3'])
plt.title('Network Emulation Delay vs. End-to-End Delay')
plt.xlabel('Experiment')
plt.ylabel('Time (ms)')
plt.legend()

# Show graphic
plt.show()
