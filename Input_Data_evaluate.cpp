#include <fstream>
#include <vector>
#include <cmath>
#include "Input_Data.hpp"
#include "Polynomial.hpp"
#include "matrix.hpp"
#include "vector_overloads.hpp"
#include "Error_Handling.hpp"

// ======================================================================
// Private member functions of Input_Data: evaluate/perform algorithms
// ======================================================================
// see Input_Data.hpp for description

using namespace std;

long double Input_Data::euclidean_norm(const vector<long double> &vec_en) const
{
    long double result_en{0}; // initialize the return value to be 0
    size_t vec_size_en{vec_en.size()};
    for (size_t value_idx_en{0}; value_idx_en < vec_size_en; value_idx_en++)
    {
        // assign the partial derivative(f with respect to x_1, x_2, ...) the the gradient
        result_en += pow(vec_en.at(value_idx_en), 2); // square the value and add it to result_en
    };
    return sqrt(result_en);
}

long double Input_Data::backtracking_line_search(const vector<long double> &x_vec_bls,
                                                 const vector<long double> &curr_gradient_bls,
                                                 const vector<long double> &directional_bls) const
{
    long double max_or_min_bls{min_max * 1.0L};   // convert min_max to a long double
    constexpr long double control_para_c{0.5L};   // search control parameter c in the interval [0, 1]
    constexpr long double control_para_tao{0.5L}; // search control parameter tao in the interval [0, 1]
    long double step_size_bls{1};                 // start from step size 1
    long double eval_f_x_bls{polyn.eval_f(x_vec_bls)};
    long double lhs_cond{polyn.eval_f(x_vec_bls + step_size_bls * directional_bls) - eval_f_x_bls};
    long double rhs_cond{control_para_c * step_size_bls * curr_gradient_bls * directional_bls};
    while (lhs_cond * max_or_min_bls < rhs_cond * max_or_min_bls)
    {
        // update the step size and its conditions
        step_size_bls *= control_para_tao;
        lhs_cond = polyn.eval_f(x_vec_bls + step_size_bls * directional_bls) - eval_f_x_bls;
        rhs_cond = control_para_c * step_size_bls * curr_gradient_bls * directional_bls;
    }
    return step_size_bls;
}

// ======================================================================
// Public member functions of Input_Data: evaluate/perform algorithms
// ======================================================================
// see Input_Data.hpp for description

int Input_Data::gradient_descent()
{
    long double max_or_min_gd{min_max * 1.0L};
    constexpr long double max_norm{1000000000}; // larger norm may result in overflow
    x_current = x_initial;

    ofstream store_iters_gd{"output_iterations_gradient_descent.txt"};
    if (!store_iters_gd)
        throw err_fail_to_read_iter_txt();

    for (size_t curr_iter_gd{0}; curr_iter_gd < max_iter; curr_iter_gd++)
    {
        vector<long double> curr_gradient_gd{polyn.gradient_f(x_current)};

        store_iters_gd << "Iteration " << curr_iter_gd + 1 << ": " << '\n';     // store each iteration
        store_iters_gd.precision(10);                                           // store each iteration
        store_iters_gd << "Current_x: " << x_current;                           // store each iteration
        store_iters_gd << "Gradient: " << curr_gradient_gd;                     // store each iteration
        store_iters_gd << "Norm: " << euclidean_norm(curr_gradient_gd) << '\n'; // store each iteration
        // found a local minimum if the norm of the current gradient is with the tolerance
        if (euclidean_norm(curr_gradient_gd) < tolerance)
        {
            store_iters_gd.close();
            return 1;
        }
        // larger norm may result in overflow
        if (euclidean_norm(curr_gradient_gd) > max_norm)
        {
            store_iters_gd.close();
            return 3;
        }

        // update step size using backtracking_line_search
        long double step_size_gd{backtracking_line_search(x_current, curr_gradient_gd,
                                                          max_or_min_gd * curr_gradient_gd)};
        store_iters_gd << "Step_size: " << step_size_gd << '\n';      // store each iteration
        x_current += max_or_min_gd * step_size_gd * curr_gradient_gd; // move to the next x
        store_iters_gd << "Next_x: " << x_current << '\n';            // store each iteration
    };
    store_iters_gd.close();
    return 2;
}

int Input_Data::newtons_method()
{
    x_current = x_initial;
    constexpr long double max_norm{1000000000}; // larger norm may result in overflow

    ofstream store_iters_nm{"output_iterations_newtons_method.txt"};
    if (!store_iters_nm)
        throw err_fail_to_read_iter_txt();

    for (size_t curr_iter_nm{0}; curr_iter_nm < max_iter; curr_iter_nm++)
    {
        vector<long double> curr_gradient_nm{polyn.gradient_f(x_current)};
        store_iters_nm << "Iteration " << curr_iter_nm + 1 << ": " << '\n';     // store each iteration
        store_iters_nm.precision(10);                                           // store each iteration
        store_iters_nm << "Current_x: " << x_current;                           // store each iteration
        store_iters_nm << "Gradient: " << curr_gradient_nm;                     // store each iteration
        store_iters_nm << "Norm: " << euclidean_norm(curr_gradient_nm) << '\n'; // store each iteration
        // found a local minimum if the norm of the current gradient is with the tolerance
        if (euclidean_norm(curr_gradient_nm) < tolerance)
        {
            store_iters_nm.close();
            return 1;
        }
        // larger norm may result in overflow
        if (euclidean_norm(curr_gradient_nm) > max_norm)
        {
            store_iters_nm.close();
            return 3;
        }

        size_t x_size{polyn.get_num_of_x_vars()};
        vector<long double> curr_direction_vec(x_size);
        matrix<long double> curr_direction_mtx(x_size, 1, curr_gradient_nm);
        matrix<long double> inverse_hessian(x_size, x_size);
        inverse_hessian = polyn.inverse_hessian_f(x_current);
        curr_direction_mtx = -1.0L * inverse_hessian * curr_direction_mtx;
        for (size_t mtx_to_vec{0}; mtx_to_vec < x_size; mtx_to_vec++)
        {
            curr_direction_vec.at(mtx_to_vec) = curr_direction_mtx(mtx_to_vec, 0);
        }
        store_iters_nm << "Direction: " << curr_direction_vec; // store each iteration
        x_current += curr_direction_vec;                       // move to the next x
        store_iters_nm << "Next_x: " << x_current << '\n';     // store each iteration
    };
    store_iters_nm.close();
    return 2;
}