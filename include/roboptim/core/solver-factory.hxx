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

#ifndef ROBOPTIM_CORE_SOLVER_FACTORY_HXX
# define ROBOPTIM_CORE_SOLVER_FACTORY_HXX
# include <cstddef>
# include <sstream>
# include <typeinfo>

# include <roboptim/core/util.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  // This union is here to workaround a limitation of the C++
  // standard. There is *no* way to cast a pointer to object into a
  // pointer to function.
  //
  // Unfortunately lt_dlsym is giving us a void* so we have to use
  // this kind of trick to transform it into a pointer to function.
  template <typename S>
  S* unionCast(void* ptr)
  {
    union
    {
      void* ptr;
      S* real_ptr;
    } u;
    u.ptr = ptr;
    return u.real_ptr;
  }

  template <typename S>
  SolverFactory<S>::SolverFactory (std::string plugin, const problem_t& pb)
    : handle_ (),
      solver_ ()
  {
    typedef std::size_t getsizeofproblem_t ();
    typedef const char* gettypeidofconstraintslist_t ();
    typedef solver_t* create_t (const problem_t&);

    if (lt_dlinit () > 0)
      throw std::runtime_error ("failed to initialize libltdl.");

    std::stringstream ss;
    ss << "roboptim-core-plugin-" << plugin;
    handle_ = lt_dlopenext (ss.str ().c_str ());
    if (!handle_)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to load plug-in ``"
		<< ss.str () << "'': " << lt_dlerror ();
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
	throw std::runtime_error (sserror.str ().c_str ());
      }

    getsizeofproblem_t* getSizeOfProblem =
      unionCast<getsizeofproblem_t> (lt_dlsym (handle_, "getSizeOfProblem"));
    if (!getSizeOfProblem)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to find symbol ``getSizeOfProblem'': "
		<< lt_dlerror ();

	if (lt_dlclose (handle_))
	  sserror << " (lt_dlclose failed too)";
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
	throw std::runtime_error (sserror.str ().c_str ());
      }

    gettypeidofconstraintslist_t* getTypeIdOfConstraintsList =
      unionCast<gettypeidofconstraintslist_t>
      (lt_dlsym (handle_, "getTypeIdOfConstraintsList"));
    if (!getTypeIdOfConstraintsList)
      {
        std::stringstream sserror;
        sserror << "libltdl failed to find symbol"
                << " ``getTypeIdOfConstraintsList'': "
                << lt_dlerror ();

	if (lt_dlclose (handle_))
	  sserror << " (lt_dlclose failed too)";
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
        throw std::runtime_error (sserror.str ().c_str ());
      }

    std::size_t sizeOfProblem = getSizeOfProblem ();
    if (sizeOfProblem != sizeof (typename solver_t::problem_t))
      {
	std::stringstream sserror;
	sserror
	  << "``Problem'' type size does not match in application and plug-in"
	  << " (size is " << sizeOfProblem
	  << " byte(s) but " << sizeof (typename solver_t::problem_t)
	  << " byte(s) was expected by application)";

	if (lt_dlclose (handle_))
	  sserror << " (lt_dlclose failed too)";
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
	throw std::runtime_error (sserror.str ().c_str ());
      }

    const std::string typeIdOfConstraintsList
      = demangle(getTypeIdOfConstraintsList ());
    const std::string expectedTypeIdOfConstraintsList
      = demangle(typeid
                 (typename solver_t::problem_t::constraintsList_t).name ());
    if (typeIdOfConstraintsList != expectedTypeIdOfConstraintsList)
      {
        std::stringstream sserror;
        sserror
          << "``Problem::constraintsList_t'' type id does not match in"
          << " application and plug-in. Type id is:\n"
          << typeIdOfConstraintsList
          << "\nbut application expected:\n"
          << expectedTypeIdOfConstraintsList;

	if (lt_dlclose (handle_))
	  sserror << " (lt_dlclose failed too)";
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
        throw std::runtime_error (sserror.str ().c_str ());
      }

    create_t* c =
      unionCast<create_t> (lt_dlsym (handle_, "create"));
    if (!c)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to find symbol ``create'': "
		<< lt_dlerror ();

	if (lt_dlclose (handle_))
	  sserror << " (lt_dlclose failed too)";
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
	throw std::runtime_error (sserror.str ().c_str ());
      }

    solver_ = c (pb);

    if (!solver_)
      {
	std::stringstream sserror;
	sserror << "libltdl failed to call ``create'': "
		<< lt_dlerror ();

	if (lt_dlclose (handle_))
	  sserror << " (lt_dlclose failed too)";
	if (lt_dlexit ())
	  sserror << " lt_dlexit failed too";
	throw std::runtime_error (sserror.str ().c_str ());
      }

    solver_->pluginName () = plugin;
  }

  template <typename S>
  SolverFactory<S>::~SolverFactory ()
  {
    typedef void destroy_t (solver_t*);

    destroy_t* destructor =
      unionCast<destroy_t> (lt_dlsym (handle_, "destroy"));
    if (destructor)
      {
        destructor (solver_);
        solver_ = 0;
      }
    else
      {
	std::stringstream sserror;
	sserror << "libltdl failed to call ``destroy'': "
		<< lt_dlerror ();
	std::cerr << sserror.str () << std::endl;
      }

    if (lt_dlclose (handle_))
      {
	std::stringstream sserror;
	sserror << "libltdl failed to close plug-in: "
		<< lt_dlerror ();
	std::cerr << sserror.str () << std::endl;
      }

    if (lt_dlexit ())
      {
	std::stringstream sserror;
	sserror << "libltdl failed to call ``create'': "
		<< lt_dlerror ();
	std::cerr << sserror.str () << std::endl;
      }
  }

  template <typename S>
  typename SolverFactory<S>::solver_t&
  SolverFactory<S>::operator () ()
  {
    assert (solver_ != 0);
    return *solver_;
  }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
  extern template class SolverFactory<Solver<EigenMatrixDense> >;
  extern template class SolverFactory<Solver<EigenMatrixSparse> >;
# endif //! ROBOPTIM_PRECOMPILED_DENSE_SPARSE

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SOLVER_FACTORY_HXX
