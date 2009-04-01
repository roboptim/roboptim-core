// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


#ifndef OPTIMIZATION_TESTS_HS071_HH
# define OPTIMIZATION_TESTS_HS071_HH
# include <utility>
# include <function.hh>

using namespace optimization;

struct F : public Function
{
  F () : Function (4)
  {
    // Set bound for all variables.
    // 1. < x_i < 5. (x_i in [1.;5.])
    for (size_type i = 0; i < n; ++i)
      argBounds[i] = std::make_pair(1., 5.);
  }

  virtual value_type
  operator () (const vector_t& x) const throw ()
  {
    return x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);

    grad[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
    grad[1] = x[0] * x[3];
    grad[2] = x[0] * x[3] + 1;
    grad[3] = x[0] * (x[0] + x[1] + x[2]);
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x) const throw ()
  {
    matrix_t h (n, n);
    h (0, 0) = 2 * x[3];
    h (0, 1) = x[3];
    h (0, 2) = x[3];
    h (0, 3) = 2 * x[0] + x[1] + x[2];

    h (1, 0) = x[3];
    h (1, 1) = 0.;
    h (1, 2) = 0.;
    h (1, 3) = x[0];

    h (2, 0) = x[3];
    h (2, 1) = 0.;
    h (2, 2) = 0.;
    h (2, 3) = x[1];

    h (3, 0) = 2 * x[0] + x[1] + x[2];
    h (3, 1) = x[0];
    h (3, 2) = x[0];
    h (3, 3) = 0.;
    return h;
  }
};

struct G0 : public Function
{
  G0 ()
    : Function (4)
  {
    bound.first = 25.;
  }

  virtual value_type
  operator () (const vector_t& x) const throw ()
  {
    return x[0] * x[1] * x[2] * x[3];
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);

    grad[0] = x[1] * x[2] * x[3];
    grad[1] = x[0] * x[2] * x[3];
    grad[2] = x[0] * x[1] * x[3];
    grad[3] = x[0] * x[1] * x[2];
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x) const throw ()
  {
    matrix_t h (n, n);
    h (0, 0) = 0.;
    h (0, 1) = x[2] * x[3];
    h (0, 2) = x[1] * x[3];
    h (0, 3) = x[1] * x[2];

    h (1, 0) = x[2] * x[3];
    h (1, 1) = 0.;
    h (1, 2) = x[0] * x[3];
    h (1, 3) = x[0] * x[2];

    h (2, 0) = x[1] * x[3];
    h (2, 1) = x[0] * x[3];
    h (2, 2) = 0.;
    h (2, 3) = x[0] * x[1];

    h (3, 0) = x[1] * x[2];
    h (3, 1) = x[0] * x[2];
    h (3, 2) = x[0] * x[1];
    h (3, 3) = 0.;
    return h;
  }
};

struct G1 : public Function
{
  G1 ()
    : Function (4)
  {
    bound.first = 40.;
    bound.second = 40.;
  }

  virtual value_type
  operator () (const vector_t& x) const throw ()
  {
    return x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);

    grad[0] = 2 * x[0];
    grad[1] = 2 * x[1];
    grad[2] = 2 * x[2];
    grad[3] = 2 * x[3];
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x) const throw ()
  {
    matrix_t h (n, n);
    h (0, 0) = 2.;
    h (0, 1) = 0.;
    h (0, 2) = 0.;
    h (0, 3) = 0.;

    h (1, 0) = 0.;
    h (1, 1) = 2.;
    h (1, 2) = 0.;
    h (1, 3) = 0.;

    h (2, 0) = 0.;
    h (2, 1) = 0.;
    h (2, 2) = 2.;
    h (2, 3) = 0.;

    h (3, 0) = 0.;
    h (3, 1) = 0.;
    h (3, 2) = 0.;
    h (3, 3) = 2.;
    return h;
  }
};

#endif //! OPTIMIZATION_TESTS_HS071_HH
