// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
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

#ifndef ROBOPTIM_CORE_SCALING_HELPER_HH
# define ROBOPTIM_CORE_SCALING_HELPER_HH

# include <vector>
# include <utility>
# include <ostream>

# include <boost/shared_ptr.hpp>

# include <roboptim/core/problem.hh>

namespace roboptim
{
  /// \brief Helper class used to help the user find good scaling parameters.
  /// We want the gradients to be of the order 0.1 to 100.
  ///
  /// \tparam T matrix type (dense or sparse).
  template <typename T>
  class ScalingHelper
  {
  public:
    typedef Problem<T> problem_t;
    typedef typename problem_t::value_type value_type;
    typedef typename problem_t::size_type size_type;
    typedef typename problem_t::argument_t argument_t;
    typedef typename problem_t::scaling_t scaling_t;
    typedef typename problem_t::interval_t interval_t;
    typedef typename problem_t::constraints_t constraints_t;
    typedef typename problem_t::function_t function_t;
    typedef typename problem_t::jacobian_t jacobian_t;
    typedef boost::shared_ptr<function_t> constraint_t;
    typedef std::pair<constraint_t, scaling_t> scalingFunc_t;

    /// \brief Constructor.
    /// \param pb input problem.
    /// \param gradRange valid absolute gradient range after scaling.
    ScalingHelper (const problem_t& pb,
                   const interval_t& gradRange = function_t::makeInterval (0.1, 10.));
    virtual ~ScalingHelper ();

    /// \brief Get a possible scaling vector based on the gradient's infinity
    /// norm.
    /// \param x vector of arguments to consider.
    /// \return scaling vector.
    void computeScaling (const std::vector<argument_t>& x);

    /// \brief Print method.
    /// \param o output stream.
    /// \return modified output stream.
    virtual std::ostream& print (std::ostream& o) const;

  private:
    /// \brief Get the infinite norm of a row of the Jacobian.
    /// \param jac Jacobian matrix.
    /// \param i row index.
    /// \return infinity norm.
    value_type maxRow (const jacobian_t& jac, size_type i) const;

  private:
    /// \brief Copy of the input problem.
    const problem_t pb_;

    /// \brief Expected gradient range.
    const interval_t gradRange_;

    /// \brief Vector of pairs of function/scaling parameters.
    std::vector<scalingFunc_t> scalingFunc_;
  };

  /// \brief Override operator<< to handle scaling helper display.
  /// \param o output stream used for display.
  /// \param sh scaling helper to display.
  /// \return output stream.
  template <typename T>
  std::ostream& operator<< (std::ostream& o, const ScalingHelper<T>& sh);
} // end of namespace roboptim

# include <roboptim/core/scaling-helper.hxx>

#endif //! ROBOPTIM_CORE_SCALING_HELPER_HH
