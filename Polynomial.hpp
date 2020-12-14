/**
 * @file Polynomial.hpp
 * @author Jun Yi (Jason) Shi (shij82@mcmaster.ca)
 * @version 0.1
 * @date 2020-12-13
 * @copyright Copyright (c) 2020
 * 
 * @brief A Polynomial class that can be used to validate the given polynomial string
 * and evaluate certain values based on the polynomial
 */

#pragma once
#include <string>
#include <vector>
#include "matrix.hpp"

using namespace std;

/**
 * @brief A class for polynomial
 */
class Polynomial
{
public:
    // ======================================================================
    // Constructor of Polynomial
    // ======================================================================

    /**
     * @brief Construct a new Polynomial:: Polynomial object
     * @param input_poly_str The string representation of a polynomial function
     * @throws err_missing_equal_sign if an equal sign '=' is not found in the polynomial string
     * @throws err_invalid_lhs_poly if the polynomial's LHS, the expression on the left side of '=' is invalid
     * @throws err_invalid_rhs_poly if the polynomial's RHS, the expression on the right side of '=' is invalid
     */
    Polynomial(const string &);

    // ======================================================================
    // Public member functions of Polynomial: get values
    // ======================================================================

    /**
     * @brief Public member function used to obtain (but not modify) 
     * the string representation of Polynomial
     * @return string The string representation of Polynomial
     */
    string get_poly_str() const;

    /**
     * @brief Public member function used to obtain (but not modify) 
     * the number of independent/x variables
     * @return size_t The number of independent/x variables
     */
    size_t get_num_of_x_vars() const;

    /**
     * @brief Public member function used to obtain (but not modify) 
     * the number of terms of Polynomial
     * @return size_t The number of terms of Polynomial
     */
    size_t get_num_of_terms() const;

    /**
     * @brief Public member function used to obtain (but not modify) 
     * the coefficient/exponent matrix
     * @return matrix<long double> The coefficient/exponent matrix <br>
     * - The coeff_exp_mtx matrix has the size (num_of_terms, 1 + num_of_x_vars)
     * - Each row stores a term from the polynomial function
     * - First column is the coefficient of the terms
     * - Other columns are the exponents of the x variables
     */
    matrix<long double> get_coeff_exp_mtx() const;

    // ======================================================================
    // Public member functions of Polynomial: evaluate functions
    // ======================================================================

    /**
     * @brief Public member function used to evaluate (but not modify) the polynomial 
     * f(x_1, x_2, ...) based on the given vector of x values [x_1, x_2, ...]
     * @param x_vars_ef A vector of long doubles that stores the values of [x_1, x_2, ...]
     * @return long double The resulting value of f(x_1, x_2, ...)
     * @throws err_invalid_x_vector if the given x vector and the polynomial have different numbers of x variables
     */
    long double eval_f(const vector<long double> &) const;

    /**
     * @brief Public member function used to evaluate (but not modify) the partial derivate of
     * f(x_1, x_2, ...) with respect to x_i, where i is the index of the x variable
     * @param x_vars_ed A vector of long doubles that stores the values of [x_1, x_2, ...]
     * @param x_idx_ed The index of the x variable (with respect to) used to evaluate partial derivate
     * @return long double The partial derivate of f(x_1, x_2, ...) with respect to x_i
     * @throws err_invalid_x_vector if the given x vector and the polynomial have different numbers of x variables
     */
    long double eval_df(const vector<long double> &, const size_t &) const;

    /**
     * @brief Public member function used to evaluate (but not modify) 
     * the gradient of f(x_1, x_2, ...) based on the given vector of x values
     * @param x_vars_gf A vector of long doubles that stores the values of [x_1, x_2, ...]
     * @return vector<long double> A vector of long doubles that represents the gradient of the given vector 
     * @throws err_invalid_x_vector if the given x vector and the polynomial have different numbers of x variables
     */
    vector<long double> gradient_f(const vector<long double> &) const;

    /**
     * @brief Public member function used to evaluate (but not modify) 
     * the hessian of f(x_1, x_2, ...) based on the given vector of x values
     * @param x_vars_hf A vector of long doubles that stores the values of [x_1, x_2, ...]
     * @return matrix<long double> A matrix of long doubles that represents the hessian of the given vector 
     * @throws err_invalid_x_vector if the given x vector and the polynomial have different numbers of x variables
     */
    matrix<long double> hessian_f(const vector<long double> &) const;

    /**
     * @brief Public member function used to evaluate (but not modify) 
     * the inverse hessian of f(x_1, x_2, ...) based on the given vector of x values
     * @param x_vars_ihf A vector of long doubles that stores the values of [x_1, x_2, ...]
     * @return matrix<long double> A matrix of long doubles that represents the inverse hessian of the given vector 
     * @throws err_invalid_x_vector if the given x vector and the polynomial have different numbers of x variables
     */
    matrix<long double> inverse_hessian_f(const vector<long double> &) const;

private:
    // ======================================================================
    // Private data of Polynomial
    // ======================================================================

    string poly_str;      // string representation of Polynomial
    size_t num_of_x_vars; // number of x variables in the polynomial function
    size_t num_of_terms;  // number of terms on the RHS of the polynomial function

    // A matrix storing the coefficients and the exponents of the polynomial function
    // coeff_exp_mtx matrix should have the size (num_of_terms, 1+num_of_x_vars)
    // Each row stores a term from the polynomial function
    // First column is the coefficient of the terms
    // Other columns are the exponents of the different x variables
    // Ex: polynomial function "f(x_1,x_2)=x_1^2-x_2^2" has the following coeff_exp_mtx:
    //     ( 1   2   0 )   first row represents:  1 * x_1^2 * x_2^0   <-  term index 0: x_1^2
    //     ( -1  0   2 )   second row represents: -1 * x_1^0 * x_2^2  <-  term index 1: -x_2^2
    matrix<long double> coeff_exp_mtx;

    // ======================================================================
    // Private member functions of Polynomial: validate polynomial string
    // ======================================================================

    /**
     * @brief Private member function used to check whether the given string represents
     * a valid non negative integer
     * @param int_str The string representation of the integer we want to check
     * @return true The string represents a non-negative integer
     * @return false The string does not represent a non-negative integer
     */
    bool valid_non_negative_int(const string &) const;

    /**
     * @brief Private member function used to check the validity of the expression 
     * on the left side (LHS) of the polynomial string. <br>
     * The private Polynomial data num_of_x_vars is updated if the LHS is valid
     * @param lhs_str_cvlp The string representation of the LHS of the polynomial function
     * @throws err_invalid_lhs_poly if the polynomial's LHS, the expression on the left side of '=' is invalid
     */
    void check_valid_lhs_poly(const string &);

    /**
     * @brief Private member function used to check the validity of the expression 
     * on the right side (RHS) of the polynomial string. <br>
     * The private Polynomial data num_of_terms and coeff_exp_mtx are updated if the RHS is valid
     * @param rhs_str_cvrp The string representation of the RHS of the polynomial function
     * @throws err_invalid_rhs_poly if the polynomial's RHS, the expression on the right side of '=' is invalid
     */
    void check_valid_rhs_poly(const string &);

    // ======================================================================
    // Private member functions of Polynomial: evaluate functions
    // ======================================================================

    /**
     * @brief Private member function used to evaluate the (i, j) cofactor of a given matrix
     * @param mtx_cof A square matrix of long doubles
     * @param mtx_size_cof The size of the square matrix
     * @param remove_row_cof The row index i of the (i, j) cofactor
     * @param remove_col_cof The col index j of the (i, j) cofactor
     * @return long double The (i, j) cofactor of the given matrix
     */
    long double cofactor(const matrix<long double> &,
                         const size_t &, const size_t &, const size_t &) const;

    /**
     * @brief Private member function used to evaluate the determinant of a given matrix
     * @param mtx_det A square matrix of long doubles
     * @param mtx_size_det The size of the square matrix
     * @return long double The determinant of the given matrix
     */
    long double determinant(const matrix<long double> &, const size_t &) const;

    /**
     * @brief Private member function used to evaluate the adjugate matrix of a given matrix
     * @param mtx_adj A square matrix of long doubles
     * @param mtx_size_adj The size of the square matrix
     * @return matrix<long double> The adjugate matrix of the given matrix
     */
    matrix<long double> adjugate_mtx(const matrix<long double> &, const size_t &) const;

    /**
     * @brief Private member function used to evaluate the inverse matrix of a given matrix
     * @param mtx_inv A square matrix of long doubles
     * @param mtx_size_inv The size of the square matrix
     * @return matrix<long double> The inverse matrix of the given matrix
     */
    matrix<long double> inverse_mtx(const matrix<long double> &, const size_t &) const;
};

// ======================================================================
// Overloaded operator of Polynomial
// ======================================================================

/**
 * @brief Overloaded binary operator `<<` used to print out the polynomial 
 * string to a stream
 * @param out The output stream
 * @param poly The polynomial to be printed
 * @return ostream& A reference to the output stream
 */
// Overloaded binary operator << used to easily print out a Polynomial to a stream
ostream &operator<<(ostream &, const Polynomial &);