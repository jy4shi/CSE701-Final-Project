#include <string>
#include <vector>
#include <cmath>
#include "Polynomial.hpp"
#include "matrix.hpp"
#include "vector_overloads.hpp"
#include "Error_Handling.hpp"

using namespace std;

// ======================================================================
// Public member functions of Polynomial: evaluate functions
// ======================================================================
// see Polynomial.hpp for description

long double Polynomial::eval_f(const vector<long double> &x_vars_ef) const
{
    long double result_value{0};           // initialize the resulting value
    if (x_vars_ef.size() != num_of_x_vars) // if x_vars_ef has incorrect size
        throw err_invalid_x_vector();
    for (size_t term_idx_e{0}; term_idx_e < num_of_terms; term_idx_e++)
    {
        long double term_value{0}; // each term has its own term value
        for (size_t x_var_idx_e{0}; x_var_idx_e < num_of_x_vars + 1; x_var_idx_e++)
        {
            long double curr_mtx_value{coeff_exp_mtx(term_idx_e, x_var_idx_e)};
            if (x_var_idx_e == 0)
            {
                term_value = curr_mtx_value; // coefficient
            }
            else
            {
                // term = coefficient * x_1^exponent * x_2^exponent * ...
                term_value *= pow(x_vars_ef.at(x_var_idx_e - 1), curr_mtx_value);
            }
        };
        result_value += term_value; // result = term_0 + term_1 + term_2 + ...
    };
    return result_value;
}

long double Polynomial::eval_df(const vector<long double> &x_vars_ed, const size_t &x_idx_ed) const
{
    constexpr long double h{0.0000000001L};                 // 1*10^-10, quotient h approaches to zero
    vector<long double> x_vars_plus_h(num_of_x_vars, 0.0L); // vector of x variables when x_i+h
    if (x_vars_ed.size() != num_of_x_vars)                  // if x_vars_ed has incorrect size
        throw err_invalid_x_vector();
    x_vars_plus_h.at(x_idx_ed - 1) = h; // [0, ..., 0, h, 0, ...]
    x_vars_plus_h += x_vars_ed;         // [x_1, ..., x_(i-1), (x_i)+h, x_(i+1), ...]
    return (eval_f(x_vars_plus_h) - eval_f(x_vars_ed)) / h;
}

vector<long double> Polynomial::gradient_f(const vector<long double> &x_vars_gf) const
{
    vector<long double> gradient_vec(num_of_x_vars, 0); // gradient, a vector of long double
    if (x_vars_gf.size() != num_of_x_vars)              // if x_vars_gf has incorrect size
        throw err_invalid_x_vector();
    for (size_t x_idx_gf{0}; x_idx_gf < num_of_x_vars; x_idx_gf++)
    {
        // assign the partial derivative(f with respect to x_1, x_2, ...) the the gradient
        gradient_vec.at(x_idx_gf) = eval_df(x_vars_gf, x_idx_gf + 1);
    };
    return gradient_vec;
}

matrix<long double> Polynomial::hessian_f(const vector<long double> &x_vars_hf) const
{
    constexpr long double h{0.00001L}; // 1*10^-5, quotient h approaches to zero

    matrix<long double> hessian_mtx(num_of_x_vars, num_of_x_vars); // hessian, a matrix of long double
    vector<long double> gd_hf(gradient_f(x_vars_hf));              // gradient for current x
    vector<long double> gd_gd_hf(num_of_x_vars, 0);                // gradient of gradient

    if (x_vars_hf.size() != num_of_x_vars) // if x_vars_gf has incorrect size
        throw err_invalid_x_vector();

    // consider the hessian matrix as the gradients of the current gradient gd_hf
    for (size_t row_idx_hf{0}; row_idx_hf < num_of_x_vars; row_idx_hf++)
    {
        vector<long double> curr_x_plus_h(num_of_x_vars, 0); // vector of x variables when x_i+h
        curr_x_plus_h.at(row_idx_hf) = h;                    // [0, ..., 0, h, 0, ...]
        curr_x_plus_h += x_vars_hf;                          // [x_1, ..., x_(i-1), (x_i)+h, x_(i+1), ...]
        gd_gd_hf = gradient_f(curr_x_plus_h);                // update gradient of gradient
        gd_gd_hf = 1.0L / h * (gd_gd_hf - gd_hf);            // vector/h -> 1/h*vector
        for (size_t col_idx_hf{0}; col_idx_hf < num_of_x_vars; col_idx_hf++)
        {
            // since the hessian is calculated as the gradients of the current gradient
            // the rounding error becomes significant
            // round the values of hessian to 3 decimal places to reduce the effect
            long double round_3{round(gd_gd_hf.at(col_idx_hf) * 1000) / 1000};
            hessian_mtx(row_idx_hf, col_idx_hf) = round_3;
        }
    };
    return hessian_mtx;
}

matrix<long double> Polynomial::inverse_hessian_f(const vector<long double> &x_vars_ihf) const
{
    return inverse_mtx(hessian_f(x_vars_ihf), num_of_x_vars);
}

// ======================================================================
// Private member functions of Polynomial: evaluate functions
// ======================================================================
// see Polynomial.hpp for description

long double Polynomial::cofactor(const matrix<long double> &mtx_cof, const size_t &mtx_size_cof,
                                 const size_t &remove_row_cof, const size_t &remove_col_cof) const
{
    // create a minor matrix based on the given matrix
    matrix<long double> minor_mtx(mtx_size_cof - 1, mtx_size_cof - 1);
    size_t minor_row_idx_{0};
    // fill in the elements of the minor matrix
    for (size_t row_idx_cof{0}; row_idx_cof < mtx_size_cof; row_idx_cof++)
    {
        size_t minor_col_idx_{0};
        if (row_idx_cof == remove_row_cof)
            continue;
        for (size_t col_idx_cof{0}; col_idx_cof < mtx_size_cof; col_idx_cof++)
        {
            if (col_idx_cof == remove_col_cof)
                continue;
            minor_mtx(minor_row_idx_, minor_col_idx_) = mtx_cof(row_idx_cof, col_idx_cof);
            minor_col_idx_++;
        };
        minor_row_idx_++;
    };
    // return the cofactor based on the minor matrix
    return pow(-1.0L, remove_row_cof + remove_col_cof) * determinant(minor_mtx, mtx_size_cof - 1);
}

long double Polynomial::determinant(const matrix<long double> &mtx_det,
                                    const size_t &mtx_size_det) const
{
    long double det{0.0L};
    matrix<long double> cof_mtx(mtx_size_det, mtx_size_det);

    if (mtx_size_det == 1)
        return mtx_det(0, 0);
    if (mtx_size_det == 2)
        return mtx_det(0, 0) * mtx_det(1, 1) - mtx_det(0, 1) * mtx_det(1, 0);
    // find the determinant by applying the Laplace expansion
    for (size_t row_idx_del{0}; row_idx_del < mtx_size_det; row_idx_del++)
    {
        det += mtx_det(row_idx_del, 0) * cofactor(mtx_det, mtx_size_det, row_idx_del, 0);
    }
    return det;
}

matrix<long double> Polynomial::adjugate_mtx(const matrix<long double> &mtx_adj,
                                             const size_t &mtx_size_adj) const
{
    matrix<long double> return_mtx_adj(mtx_size_adj, mtx_size_adj);
    if (mtx_size_adj == 1)
    {
        return_mtx_adj(0, 0) = 1.0L;
        return return_mtx_adj;
    }
    // create the adjugate matrix by finding the cofactor of each element
    for (size_t row_idx_adj{0}; row_idx_adj < mtx_size_adj; row_idx_adj++)
    {
        for (size_t col_idx_adj{0}; col_idx_adj < mtx_size_adj; col_idx_adj++)
        {
            return_mtx_adj(row_idx_adj, col_idx_adj) = cofactor(mtx_adj, mtx_size_adj,
                                                                col_idx_adj, row_idx_adj);
        };
    };
    return return_mtx_adj;
}

matrix<long double> Polynomial::inverse_mtx(const matrix<long double> &mtx_inv,
                                            const size_t &mtx_size_inv) const
{
    matrix<long double> return_mtx_inv(mtx_size_inv, mtx_size_inv);
    long double temp_det{determinant(mtx_inv, mtx_size_inv)};
    return_mtx_inv = adjugate_mtx(mtx_inv, mtx_size_inv);
    // Inverse A = 1/det(A) * adj(A)
    return 1 / temp_det * return_mtx_inv;
}
