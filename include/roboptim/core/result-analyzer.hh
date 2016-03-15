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

# pragma message "ResultAnalyzer is an unfinished experimental feature."

# include <vector>
# include <ostream>

# include <boost/spirit/include/classic_safe_bool.hpp>

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
    typedef typename functionTraits_t::vector_t   vector_t;
    typedef typename problem_t::interval_t        interval_t;
    typedef typename problem_t::size_type         size_type;
    typedef typename problem_t::value_type        value_type;

    struct ConstraintIndex
    {
      /// \brief Index in the global constraint vector.
      size_t global;

      /// \brief Index in the local constraint representation.
      size_type local;

      /// \brief Index in the global active set.
      size_type active;
    };

    typedef GenericDifferentiableFunction<T> differentiableFunction_t;

    /// \brief Constructor.
    ///
    /// \param pb problem to analyze.
    /// \param res result of an optimization.
    /// \param eps epsilon used for zero checking.
    ResultAnalyzer (const problem_t& pb, const optimResult_t& res,
                    value_type eps = 1e-12);
    virtual ~ResultAnalyzer ();

    /// \brief Interface for checks.
    /// Note: we use the safe-bool idiom, which is deprecated in C++11.
    struct QueryData : public boost::spirit::classic::safe_bool<QueryData>
    {
      /// \brief Evaluate the query.
      bool operator_bool () const;

      /// \brief Whether the input data is valid w.r.t. that query.
      virtual bool isValid () const = 0;

      /// \brief Print method.
      virtual std::ostream& print (std::ostream& o) const = 0;

      /// \brief Override operator<< to display data.
      /// \param o output stream.
      /// \param d data.
      /// \return output stream.
      friend std::ostream& operator<< (std::ostream& o, const QueryData& d)
      {
        return d.print (o);
      }
    };

    /// \brief Data for LICQ check.
    struct LICQData : public QueryData
    {
      LICQData ();
      bool isValid () const;
      std::ostream& print (std::ostream& o) const;

      size_type rank;
      size_type max_rank;
    };

    /// \brief Data for KKT check.
    struct KKTData : public QueryData
    {
      KKTData ();
      bool isValid () const;
      std::ostream& print (std::ostream& o) const;

      // \brief ∇L(x*,λ*)
      gradient_t grad_L;

      /// \brief Lagrange multipliers λ.
      vector_t lambda;

      /// \brief Constraint violation.
      value_type violation;

      /// \brief Complementary slackness.
      value_type complementary_slackness;

      /// \brief Whether dual feasibility is satisfied.
      bool dual_feasible;

      // \brief Epsilon used for checking.
      value_type eps;
    };

    /// \brief Data for null-gradient check.
    struct NullGradientData : public QueryData
    {
      typedef typename problem_t::constraint_t constraint_t;

      NullGradientData();
      bool isValid () const;
      std::ostream& print (std::ostream& o) const;

      size_type null_rows;
      std::map<const constraint_t, std::vector<size_type> >
        constraint_indices;
    };

    /// \brief Check the LICQ (Linear Independence Constraint Qualification)
    /// conditions.
    LICQData checkLICQ () const;

    /// \brief Check the KKT (Karush-Kuhn-Tucker) conditions.
    KKTData checkKKT () const;

    /// \brief Check for null gradients.
    NullGradientData checkNullGradient () const;

  private:
    void computeJacobian () const;
    void computeActiveJacobian () const;
    void findIneqIndices () const;

    /// \brief Compute the rank of the Jacobian matrix.
    ///
    /// \param jac Jacobian matrix. It is not const as it needs to be
    /// compressed in sparse mode.
    ///
    /// \return rank of the matrix
    size_type computeRank (jacobian_t& jac) const;

    void copyRow (jacobian_t& dst, size_type row_dst,
                  const jacobian_t& src, size_type row_src) const;

    gradient_t gradLagrangian () const;

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

    /// \brief Indices of the active constraints.
    mutable std::vector<ConstraintIndex> activeCstrIndices_;

    /// \brief Indices of inequality constraints.
    mutable std::vector<size_type> ineqIndices_;

    /// \brief Whether an active constraint is a lower bound.
    mutable std::vector<bool> isLowerBound_;

    /// \brief Epsilon.
    value_type eps_;
  };
} // end of namespace roboptim

# include <roboptim/core/result-analyzer.hxx>

#endif //! ROBOPTIM_CORE_RESULT_ANALYZER_HH
