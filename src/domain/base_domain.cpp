#include "base_domain.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

GridPoint::GridPoint(float x_, float y_, std::complex<float> wave_function_) : x(x_), y(y_), value(wave_function_){}
GridPoint::~GridPoint(){};

BaseSpatialGrid::BaseSpatialGrid(int num_grid_1, int num_grid_2)
{
    this->num_grid_1 = num_grid_1;
    this->num_grid_2 = num_grid_2;
    //Spatial_data: 2D matrix of grid points 
    this->spatial_data = std::vector<std::vector<GridPoint>>(num_grid_1);
    for (auto i = 0; i < num_grid_1; ++i)
    {
        this->spatial_data[i] = std::vector<GridPoint>(num_grid_2);
    }
}

void BaseSpatialGrid::normalize(){
    float sum = 0.;
    for (auto i = 0; i < this->num_grid_1; ++i){
        for (auto j = 0; j < this->num_grid_2;++j){
            auto wave_func = this->at(i, j)->value;
            sum += std::pow(std::abs(wave_func), 2);
        }
    }
    sum = std::sqrt(sum * this->infinitesimal_distance_1 * this->infinitesimal_distance_2);
    for (auto i = 0; i < this->num_grid_1; ++i)
    {
        for (auto j = 0; j < this->num_grid_2; ++j)
        {
            this->at(i, j)->value /= sum;
        }
    }
}
BaseSpatialGrid::~BaseSpatialGrid(){

    for (auto i = 0; i < num_grid_1; ++i)
    {
        this->spatial_data[i].clear();
        std::vector<GridPoint>().swap(this->spatial_data[i]);

    }this ->spatial_data.clear();
    std::vector<std::vector<GridPoint>>().swap(this ->spatial_data);
}

// Getter function of infinitesimal_distance1
float BaseSpatialGrid::get_infinitesimal_distance1()
{
    return this->infinitesimal_distance_1;
}

// Getter function of infinitesimal_distance2
float BaseSpatialGrid::get_infinitesimal_distance2()
{
    return this->infinitesimal_distance_2;
}
// Getter function of grid point at index_1, index_2.
// roll up for boundary
GridPoint *BaseSpatialGrid::at(int index_1, int index_2)
{
    return &this->spatial_data[index_1 % this->num_grid_1][index_2 % this->num_grid_2];
}

BaseDomain::BaseDomain(
    int num_grid_1,
    int num_grid_2,
    float t_start,
    float t_end,
    int num_times)
{
    this->t_start = t_start;
    this->t_end = t_end;
    this->num_times = num_times;
    // dt: interval of time
    this->dt = (t_end - t_start) / (num_times - 1);
    this->num_grid_1 = num_grid_1;
    this->num_grid_2 = num_grid_2;
    this->old_grid = new BaseSpatialGrid(num_grid_1, num_grid_2);
    this->current_grid= new BaseSpatialGrid(num_grid_1, num_grid_2);
    this -> null_gridpt = new GridPoint(0,0,std::complex<float> {0});
    
}
BaseDomain::~BaseDomain(){
    free (this->old_grid);
    free (this->current_grid);
}
void BaseDomain::normalize(int time_index){
    this->current_grid->normalize();
}
// Getter functions
float BaseDomain::get_t_start()
{
    return this->t_start;
}

float BaseDomain::get_t_end()
{
    return this->t_end;
}
float BaseDomain::get_dt()
{
    return this->dt;
}
int BaseDomain::get_num_times()
{
    return this->num_times;
}

int BaseDomain::get_num_grid_1()
{
    return this->num_grid_1;
}
int BaseDomain::get_num_grid_2()
{
    return this->num_grid_2;
}


float BaseDomain::get_infinitesimal_distance1()
{
    return this->old_grid->get_infinitesimal_distance1();
}
float BaseDomain::get_infinitesimal_distance2()
{
    return this->old_grid->get_infinitesimal_distance2();
}

// Getter function of grid point at index_1, index_2 and time_index
GridPoint *BaseDomain::at(int index_1, int index_2, int time_index)
{
    if(time_index == this -> current_time_index){
        return this ->current_grid->at(index_1, index_2);
    }else if (time_index == this -> current_time_index-1 ){
        return this -> old_grid -> at(index_1, index_2);
    }
    else{
        std::cout<<"error in base domin at function"<<std::endl;
        return this ->current_grid->at(index_1, index_2);
    }
}
//Getter function for boundary 
GridPoint* BaseDomain::get_null_gridpt(){
    return this->null_gridpt;
}


/**
 * @brief Assign initial value
 *
 * @param index_1 x = x_start + index_1*dx
 * @param index_2 y = y_start + index_2 * dy
 * @param value initial value at x, y
 */
void BaseDomain::assign_initial_value(int index_1, int index_2, std::complex<float> value)
{
    this->at(index_1, index_2, 0)->value = value;
}
void BaseDomain::assign_wave_function(int index_1, int index_2, int time_index, std::complex<float> value)
{
    this->at(index_1, index_2, time_index)->value = value;
}
/**
 * @brief create directory and return directory name with "/"
 *         directory name : "./results/%d-%m-%Y %H-%M-%S_"+info
 *         for exmaple, "./results/"
 * @param info  information about domain type and solver type
 * @return std::string directory name with "/"
 */
void BaseDomain::generate_directory_name(std::string info)
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    auto str = oss.str();
    std::string directory_name = "../results/" + str + "_" + info;

    if (fs::create_directory(directory_name.c_str()))
    {
        std::cout << "Created directory " << directory_name << std::endl;
    }
    else
    {
        std::cout << "Creating directory failed" << std::endl;
    };
    this->PATH =  directory_name + "/";
}

/**
 * @brief export results of probability(|psi|^2) as txt
 *        it generates (num_times) txt files
 *        each txt file contains  |psi|^2 data
 *
 * @param grid grid at t
 * @param filename file name to store data
 */
void BaseDomain::generate_single_txt_file(std::string filename)
{
    std::ofstream outfile(this->PATH+filename + ".txt");
    outfile << "x, y, probability" << std::endl;
    for (auto i = 0; i < num_grid_1; ++i)
    {
        for (auto j = 0; j < num_grid_2; ++j)
        {       
            float magnitude = std::abs(this->current_grid->at(i, j)->value);
            outfile <<this->current_grid->at(i, j)->x << ", " << this->current_grid->at(i, j)->y << ", ";
            outfile << magnitude * magnitude;
            outfile << std::endl;
        }
    }
    outfile.close();
    //After saving data, update domain 
    this -> update_time();
};

void BaseDomain::print_directory_info(){
    std::cout << this->num_times;
    std::cout << " text files are generated in \n";
    std::cout << this->PATH << std::endl;
}

void BaseDomain::update_time(){

    this -> current_time_index+=1; 
    //free( this -> old_grid);
    this -> old_grid = this ->current_grid;
    this -> current_grid = new BaseSpatialGrid(num_grid_1, num_grid_2);
}