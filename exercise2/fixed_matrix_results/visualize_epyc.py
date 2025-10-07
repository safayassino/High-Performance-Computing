import pandas as pd
import matplotlib.pyplot as plt

blis = pd.read_csv('EPYC-openblas-matrix.csv')
openblas = pd.read_csv('EPYC-blis-matrix.csv')

blis.columns = blis.columns.str.strip()
openblas.columns = openblas.columns.str.strip()

fixed_size = 10000
blis_fixed = blis[blis['M'] == fixed_size]
openblas_fixed = openblas[openblas['M'] == fixed_size]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

configs_float = [
    ('BLIS', 'close', 'red', '--'),
    ('BLIS', 'spread', 'darkred', '-'),
    ('OpenBLAS', 'close', 'green', '--'),
    ('OpenBLAS', 'spread', 'darkgreen', '-'),
]

configs_double = [
    ('BLIS', 'close', 'red', '--'),
    ('BLIS', 'spread', 'darkred', '-'),
    ('OpenBLAS', 'close', 'green', '--'),
    ('OpenBLAS', 'spread', 'darkgreen', '-'),
]

for lib, bind, color, style in configs_float:
    df = blis_fixed if lib == 'BLIS' else openblas_fixed
    data = df[(df['Precision'] == 'float') & (df['Binding'] == bind)].sort_values('Cores').drop_duplicates('Cores')
    if len(data) > 0:
        ax1.plot(data['Cores'], data['GFLOPS'], color=color, linestyle=style, 
                label=f'{lib} {bind}', linewidth=2)

ax1.axhline(y=5324.8, color='black', linestyle=':', linewidth=2, label='TPP')
ax1.set_xlabel('Number of Cores', fontsize=12)
ax1.set_ylabel('GFLOPS', fontsize=12)
ax1.set_title('Single Precision (float)', fontsize=13, fontweight='bold')
ax1.legend(loc='best')
ax1.grid(True, alpha=0.2, linewidth=0.5)

for lib, bind, color, style in configs_double:
    df = blis_fixed if lib == 'BLIS' else openblas_fixed
    data = df[(df['Precision'] == 'double') & (df['Binding'] == bind)].sort_values('Cores').drop_duplicates('Cores')
    if len(data) > 0:
        ax2.plot(data['Cores'], data['GFLOPS'], color=color, linestyle=style, 
                label=f'{lib} {bind}', linewidth=2)

ax2.axhline(y=2662.4, color='black', linestyle=':', linewidth=2, label='TPP')
ax2.set_xlabel('Number of Cores', fontsize=12)
ax2.set_ylabel('GFLOPS', fontsize=12)
ax2.set_title('Double Precision (double)', fontsize=13, fontweight='bold')
ax2.legend(loc='best')
ax2.grid(True, alpha=0.2, linewidth=0.5)

fig.suptitle('GEMM Performance - Fixed Matrix (10000x10000) on EPYC node', fontsize=14, fontweight='bold', y=0.98)
plt.tight_layout()
plt.show()
plt.savefig("EPYC.png")