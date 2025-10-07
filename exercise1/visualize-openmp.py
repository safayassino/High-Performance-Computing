import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('results_openmp.csv',
                 names=['study', 'rows', 'cols', 'steps', 'mode', 'mpi_procs', 'omp_threads', 'time'])

df = df[df['rows'] == 10000].sort_values('omp_threads')

threads = df['omp_threads'].values
times = df['time'].values
speedup = times[0] / times
linear_speedup = threads

plt.figure(figsize=(10, 6))
plt.plot(threads, speedup, linewidth=2, label='Actual Speedup')
plt.plot(threads, linear_speedup, '--', linewidth=2, label='Ideal Linear Speedup')

plt.xlabel('Number of OpenMP Threads', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.title('OpenMP Scalability - Game of Life (10000x10000, 50 steps)', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('openmp_scalability.png', dpi=300, bbox_inches='tight')
print("Plot saved as openmp_scalability.png")