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
 * \brief Implementation of the Solver class.
 */

#ifndef ROBOPTIM_CORE_SOLVER_FACTORY_HXX
# define ROBOPTIM_CORE_SOLVER_FACTORY_HXX
# include <sstream>

namespace roboptim
{
  template <typename T>
  SolverFactory<T>::SolverFactory (std::string plugin, const problem_t& pb)
  throw (std::runtime_error)
    : handle_ (),
      solver_ ()
  {
    typedef solver_t* create_t (const problem_t&);

    if (lt_dlinit () > 0)
      throw std::runtime_error ("failed to initialize libltdl.");

    std::stringstream ss;
    ss << "roboptim-core-" << plugin << "-plugin";
    handle_ = lt_dlopenext (ss.str ().c_str ());
    if (!handle_)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to load plug-in ``"
		<< ss.str () << "'': " << lt_dlerror ();
	throw std::runtime_error (sserror.str ().c_str ());
      }

    create_t* c =
      reinterpret_cast<create_t*> (lt_dlsym (handle_, "create"));
    if (!c)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to find symbol ``create'': "
		<< lt_dlerror ();
	throw std::runtime_error (sserror.str ().c_str ());
      }
    solver_ = c (pb);

    if (!solver_)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to call ``create'': "
		<< lt_dlerror ();
	throw std::runtime_error (sserror.str ().c_str ());
      }
  }

  template <typename T>
  SolverFactory<T>::~SolverFactory () throw ()
  {
    typedef void destroy_t (solver_t*);
    destroy_t* destructor =
      reinterpret_cast<destroy_t*> (lt_dlsym (handle_, "destroy"));
    if (destructor)
      {
        destructor (solver_);
        solver_ = 0;
      }

    lt_dlclose (handle_);
    lt_dlexit ();
  }

  template <typename T>
  typename SolverFactory<T>::solver_t&
  SolverFactory<T>::operator () () throw ()
  {
    assert (solver_ != 0);
    return *solver_;
  }

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SOLVER_FACTORY_HH
