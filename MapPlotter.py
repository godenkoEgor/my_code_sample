import matplotlib.pyplot as plt
import numpy as np


"""
Map distribution (of density, speed, temperature ...) at zero plane (X = 0, Y = 0, Z = 0).

'x-direction' is the solar rotation axis;
'z-direction' is in the plane containing solar rotation axis and velocity vector of undisturbed LISM,
perpendicular to the x-axis;
'y-direction' is the direction for the right-handled coordinate system.
"""


def plot_map(plane, year, direction, title=None):
    """
    Function for the plotting of 2D distribution (with value by colors).
    :param plane: coordinate of the plane.
    :param year: year corresponding to the plane.
    :param direction: coordinate-axis of the plane ("X", "Y" or "Z").
    :param title: title of the plot.
    """

    filename = "Maps/" + direction + "_slice_plane_" + plane + "_time_" + year + ".dat"

    coordinate_1 = []
    coordinate_2 = []
    density = []

    with open(filename) as f:
        init_data = f.readline().split()
        count_1, count_2 = map(int, init_data[3:])

        for line in f:
            data_list = list(map(float, line.split()))
            coordinate_1.append(data_list[0])
            coordinate_2.append(data_list[1])
            density.append(data_list[2])

    coordinate_1 = np.array(coordinate_1).reshape(count_1, count_2)
    coordinate_2 = np.array(coordinate_2).reshape(count_1, count_2)
    density = np.array(density).reshape(count_1, count_2)

    cr = plt.contourf(coordinate_2, coordinate_1, density, cmap='jet', extend='both',
                      levels=list(np.linspace(0.0, 5.0, 101)))

    cbar = plt.colorbar(cr, ticks=list(np.linspace(0.0, 5.0, 11)))

    # amount of ticks on both directions
    plt.xticks()
    plt.yticks()

    # grid lines
    # plt.grid(linestyle='--', linewidth=0.5)

    if title is None:
        plt.title('Density, σ = 0 m/s, Plane ' + direction + ' = ' + str(int((float(plane)) / 2)), color='black')
    else:
        plt.title('Density, σ = 0 m/s, Plane ' + direction + ' = ' + str(int((float(plane)) / 2)) + ", size: " +
                  title + " mcm", color='black')
    if direction == "X":
        plt.xlim(-400, 400)
        plt.ylim(400, -400)
        plt.xlabel('Z (a.u.)', size=20)
        plt.ylabel('Y (a.u.)', size=20)
    elif direction == "Y":
        plt.axis([-400, 400, -400, 400])
        plt.xlabel('Z (a.u.)', size=20)
        plt.ylabel('X (a.u.)', size=20)
    elif direction == "Z":
        plt.xlim(400, -400)
        plt.ylim(-400, 400)
        plt.xlabel('Y (a.u.)', size=20)
        plt.ylabel('X (a.u.)', size=20)
    else:
        print("Unexpected direction!")
    cbar.ax.set_ylabel('N / N_0', size=20)


plot_map("0.000000", "0.0", "X")
plt.show()
