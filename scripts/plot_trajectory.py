from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


data_path = Path("results/particle_trajectory.csv")
fig_path = Path("results/particle_trajectory.png")

data = np.genfromtxt(
    data_path,
    delimiter=",",
    names=True,
)

x = data["x"]
y = data["y"]
time = data["time"]
speed = data["speed"]

fig, axes = plt.subplots(
    1,
    2,
    figsize=(10, 4),
)

# The magnetic field produces circular motion in the x-y plane.
axes[0].plot(x, y)
axes[0].scatter(
    x[0],
    y[0],
    color="black",
    label="Start",
)

axes[0].set_xlabel("x")
axes[0].set_ylabel("y")
axes[0].set_title("Particle trajectory")
axes[0].set_aspect("equal")
axes[0].grid(alpha=0.3)
axes[0].legend()

# Speed should remain constant in a purely magnetic field.
axes[1].plot(time, speed)
axes[1].set_xlabel("Time")
axes[1].set_ylabel("|v|")
axes[1].set_title("Speed conservation")
axes[1].grid(alpha=0.3)

fig.tight_layout()
fig.savefig(
    fig_path,
    dpi=200,
)

plt.show()
