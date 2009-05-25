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

/**
 * \brief Declaration of the Function class.
 */

#ifndef ROBOPTIM_CORE_FUNCTION_HH
# define ROBOPTIM_CORE_FUNCTION_HH
# include <iostream>
# include <limits>
# include <utility>

# include <boost/numeric/ublas/matrix.hpp>
# include <boost/numeric/ublas/vector.hpp>
# include <boost/tuple/tuple.hpp>

# include <roboptim/core/fwd.hh>

namespace roboptim
{
  namespace ublas = boost::numeric::ublas;

  /**
     \addtogroup roboptim_function
     @{
  */

  /// \brief Define a mathematical function.
  class Function
  {
  public:
    /// Values type.
    typedef double value_type;
    /// Size type.
    typedef unsigned size_type;

    /// Vector type.
    typedef ublas::vector<value_type> vector_t;

    /// Matrix type.
    typedef ublas::matrix<value_type> matrix_t;

    /// Result type.
    typedef vector_t result_t;

    /// Argument type.
    typedef vector_t argument_t;


    /// Bound type (lower, upper).
    /// Use -infinity / +infinity to disable a bound.
    typedef std::pair<value_type, value_type> bound_t;
    /// Vector of bound.
    typedef std::vector<bound_t> bounds_t;

    typedef boost::tuple<value_type,
			 value_type,
			 value_type> discreteInterval_t;

    /// Get the value that symbolizes infinity.
    static const value_type infinity () throw ()
    {
      return std::numeric_limits<Function::value_type>::infinity ();
    }

    /// Construct a bound from a lower and upper bound.
    static bound_t makeBound (value_type l, value_type u) throw ()
    {
      assert (l <= u);
      return std::make_pair (l, u);
    }

    /// Construct an infinite bound.
    static bound_t makeInfiniteBound () throw ()
    {
      return std::make_pair (-Function::infinity (), Function::infinity  ());
    }

    /// Construct a bound from a lower bound.
    static bound_t makeLowerBound (value_type u) throw ()
    {
      return makeBound (-Function::infinity  (), u);
    }

    /// Construct a bound from an upper bound.
    static bound_t makeUpperBound (value_type l) throw ()
    {
      return makeBound (l, Function::infinity  ());
    }

    static discreteInterval_t makeDiscreteInterval (value_type min,
						    value_type max,
						    value_type step)
    {
      return discreteInterval_t (min, max, step);
    }



    /// Destructor.
    virtual ~Function () throw ();


    /// Evaluate the function.
    result_t operator () (const argument_t& argument) const throw ()
    {
      result_t result (outputSize ());
      result.clear ();
      (*this) (result, argument);
      return result;
    }

    /// Evaluate the function.
    void operator () (result_t& result, const argument_t& argument)
      const throw ()
    {
      assert (isValidResult (result));
      this->impl_compute (result, argument);
      assert (isValidResult (result));
    }

    /// Check the given result is valid (check sizes),
    bool isValidResult (const result_t& result) const throw ()
    {
      return result.size () == outputSize ();
    }

    /// Return the input size.
    size_type inputSize () const throw ()
    {
      return inputSize_;
    }

    /// Return the result's size.
    size_type  outputSize () const throw ()
    {
      return outputSize_;
    }

    /// Display function type.
    virtual std::ostream& print (std::ostream&) const throw ();


  protected:
    /// Constructor.
    /// \param n function arity
    /// \param m result size
    Function (size_type n, size_type m = 1) throw ();


    /// Evaluate the function, has to be implemented in concrete class.
    virtual void impl_compute (result_t&, const argument_t&)
      const throw () = 0;

  private:
    /// Problem dimension.
    const size_type inputSize_;

    /// Result dimension.
    const size_type outputSize_;
  };

  std::ostream& operator<< (std::ostream&, const Function&);
  /**
     @}
  */

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_HH
