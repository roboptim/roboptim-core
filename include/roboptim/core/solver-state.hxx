// Copyright (C) 2013 by Benjamin Chretien, CNRS.
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

#ifndef ROBOPTIM_CORE_SOLVER_STATE_HXX
# define ROBOPTIM_CORE_SOLVER_STATE_HXX
# include <boost/foreach.hpp>
# include <roboptim/core/io.hh>

namespace roboptim
{
  namespace detail
  {
    /// \brief Print the value of a state parameter.
    /// \tparam T value type.
    /// \param val value.
    /// \return string describing the value.
    template <typename T>
    void StateParameterPrint (std::ostream& o, const T& val)
    {
      o << val;
    }

    template <>
    inline void StateParameterPrint<bool> (std::ostream& o, const bool& val)
    {
      o << (val ? "true" : "false");
    }

    template <>
    inline void StateParameterPrint<std::string>
    (std::ostream& o, const std::string& val)
    {
      o << "\"" << val << "\"";
    }

    /// \brief Visitor used to print state parameters (variant).
    /// \tparam F function type.
    struct StateParameterPrintVisitor : boost::static_visitor<void>
    {
      StateParameterPrintVisitor (std::ostream& o)
        : boost::static_visitor<void> (),
          o_ (o)
      {
      }

      template <typename T>
      void operator () (const T& val) const
      {
        StateParameterPrint<T> (o_, val);
      }

      std::ostream& o_;
    };
  } // end of namespace detail

  template <typename F>
  std::ostream&
  StateParameter<F>::print (std::ostream& o) const
  {
    o << "(" << description << ")" << ": ";
    boost::apply_visitor (detail::StateParameterPrintVisitor (o), value);

    return o;
  }

  template <typename P>
  SolverState<P>::SolverState (const problem_t& pb)
    : boost::noncopyable (),
      cost_ (),
      constraintViolation_ ()
  {
    x_.resize (pb.function ().inputSize ());
    x_.setZero ();
  }

  template <typename P>
  SolverState<P>::~SolverState ()
  {
  }

  template <typename P>
  typename SolverState<P>::const_argument_ref
  SolverState<P>::x () const
  {
    return x_;
  }

  template <typename P>
  typename SolverState<P>::argument_ref
  SolverState<P>::x ()
  {
    return x_;
  }

  template <typename P>
  const boost::optional<typename SolverState<P>::value_type>&
  SolverState<P>::cost () const
  {
    return cost_;
  }

  template <typename P>
  boost::optional<typename SolverState<P>::value_type>&
  SolverState<P>::cost ()
  {
    return cost_;
  }

  template <typename P>
  const boost::optional<typename SolverState<P>::value_type>&
  SolverState<P>::constraintViolation () const
  {
    return constraintViolation_;
  }

  template <typename P>
  boost::optional<typename SolverState<P>::value_type>&
  SolverState<P>::constraintViolation ()
  {
    return constraintViolation_;
  }

  template <typename P>
  const typename SolverState<P>::parameters_t&
  SolverState<P>::parameters () const
  {
    return parameters_;
  }

  template <typename P>
  typename SolverState<P>::parameters_t&
  SolverState<P>::parameters ()
  {
    return parameters_;
  }

  template <typename P>
  template <typename T>
  const T&
  SolverState<P>::getParameter (const std::string& key) const
  {
    typename parameters_t::const_iterator it = parameters_.find (key);
    if(it == parameters_.end())
      throw std::out_of_range("key "+ key +" not found");
    return boost::get<T> (it->second.value);
  }

  template <typename P>
  template <typename T>
  T&
  SolverState<P>::getParameter (const std::string& key)
  {
    typename parameters_t::iterator it = parameters_.find (key);
    if (it == parameters_.end ())
      throw std::out_of_range ("key "+ key +" not found");
    return boost::get<T> (it->second.value);
  }

  template <typename P>
  std::ostream&
  SolverState<P>::print (std::ostream& o) const
  {
    o << "Solver state:" << incindent;

    o << iendl << "x: " << x_;

    if (cost_)
      o << iendl << "Cost: " << *cost_;

    if (constraintViolation_)
      o << iendl << "Constraint violation: " << *constraintViolation_;

    if (!parameters_.empty ())
      {
        o << iendl << "Parameters:" << incindent;
        typedef const std::pair<const std::string, StateParameter<function_t> >
          const_iterator_t;
        BOOST_FOREACH (const_iterator_t& it, parameters_)
          o << iendl << it.first << " " << it.second;
        o << decindent;
      }
    o << decindent;
    return o;
  }

  template <typename F>
  std::ostream&
  operator<< (std::ostream& o, const StateParameter<F>& parameter)
  {
    return parameter.print (o);
  }

  template <typename P>
  std::ostream&
  operator<< (std::ostream& o, const SolverState<P>& state)
  {
    return state.print (o);
  }
} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_SOLVER_STATE_HXX
