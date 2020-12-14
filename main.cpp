/**
 * @file main.cpp
 * @author Jun Yi (Jason) Shi (shij82@mcmaster.ca)
 * @version 0.1
 * @date 2020-12-13
 * @copyright Copyright (c) 2020
 * 
 * @brief The main file running optimization algorithms based on the data stored in input_function.txt
 */

#include <iostream>
#include "Input_Data.hpp"
#include "Result_Handling.hpp"

using namespace std;

int main()
{
    try
    {
        // (string) constructor: create an Input_Data class based on input_function.txt
        Input_Data input_f("input_function.txt");

        // (Input_Data, string) constructor: create a Result_Handling class by applying the gradient_descent algorithm
        Result_Handling result_f1(input_f, "gradient_descent");

        // (Input_Data, string) constructor: create a Result_Handling class by applying the newtons_method algorithm
        Result_Handling result_f2(input_f, "newtons_method");

        cout << "Successfully performed the optimization algorithms!" << '\n';
        cout << "Please check the following files:" << '\n';
        cout << "    output_results_gradient_descent.txt" << '\n';
        cout << "    output_results_newtons_method.txt" << '\n';
        cout << "    output_iterations_gradient_descent.txt" << '\n';
        cout << "    output_iterations_newtons_method.txt" << '\n';
    }
    catch (const exception &e)
    {
        cout << e.what() << '\n';
        return -1;
    }
}
