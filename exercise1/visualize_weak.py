import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('results_weak.csv')

# Convert to numeric
df['mpi_procs'] = pd.to_numeric(df['mpi_procs'], errors='coerce')
df['time'] = pd.to_numeric(df['time'], errors='coerce')
df['rows'] = pd.to_numeric(df['rows'], errors='coerce')
df['cols'] = pd.to_numeric(df['cols'], errors='coerce')
df = df.dropna()

# Create a matrix size identifier
df['matrix_size'] = df['rows'].astype(str) + 'x' + df['cols'].astype(str)

# Get unique matrix sizes
matrix_sizes = df.groupby('rows')['matrix_size'].first().sort_index()

plt.figure(figsize=(12, 7))

# Plot each matrix size separately
for rows_val in sorted(df['rows'].unique()):
    subset = df[df['rows'] == rows_val].copy()
    subset = subset.sort_values('mpi_procs')
    
    # Calculate speedup for this subset
    baseline_time = subset[subset['mpi_procs'] == 1]['time'].values[0]
    subset['speedup'] = baseline_time / subset['time']
    
    # Get the matrix size label
    matrix_label = subset['matrix_size'].iloc[0]
    
    plt.plot(subset['mpi_procs'], subset['speedup'], 
             marker='o', linewidth=2, label=f'{matrix_label}')

# Add ideal line
plt.axhline(y=1, linestyle='--', linewidth=2, color='gray', label='Ideal (Speedup=1)')

plt.xlabel('Number of MPI Processes', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.title('MPI Weak Scalability - Game of Life', fontsize=14)
plt.ylim([0, 1.5])
plt.legend(fontsize=10, loc='best')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('mpi_weak_scalability_24.png', dpi=300, bbox_inches='tight')
print("Plot saved as mpi_weak_scalability.png")
print(f"\nProcessed {len(matrix_sizes)} different matrix sizes:")
for size in matrix_sizes:
    print(f"  - {size}")