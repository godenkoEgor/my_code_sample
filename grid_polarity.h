#pragma once

#include <fstream>
#include <vector>


#include "grid_plasma.h"
#include "point.h"
#include "parameters.h"
#include "constants.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Structure for cells of the polarity grid.
struct CellPolarity {

// [au], coordinates of the cell's center
    double x_center;
    double y_center;
    double z_center;

// [au], coordinates of the corresponding stellar wind particle at the sphere of radius "BACK_RADIUS". 
    double x_sphere;
    double y_sphere;
    double z_sphere;

// [s], back time from the center of the cell to the point at "BACK_RADIUS" sphere.
    double time_duration;

// ID number of the cell in the grid.
    int index;

// Identificator: is cell inside the heliosphere or not?
    int inside_heliosphere;
};

// Print operator for class CellPolarity.
std::ofstream& operator<<(std::ofstream& out, CellPolarity cell);

// Scan operator for class CellPolarity.
std::ifstream& operator>>(std::ifstream& in, CellPolarity &cell);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Axes of the grid for the polarity (as for the dust modelling):
// z-axis: antiparallel to the undisturbed LISM flow.
// Same to the axes from Izmodenov & Alexashov, 2015.


class GridPolarity {
public:
// Fill cells by default values. This constructor is for the BUILDING polarity grid.
	GridPolarity();

// Fill cells by data from file "filename". This constructor is for the USING polarity grid.
	GridPolarity(std::string filename);

// Get ID number of the cell which contains point with coordinates x, y, z (dimensional form, polarity axes).
    int GetCell(double x, double y, double z) const;

// Fill cells by the simulating of solar wind tracks in back to the time.
    void FillCells(GridPlasma &grid_plasma);

// Make a heliospheric current sheet file at the time moment "year"
    void MakeHCS(double year, TransitionMatrix transition_matrix);

// Make a heliospheric current sheet file at the moment year for Parker's solution.
//   void MakeParkerHCS(double year); 

// Getter for cells_.
    const std::vector<CellPolarity>& GetCells() const;

private:
// Vector of cells of the grid.
    std::vector<CellPolarity> cells_;

// Initialize cells of the grid: fill cooridnates of centers, sizes of cells, ID number.
    void initCells();

// Scan cells from file "filename".
    void scanCells(std::string filename);

// Fill the cell with ID number idx by calculated parameters.
    void fillCellPolarity(GridPlasma &grid_plasma, size_t idx);

// Complete the cell with 
    void completeCell(Pack finish, size_t idx);

// Get finish point by runge-kutta back integration.
    Pack getFinishPoint(GridPlasma &grid_plasma, size_t idx);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Function for calculating of polarity at the sphere with radius 1 AU.
double DeterminePolarity(double t, Point p, TransitionMatrix transition_matrix);

// Function for checking: is point inside the heliosphere? (Point is in dimensionless form)
bool InsideGridPolarity(Point p);
