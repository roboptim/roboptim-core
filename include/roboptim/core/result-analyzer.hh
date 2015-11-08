// Copyright (C) 2015 by Benjamin Chrétien, CNRS-UM LIRMM.
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

#ifndef ROBOPTIM_CORE_RESULT_ANALYZER_HH
# define ROBOPTIM_CORE_RESULT_ANALYZER_HH

# include <roboptim/core/problem.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  template <typename T>
  class ResultAnalyzer
  {
  public:
    typedef GenericFunctionTraits<T>              functionTraits_t;
    typedef Problem<T>                            problem_t;
    typedef Result                                optimResult_t;
    typedef typename functionTraits_t::argument_t argument_t;
    typedef typename functionTraits_t::result_t   result_t;
    typedef typename functionTraits_t::gradient_t gradient_t;
    typedef typename functionTraits_t::jacobian_t jacobian_t;
    typedef typename problem_t::interval_t        interval_t;
    typedef typename problem_t::size_type         size_type;
    typedef typename problem_t::value_type        value_type;

    typedef GenericDifferentiableFunction<T> differentiableFunction_t;

    ResultAnalyzer (const problem_t& pb, const optimResult_t& res);
    virtual ~ResultAnalyzer ();

    struct QueryData
    {
      operator bool () const;

      virtual bool isValid () const = 0;
    };

    struct LICQData : public QueryData
    {
      LICQData ();
      bool isValid () const;

      size_type rank;
      size_type max_rank;
    };

    struct KKTData : public QueryData
    {
      KKTData ();
      bool isValid () const;

      gradient_t grad_L;
      value_type eps;
    };

    struct NullGradientData : public QueryData
    {
      NullGradientData();
      bool isValid () const;

      size_type null_rows;
    };

    /// \brief Check the LICQ conditions.
    LICQData checkLICQ () const;

    /// \brief Check the KKT conditions.
    KKTData checkKKT () const;

    /// \brief Check for null gradients.
    NullGradientData checkNullGradient () const;

  private:
    void computeJacobian () const;
    void computeActiveJacobian () const;

    size_type computeRank (const jacobian_t& jac) const;

    bool hasLambda () const;

  private:
    /// \brief Problem.
    const problem_t pb_;

    /// \brief Optimization result.
    const optimResult_t res_;

    /// \brief Jacobian of the active constraints.
    mutable jacobian_t activeJac_;

    /// \brief Jacobian of the constraints.
    mutable jacobian_t jac_;

    /// \brief Epsilon.
    value_type eps_;
  };
} // end of namespace roboptim

# include <roboptim/core/result-analyzer.hxx>

#endif //! ROBOPTIM_CORE_RESULT_ANALYZER_HH
