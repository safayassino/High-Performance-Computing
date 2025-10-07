import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('epyc_times.csv',
                 names=['study', 'rows', 'cols', 'mpi_procs', 'mode', 'time'])

print("Data preview:")
print(df.head(12))
print("\nUnique values:")
print(f"Rows: {sorted(df['rows'].unique())}")
print(f"Cols: {sorted(df['cols'].unique())}")
print(f"MPI procs: {sorted(df['mpi_procs'].unique())}")
print(f"Mode: {sorted(df['mode'].unique())}")

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
    print(f"\nSize {int(size)}x{int(size)}:")
    print(f"  Procs: {procs}")
    print(f"  Times: {times}")
    print(f"  Speedup: {times[0] / times}")
    
    speedup = times[0] / times
    plt.plot(procs, speedup, marker='o', linewidth=2, markersize=8,
             label=f'{int(size)}×{int(size)}', color=colors[i % len(colors)])

all_procs = sorted(df['mpi_procs'].unique())
plt.plot(all_procs, all_procs, '--', linewidth=2, color='gray', 
         label='Ideal Linear Speedup')

plt.xlabel('Number of MPI Processes', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.title('MPI Strong Scalability - Game of Life (EPYC Nodes)', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('mpi_strong_scalability_epyc.png', dpi=300, bbox_inches='tight')
print("\nPlot saved as mpi_strong_scalability_epyc.png")