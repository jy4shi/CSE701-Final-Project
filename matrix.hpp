#pragma once

/**
 * @file matrix.hpp
 * @author Barak Shoshany (baraksh@gmail.com) (http://baraksh.com)
 * @version Class template example: the matrix class
 * @date 2020-11-30
 * @copyright Copyright (c) 2020
 * @brief A matrix class template with overloaded operators for common matrix operations.
 * @details This library contains a class template for matrices. The matrices can be of arbitrary size. Overloaded operators for common matrix operations such as addition and multiplication are defined.
 */

#include <initializer_list>
#include <iostream>
#include <vector>

using namespace std;

// ============
// Declarations
// ============

template <typename T>
class matrix
{
public:
    // Constructor to create a zero matrix
    // First argument: number of rows
    // Second argument: number of columns
    matrix(const size_t &, const size_t &);

    // Constructor to create a diagonal matrix
    // Argument: a vector containing the elements on the diagonal
    // Number of rows and columns is inferred automatically
    matrix(const vector<T> &);

    // Constructor to create a diagonal matrix
    // Argument: an initializer_list containing the elements on the diagonal
    // Number of rows and columns is inferred automatically
    matrix(const initializer_list<T> &);

    // Constructor to create a matrix and initialize it to the given elements
    // First argument: number of rows
    // Second argument: number of columns
    // Third argument: a vector containing the elements (flattened)
    matrix(const size_t &, const size_t &, const vector<T> &);

    // Constructor to create a matrix and initialize it to the given elements
    // First argument: number of rows
    // Second argument: number of columns
    // Third argument: an initializer_list containing the elements (flattened)
    matrix(const size_t &, const size_t &, const initializer_list<T> &);

    // Member function used to obtain (but not modify) the number of rows in the matrix
    size_t get_rows() const;

    // Member function used to obtain (but not modify) the number of columns in the matrix
    size_t get_cols() const;

    // Overloaded operator () used to access matrix elements WITHOUT range
    // The indices start from 0: m(0, 1) would be the element at row 1, column 2
    // First version: returns a reference, thus allows modification of the element
    T &operator()(const size_t &, const size_t &);

    // Overloaded operator () used to access matrix elements WITHOUT range checking
    // The indices start from 0: m(0, 1) would be the element at row 1, column 2
    // Second version: does not return a reference and declared as const, does not allow modification of the element
    T operator()(const size_t &, const size_t &) const;

    // Member function used to access matrix elements WITH range checking (throws out_of_range via vector::at)
    // The indices start from 0: m.at(0, 1) would be the element at row 1, column 2
    // First version: returns a reference, thus allows modification of the element
    T &at(const size_t &, const size_t &);

    // Member function used to access matrix elements WITH range checking (throws out_of_range via vector::at)
    // The indices start from 0: m.at(0, 1) would be the element at row 1, column 2
    // Second version: does not return a reference and declared as const, does not allow modification of the element
    T at(const size_t &, const size_t &) const;

    // Exception to be thrown if the number of rows or columns given to the constructor is zero
    class zero_size
    {
    };

    // Exception to be thrown if the vector of elements provided to the constructor is of the wrong size
    class initializer_wrong_size
    {
    };

    // Exception to be thrown if two matrices of different sizes are added or subtracted
    class incompatible_sizes_add
    {
    };

    // Exception to be thrown if two matrices of incompatible sizes are multiplied
    class incompatible_sizes_multiply
    {
    };

private:
    // The number of rows
    size_t rows{0};

    // The number of columns
    size_t cols{0};

    // A vector storing the elements of the matrix in flattened (1-dimensional) form
    vector<T> elements;
};

// Overloaded binary operator << used to easily print out a matrix to a stream
template <typename T>
ostream &operator<<(ostream &, const matrix<T> &);

// Overloaded binary operator + used to add two matrices
template <typename T>
matrix<T> operator+(const matrix<T> &, const matrix<T> &);

// Overloaded binary operator += used to add two matrices and assign the result to the first one
template <typename T>
matrix<T> operator+=(matrix<T> &, const matrix<T> &);

// Overloaded unary operator - used to take the negative of a matrix<T>
template <typename T>
matrix<T> operator-(const matrix<T> &);

// Overloaded binary operator - used to subtract two matrices
template <typename T>
matrix<T> operator-(const matrix<T> &, const matrix<T> &);

// Overloaded binary operator -= used to subtract two matrices and assign the result to the first one
template <typename T>
matrix<T> operator-=(matrix<T> &, const matrix<T> &);

// Overloaded binary operator * used to multiply two matrices
template <typename T>
matrix<T> operator*(const matrix<T> &, const matrix<T> &);

// Overloaded binary operator * used to multiply a scalar on the left and a matrix<T> on the right
template <typename T>
matrix<T> operator*(const T &, const matrix<T> &);

// Overloaded binary operator * used to multiply a matrix<T> on the left and a scalar on the right
template <typename T>
matrix<T> operator*(const matrix<T> &, const T &);

// ==============
// Implementation
// ==============

template <typename T>
matrix<T>::matrix(const size_t &input_rows, const size_t &input_cols)
    : rows(input_rows), cols(input_cols)
{
    if (rows == 0 or cols == 0)
        throw zero_size{};
    elements = vector<T>(rows * cols);
}

template <typename T>
matrix<T>::matrix(const vector<T> &input_diagonal)
    : rows(input_diagonal.size()), cols(input_diagonal.size())
{
    if (rows == 0)
        throw zero_size{};
    elements = vector<T>(rows * cols);
    for (size_t i{0}; i < rows; i++)
        elements[(cols * i) + i] = input_diagonal[i];
}

template <typename T>
matrix<T>::matrix(const initializer_list<T> &input_diagonal)
    : matrix(vector<T>{input_diagonal}) {}

template <typename T>
matrix<T>::matrix(const size_t &input_rows, const size_t &input_cols, const vector<T> &input_elements)
    : rows(input_rows), cols(input_cols), elements(input_elements)
{
    if (rows == 0 or cols == 0)
        throw zero_size{};
    if (input_elements.size() != rows * cols)
        throw initializer_wrong_size{};
}

template <typename T>
matrix<T>::matrix(const size_t &input_rows, const size_t &input_cols, const initializer_list<T> &input_elements)
    : matrix(input_rows, input_cols, vector<T>{input_elements}) {}

template <typename T>
size_t matrix<T>::get_rows() const
{
    return rows;
}

template <typename T>
size_t matrix<T>::get_cols() const
{
    return cols;
}

template <typename T>
T &matrix<T>::operator()(const size_t &row, const size_t &col)
{
    return elements[(cols * row) + col];
}

template <typename T>
T matrix<T>::operator()(const size_t &row, const size_t &col) const
{
    return elements[(cols * row) + col];
}

template <typename T>
T &matrix<T>::at(const size_t &row, const size_t &col)
{
    return elements.at((cols * row) + col);
}

template <typename T>
T matrix<T>::at(const size_t &row, const size_t &col) const
{
    return elements.at((cols * row) + col);
}

template <typename T>
ostream &operator<<(ostream &out, const matrix<T> &m)
{
    size_t rows{m.get_rows()}, cols{m.get_cols()};
    for (size_t i{0}; i < rows; i++)
    {
        out << "( ";
        for (size_t j{0}; j < cols; j++)
            out << m(i, j) << '\t';
        out << ")\n";
    }
    out << '\n';
    return out;
}

template <typename T>
matrix<T> operator+(const matrix<T> &a, const matrix<T> &b)
{
    size_t rows{a.get_rows()}, cols{a.get_cols()};
    if ((rows != b.get_rows()) or (cols != b.get_cols()))
        throw typename matrix<T>::incompatible_sizes_add{};
    matrix<T> c(rows, cols);
    for (size_t i{0}; i < rows; i++)
        for (size_t j{0}; j < cols; j++)
            c(i, j) = a(i, j) + b(i, j);
    return c;
}

template <typename T>
matrix<T> operator+=(matrix<T> &a, const matrix<T> &b)
{
    a = a + b;
    return a;
}

template <typename T>
matrix<T> operator-(const matrix<T> &m)
{
    size_t rows{m.get_rows()}, cols{m.get_cols()};
    matrix<T> c(rows, cols);
    for (size_t i{0}; i < rows; i++)
        for (size_t j{0}; j < cols; j++)
            c(i, j) = -m(i, j);
    return c;
}

template <typename T>
matrix<T> operator-(const matrix<T> &a, const matrix<T> &b)
{
    size_t rows{a.get_rows()}, cols{a.get_cols()};
    if ((rows != b.get_rows()) or (cols != b.get_cols()))
        throw typename matrix<T>::incompatible_sizes_add{};
    matrix<T> c(rows, cols);
    for (size_t i{0}; i < rows; i++)
        for (size_t j{0}; j < cols; j++)
            c(i, j) = a(i, j) - b(i, j);
    return c;
}

template <typename T>
matrix<T> operator-=(matrix<T> &a, const matrix<T> &b)
{
    a = a - b;
    return a;
}

template <typename T>
matrix<T> operator*(const matrix<T> &a, const matrix<T> &b)
{
    size_t a_rows{a.get_rows()}, a_cols{a.get_cols()};
    size_t b_rows{b.get_rows()}, b_cols{b.get_cols()};
    if (a_cols != b_rows)
        throw typename matrix<T>::incompatible_sizes_multiply{};
    matrix<T> c(a_rows, b_cols);
    for (size_t i{0}; i < a_rows; i++)
        for (size_t j{0}; j < b_cols; j++)
            for (size_t k{0}; k < a_cols; k++)
                c(i, j) += a(i, k) * b(k, j);
    return c;
}

template <typename T>
matrix<T> operator*(const T &s, const matrix<T> &m)
{
    size_t rows{m.get_rows()}, cols{m.get_cols()};
    matrix<T> c(rows, cols);
    for (size_t i{0}; i < rows; i++)
        for (size_t j{0}; j < cols; j++)
            c(i, j) = s * m(i, j);
    return c;
}

template <typename T>
matrix<T> operator*(const matrix<T> &m, const T &s)
{
    return s * m;
}