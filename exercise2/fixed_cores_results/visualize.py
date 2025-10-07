import pandas as pd
import matplotlib.pyplot as plt
import os

blis = pd.read_csv('/orfeo/cephfs/home/dssc/syassin/projects/High-Performance-Computing/exercise2/blis_matrix_thin.csv')
openblas = pd.read_csv('/orfeo/cephfs/home/dssc/syassin/projects/High-Performance-Computing/exercise2/openblas.csv')

fixed_cores = 12
blis_fixed = blis[blis['Cores'] == fixed_cores]
openblas_fixed = openblas[openblas['Cores'] == fixed_cores]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

configs_float = [
    ('BLIS', 'close', 'red', '--'),
    ('BLIS', 'spread', 'red', '-'),
    ('OpenBLAS', 'close', 'darkgreen', '--'),
    ('OpenBLAS', 'spread', 'darkgreen', '-'),
]

configs_double = [
    ('BLIS', 'close', 'red', '--'),
    ('BLIS', 'spread', 'red', '-'),
    ('OpenBLAS', 'close', 'darkgreen', '--'),
    ('OpenBLAS', 'spread', 'darkgreen', '-'),
]

# Float precision plot
for lib, bind, color, style in configs_float:
    df = blis_fixed if lib == 'BLIS' else openblas_fixed
    data = df[(df['Precision'] == 'float') & (df['Binding'] == bind)].sort_values('M').drop_duplicates('M')
    if len(data) > 0:  # Properly indented now
        ax1.plot(data['M'], data['GFLOPS'], color=color, linestyle=style,
                 label=f'{lib} {bind}', linewidth=2)

ax1.axhline(y=1996.8, color='black', linestyle=':', linewidth=2, label='TPP')
ax1.set_xlabel('Matrix Size', fontsize=12)
ax1.set_ylabel('GFLOPS', fontsize=12)
ax1.set_title('Single Precision (float)', fontsize=13, fontweight='bold')
ax1.legend(loc='best')
ax1.grid(True, alpha=0.2, linewidth=0.5)

# Double precision plot
for lib, bind, color, style in configs_double:
    df = blis_fixed if lib == 'BLIS' else openblas_fixed
    data = df[(df['Precision'] == 'double') & (df['Binding'] == bind)].sort_values('M').drop_duplicates('M')
    if len(data) > 0:  # Properly indented now
        ax2.plot(data['M'], data['GFLOPS'], color=color, linestyle=style, 
                 label=f'{lib} {bind}', linewidth=2)

ax2.axhline(y=998.4, color='black', linestyle=':', linewidth=2, label='TPP')
ax2.set_xlabel('Matrix Size', fontsize=12)
ax2.set_ylabel('GFLOPS', fontsize=12)
ax2.set_title('Double Precision (double)', fontsize=13, fontweight='bold')
ax2.legend(loc='best')
ax2.grid(True, alpha=0.2, linewidth=0.5)

fig.suptitle('GEMM Performance - Fixed Cores (12) on THIN node', fontsize=14, fontweight='bold', y=0.98)
plt.tight_layout()
plt.savefig("thin-improved.png")  # Save before show
plt.show()