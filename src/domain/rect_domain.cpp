# include "rect_domain.h"
#include <iostream>
#include <fstream>
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
    float x_start, 
    float x_end, 
    float y_start, 
    float y_end): BaseSpatialGrid(num_grid_1, num_grid_2)
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
                                                     std::complex<float>{0,0});
            }
        }

}
RectangularSpatialGrid::~RectangularSpatialGrid(){};
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
    float t_start, 
    float t_end, 
    int num_times,
    float x_start,
    float x_end, 
    float y_start, 
    float y_end)
    : BaseDomain(num_grid_1, num_grid_2, t_start, t_end, num_times), 
    x_start(x_start), 
    x_end(x_end),
    y_start(y_start),
    y_end(y_end)
    {
        this -> old_grid = new RectangularSpatialGrid(num_grid_1, num_grid_2, x_start, x_end, y_start, y_end);
        this -> current_grid = new RectangularSpatialGrid(num_grid_1, num_grid_2, x_start, x_end, y_start, y_end);
        
    };
RectangularDomain::~RectangularDomain(){};

float RectangularDomain::get_x_start()
{
    return this->x_start;
}
float RectangularDomain::get_y_start(){
    return this->y_start;
}
float RectangularDomain::get_x_end(){
    this->x_end;
}
float RectangularDomain::get_y_end(){
    this->x_end;
}

void RectangularDomain::update_time(){
    this -> current_time_index+=1;
    free( this -> old_grid);
    this -> old_grid = &(*(this ->current_grid));
    //std::cout<<this->old_grid->at(1, 1)->wave_function<<std::endl;
    this -> current_grid = new RectangularSpatialGrid(num_grid_1, num_grid_2, x_start, x_end, y_start, y_end);

}
//replace function again since update_time function is changed
void RectangularDomain::generate_single_txt_file(std::string filename)
{
    std::ofstream outfile(this->PATH+filename + ".txt");
    outfile << "x, y, probability" << std::endl;
    for (auto i = 0; i < num_grid_1; ++i)
    {
        for (auto j = 0; j < num_grid_2; ++j)
        {
            float magnitude = std::abs(this->current_grid->at(i, j)->wave_function);
            outfile <<this->current_grid->at(i, j)->x << ", " << this->current_grid->at(i, j)->y << ", ";
            outfile << magnitude * magnitude;
            outfile << std::endl;
        }
    }
    outfile.close();
    //After saving data, update domain 
    this -> update_time();

};

