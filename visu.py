import matplotlib
matplotlib.use('TkAgg')
import tkinter as tk
from tkinter import filedialog, messagebox, simpledialog
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
import numpy as np
import re
import threading

def parse_trace_file(filename):
    miss_re = re.compile(
        r'^(?P<cache>[A-Z0-9]+)\s+MISS:(?:addr=)?(?P<addr>0x[0-9a-fA-F]+)?'
        r'(?:\s*evicted_addr=(?P<evicted_addr>0x[0-9a-fA-F]+))?'
        r'(?:\s*set=(?P<set>\d+))?'
        r'(?:\s*way=(?P<way>\d+))?'
        r'(?:\s*cache_line=(?P<cache_line>\d+))?'
        r'(?:\s*evicted_cache_line=(?P<evicted_cache_line>\d+))?'
        r'.*miss_type=(?P<miss_type>[a-zA-Z]+)'
        r'(?:\s*line_num=(?P<line_num>\d+))?'
    )
    hit_re = re.compile(
        r'^(?P<cache>[A-Z0-9]+)\s+HIT:.*set=(?P<set>\d+)\s+way=(?P<way>\d+)'
        r'(?:\s*cache_line=(?P<cache_line>\d+))?'
        r'(?:.*line_num=(?P<line_num>\d+))?'
    )
    rows = []
    with open(filename, 'r') as f:
        for i, line in enumerate(f):
            m = miss_re.search(line)
            if m:
                d = m.groupdict()
                rows.append({
                    'cache': d['cache'],
                    'set': int(d['set']) if d['set'] else None,
                    'way': int(d['way']) if d['way'] else None,
                    'miss_type': d['miss_type'],
                    'addr': d['addr'],
                    'evicted_addr': d['evicted_addr'],
                    'cache_line': int(d['cache_line']) if d['cache_line'] else None,
                    'evicted_cache_line': int(d['evicted_cache_line']) if d['evicted_cache_line'] else None,
                    'line_num': int(d['line_num']) if d['line_num'] else i+1
                })
                continue
            m = hit_re.search(line)
            if m:
                d = m.groupdict()
                rows.append({
                    'cache': d['cache'],
                    'set': int(d['set']) if d['set'] else None,
                    'way': int(d['way']) if d['way'] else None,
                    'miss_type': 'hit',
                    'addr': None,
                    'evicted_addr': None,
                    'cache_line': int(d['cache_line']) if d['cache_line'] else None,
                    'evicted_cache_line': None,
                    'line_num': int(d['line_num']) if d['line_num'] else i+1
                })
    return pd.DataFrame(rows)

def plot_miss_heatmaps(df):
    miss_types = df['miss_type'].unique()
    n_types = len(miss_types)
    fig, axs = plt.subplots(1, n_types, figsize=(7 * n_types, 7), squeeze=False)
    for i, mtype in enumerate(miss_types):
        filtered = df[df['miss_type'] == mtype]
        pivot = filtered.pivot_table(
            index='set', columns='way', values='miss_type',
            aggfunc='count', fill_value=0
        )
        sns.heatmap(
            pivot, annot=False, cmap='Reds', ax=axs[0, i]
        )
        axs[0, i].set_title(f'Cache Miss Intensity\nMiss Type: {mtype}')
        axs[0, i].set_xlabel('Way')
        axs[0, i].set_ylabel('Set')
    plt.tight_layout()
    plt.show()

def plot_all_miss_types_overlay(df):
    color_map = {
        "capacity": "blue",
        "conflict": "red",
        "compulsory": "black"
    }
    sets = sorted(df['set'].dropna().unique())
    ways = sorted(df['way'].dropna().unique())
    fig, ax = plt.subplots(figsize=(12, 8))
    max_count = 0
    for miss_type, color in color_map.items():
        filtered = df[df['miss_type'] == miss_type]
        if filtered.empty:
            continue
        pivot = filtered.pivot_table(
            index='set', columns='way', values='miss_type',
            aggfunc='count', fill_value=0
        ).reindex(index=sets, columns=ways, fill_value=0)
        xs, ys, cs = [], [], []
        for i, s in enumerate(sets):
            for j, w in enumerate(ways):
                count = pivot.loc[s, w]
                if count > 0:
                    xs.append(w)
                    ys.append(s)
                    cs.append(count)
                    max_count = max(max_count, count)
        ax.scatter(xs, ys, s=[c * 15 for c in cs], c=color, alpha=0.5, label=miss_type.capitalize(), edgecolors='k' if color != "black" else None)
    ax.set_xlabel("Way")
    ax.set_ylabel("Set")
    ax.set_title("Overlay: All Cache Miss Types (Color Coded)")
    ax.legend()
    plt.gca().invert_yaxis()
    plt.show()

def plot_heatmap_all_types_and_hit(df):
    types = ['compulsory', 'capacity', 'conflict', 'hit']
    cmap_dict = {
        'compulsory': 'Greys',
        'capacity': 'Blues',
        'conflict': 'Reds',
        'hit': 'Greens'
    }
    colorbar_labels = {
        'compulsory': 'Compulsory Misses',
        'capacity': 'Capacity Misses',
        'conflict': 'Conflict Misses',
        'hit': 'Hits'
    }
    sets = sorted(df['set'].dropna().unique())
    ways = sorted(df['way'].dropna().unique())
    fig, axs = plt.subplots(1, len(types), figsize=(6 * len(types), 7), squeeze=False)
    vmax = {}
    for t in types:
        filtered = df[df['miss_type'] == t]
        if filtered.empty:
            vmax[t] = 1
        else:
            pivot = filtered.pivot_table(
                index='set', columns='way', values='miss_type',
                aggfunc='count', fill_value=0
            ).reindex(index=sets, columns=ways, fill_value=0)
            vmax[t] = pivot.values.max()
    for i, t in enumerate(types):
        filtered = df[df['miss_type'] == t]
        pivot = filtered.pivot_table(
            index='set', columns='way', values='miss_type',
            aggfunc='count', fill_value=0
        ).reindex(index=sets, columns=ways, fill_value=0)
        sns.heatmap(
            pivot,
            ax=axs[0, i],
            cmap=cmap_dict[t],
            cbar=True,
            annot=False,
            linewidths=0.1,
            linecolor='lightgrey',
            vmax=max(1, vmax[t]),
            square=False
        )
        axs[0, i].set_title(f"Heatmap: {colorbar_labels[t]}")
        axs[0, i].set_xlabel("Way")
        axs[0, i].set_ylabel("Set")
    plt.tight_layout()
    plt.show()

def plot_combined_intensity_heatmap(df):
    sets = sorted(df['set'].dropna().unique())
    ways = sorted(df['way'].dropna().unique())
    miss_types = ['compulsory', 'capacity', 'conflict', 'hit']
    color_map = {
        'compulsory': np.array([0, 0, 0]),
        'capacity': np.array([0, 0, 255]),
        'conflict': np.array([255, 0, 0]),
        'hit': np.array([0, 180, 0])
    }
    grid_shape = (len(sets), len(ways), 3)
    heat_grid = np.zeros(grid_shape, dtype=np.float64)
    max_intensity = 0
    for mtype in miss_types:
        filtered = df[df['miss_type'] == mtype]
        if filtered.empty:
            continue
        pivot = filtered.pivot_table(
            index='set', columns='way', values='miss_type',
            aggfunc='count', fill_value=0
        ).reindex(index=sets, columns=ways, fill_value=0)
        max_count = pivot.values.max()
        if max_count > max_intensity:
            max_intensity = max_count
        for i, s in enumerate(sets):
            for j, w in enumerate(ways):
                count = pivot.loc[s, w]
                if count > 0:
                    heat_grid[i, j] += color_map[mtype] * count
    if max_intensity > 0:
        heat_grid = np.clip(heat_grid / max_intensity, 0, 1)
    else:
        heat_grid = np.zeros_like(heat_grid)
    plt.figure(figsize=(15, 10))
    plt.imshow(heat_grid, aspect='auto', origin='upper')
    plt.title('Combined Cache Events Intensity Heatmap\n(Compulsory: black, Capacity: blue, Conflict: red, HIT: green)')
    plt.xlabel('Way')
    plt.ylabel('Set')
    plt.xticks(np.arange(len(ways)), ways)
    plt.yticks(np.arange(len(sets)), sets)
    import matplotlib.patches as mpatches
    legend_patches = [
        mpatches.Patch(color='#000000', label='Compulsory Miss'),
        mpatches.Patch(color='#0000ff', label='Capacity Miss'),
        mpatches.Patch(color='#ff0000', label='Conflict Miss'),
        mpatches.Patch(color='#00b400', label='HIT')
    ]
    plt.legend(handles=legend_patches, bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.show()

def plot_eviction_heatmap(df):
    eviction_counts = (
        df[df['evicted_addr'].notna()]
        .groupby(['set', 'way'])
        .size()
        .unstack(fill_value=0)
    )
    plt.figure(figsize=(12, 8))
    sns.heatmap(eviction_counts, cmap='magma', annot=True, fmt='d', cbar=True)
    plt.title("Eviction Count Heatmap (per Set/Way)")
    plt.xlabel("Way")
    plt.ylabel("Set")
    plt.tight_layout()
    plt.show()

def plot_cache_animation(df):
    df = df[df['set'].notna() & df['way'].notna()]
    cache_types = df['cache'].dropna().unique()
    if len(cache_types) > 1:
        cache_type = simpledialog.askstring(
            "Cache selection",
            f"Multiple cache types found: {', '.join(cache_types)}\nEnter which to animate:",
            initialvalue=cache_types[0]
        )
        if cache_type is None or cache_type not in cache_types:
            messagebox.showinfo("Cancelled", "Animation cancelled.")
            return
    else:
        cache_type = cache_types[0]
    cdf = df[df['cache'] == cache_type].sort_values("line_num")
    sets = sorted(cdf['set'].dropna().unique())
    ways = sorted(cdf['way'].dropna().unique())
    n_sets, n_ways = len(sets), len(ways)
    cache_state = np.full((n_sets, n_ways), "", dtype=object)
    events = []
    for _, row in cdf.iterrows():
        set_idx = sets.index(row['set'])
        way_idx = ways.index(row['way'])
        addr = row['addr'] if row['addr'] else ("HIT" if row['miss_type'] == "hit" else "")
        miss_type = row['miss_type']
        line_num = row['line_num']
        events.append((set_idx, way_idx, addr, miss_type, line_num))
    max_frames = 1800  # 60 seconds at 30fps
    step = max(1, len(events) // max_frames)
    sampled_events = events[::step]
    fig, ax = plt.subplots(figsize=(n_ways + 2, n_sets + 2))
    plt.title(f"Animated Cache State: {cache_type} ({len(events)} events, every {step} shown)")
    table = ax.table(
        cellText=[["" for _ in range(n_ways)] for _ in range(n_sets)],
        rowLabels=[f"Set {s}" for s in sets],
        colLabels=[f"Way {w}" for w in ways],
        loc='center',
        cellLoc='center',
        rowLoc='center'
    )
    ax.axis('off')
    def init():
        for i in range(n_sets):
            for j in range(n_ways):
                table[(i, j)].set_facecolor('white')
                table[(i, j)].get_text().set_text("")
        return [table]
    def update(frame):
        set_idx, way_idx, addr, miss_type, line_num = sampled_events[frame]
        cache_state[set_idx, way_idx] = addr if addr else ""
        for i in range(n_sets):
            for j in range(n_ways):
                table[(i, j)].get_text().set_text(str(cache_state[i, j]))
                table[(i, j)].set_facecolor('white')
        table[(set_idx, way_idx)].set_facecolor(
            '#d62728' if miss_type != 'hit' else '#2ca02c'
        )
        ax.set_title(f"Frame {frame+1}/{len(sampled_events)} | Trace line {line_num} | {cache_type}")
        return [table]
    ani = animation.FuncAnimation(
        fig, update, frames=len(sampled_events),
        init_func=init, interval=1000 * 60 / len(sampled_events), blit=False, repeat=False
    )
    # Keep the animation object alive!
    plt.show()
    return ani

class CacheVisualizerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Cache Trace Visualizer")
        self.trace_file = None
        self._cache_anim = None  # For animation reference
        self.label = tk.Label(root, text="No file selected.")
        self.label.pack(pady=10)
        self.select_btn = tk.Button(root, text="Select Trace File", command=self.select_file)
        self.select_btn.pack(pady=5)
        self.option1_btn = tk.Button(
            root, text="1. Visualize Cache Miss Heatmap", command=self.visualize_option1, state='disabled'
        )
        self.option1_btn.pack(pady=8)
        self.option2_btn = tk.Button(
            root, text="2. Overlay All Cache Miss Types (Color Coded)", command=self.visualize_option2, state='disabled'
        )
        self.option2_btn.pack(pady=8)
        self.option3_btn = tk.Button(
            root, text="3. Heatmaps: All Miss Types + HIT (Color)", command=self.visualize_option3, state='disabled'
        )
        self.option3_btn.pack(pady=8)
        self.option4_btn = tk.Button(
            root, text="4. Combined Intensity Heatmap (All Events)", command=self.visualize_option4, state='disabled'
        )
        self.option4_btn.pack(pady=8)
        self.option5_btn = tk.Button(
            root, text="5. Eviction Count Heatmap (per Set/Way)", command=self.visualize_option5, state='disabled'
        )
        self.option5_btn.pack(pady=8)
        self.option6_btn = tk.Button(
            root, text="6. Animated Cache Table (1 Minute Preview)", command=self.visualize_option6, state='disabled'
        )
        self.option6_btn.pack(pady=8)
        self.more_label = tk.Label(root, text="(More visualization options coming soon!)")
        self.more_label.pack(pady=10)
    def select_file(self):
        filetypes = [("Text files", "*.txt"), ("All files", "*.*")]
        filename = filedialog.askopenfilename(title="Open trace file", filetypes=filetypes)
        if filename:
            self.trace_file = filename
            self.label.config(text=f"Selected file: {filename}")
            self.option1_btn.config(state='normal')
            self.option2_btn.config(state='normal')
            self.option3_btn.config(state='normal')
            self.option4_btn.config(state='normal')
            self.option5_btn.config(state='normal')
            self.option6_btn.config(state='normal')
        else:
            self.label.config(text="No file selected.")
            self.option1_btn.config(state='disabled')
            self.option2_btn.config(state='disabled')
            self.option3_btn.config(state='disabled')
            self.option4_btn.config(state='disabled')
            self.option5_btn.config(state='disabled')
            self.option6_btn.config(state='disabled')
    def visualize_option1(self):
        if not self.trace_file:
            messagebox.showerror("Error", "No trace file selected!")
            return
        threading.Thread(target=self._visualize_option1_worker).start()
    def _visualize_option1_worker(self):
        try:
            df = parse_trace_file(self.trace_file)
            if df.empty:
                messagebox.showerror("Error", "No cache misses found in the trace file!")
                return
            plot_miss_heatmaps(df)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to visualize: {e}")
    def visualize_option2(self):
        if not self.trace_file:
            messagebox.showerror("Error", "No trace file selected!")
            return
        threading.Thread(target=self._visualize_option2_worker).start()
    def _visualize_option2_worker(self):
        try:
            df = parse_trace_file(self.trace_file)
            if df.empty:
                messagebox.showerror("Error", "No cache misses found in the trace file!")
                return
            plot_all_miss_types_overlay(df)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to visualize: {e}")
    def visualize_option3(self):
        if not self.trace_file:
            messagebox.showerror("Error", "No trace file selected!")
            return
        threading.Thread(target=self._visualize_option3_worker).start()
    def _visualize_option3_worker(self):
        try:
            df = parse_trace_file(self.trace_file)
            if df.empty:
                messagebox.showerror("Error", "No cache hits or misses found in the trace file!")
                return
            plot_heatmap_all_types_and_hit(df)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to visualize: {e}")
    def visualize_option4(self):
        if not self.trace_file:
            messagebox.showerror("Error", "No trace file selected!")
            return
        threading.Thread(target=self._visualize_option4_worker).start()
    def _visualize_option4_worker(self):
        try:
            df = parse_trace_file(self.trace_file)
            if df.empty:
                messagebox.showerror("Error", "No cache hits or misses found in the trace file!")
                return
            plot_combined_intensity_heatmap(df)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to visualize: {e}")
    def visualize_option5(self):
        if not self.trace_file:
            messagebox.showerror("Error", "No trace file selected!")
            return
        threading.Thread(target=self._visualize_option5_worker).start()
    def _visualize_option5_worker(self):
        try:
            df = parse_trace_file(self.trace_file)
            if df.empty or df['evicted_addr'].isnull().all():
                messagebox.showerror("Error", "No eviction data found in the trace file!")
                return
            plot_eviction_heatmap(df)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to visualize: {e}")
    def visualize_option6(self):
        if not self.trace_file:
            messagebox.showerror("Error", "No trace file selected!")
            return
        threading.Thread(target=self._visualize_option6_worker).start()
    def _visualize_option6_worker(self):
        try:
            df = parse_trace_file(self.trace_file)
            if df.empty:
                messagebox.showerror("Error", "No cache data found in the trace file!")
                return
            # Keep reference so animation is not garbage collected
            self._cache_anim = plot_cache_animation(df)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to animate: {e}")

if __name__ == "__main__":
    root = tk.Tk()
    app = CacheVisualizerApp(root)
    root.mainloop()
