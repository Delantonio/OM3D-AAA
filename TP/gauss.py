import math

rad = 14.
LOD = 6.
dev = 7.

m = 0.398942280401/dev

def gau(x : float):
    return m*math.exp(-x*x*0.5/(dev*dev))
    
for i in range(20):
    print(f'{round(gau(float(i) - (20 / 2)), 5)},')
