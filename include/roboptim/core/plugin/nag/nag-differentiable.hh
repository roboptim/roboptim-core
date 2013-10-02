// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS.
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

#ifndef ROBOPTIM_CORE_PLUGING_NAG_NAG_DIFFERENTIABLE_HH
# define ROBOPTIM_CORE_PLUGING_NAG_NAG_DIFFERENTIABLE_HH
# include <vector>

# include <roboptim/core/solver.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_solver
  /// @{

  /// \brief Solver for C1 function with gradient computation, no
  ///        constraint.
  ///
  /// Search for a minimum, in a given finite interval, of a
  /// continuous function of a single variable, using function and
  /// first derivative values. The method (based on cubic
  /// interpolation) is intended for functions which have a continuous
  /// first derivative (although it will usually work if the
  /// derivative has occasional discontinuities).
  ///
  /// \see http://www.nag.com/numeric/CL/nagdoc_cl23/html/E04/e04bbc.html
  class ROBOPTIM_DLLEXPORT NagSolverDifferentiable
    : public Solver<DifferentiableFunction, boost::mpl::vector<> >
  {
  public:
    typedef Solver<DifferentiableFunction, boost::mpl::vector<> > parent_t;

    explicit NagSolverDifferentiable (const problem_t& pb) throw ();
    virtual ~NagSolverDifferentiable () throw ();

    /// \brief Solve the problem.
    void solve () throw ();

    void
    setIterationCallback (callback_t callback) throw (std::runtime_error)
    {
      callback_ = callback;
    }

    const callback_t& callback () const throw ()
    {
      return callback_;
    }

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
    /// \brief Current gradient.
    Function::vector_t g_;

    /// \brief Per-iteration callback function.
    callback_t callback_;
  };

  /// @}
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGING_NAG_NAG_DIFFERENTIABLE_HH
