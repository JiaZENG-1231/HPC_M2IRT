from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


# The script is run from the project root directory.
data_path = Path("results/particle_motion.csv")
figure_path = Path("results/boris_validation.png")

# Read the column names and simulation values from the CSV file.
data = np.genfromtxt(
    data_path,
    delimiter=",",
    names=True,
)

time = data["time"]

x = data["x"]
y = data["y"]

vx = data["vx"]
vy = data["vy"]

speed = data["speed"]

bx = data["Bx"]
by = data["By"]
bz = data["Bz"]


fig, axes = plt.subplots(
    1,
    3,
    figsize=(15, 4.5),
)


# Particle orbit in the plane perpendicular to the magnetic field.
axes[0].plot(x, y, color="tab:blue", linewidth=1.5)
axes[0].scatter(x[0], y[0], color="black", label="Start", zorder=3)

axes[0].set_xlabel("x")
axes[0].set_ylabel("y")
axes[0].set_title("Particle trajectory")
axes[0].set_aspect("equal")
axes[0].grid(alpha=0.3)
axes[0].legend()


# Boris should conserve speed when only a magnetic field is present.
axes[1].plot(time, speed, color="tab:red")

axes[1].set_xlabel("Time")
axes[1].set_ylabel(r"$|\mathbf{v}|$")
axes[1].set_title("Speed conservation")
axes[1].grid(alpha=0.3)


# Magnetic-field components used during the simulation.
axes[2].plot(time, bx, label=r"$B_x$")
axes[2].plot(time, by, label=r"$B_y$")
axes[2].plot(time, bz, label=r"$B_z$")

axes[2].set_xlabel("Time")
axes[2].set_ylabel("Magnetic field")
axes[2].set_title("Applied magnetic field")
axes[2].grid(alpha=0.3)
axes[2].legend()


fig.tight_layout()
fig.savefig(figure_path, dpi=200)

print(f"Figure saved in {figure_path}")

plt.show()