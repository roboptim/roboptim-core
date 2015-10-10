// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
//               2015 by Benjamin Chrétien, CNRS-UM LIRMM.
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

#ifndef ROBOPTIM_CORE_FUNCTION_HXX
# define ROBOPTIM_CORE_FUNCTION_HXX

# include <typeinfo>
# include <limits>
# include <stdexcept>
# include <sstream>
# include <utility>
# include <iomanip>

# include <boost/format.hpp>
# include <boost/algorithm/string/replace.hpp>

// WARNING: careful with circular includes
# include <roboptim/core/util.hh>

namespace roboptim
{
  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::epsilon ()
  {
    return std::numeric_limits<value_type>::epsilon ();
  }

  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::infinity ()
  {
    return std::numeric_limits<value_type>::infinity ();
  }

  template <typename T>
  typename GenericFunction<T>::interval_t
  GenericFunction<T>::makeInterval (value_type l, value_type u)
  {
    assert (l <= u);
    return std::make_pair (l, u);
  }

  template <typename T>
  typename GenericFunction<T>::interval_t
  GenericFunction<T>::makeInfiniteInterval ()
  {
    return std::make_pair (-infinity (), infinity  ());
  }

  template <typename T>
  typename GenericFunction<T>::interval_t
  GenericFunction<T>::makeLowerInterval (value_type l)
  {
    return makeInterval (l, infinity  ());
  }

  template <typename T>
  typename GenericFunction<T>::interval_t
  GenericFunction<T>::makeUpperInterval (value_type u)
  {
    return makeInterval (-infinity  (), u);
  }

  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::getLowerBound (const interval_t& interval)
  {
    return interval.first;
  }

  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::getUpperBound (const interval_t& interval)
  {
    return interval.second;
  }

  template <typename T>
  typename GenericFunction<T>::discreteInterval_t
  GenericFunction<T>::makeDiscreteInterval (value_type min,
                                            value_type max,
                                            value_type step)
  {
    return discreteInterval_t (min, max, step);
  }

  template <typename T>
  typename GenericFunction<T>::discreteInterval_t
  GenericFunction<T>::makeDiscreteInterval (interval_t interval,
                                            value_type step)
  {
    return discreteInterval_t (getLowerBound (interval),
                               getUpperBound (interval),
                               step);
  }

  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::getLowerBound (const discreteInterval_t& interval)
  {
    return boost::get<0> (interval);
  }

  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::getUpperBound (const discreteInterval_t& interval)
  {
    return boost::get<1> (interval);
  }

  template <typename T>
  typename GenericFunction<T>::value_type
  GenericFunction<T>::getStep (const discreteInterval_t& interval)
  {
    return boost::get<2> (interval);
  }

  template <typename T>
  GenericFunction<T>::GenericFunction (size_type inputSize,
                                       size_type outputSize,
                                       std::string name)
    : inputSize_ (inputSize),
      outputSize_ (outputSize),
      name_ (name)
  {
    // Positive size is required.
    assert (inputSize > 0 && outputSize > 0);
  }

  template <typename T>
  GenericFunction<T>::~GenericFunction ()
  {
  }

  template <typename T>
  std::ostream&
  GenericFunction<T>::print (std::ostream& o) const
  {
    if (getName ().empty ())
      return o << "Function";

    std::stringstream ss;
    ss << std::endl;
    char fill = o.fill (' ');
    ss << std::setw ((int)indent (o))
       << ""
       << std::setfill (fill);
    std::string name = getName ();
    boost::algorithm::replace_all (name, "\n", ss.str ());
    return o << name << " (not differentiable)";
  }

  template <typename T>
  void GenericFunction<T>::operator () (result_ref result,
                                        const_argument_ref argument) const
  {
    LOG4CXX_TRACE
      (logger, "Evaluating function at point: " << argument);
    assert (argument.size () == inputSize ());
    assert (isValidResult (result));

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    bool cur_malloc_allowed = is_malloc_allowed ();
    set_is_malloc_allowed (false);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

    this->impl_compute (result, argument);

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

    assert (isValidResult (result));
  }

  template <typename T>
  typename GenericFunction<T>::result_t
  GenericFunction<T>::operator () (const_argument_ref argument) const
  {
    result_t result (outputSize ());
    result.setZero ();
    (*this) (result, argument);
    return result;
  }

  template <typename T>
  template <class ExpectedType>
  ExpectedType* GenericFunction<T>::castInto()
  {
    if (asType<ExpectedType>())
      return static_cast<ExpectedType*>(this);

    boost::format fmt ("Cannot cast to %s*");
    fmt % typeString<ExpectedType> ();
    throw std::runtime_error (fmt.str ());
  }

  template <typename T>
  template <class ExpectedType>
  const ExpectedType* GenericFunction<T>::castInto() const
  {
    if (asType<const ExpectedType>())
      return static_cast<const ExpectedType*>(this);

    boost::format fmt ("Cannot cast to const %s*");
    fmt % typeString<ExpectedType> ();
    throw std::runtime_error (fmt.str ());
  }

  template <typename T>
  template <class ExpectedType>
  bool GenericFunction<T>::asType () const
  {
    return (ExpectedType::flags & getFlags()) == ExpectedType::flags;
  }

  template <typename T>
  template <typename F>
  void GenericFunction<T>::foreach (const discreteInterval_t interval,
                                    F functor)
  {
    const value_type delta =
      getUpperBound (interval) - getLowerBound (interval);
    assert (delta >= 0.);
    assert (getStep (interval) > 0.);

    value_type n = std::floor (delta / getStep (interval));

    for (value_type i = 0.; i <= n; i += 1.)
    {
      value_type t =
        getLowerBound (interval) + (value_type)i * getStep (interval);
      if (t > getUpperBound (interval))
        t = getUpperBound (interval);
      assert (getLowerBound (interval) <= t
              && t <= getUpperBound (interval));
      functor (t);
    }
  }

  template <typename T>
  template <typename F>
  void GenericFunction<T>::foreach (const interval_t interval,
                                    const size_type n,
                                    F functor)
  {
    const value_type delta =
      getUpperBound (interval) - getLowerBound (interval);
    assert (delta >= 0.);

    if (!n)
      return;

    for (size_type i = 0; i < n; ++i)
    {
      value_type t =
        getLowerBound (interval)
        + (value_type)i * (delta / ((value_type)n - 1));
      if (t > getUpperBound (interval))
        t = getUpperBound (interval);
      assert (getLowerBound (interval) <= t
              && t <= getUpperBound (interval));
      functor (t);
    }
  }

  template <typename T>
  bool GenericFunction<T>::isValidResult (const_result_ref result) const
  {
    return result.size () == outputSize ();
  }

  template <typename T>
  typename GenericFunction<T>::size_type
  GenericFunction<T>::inputSize () const
  {
    return inputSize_;
  }

  template <typename T>
  typename GenericFunction<T>::size_type
  GenericFunction<T>::outputSize () const
  {
    return outputSize_;
  }

  template <typename T>
  const std::string& GenericFunction<T>::getName () const
  {
    return name_;
  }

  template <typename T>
  typename GenericFunction<T>::flag_t
  GenericFunction<T>::getFlags () const
  {
    return flags;
  }

  template <typename T>
  std::ostream&
  operator<< (std::ostream& o, const GenericFunction<T>& f)
  {
    return f.print (o);
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_HXX
