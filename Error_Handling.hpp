/**
 * @file Error_Handling.hpp
 * @author Jun Yi (Jason) Shi (shij82@mcmaster.ca)
 * @version 0.1
 * @date 2020-12-13
 * @copyright Copyright (c) 2020
 * 
 * @brief A header file containing most of the exceptions that are used in this program
 */

#pragma once

using namespace std;

/**
 * @brief Exception to be thrown if the program fails to open the input file input_function.txt
 */
class err_fail_to_read_func_txt : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        return "Error occurred when opening the text file input_function.txt";
    }
};

/**
 * @brief Exception to be thrown if the program fails to open the output file(s) output_results
 */
class err_fail_to_read_results_txt : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        return "Error occurred when opening the output_results text file(s)";
    }
};

/**
 * @brief Exception to be thrown if the program fails to open the output file(s) output_iterations
 */
class err_fail_to_read_iter_txt : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        return "Error occurred when opening the output_iterations text file(s)";
    }
};

/**
 * @brief Exception to be thrown if the input file input_function.txt has more than 5 lines
 */
class err_more_than_5_lines : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid input: more than 5 lines in input_function.txt\n"
                              "input_function.txt needs to have exactly 5 lines:\n"
                              "max/min, polynomial, initial point, tolerance, and max iterations";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the input file input_function.txt has less than 5 lines
 */
class err_less_than_5_lines : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid input: less than 5 lines in input_function.txt\n"
                              "input_function.txt needs to have exactly 5 lines:\n"
                              "max/min, polynomial, initial point, tolerance, and max iterations";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the first line of input_function.txt is invalid
 */
class err_invalid_first_line : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid first line\n"
                              "Please have either \"min\" or \"max\" on the first line";
        return message;
    }
};

/**
 * @brief Exception to be thrown if an equal sign '=' is not found in the polynomial string
 */
class err_missing_equal_sign : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid polynomial function on the second line.\n"
                              "Error: the polynomial is missing an equal sign\n"
                              "See README.md for a valid polynomial function";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the polynomial's LHS, the expression on the left side of '=' is invalid
 */
class err_invalid_lhs_poly : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid polynomial function on the second line.\n"
                              "Error: the left-hand side of the polynomial is invalid\n"
                              "See README.md for a valid polynomial function";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the polynomial's RHS, the expression on the right side of '=' is invalid
 */
class err_invalid_rhs_poly : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid polynomial function on the second line.\n"
                              "Error: the right-hand side of the polynomial is invalid\n"
                              "Please expand and simplify the polynomial function\n"
                              "See README.md for a valid polynomial function";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the third line of input_function.txt is invalid
 */
class err_invalid_third_line : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid third line: Failed to store the initial point\n"
                              "A valid third line has the format initial_point=x_1,x_2,...\n"
                              "For example: \"initial_point=1,0,-1\"\n"
                              "See README.md for more information on a valid initial point";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the initial point entered in input_function.txt is invalid
 */
class err_invalid_initial_point : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid third line: Failed to store the initial point\n"
                              "The initial point needs to have the same number of x variables as\n"
                              "the number of x variables of the input function.";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the fourth line of input_function.txt is invalid
 */
class err_invalid_fourth_line : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid fourth line: Failed to store the tolerance\n"
                              "A valid fourth line has the format tolerance=...\n"
                              "For example: \"tolerance=0.0001\"\n"
                              "See README.md for more information on a valid tolerance";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the tolerance entered in input_function.txt is invalid
 */
class err_invalid_tolerance : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Tolerance cannot be zero or negative\n"
                              "See README.md for more information on a valid tolerance";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the fifth line of input_function.txt is invalid
 */
class err_invalid_fifth_line : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Invalid fifth line: Failed to store the max_iter\n"
                              "A valid fifth line has the format max_iter=...\n"
                              "For example: \"max_iter=10000\"\n"
                              "See README.md for more information on a valid max_iter";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the max_iter entered in input_function.txt is invalid
 */
class err_invalid_max_iter : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "max_iter cannot be zero or negative\n"
                              "See README.md for more information on a valid tolerance";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the given x vector and the polynomial have different numbers of x variables
 */
class err_invalid_x_vector : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Error: failed to evaluate the algorithm\n"
                              "The input x vector needs to have the same number of x variables as\n"
                              "the number of x variables of the input Polynomial.";
        return message;
    }
};

/**
 * @brief Exception to be thrown if the input algorithm string is not "gradient_descent" or "newtons_method"
 */
class err_unknown_algorithm : public exception // derived from std::exception
{
    const char *what() const throw()
    {
        const char *message = "Unknown algorithm, Result_Handling only supports gradient_descent/newtons_method";
        return message;
    }
};
