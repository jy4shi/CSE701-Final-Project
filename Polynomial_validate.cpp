#include <sstream>
#include <string>
#include <vector>
#include "Polynomial.hpp"
#include "matrix.hpp"
#include "Error_Handling.hpp"

using namespace std;

// ======================================================================
// Constructor of Polynomial
// ======================================================================
// see Polynomial.hpp for description

Polynomial::Polynomial(const string &input_poly_str)
    : poly_str(input_poly_str), num_of_x_vars(0), num_of_terms(1), coeff_exp_mtx(1, 1)
{
    size_t poly_str_length{poly_str.size()};
    string lhs_str{""};
    string rhs_str{""};
    size_t equal_sign_idx{poly_str.find('=')};
    // split the polynomial string into RHS and LHS
    if (equal_sign_idx != string::npos)
    {
        lhs_str = poly_str.substr(0, equal_sign_idx);
        rhs_str = poly_str.substr(equal_sign_idx + 1, poly_str_length - equal_sign_idx - 1);
    }
    else
        throw err_missing_equal_sign{};
    check_valid_lhs_poly(lhs_str);
    check_valid_rhs_poly(rhs_str);
}

// ======================================================================
// Public member functions of Polynomial: get values
// ======================================================================
// see Polynomial.hpp for description

string Polynomial::get_poly_str() const
{
    return poly_str;
}

size_t Polynomial::get_num_of_x_vars() const
{
    return num_of_x_vars;
}

size_t Polynomial::get_num_of_terms() const
{
    return num_of_terms;
}

matrix<long double> Polynomial::get_coeff_exp_mtx() const
{
    return coeff_exp_mtx;
}

// ======================================================================
// Overloaded operator of Polynomial
// ======================================================================
// see Polynomial.hpp for description

ostream &operator<<(ostream &out, const Polynomial &poly)
{
    out << poly.get_poly_str();
    return out;
}

// ======================================================================
// Private member functions of Polynomial: validate polynomial string
// ======================================================================
// see Polynomial.hpp for description

bool Polynomial::valid_non_negative_int(const string &int_str) const
{
    string valid_numbers{"1234567890"}; // valid characters of numbers
    if (int_str.size() == 0)
        return false;
    for (char curr_char_vnni : int_str)
    {
        // if curr_char_vnni is not found in valid_numbers, invalid number, thus return false
        if (valid_numbers.find(curr_char_vnni) == string::npos)
            return false;
    };
    return true;
}

void Polynomial::check_valid_lhs_poly(const string &lhs_str_cvlp)
{
    // lhs_str_len_cvlp stores the length of lhs_str_cvlp
    size_t lhs_str_len_cvlp{lhs_str_cvlp.size()};
    string x_vars_cvlp{""};        // initialize the string that stores all x variables
    size_t x_vars_len_cvlp{0};     // initialize the length of the x_vars_cvlp
    size_t char_idx_cvlp{0};       // initialize the character index of x_vars_cvlp to be 0
    string curr_x_str_cvlp{"x_1"}; // store the string of the current x variable, start from x_1, not x_0
    size_t curr_x_idx_cvlp{1};     // store the index of the current x variable, start from 1
    bool store_curr_x_var{true};   // whether we should store the current x variable

    // throw the invalid_lhs_poly exception if lhs_str_cvlp does not start with "f(" and end with ')'
    if ((lhs_str_cvlp.substr(0, 2) != "f(") or
        (lhs_str_cvlp.at(lhs_str_len_cvlp - 1) != ')'))
        throw err_invalid_lhs_poly{};
    // stores the string of x variables without "f(" and ')'
    x_vars_cvlp = lhs_str_cvlp.substr(2, lhs_str_len_cvlp - 3);
    x_vars_len_cvlp = x_vars_cvlp.size();
    // throw the invalid_lhs_poly exception if there are no x variables
    if (x_vars_len_cvlp == 0)
        throw err_invalid_lhs_poly{};

    while (char_idx_cvlp < x_vars_len_cvlp) // use while as char_idx_cvlp doesn't always increment by 1
    {
        // update the values when store_curr_x_var is true
        if (store_curr_x_var)
        {
            curr_x_str_cvlp = "x_" + to_string(curr_x_idx_cvlp);
            size_t curr_x_len_cvlp{curr_x_str_cvlp.size()}; // stores length of "x_1", or "x_2", or ...
            // throw the invalid_lhs_poly exception if we don't have x_1 or x_2 or x_3 or ...
            if (curr_x_str_cvlp != x_vars_cvlp.substr(char_idx_cvlp, curr_x_len_cvlp))
                throw err_invalid_lhs_poly{};
            // in order to move on to the next x variable, change three values:
            // ex: current x index changes from 1 to 2, so that string changes from "x_1" to "x_2"
            //     current char index changes from 0 to 3, since we already checked the string "x_1"
            //     boolean store_curr_x_var changes to false since "x_1" should be followed by ','
            //          or the end of the string, and we don't want to store that
            curr_x_idx_cvlp++; // x index +1,
            char_idx_cvlp += curr_x_len_cvlp;
            store_curr_x_var = false;
        }
        else // store_curr_x_var is false, expect the current character to be ','
        {
            if (x_vars_cvlp.at(char_idx_cvlp) == ',') // current character is ','
            {
                char_idx_cvlp++;
                store_curr_x_var = true;
            }
            else // invalid LHS if the current character is any other character
                throw err_invalid_lhs_poly{};
        }
    }
    num_of_x_vars = curr_x_idx_cvlp - 1; // curr_x_idx_cvlp would count one more unit, thus reduce 1
}

void Polynomial::check_valid_rhs_poly(const string &rhs_str_cvrp)
{
    size_t rhs_str_len_cvrp{rhs_str_cvrp.size()}; // store the length of rhs_str_cvrp
    string curr_term_str_cvrp{""};                // store the string of the current term

    // first, check that the length of rhs_str_cvrp is not 0 (not an empty string)
    if (rhs_str_len_cvrp == 0)
        throw err_invalid_rhs_poly{};
    // go over all the characters in rhs_str_cvrp to count the number of terms seperated
    // by '+', '-', this prevents the program to resize vectors/matrices when we have a new term
    // note that num_of_terms is initialized to be 1
    for (char char_of_term : rhs_str_cvrp)
        if (char_of_term == '+' or char_of_term == '-')
            num_of_terms++; // start from 1 them increment
    // terms are seperated by '+' or '-', for example: x_1*x_2 has 1 term, and 1+x_1 has 2 terms
    //      -x_1 or -5*x_1 has 1 term since '-' at the begining is part of the coefficient
    //      x_1*-x_2 is an invalid input, user should simplify the term to -x_1*x_2
    if (rhs_str_cvrp.at(0) == '+' or rhs_str_cvrp.at(0) == '-')
        num_of_terms--; // num_of_terms decreases by 1 if rhs_str_cvrp start with '+' or '-'

    // initialize a vector of strings with size num_of_terms, it stores the terms in string
    vector<string> terms_str_vec(num_of_terms);
    curr_term_str_cvrp = ""; //make sure we have a empty string for current term before the loop
    // for loop that stores the all of the terms in the vector terms_str_vec
    for (size_t char_idx_cvrp_loop1{0}, curr_term_idx_cvrp_loop1{0};
         char_idx_cvrp_loop1 < rhs_str_len_cvrp; char_idx_cvrp_loop1++)
    {
        char curr_char_cvrp{rhs_str_cvrp.at(char_idx_cvrp_loop1)}; // current character
        // create a new term only if the current character is '+' or '-', and it
        // is not the first character
        if ((curr_char_cvrp == '+' or curr_char_cvrp == '-') and (char_idx_cvrp_loop1 != 0))
        {
            // first store the previous term before move to the current term
            terms_str_vec.at(curr_term_idx_cvrp_loop1) = curr_term_str_cvrp;
            // then start an empty string for the new current term
            curr_term_str_cvrp = "";
            curr_term_idx_cvrp_loop1++;
        }
        curr_term_str_cvrp += curr_char_cvrp;
        // if we are at the last character of rhs_str_cvrp, then store the current term
        if (char_idx_cvrp_loop1 + 1 == rhs_str_len_cvrp)
            terms_str_vec.at(curr_term_idx_cvrp_loop1) = curr_term_str_cvrp;
    }

    // create a temporary matrix with size (num_of_terms, 1+num_of_x_vars),
    // then assign it to the coeff_exp_mtx matrix, which is the private data of Polynomial
    matrix<long double> temp_mtx(num_of_terms, 1 + num_of_x_vars);
    coeff_exp_mtx = temp_mtx; // this process basically resizes coeff_exp_mtx

    // now update the coeff_exp_mtx matrix based on the string vector terms_str_vec
    for (size_t curr_term_idx_cvrp_loop2{0}; // for each term in the num_of_terms
         curr_term_idx_cvrp_loop2 < num_of_terms; curr_term_idx_cvrp_loop2++)
    {
        curr_term_str_cvrp = terms_str_vec.at(curr_term_idx_cvrp_loop2);
        size_t curr_term_len_cvrp{curr_term_str_cvrp.size()};
        // check that the length of the current term is not 0 (not an empty string)
        if (curr_term_len_cvrp == 0)
            throw err_invalid_rhs_poly{};
        // default coefficient for a term is 1
        coeff_exp_mtx(curr_term_idx_cvrp_loop2, 0) = 1;

        // only the first character of each term can start with a '+' or '-' char
        if (curr_term_str_cvrp.at(0) == '+')
        {
            curr_term_len_cvrp--; // remove the first character '+'
            curr_term_str_cvrp = curr_term_str_cvrp.substr(1, curr_term_len_cvrp);
            if (curr_term_len_cvrp == 0) // check the term is not empty after removing '+'
                throw err_invalid_rhs_poly{};
        };
        if (curr_term_str_cvrp.at(0) == '-')
        {
            // default coefficient for a negative term becomes -1
            coeff_exp_mtx(curr_term_idx_cvrp_loop2, 0) = -1;
            curr_term_len_cvrp--; // remove the first character '-'
            curr_term_str_cvrp = curr_term_str_cvrp.substr(1, curr_term_len_cvrp);
            if (curr_term_len_cvrp == 0) // check the term is not empty after removing '-'
                throw err_invalid_rhs_poly{};
        };

        // now for each term, we further divide it into a vector of string elements
        // an element can be a constant/coefficient number, or an x var such as x_1, x_2, x_3, ...
        // stringstream is used here to split curr_term_str_cvrp into elements by '*'
        vector<string> elems_str_vec;
        stringstream term_str_stream(curr_term_str_cvrp);
        string curr_value_stream;
        while (getline(term_str_stream, curr_value_stream, '*'))
        {
            elems_str_vec.push_back(curr_value_stream);
        };
        for (string curr_elem : elems_str_vec) // for each character in the element
        {
            if (curr_elem.substr(0, 2) == "x_") // current element is a string of an x variable
            {
                string x_var_idx_str{""}; // Ex: x_1: index="1", x_2:index="2" ...
                string x_var_exp_str{""}; // Ex: x_1^2: exponent="2"
                size_t x_var_idx_ull{1};  // default index for an x variable is 1
                size_t x_var_exp_ull{1};  // default exponent for an x variable is 1
                size_t caret_idx{curr_elem.find('^')};
                if (caret_idx == string::npos) // didn't find '^'
                {
                    x_var_idx_str = curr_elem.substr(2, curr_elem.size() - 2);
                    x_var_exp_str = "1"; // default exponent for an x variable is 1
                }
                else // found '^'
                {
                    x_var_idx_str = curr_elem.substr(2, caret_idx - 2);
                    x_var_exp_str = curr_elem.substr(caret_idx + 1, curr_elem.size() - caret_idx - 1);
                };

                if (valid_non_negative_int(x_var_idx_str) and valid_non_negative_int(x_var_exp_str))
                // if both the index string and the exponent string are non negative integers:
                {
                    x_var_idx_ull = stoull(x_var_idx_str);
                    x_var_exp_ull = stoull(x_var_exp_str);
                    // then we need to check the index of the x_variable is in num_of_x_vars
                    if ((0 < x_var_idx_ull) and (x_var_idx_ull <= num_of_x_vars))
                    {
                        // only then we can store the x_variable's exponent in the coeff_exp_mtx
                        coeff_exp_mtx(curr_term_idx_cvrp_loop2, x_var_idx_ull) += x_var_exp_ull;
                        // this may cause loss of precision, since x_var_exp_ull is size_t,
                        // while coeff_exp_mtx is a matrix of double, however the exponent of an x
                        // variable shouldn't be extreme large, thus the loss is unlikely to happen
                    }
                    else // x_variable is either x_0 or more than num_of_x_vars
                        throw err_invalid_rhs_poly{};
                }
                else // invalid index string or exponent string
                    throw err_invalid_rhs_poly{};
            }
            else // current element is a string of a coefficient
            {
                size_t dec_point_idx{curr_elem.find('.')}; // find the index of the decimal point
                if (dec_point_idx == string::npos)         // didn't find '.' -> expect an integer
                {
                    if (valid_non_negative_int(curr_elem)) // curr_elem is a non-negative integer
                        coeff_exp_mtx(curr_term_idx_cvrp_loop2, 0) *= stod(curr_elem);
                    else // curr_elem does not have a decimal point and is not a non-negative integer
                        throw err_invalid_rhs_poly{};
                }
                else // found '.' -> expect a non-integer value
                {
                    string before_dec_point_str{curr_elem.substr(0, dec_point_idx)};
                    string after_dec_point_str{curr_elem.substr(dec_point_idx + 1,
                                                                curr_elem.size() - dec_point_idx - 1)};
                    if (valid_non_negative_int(before_dec_point_str) and
                        valid_non_negative_int(after_dec_point_str))
                    {
                        // curr_elem is composed of "non_negative_int + '.' + non_negative_int"
                        coeff_exp_mtx(curr_term_idx_cvrp_loop2, 0) *= stod(curr_elem);
                    }
                    else
                        throw err_invalid_rhs_poly{};
                };
            }
        }
    }
}
