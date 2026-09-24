from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


data_path = Path("results/multi_particle_motion.csv")
figure_path = Path("results/multi_particle_validation.png")

# Read all particle states from the simulation output.
data = np.genfromtxt(
    data_path,
    delimiter=",",
    names=True,
)

particle_ids = np.unique(data["particle"]).astype(int)

fig, axes = plt.subplots(
    1,
    3,
    figsize=(15, 4.5),
)


for particle_id in particle_ids:
    # Select the rows belonging to one particle.
    mask = data["particle"] == particle_id
    particle = data[mask]

    charge = particle["q"][0]
    initial_speed = particle["speed"][0]

    label = (
        f"Particle {particle_id}: "
        f"q={charge:.0f}, "
        f"|v₀|={initial_speed:.1f}"
    )

    # Plot the orbit in the plane perpendicular
    # to the applied magnetic field.
    axes[0].plot(
        particle["x"],
        particle["y"],
        linewidth=1.5,
        label=label,
    )

    # Mark the initial position of each particle.
    axes[0].scatter(
        particle["x"][0],
        particle["y"][0],
        s=20,
        color="black",
        zorder=3,
    )

    # Check that the magnetic field does not change speed.
    axes[1].plot(
        particle["time"],
        particle["speed"],
        linewidth=1.5,
        label=f"Particle {particle_id}",
    )

    # The y position shows the opposite rotation
    # of positive and negative particles.
    axes[2].plot(
        particle["time"],
        particle["y"],
        linewidth=1.5,
        label=f"Particle {particle_id}",
    )


# Particle trajectories in the x-y plane.
axes[0].set_xlabel("x")
axes[0].set_ylabel("y")
axes[0].set_title("Multi-particle trajectories")
axes[0].set_aspect("equal")
axes[0].grid(alpha=0.3)
axes[0].legend(fontsize=8)


# Speed conservation in a purely magnetic field.
axes[1].set_xlabel("Time")
axes[1].set_ylabel(r"$|\mathbf{v}|$")
axes[1].set_title("Speed conservation")
axes[1].grid(alpha=0.3)
axes[1].legend(fontsize=8)


# Positive and negative charges move in opposite y directions.
axes[2].axhline(
    0.0,
    color="black",
    linewidth=0.8,
    alpha=0.5,
)

axes[2].set_xlabel("Time")
axes[2].set_ylabel("y")
axes[2].set_title("Particle y positions")
axes[2].grid(alpha=0.3)
axes[2].legend(fontsize=8)


fig.tight_layout()
fig.savefig(
    figure_path,
    dpi=200,
    bbox_inches="tight",
)

print(f"Figure saved in {figure_path}")

plt.show()