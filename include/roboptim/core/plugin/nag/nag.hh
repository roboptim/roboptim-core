// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_PLUGING_NAG_NAG_HH
# define ROBOPTIM_CORE_PLUGING_NAG_NAG_HH
# include <roboptim/core/solver.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_solver
  /// @{

  class ROBOPTIM_DLLEXPORT NagSolver : public Solver<Function, boost::mpl::vector<> >
  {
  public:
    typedef Solver<Function, boost::mpl::vector<> > parent_t;

    explicit NagSolver (const problem_t& pb) throw ();
    virtual ~NagSolver () throw ();

    /// \brief Solve the problem.
    void solve () throw ();
  };

  /// @}
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGING_NAG_NAG_HH
