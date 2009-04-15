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

/**
 * \brief Declaration of the NumericQuadraticFunction class.
 */

#ifndef OPTIMIZATION_NUMERIC_QUADRATIC_FUNCTION_HH
# define OPTIMIZATION_NUMERIC_QUADRATIC_FUNCTION_HH
# include <boost/numeric/ublas/symmetric.hpp>

# include <liboptimization/quadratic-function.hh>

namespace optimization
{
  namespace ublas = boost::numeric::ublas;

  /// Define a quadratic function.
  class NumericQuadraticFunction : public QuadraticFunction
  {
  public:
    NumericQuadraticFunction (const matrix_t&, const vector_t&)
      throw ();

    virtual vector_t operator () (const vector_t&) const throw ();
    virtual gradient_t gradient (const vector_t&, int) const throw ();
    virtual hessian_t hessian (const vector_t&, int) const throw ();

    virtual std::ostream& print (std::ostream&) const throw ();
  private:
    ublas::symmetric_matrix<double, ublas::lower> a_;
    vector_t b_;
  };

}; // end of namespace optimization

#endif //! OPTIMIZATION_QUADRATIC_FUNCTION_HH
