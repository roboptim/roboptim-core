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
# include <vector>

# include <roboptim/core/solver.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_solver
  /// @{

  /// \brief Solver for C1 function without gradient computation, no
  ///        constraint.
  ///
  /// Searches for a minimum, in a given finite interval, of a
  /// continuous function of a single variable, using function values
  /// only. The method (based on quadratic interpolation) is intended
  /// for functions which have a continuous first derivative (although
  /// it will usually work if the derivative has occasional
  /// discontinuities).
  ///
  /// \see http://www.nag.com/numeric/CL/nagdoc_cl23/html/E04/e04abc.html
  class ROBOPTIM_DLLEXPORT NagSolver : public Solver<Function,
						     boost::mpl::vector<> >
  {
  public:
    typedef Solver<Function, boost::mpl::vector<> > parent_t;

    explicit NagSolver (const problem_t& pb) throw ();
    virtual ~NagSolver () throw ();

    /// \brief Solve the problem.
    void solve () throw ();

  private:
    /// \brief Relative accuracy.
    double e1_;
    /// \brief Absolute accuracy.
    double e2_;
    /// \brief Lower bound.
    std::vector<double> a_;
    /// \brief Upper bound.
    std::vector<double> b_;
    /// \brief Current minimum estimation.
    Function::vector_t x_;
    /// \brief Current cost.
    Function::vector_t f_;
  };

  /// @}
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGING_NAG_NAG_HH
