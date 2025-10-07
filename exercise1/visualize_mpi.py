import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('results_strong.csv',
                 names=['study', 'rows', 'cols', 'steps', 'mode', 'mpi_procs', 'omp_threads', 'time'])

df['rows'] = pd.to_numeric(df['rows'], errors='coerce')
df['mpi_procs'] = pd.to_numeric(df['mpi_procs'], errors='coerce')
df['time'] = pd.to_numeric(df['time'], errors='coerce')
df = df.dropna()

sizes = df.groupby('rows')['rows'].first().sort_values()

plt.figure(figsize=(10, 6))

colors = ['#2E86AB', '#06A77D', '#F18F01', '#A23B72']

for i, size in enumerate(sizes):
    df_size = df[df['rows'] == size].sort_values('mpi_procs')
    procs = df_size['mpi_procs'].values
    times = df_size['time'].values
    speedup = times[0] / times
    
    plt.plot(procs, speedup, linewidth=2, label=f'{int(size)}×{int(size)}', color=colors[i % len(colors)])

all_procs = sorted(df['mpi_procs'].unique())
plt.plot(all_procs, all_procs, '--', linewidth=2, color='gray', label='Ideal Linear Speedup')

plt.xlabel('Number of MPI Processes', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.title('MPI Strong Scalability - Game of Life', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('mpi_strong_scalability.png', dpi=300, bbox_inches='tight')
print("Plot saved as mpi_strong_scalability.png")