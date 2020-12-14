/**
 * @file Result_Handling.hpp
 * @author Jun Yi (Jason) Shi (shij82@mcmaster.ca)
 * @version 0.1
 * @date 2020-12-13
 * @copyright Copyright (c) 2020
 * 
 * @brief A Result_Handling class that process the Input_Data class and store results
 * 
 * @details This library contains a class Result_Handling to handle results from Input_Data.
 * Result_Handling takes an Input_Data and a string representation of the optimization algorithm
 * to perform. The constructor implements the corresponding algorithm and record its runtime,
 * then it stores all the results from the optimization algorithm to separate text files
 * in the current workspace folder
 */

#pragma once
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "Input_Data.hpp"
#include "matrix.hpp"
#include "vector_overloads.hpp"
#include "Error_Handling.hpp"

using namespace std;

/**
 * @brief A class for result handling
 */
class Result_Handling
{
public:
    // ============
    // Constructors
    // ============

    /**
     * @brief Construct a new Result_Handling object
     * @param input An Input_Data class that stores the input data from input_function.txt
     * @param algorithm_str A string representation of the optimization algorithm to perform
     * @throws err_fail_to_read_results_txt if the program fails to open the output file(s) output_results
     * @throws err_unknown_algorithm if the input algorithm string is not "gradient_descent" or "newtons_method"
     * 
     * @details The constructor implements the corresponding algorithm and record its runtime,
     * then it stores all the results from the optimization algorithm to separate text files
     * in the current workspace folder
     */
    Result_Handling(Input_Data &input, const string &algorithm_str)
    {
        int case_num{0};
        chrono::duration<double> elapsed_time_seconds{0};

        ofstream store_results{"output_results_" + algorithm_str + ".txt"};
        if (!store_results)
            throw err_fail_to_read_results_txt();

        if (algorithm_str == "gradient_descent")
        {
            store_results << "========== Gradient Descent/Ascent Results =========="
                          << "\n\n";
            // time the algorithm
            chrono::time_point start_time{chrono::steady_clock::now()};
            case_num = input.gradient_descent();
            chrono::time_point end_time{chrono::steady_clock::now()};
            elapsed_time_seconds = end_time - start_time;
        }
        else if (algorithm_str == "newtons_method")
        {
            store_results << "========== Newton's Method Results =========="
                          << "\n\n";
            // time the algorithm
            chrono::time_point start_time{chrono::steady_clock::now()};
            case_num = input.newtons_method();
            chrono::time_point end_time{chrono::steady_clock::now()};
            elapsed_time_seconds = end_time - start_time;
        }
        else
        {
            // algorithm other than gradient_descent or newtons_method is entered
            store_results.close();
            throw err_unknown_algorithm();
        }

        store_results << "min/max: " << input.get_min_max() << '\n';
        store_results << input.get_polynomial() << '\n';
        store_results << "initial_point: " << input.get_x_initial();
        store_results << "tolerance: " << input.get_tolerance() << '\n';
        store_results << "max_iter: " << input.get_max_iter() << '\n';
        store_results << "coeff_exp_mtx:" << '\n';
        store_results << input.get_polynomial().get_coeff_exp_mtx();

        store_results << "The algorithm took " << elapsed_time_seconds.count() << " seconds"
                      << "\n\n";

        // to improve readability, round the values of the current x point to 3 decimal places
        vector<long double> x_round_3{input.get_x_current()};
        for_each(x_round_3.begin(), x_round_3.end(),
                 [](long double &n) { n = round(n * 1000) / 1000; });

        switch (case_num)
        {
        case 1:
            // Case # 1: the current x point is now updated to a stationary point
            store_results << "Case 1:" << endl;
            if (algorithm_str == "newtons_method")
                // Newton's method can only be used to find a stationary point
                store_results << "A stationary point is found at: ";
            else if (input.get_min_max() == 1)
                store_results << "A local maximum is found at: ";
            else
                store_results << "A local minimum is found at: ";
            store_results << x_round_3;
            break;
        case 2:
            // Case # 2: reached the given maximum number of iterations,
            //           the current x point is updated, but it might not be a stationary point
            store_results << "Case 2:" << endl;
            store_results << "Reached the maximum number of iterations, " << endl;
            store_results << "The current point is at: " << x_round_3;
            store_results << "A point at nan might suggest that there is no stationary point around the initial point." << endl;
            break;
        case 3:
            // Case # 3: the norm of current gradient is extremely large,
            //           algorithm is stopped to prevent overflow
            store_results << "Case 3:" << endl;
            store_results << "The gradient norm of the current point is extremely large," << endl;
            store_results << "The algorithm is stopped to prevent overflow," << endl;
            store_results << "The current point is at: " << x_round_3;
            store_results << "This might suggest that there is no stationary point around the initial point," << endl;
            store_results << "or that the current x values are too large/small." << endl;
            break;
        default:
            // failed to perform the optimization algorithm
            store_results << "Failed to perform the optimization algorithm, please check the input polynomial" << endl;
            break;
        }
        store_results.close();
    }
};