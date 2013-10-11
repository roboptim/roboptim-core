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
# include <roboptim/core/linear-function.hh>
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
		      GenericLinearFunction<EigenMatrixSparse>,
		      GenericDifferentiableFunction<EigenMatrixSparse> > >
  {
  public:
    typedef Solver<GenericDifferentiableFunction<EigenMatrixSparse>,
      boost::mpl::vector<
	GenericLinearFunction<EigenMatrixSparse>,
	GenericDifferentiableFunction<EigenMatrixSparse> > >
    parent_t;
  typedef GenericLinearFunction<EigenMatrixSparse>
  linearFunction_t;
  typedef GenericNumericLinearFunction<EigenMatrixSparse>
  numericLinearFunction_t;

  typedef GenericDifferentiableFunction<EigenMatrixSparse>
  nonlinearFunction_t;
  typedef problem_t::function_t function_t;

  static const int linearFunctionId = 0;
  static const int nonlinearFunctionId = 1;

  explicit NagSolverNlpSparse (const problem_t& pb) throw ();
  virtual ~NagSolverNlpSparse () throw ();

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
  void compute_nf ();
  void fill_xlow_xupp ();
  void fill_flow_fupp ();
  void fill_iafun_javar_lena_nea ();
  void fill_igfun_jgvar_leng_neg ();
  void fill_fnames ();

  function_t::vector_t lookForX ();
  function_t::vector_t lookForX (unsigned constraintId);

  Integer nf_;
  Integer n_;
  Integer nxname_;
  Integer nfname_;
  double objadd_;
  Integer objrow_;
  std::string prob_;

  std::vector<Integer> iafun_;
  std::vector<Integer> javar_;

  std::vector<double> a_;

  Integer lena_;
  Integer nea_;

  std::vector<Integer> igfun_;
  std::vector<Integer> jgvar_;

  Integer leng_;

  Integer neg_;

  Function::vector_t xlow_;
  Function::vector_t xupp_;

  std::vector<const char*> xnames_;

  Function::vector_t flow_;
  Function::vector_t fupp_;

  std::vector<const char*> fnames_;

  Function::vector_t x_;
  std::vector<Integer> xstate_;
  Function::vector_t xmul_;
  Function::vector_t f_;
  std::vector<Integer> fstate_;
  Function::vector_t fmul_;
  Integer ns_;

  Integer ninf_;
  double sinf_;

  callback_t callback_;
};

/// @}
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGING_NAG_NAG_NLP_SPARSE_HH
