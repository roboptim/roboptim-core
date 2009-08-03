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

#ifndef ROBOPTIM_CORE_SOLVER_FACTORY_HH
# define ROBOPTIM_CORE_SOLVER_FACTORY_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <stdexcept>
# include <string>

# include <ltdl.h>

# include <boost/static_assert.hpp>
# include <boost/type_traits/is_base_of.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/solver.hh>
# include <roboptim/core/solver-error.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Define a solver factory that instanciate the plug-ins.
  ///
  /// The solver factory has to be used to instantiate solvers which
  /// are packaged as roboptim-core plug-ins.
  ///
  /// This class uses libltdl to handle plug-ins. This lightweight library
  /// is provided with GNU Libtool and wraps OS specific behavior into
  /// a uniform interface.
  ///
  /// \warning The solver lifetime is bound to the factory lifetime,
  /// when the factory goes out of scope, the solver is destroyed too.
  ///
  /// \tparam T solver type
  /// \pre T has to be a subtype of Solver<F,C>.
  template <typename T>
  class SolverFactory
  {
    BOOST_STATIC_ASSERT((boost::is_base_of<GenericSolver, T>::value));
  public:
    /// \brief Solver type.
    typedef T solver_t;
    /// \brief Problem type.
    typedef typename T::problem_t problem_t;

    /// \brief Instantiate a factory and load the plug-in.
    ///
    /// The constructor search for the plug-in and load it.
    /// If the wanted plug-in can not be found, an exception is thrown.
    ///
    /// \param solver solver name (for instance ``cfsqp'')
    /// \param problem problem that will be solved
    explicit SolverFactory (std::string solver, const problem_t& problem)
      throw (std::runtime_error);

    /// Unload the plug-in and free the instantiated solver.
    ~SolverFactory () throw ();

    /// \brief Retrieve a reference on the solver.
    /// \return reference on a solver
    solver_t& operator () () throw ();

  private:
    /// \brief ltdl plug-in handle.
    lt_dlhandle handle_;
    /// \brief Allocated solver.
    solver_t* solver_;
  };

  /// @}

} // end of namespace roboptim

# include <roboptim/core/solver-factory.hxx>
#endif //! ROBOPTIM_CORE_SOLVER_FACTORY_HH
