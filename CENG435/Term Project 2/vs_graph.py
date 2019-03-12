# loss05
# 123, 95% CI [119, 127]
# Margin of Error: 3.83
# Sample Size: 20
# Sample Mean: 123
# Standard Deviation: 8.75
# Confidence Level: 95%

# loss10
# 623, 95% CI [617, 629]
# Margin of Error: 6.22
# Sample Size: 20
# Sample Mean: 623
# Standard Deviation: 14.2
# Confidence Level: 95%

# loss20
# 1020, 95% CI [1010, 1030]
# Margin of Error: 6.09
# Sample Size: 20
# Sample Mean: 1020
# Standard Deviation: 13.9
# Confidence Level: 95%

# corrupt05
# 129, 95% CI [125, 133]
# Margin of Error: 3.72
# Sample Size: 20
# Sample Mean: 129
# Standard Deviation: 8.49
# Confidence Level: 95%

# corrupt10
# 599, 95% CI [593, 605]
# Margin of Error: 6.4
# Sample Size: 20
# Sample Mean: 599
# Standard Deviation: 14.6
# Confidence Level: 95%

# corrupt20
# 999, 95% CI [995, 1000]
# Margin of Error: 3.65
# Sample Size: 20
# Sample Mean: 999
# Standard Deviation: 8.32
# Confidence Level: 95%

# reorder1
# 91.2, 95% CI [90.8, 91.6]
# Margin of Error: 0.416
# Sample Size: 20
# Sample Mean: 91.2
# Standard Deviation: 0.949
# Confidence Level: 95%

# reorder10
# 91.1, 95% CI [90.8, 91.4]
# Margin of Error: 0.35
# Sample Size: 20
# Sample Mean: 91.1
# Standard Deviation: 0.798
# Confidence Level: 95%

# reorder35
# 124, 95% CI [120, 128]
# Margin of Error: 4.38
# Sample Size: 20
# Sample Mean: 124
# Standard Deviation: 10
# Confidence Level: 95%


lossExps = [
	{'mean': 123, 'error': 8.75},
	{'mean': 623, 'error': 6.22},
	{'mean': 1020, 'error': 6.09},
]
corruptExps =  [
	{'mean': 129, 'error': 8.49},
	{'mean': 599, 'error': 6.4},
	{'mean': 999, 'error': 3.65},
]
reorderExps =  [
	{'mean': 91.2, 'error': 0.416},
	{'mean': 91.1, 'error': 0.35},
	{'mean': 124, 'error': 4.38},
]

%matplotlib inline
# libraries
import numpy as np
import matplotlib.pyplot as plt

# width of the bars
barWidth = 0.3

# Choose the height of the cyan bars
means = [reorderExps[0]['mean'], reorderExps[1]['mean'], reorderExps[2]['mean']]

# Choose the height of the error bars (means)
errors = [reorderExps[0]['error'], reorderExps[1]['error'], reorderExps[2]['error']]

# The x position of bars
r1 = np.arange(len(means))
r2 = [x + barWidth for x in r1]
plt.figure(figsize=(10,5))

# Create cyan bars
plt.bar(r2, means, width=barWidth, color='blue', edgecolor='black', yerr=errors, capsize=7, label='Fire Transfer Time (seconds)')

# general layout
plt.xticks([r + barWidth for r in range(len(means))], ['1', '10', '35'])
plt.title('File Transfer Time vs. Reordering Percentage')
plt.xlabel('Reordering Percentage (%)')
plt.ylabel('Fire Transfer Time (seconds)')
plt.legend()

# Show graphic
plt.show()
