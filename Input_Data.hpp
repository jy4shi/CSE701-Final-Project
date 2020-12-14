/**
 * @file Input_Data.hpp
 * @author Jun Yi (Jason) Shi (shij82@mcmaster.ca)
 * @version 0.1
 * @date 2020-12-13
 * @copyright Copyright (c) 2020
 * 
 * @brief An Input_Data class that can be used to validate the stored data from a text file
 * and evaluate certain values based on the input data
 */

#pragma once
#include <string>
#include <vector>
#include "Polynomial.hpp"

using namespace std;

/**
 * @brief A class for input data
 */
class Input_Data
{
public:
    // ======================================================================
    // Constructor of Input_Data
    // ======================================================================

    /**
     * @brief Construct a new Input_Data::Input_Data object
     * @param filename The name of a text file where the input data are stored
     * @throws err_fail_to_read_func_txt if the program fails to open the input file input_function.txt
     * @throws err_more_than_5_lines if the input file input_function.txt has more than 5 lines
     * @throws err_less_than_5_lines if the input file input_function.txt has less than 5 lines
     * @throws err_invalid_first_line if the first line of input_function.txt is invalid
     * @throws err_missing_equal_sign if an equal sign '=' is not found in the polynomial string
     * @throws err_invalid_lhs_poly if the polynomial's LHS, the expression on the left side of '=' is invalid
     * @throws err_invalid_rhs_poly if the polynomial's RHS, the expression on the right side of '=' is invalid
     * @throws err_invalid_third_line if the third line of input_function.txt is invalid
     * @throws err_invalid_initial_point if the initial point entered in input_function.txt is invalid
     * @throws err_invalid_fourth_line if the fourth line of input_function.txt is invalid
     * @throws err_invalid_tolerance if the tolerance entered in input_function.txt is invalid
     * @throws err_invalid_fifth_line if the fifth line of input_function.txt is invalid
     * @throws err_invalid_max_iter if the max_iter entered in input_function.txt is invalid
     */
    Input_Data(const string &);

    // ======================================================================
    // Public member functions of Input_Data: get values
    // ======================================================================

    /**
     * @brief Public member function used to obtain (but not modify) the min_max value 
     * stored in Input_data
     * @return int 1 represents maximum, -1 represents minimum
     */
    int get_min_max() const;

    /**
     * @brief Public member function used to obtain (but not modify) the Polynomial
     * stored in Input_data
     * @return Polynomial Generated based on the polynomial string from input_function.txt
     */
    Polynomial get_polynomial() const;

    /**
     * @brief Public member function used to obtain (but not modify) the x_initial
     * value stored in Input_data
     * @return vector<long double> A vector of x values generated based on 
     * the initial_point from input_function.txt
     */
    vector<long double> get_x_initial() const;

    /**
     * @brief Public member function used to obtain (but not modify) the x_current
     * value stored in Input_data
     * @return vector<long double> A vector of the current x values generated after
     * performing the optimization algorithms
     */
    vector<long double> get_x_current() const;

    /**
     * @brief Public member function used to obtain (but not modify) the tolerance
     * value stored in Input_data
     * @return long double Generated based on the tolerance from input_function.txt
     */
    long double get_tolerance() const;

    /**
     * @brief Public member function used to obtain (but not modify) the max_iter
     * value stored in Input_data
     * @return size_t Generated based on the max_iter from input_function.txt
     */
    size_t get_max_iter() const;

    // ======================================================================
    // Public member functions of Input_Data: evaluate/perform algorithms
    // ======================================================================

    /**
     * @brief Public member function used to perform the gradient descent algorithm based on Input_Data, 
     * in order to improve efficiency, the step size is determined using the backtracking line search method
     * @return int An integer that represents the case number <br> 
     * - Case # 1: the current x point is now updated to a stationary point <br>
     * - Case # 2: reached the given maximum number of iterations, 
     * the current x point is updated, but it might not be a stationary point <br>
     * - Case # 3: the norm of current gradient is extremely large, algorithm is stopped to prevent overflow
     * @details Gradient descent/ascent algorithm can be used to find either a local minimum or a local maximum,
     * which is based on the input data that the user provided in input_function.txt
     */
    int gradient_descent();

    /**
     * @brief Public member function used to perform the newton's method algorithm based on Input_Data,
     * @return int An integer that represents the case number <br> 
     * - Case # 1: the current x point is now updated to a stationary point <br>
     * - Case # 2: reached the given maximum number of iterations, 
     * the current x point is updated, but it might not be a stationary point <br>
     * - Case # 3: the norm of current gradient is extremely large, algorithm is stopped to prevent overflow
     * @details Due to the nature of the newton's method algorithm, it can only be used to find a stationary point
     * near the initial point, that is, it cannot specifically find a local maximum or a local minimum
     */
    int newtons_method();

private:
    // ======================================================================
    // Private data of Input_Data
    // ======================================================================

    int min_max{-1};                // default to minimize polynomial
    Polynomial polyn;               // create a default Polynomial
    vector<long double> x_initial;  // vector of the initial point, initialized to be empty
    vector<long double> x_current;  // vector of the current point, initialized to be empty
    long double tolerance{0.0001L}; // default tolerance
    size_t max_iter{10000};         // default maximum number of iterations

    string first_line_str{""};  // store min/max
    string second_line_str{""}; // store polynomial function in string
    string third_line_str{""};  // store initial point in string
    string fourth_line_str{""}; // store tolerance in string
    string fifth_line_str{""};  // store maximum number of iterations in string

    // ======================================================================
    // Private member functions of Input_Data: validate input data
    // ======================================================================

    /**
     * @brief Private member function used to modify and remove all spaces in the given string
     * @param str_to_remove a string that may contain spaces
     */
    void remove_spaces(string &);

    /**
     * @brief Private member function used to check the validity of the text file overall, 
     * and then store each line privately in Input_Data
     * @param filename_cvf The name of a text file where the input data are stored
     * @throws err_fail_to_read_func_txt if the program fails to open the input file input_function.txt
     * @throws err_more_than_5_lines if the input file input_function.txt has more than 5 lines
     * @throws err_less_than_5_lines if the input file input_function.txt has less than 5 lines
     */
    void check_valid_file(const string &);

    /**
     * @brief Private member function used to check the validity of 
     * the first line stored in Input_Data, if the first line is a valid min_max
     * value, then store the min_max value privately in Input_Data
     * @throws err_invalid_first_line if the first line of input_function.txt is invalid
     */
    void check_valid_first_line();

    /**
     * @brief Private member function used to check the validity of 
     * the second line stored in Input_Data, if the second line is a valid Polynomial, 
     * then store the Polynomial privately in Input_Data
     * @throws err_missing_equal_sign if an equal sign '=' is not found in the polynomial string
     * @throws err_invalid_lhs_poly if the polynomial's LHS, the expression on the left side of '=' is invalid
     * @throws err_invalid_rhs_poly if the polynomial's RHS, the expression on the right side of '=' is invalid
     */
    void check_valid_second_line();

    /**
     * @brief Private member function used to check the validity of 
     * the third line stored in Input_Data, if the third line is a valid x_initial
     * value, then store the x_initial value privately in Input_Data
     * @throws err_invalid_third_line if the third line of input_function.txt is invalid
     * @throws err_invalid_initial_point if the initial point entered in input_function.txt is invalid
     */
    void check_valid_third_line();

    /**
     * @brief Private member function used to check the validity of 
     * the fourth line stored in Input_Data, if the fourth line is a valid tolerance
     * value, then store the tolerance value privately in Input_Data
     * @throws err_invalid_fourth_line if the fourth line of input_function.txt is invalid
     * @throws err_invalid_tolerance if the tolerance entered in input_function.txt is invalid
     */
    void check_valid_fourth_line();

    /**
     * @brief Private member function used to check the validity of 
     * the fifth line stored in Input_Data, if the fifth line is a valid max_iter
     * value, then store the max_iter value privately in Input_Data
     * @throws err_invalid_fifth_line if the fifth line of input_function.txt is invalid
     * @throws err_invalid_max_iter if the max_iter entered in input_function.txt is invalid
     */
    void check_valid_fifth_line();

    // ======================================================================
    // Private member functions of Input_Data: evaluate/perform algorithms
    // ======================================================================

    /**
     * @brief Private member function used to evaluate the euclidean norm of a given vector
     * @param vec_en A vector of long doubles that is used to get its euclidean norm
     * @return long double The euclidean norm of the given vector 
     */
    long double euclidean_norm(const vector<long double> &) const;

    /**
     * @brief Private member function used to perform the backtracking line search algorithm
     * @param x_vec_bls A vector of long doubles that stores the current x values
     * @param curr_gradient_bls A vector of long doubles that stores the current gradient vector
     * @param directional_bls A vector of long doubles that stores the current directional vector
     * @return long double An efficient step size for the gradient descent algorithm
     */
    long double backtracking_line_search(const vector<long double> &x_vec_bls,
                                         const vector<long double> &curr_gradient_bls,
                                         const vector<long double> &directional_bls) const;
};