# include "rect_domain_copy.h"

/**
 * @brief Construct a new Rectangular Spatial Grid:: Rectangular Spatial Grid object
 * 
 * @param num_grid_1 Number of grids in x axis
 * @param num_grid_2 Number of grids in y axis
 * @param x_start Start point of x axis 
 * @param x_end End point of x axis
 * @param y_start Start point of y axis
 * @param y_end Start point of x axis
 */
RectangularSpatialGrid::RectangularSpatialGrid(
    int num_grid_1, 
    int num_grid_2, 
    double x_start, 
    double x_end, 
    double y_start, 
    double y_end): BaseSpatialGrid(num_grid_1, num_grid_2)
    {
        this -> x_start = x_start;
        this -> x_end = x_end;
        this -> y_start = y_start;
        this -> y_end = y_end ; 
        //infinitesimal_distance 1,2 = dx, dy 
        this -> infinitesimal_distance_1 = (x_end - x_start) / (num_grid_1 - 1);
        this -> infinitesimal_distance_2 = (y_end - y_start) / (num_grid_2 - 1);
        //For each GridPoint, set x, y position. Also, set wave_function as 0+0i default value
        for (auto i=0; i<num_grid_1; ++i){
            for(auto j=0; j<num_grid_2; ++j){
                this -> spatial_data[i][j]=GridPoint(this->x_start+ infinitesimal_distance_1*i,
                                                     this->y_start+ infinitesimal_distance_2*j,
                                                     //{real value, imaginary value}
                                                     std::complex<double>{0,0});
            }
        }

}

/**
 * @brief Construct a new Rectangular Domain:: Rectangular Domain object
 * 
 * @param num_grid_1 Number of grids in x axis
 * @param num_grid_2 Number of grids in y axis
 * @param t_start Initial time
 * @param t_end Final time
 * @param num_times iteration number or number of time points
 * @param x_start Start point of x axis 
 * @param x_end End point of x axis
 * @param y_start Start point of y axis
 * @param y_end Start point of x axis
 */
RectangularDomain::RectangularDomain(
    int num_grid_1, 
    int num_grid_2, 
    double t_start, 
    double t_end, 
    int num_times,
    double x_start,
    double x_end, 
    double y_start, 
    double y_end)
    : BaseDomain(num_grid_1, num_grid_2, t_start, t_end, num_times), 
    x_start(x_start), 
    x_end(x_end),
    y_start(y_start),
    y_end(y_end)
    {
        this -> old_grid = new RectangularSpatialGrid(num_grid_1, num_grid_2, x_start, x_end, y_start, y_end);
        this -> current_grid = new RectangularSpatialGrid(num_grid_1, num_grid_2, x_start, x_end, y_start, y_end);
        

    };

    double RectangularDomain::get_x_start()
    {
        return this->x_start;
    }
    double RectangularDomain::get_y_start(){
        return this->y_start;
    }
    double RectangularDomain::get_x_end(){
        this->x_end;
    }
    double RectangularDomain::get_y_end(){
        this->x_end;
    }