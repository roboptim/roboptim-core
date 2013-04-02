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

#ifndef ROBOPTIM_CORE_PLUGING_NAG_NAG_NLP_SPARSE_HH
# define ROBOPTIM_CORE_PLUGING_NAG_NAG_NLP_SPARSE_HH
# include <vector>

# include <roboptim/core/solver.hh>
# include <roboptim/core/numeric-linear-function.hh>
# include <roboptim/core/differentiable-function.hh>
# include <roboptim/core/twice-differentiable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_solver
  /// @{

  /// \brief Non-linear solver.
  ///
  /// Minimize an arbitrary smooth function subject to constraints
  /// (which may include simple bounds on the variables, linear
  /// constraints and smooth nonlinear constraints) using a sequential
  /// quadratic programming (SQP) method. As many first derivatives as
  /// possible should be supplied by you; any unspecified derivatives
  /// are approximated by finite differences. It is not intended for
  /// large sparse problems.
  ///
  /// \see http://www.nag.com/numeric/CL/nagdoc_cl23/html/E04/e04wdc.html
  class ROBOPTIM_DLLEXPORT NagSolverNlpSparse
    : public Solver<GenericDifferentiableFunction<EigenMatrixSparse>,
		    boost::mpl::vector<
		      GenericNumericLinearFunction<EigenMatrixSparse>,
		      GenericDifferentiableFunction<EigenMatrixSparse> > >
  {
  public:
    typedef Solver<GenericDifferentiableFunction<EigenMatrixSparse>,
		   boost::mpl::vector<
		     GenericNumericLinearFunction<EigenMatrixSparse>,
		     GenericDifferentiableFunction<EigenMatrixSparse> > >
    parent_t;

    explicit NagSolverNlpSparse (const problem_t& pb) throw ();
    virtual ~NagSolverNlpSparse () throw ();

    /// \brief Solve the problem.
    void solve () throw ();

  private:
    Integer n_;
    Integer m_;
    Integer ncnln_;
    Integer nonln_;
    Integer njnln_;
    Integer iobj_;
    Integer nnz_;

    Function::matrix_t a_;
    Integer* ha_;
    Integer* ka_;

    Function::vector_t bl_;
    Function::vector_t bu_;

    Function::vector_t xs_;

    Integer ninf_;
    double sinf_;

    Function::vector_t objf_;
  };

  /// @}
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGING_NAG_NAG_NLP_SPARSE_HH
