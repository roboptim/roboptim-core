// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
//
// This file is part of the roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.


#ifndef OPTIMIZATION_TESTS_HS071_HH
# define OPTIMIZATION_TESTS_HS071_HH
# include <utility>
# include <roboptim/core/twice-derivable-function.hh>

using namespace roboptim;

struct F : public TwiceDerivableFunction
{
  F () : TwiceDerivableFunction (4, 1)
  {
  }

  virtual vector_t
  operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
    return res;
  }

  virtual gradient_t
  gradient (const vector_t& x, int) const throw ()
  {
    gradient_t grad (n);

    grad[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
    grad[1] = x[0] * x[3];
    grad[2] = x[0] * x[3] + 1;
    grad[3] = x[0] * (x[0] + x[1] + x[2]);
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x, int) const throw ()
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

struct G0 : public TwiceDerivableFunction
{
  G0 ()
    : TwiceDerivableFunction (4, 1)
  {
  }

  virtual vector_t
  operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[1] * x[2] * x[3];
    return res;
  }

  virtual gradient_t
  gradient (const vector_t& x, int) const throw ()
  {
    gradient_t grad (n);

    grad[0] = x[1] * x[2] * x[3];
    grad[1] = x[0] * x[2] * x[3];
    grad[2] = x[0] * x[1] * x[3];
    grad[3] = x[0] * x[1] * x[2];
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x, int) const throw ()
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

struct G1 : public TwiceDerivableFunction
{
  G1 ()
    : TwiceDerivableFunction (4, 1)
  {
  }

  virtual vector_t
  operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
    return res;
  }

  virtual gradient_t
  gradient (const vector_t& x, int) const throw ()
  {
    gradient_t grad (n);

    grad[0] = 2 * x[0];
    grad[1] = 2 * x[1];
    grad[2] = 2 * x[2];
    grad[3] = 2 * x[3];
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x, int) const throw ()
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


template <typename T>
void initialize_problem (T& pb, const G0& g0, const G1& g1)
{
  // Set bound for all variables.
  // 1. < x_i < 5. (x_i in [1.;5.])
  for (Function::size_type i = 0; i < pb.function ().n; ++i)
    pb.argBounds ()[i] = T::makeBound (1., 5.);

  // Add constraints.
  pb.addConstraint (&g0, T::makeUpperBound (25.));
  pb.addConstraint (&g1, T::makeBound (40., 40.));

  // Set the starting point.
  Function::vector_t start (pb.function ().n);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  pb.startingPoint () = start;
}

#endif //! OPTIMIZATION_TESTS_HS071_HH
