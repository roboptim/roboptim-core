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

#ifndef ROBOPTIM_CORE_FREEZE_HH
# define ROBOPTIM_CORE_FREEZE_HH
# include <vector>
# include <utility>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/derivable-function.hh>
# include <roboptim/core/problem.hh>

namespace roboptim
{
  /// \brief Constraint that can be used to force some arguments values.
  ///
  /// This constraint expects a vector of pairs (argument id, value) to
  /// build a function that will freeze the given arguments to their
  /// associated value.
  ///
  /// For instance, the vector: [(0, 5.), (3, -12.)] forces the first
  /// parameter to five and the fourth one to minus twelve.
  ///
  // The expression of this function is:
  /// \f[-1 + \sum_{i} (x_i - value_i)^2\f]
  /// where \f$i\f$ covers the index list specified by the vector described
  /// before, \f$x_i\f$ is i-th value of the input argument and \f$value_i\f$
  /// is this argument's fixed value.
  ///
  /// Hence, the global minimum of this function is -1 and is reached
  /// when the current position matches the frozen position.
  class Freeze : public DerivableFunction
  {
  public:
    /// \brief Pair representing an argument index and a value.
    typedef std::pair<size_t, value_type> frozenArgument_t;

    /// \brief Vector of pairs (argument index, value).
    ///
    /// This type define what are the frozen arguments and what their
    /// value.
    typedef std::vector<frozenArgument_t> frozenArguments_t;

    /// \brief Create the constraint from a vector of pairs.
    ///
    /// \param size Input size (function argument's size)
    /// \param fa Vector of pairs containing what to freeze and to what value.
    Freeze (const size_t size, const frozenArguments_t fa) throw ();

    virtual ~Freeze () throw ();

    /// \brief Add itself to an optimization problem.
    /// \param pb Problem on which the constraint will be added.
    template <typename F, typename C>
    void addToProblem (Problem<F, C>& pb) throw ()
    {
      pb.addConstraint (this, Function::makeBound (-1., -1.));
    }

  protected:
    void impl_compute (result_t& res, const argument_t& x) const throw ();
    void impl_gradient (gradient_t& grad, const argument_t& x, int i)
      const throw ();
  private:
    /// Vector which defines how to freeze values.
    const frozenArguments_t frozenArguments_;

  };
} // end of namespace roboptim.

#endif //! ROBOPTIM_FREEZE_HH
