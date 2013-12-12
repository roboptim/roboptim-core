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
  namespace nag
  {
    /// \addtogroup roboptim_solver
    /// @{

    /// \brief Simplex algorithm: no contrainsts, no gradient needed.
    ///

    /// Finds an approximation to a minimum of a function F x of n
    /// variables. You must supply a function to calculate the value of
    /// F x for any set of values of the variables.
    ///
    /// The method is iterative. A simplex of n+1 points is set up in
    /// the n-dimensional space of the variables (for example, in two
    /// dimensions the simplex is a triangle) under the assumption that
    /// the problem has been scaled so that the values of the
    /// independent variables at the minimum are of order unity. The
    /// starting point you provided is the first vertex of the simplex,
    /// the remaining n vertices are generated internally (see Parkinson
    /// and Hutchinson (1972)). The vertex of the simplex with the
    /// largest function value is reflected in the centre of gravity of
    /// the remaining vertices and the function value at this new point
    /// is compared with the remaining function values. Depending on the
    /// outcome of this test the new point is accepted or rejected, a
    /// further expansion move may be made, or a contraction may be
    /// carried out. When no further progress can be made the sides of
    /// the simplex are reduced in length and the method is repeated.
    ///
    /// The method tends to be slow, but it is robust and therefore very
    /// useful for functions that are subject to inaccuracies.
    ///
    /// \see http://www.nag.com/numeric/CL/nagdoc_cl23/html/E04/e04ccc.html
    class ROBOPTIM_DLLEXPORT Simplex
      : public Solver<DifferentiableFunction, boost::mpl::vector<> >
    {
    public:
      typedef Solver<DifferentiableFunction, boost::mpl::vector<> > parent_t;

      explicit Simplex (const problem_t& pb) throw ();
      virtual ~Simplex () throw ();

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

      solverState_t& solverState () throw ()
      {
	return solverState_;
      }

    private:
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

      /// \brief Current solver state used by callback.
      solverState_t solverState_;
    };

    /// @}

  } // end of namespace nag.
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGING_NAG_NAG_DIFFERENTIABLE_HH
