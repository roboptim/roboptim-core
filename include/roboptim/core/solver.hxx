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

#ifndef ROBOPTIM_CORE_SOLVER_HXX
# define ROBOPTIM_CORE_SOLVER_HXX
# include <boost/foreach.hpp>
# include <roboptim/core/io.hh>

namespace roboptim
{
  template <typename F, typename C>
  Solver<F, C>::Solver (const problem_t& pb)
    : GenericSolver (),
      problem_ (pb),
      plugin_name_ ("")
  {
  }

  template <typename F, typename C>
  Solver<F, C>::~Solver ()
  {
  }

  template <typename F, typename C>
  const typename Solver<F, C>::problem_t&
  Solver<F, C>::problem () const
  {
    return problem_;
  }

  template <typename F, typename C>
  const typename Solver<F, C>::parameters_t&
  Solver<F, C>::parameters () const
  {
    return parameters_;
  }

  template <typename F, typename C>
  typename Solver<F, C>::parameters_t&
  Solver<F, C>::parameters ()
  {
    return parameters_;
  }

  template <typename F, typename C>
  template <typename T>
  const T&
  Solver<F, C>::getParameter (const std::string& key) const
  {
    parameters_t::const_iterator it = parameters_.find (key);
    return boost::get<T> (it->second.value);
  }

  template <typename F, typename C>
  const std::string&
  Solver<F, C>::pluginName () const
  {
    return plugin_name_;
  }

  template <typename F, typename C>
  std::string&
  Solver<F, C>::pluginName ()
  {
    return plugin_name_;
  }

  template <typename F, typename C>
  std::ostream&
  Solver<F, C>::print (std::ostream& o) const
  {
    o << incindent << "Solver:";

    if (!this->plugin_name_.empty ())
      o << iendl << "Plugin: " << this->plugin_name_;

    o << iendl << this->problem_;

    if (result_.which () != SOLVER_NO_SOLUTION)
      {
	o << iendl << "Result:" << incindent;
	o << iendl << result_;
	o << decindent;
      }

    if (!parameters_.empty ())
      {
        o << iendl << "Parameters:" << incindent;

        typedef const std::pair<const std::string, Parameter> const_iterator_t;
        BOOST_FOREACH (const_iterator_t& it, parameters_)
          o  << iendl << it.first << it.second;
        o << decindent;
      }

    o << decindent << iendl;
    return o;
  }
} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_SOLVER_HH
