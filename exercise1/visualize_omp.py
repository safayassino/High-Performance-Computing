import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('results_openmp_epyc.csv',
                 names=['study', 'rows', 'cols', 'steps', 'mode', 'mpi_procs', 'omp_threads', 'time'])

df['rows'] = pd.to_numeric(df['rows'], errors='coerce')
df['omp_threads'] = pd.to_numeric(df['omp_threads'], errors='coerce')
df['time'] = pd.to_numeric(df['time'], errors='coerce')
df = df.dropna()

sizes = df.groupby('rows')['rows'].first().sort_values()
plt.figure(figsize=(10, 6))
colors = ['#2E86AB', '#06A77D', '#F18F01', '#A23B72']

for i, size in enumerate(sizes):
    df_size = df[df['rows'] == size].sort_values('omp_threads')
    threads = df_size['omp_threads'].values
    times = df_size['time'].values
    speedup = times[0] / times
    
    plt.plot(threads, speedup, linewidth=2, label=f'{int(size)}×{int(size)}', color=colors[i % len(colors)])

max_threads = int(df['omp_threads'].max())
linear_speedup = range(1, max_threads + 1)
plt.plot(linear_speedup, linear_speedup, '--', linewidth=2, color='gray', label='Ideal Linear Speedup')

plt.xlabel('Number of OpenMP Threads', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.title('OpenMP Scalability - on EPYC nodes', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('openmp_scalability_epyc.png', dpi=300, bbox_inches='tight')
