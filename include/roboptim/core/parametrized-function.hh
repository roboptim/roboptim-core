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

#ifndef ROBOPTIM_CORE_PARAMETRIZED_FUNCTION_HH
# define ROBOPTIM_CORE_PARAMETRIZED_FUNCTION_HH
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

  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Define an abstract parametrized mathematical function (\f$C^0\f$).
  ///
  /// A function is an object that can be evaluated for a given
  /// point.
  ///
  /// \f[ f : p \rightarrow g \f]
  /// \f$p \in \mathbb{R}^p\f$, \f$g \in G\f$ where
  /// \f$p\f$ is the input size and \f$G = \mathbb{R}^n \in \mathbb{R}^m\f$
  /// (\f$n, m \in \mathbb{R}^2\f$).
  ///
  /// Functions are pure immutable objects: evaluating a function
  /// twice at a given point <b>must</b> give the same result.
  template <typename F>
  class ParametrizedFunction
  {
  public:
    /// \brief Import value type.
    typedef typename F::value_type value_type;
    /// \brief Import size type.
    typedef typename F::size_type size_type;
    /// \brief Import vector type.
    typedef typename F::vector_t vector_t;
    /// \brief Import matrix type.
    typedef typename F::matrix_t matrix_t;
    /// \brief Import  result type.
    typedef F result_t;
    /// \brief Import argument type.
    typedef typename F::argument_t argument_t;

    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param argument point at which the function will be evaluated
    /// \return computed result
    result_t operator () (const argument_t& argument) const throw ();

    /// \brief Return the input size (i.e. argument's vector size).
    ///
    /// \return input size
    size_type inputSize () const throw ();

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize function arity
    ParametrizedFunction (size_type inputSize) throw ();

    /// \brief Function evaluation.
    ///
    /// Evaluate the function, has to be implemented in concrete classes.
    /// \warning Do not call this function directly, call #operator() instead.
    /// \param argument point at which the function will be evaluated
    virtual result_t impl_compute (const argument_t& argument)
      const throw () = 0;

  private:
    /// Problem dimension.
    const size_type inputSize_;
  };
  /// @}


  /// \brief Override operator<< to handle function display.
  ///
  /// \param o output stream used for display
  /// \param f function to be displayed
  /// \return output stream
  template <typename F>
  std::ostream& operator<< (std::ostream& o, const ParametrizedFunction<F>& f);


} // end of namespace roboptim

# include <roboptim/core/parametrized-function.hxx>
#endif //! ROBOPTIM_CORE_PARAMETRIZED_FUNCTION_HH
