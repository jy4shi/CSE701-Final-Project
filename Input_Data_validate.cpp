#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Input_Data.hpp"
#include "Polynomial.hpp"
#include "Error_Handling.hpp"

using namespace std;

// ======================================================================
// Constructor of Input_Data
// ======================================================================
// see Input_Data.hpp for description

Input_Data::Input_Data(const string &filename)
    : polyn("f(x_1)=0") //initialize a Polynomial
{
    check_valid_file(filename);
    check_valid_first_line();
    check_valid_second_line();
    check_valid_third_line();
    check_valid_fourth_line();
    check_valid_fifth_line();
}

int Input_Data::get_min_max() const
{
    return min_max;
}

Polynomial Input_Data::get_polynomial() const
{
    return polyn;
}

vector<long double> Input_Data::get_x_initial() const
{
    return x_initial;
}

vector<long double> Input_Data::get_x_current() const
{
    return x_current;
}

long double Input_Data::get_tolerance() const
{
    return tolerance;
}

size_t Input_Data::get_max_iter() const
{
    return max_iter;
}

// ======================================================================
// Private member functions of Input_Data: validate input data
// ======================================================================
// see Input_Data.hpp for description

void Input_Data::remove_spaces(string &str_to_remove)
{
    str_to_remove.erase(remove_if(str_to_remove.begin(), str_to_remove.end(),
                                  [](const char &c) { return c == ' '; }),
                        str_to_remove.end());
}

void Input_Data::check_valid_file(const string &filename_cvf)
{
    ifstream input_file{filename_cvf};
    if (!input_file)
        throw err_fail_to_read_func_txt();

    size_t input_line_idx{0}; // input line index starts from 0
    string input_str{""};     // used for getline
    while (getline(input_file, input_str))
    {
        remove_spaces(input_str); // remove spaces first
        // now store each input line to the corresponding line_str
        if (input_line_idx == 0)
            first_line_str = input_str;
        else if (input_line_idx == 1)
            second_line_str = input_str;
        else if (input_line_idx == 2)
            third_line_str = input_str;
        else if (input_line_idx == 3)
            fourth_line_str = input_str;
        else if (input_line_idx == 4)
            fifth_line_str = input_str;
        else if (input_str == "") // newline at end of file
            continue;
        else if (input_line_idx > 4)
        {
            input_file.close();
            throw err_more_than_5_lines();
        }
        input_line_idx++;
    }
    input_file.close();

    // if any of the 5 lines are empty, invalid input
    if (first_line_str == "" or second_line_str == "" or third_line_str == "" or
        fourth_line_str == "" or fifth_line_str == "")
        throw err_less_than_5_lines();
}

void Input_Data::check_valid_first_line()
{
    // check valid first line
    if (first_line_str == "min")
        min_max = -1;
    else if (first_line_str == "max")
        min_max = 1;
    else
        throw err_invalid_first_line();
}

void Input_Data::check_valid_second_line()
{
    // check valid second line

    Polynomial temp_poly(second_line_str);
    polyn = temp_poly;
}

void Input_Data::check_valid_third_line()
{
    // check valid third line
    if (third_line_str.substr(0, 14) == "initial_point=")
    {
        stringstream initial_point_stream(third_line_str.substr(14, third_line_str.size() - 14));
        string curr_x_var_stream;
        // x values should be seperated by ','
        while (getline(initial_point_stream, curr_x_var_stream, ','))
        {
            try
            {
                long double initial_x_value{stold(curr_x_var_stream)};
                x_initial.push_back(initial_x_value);
            }
            catch (const std::invalid_argument &e)
            {
                throw err_invalid_third_line();
            };
        };
        if (x_initial.size() != polyn.get_num_of_x_vars())
            throw err_invalid_initial_point();
    }
    else
        throw err_invalid_third_line();
}

void Input_Data::check_valid_fourth_line()
{
    // check valid fourth line
    if (fourth_line_str.substr(0, 10) == "tolerance=")
    {
        try
        {
            tolerance = stod(fourth_line_str.substr(10, fourth_line_str.size() - 10));
        }
        catch (const std::invalid_argument &e)
        {
            throw err_invalid_fourth_line();
        }
        if (tolerance <= 0)
            throw err_invalid_tolerance();
    }
    else
        throw err_invalid_fourth_line();
}

void Input_Data::check_valid_fifth_line()
{
    // check valid fifth line
    if (fifth_line_str.substr(0, 9) == "max_iter=")
    {
        try
        {
            max_iter = stoull(fifth_line_str.substr(9, fifth_line_str.size() - 9));
        }
        catch (const std::invalid_argument &e)
        {
            throw err_invalid_fifth_line();
        }
        if (max_iter <= 0)
            throw err_invalid_max_iter();
    }
    else
        throw err_invalid_fifth_line();
}
