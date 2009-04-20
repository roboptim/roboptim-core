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


/**
 * \brief Declaration of the IpoptSolver class.
 */

#ifndef ROBOPTIM_CORE_IPOPT_HH
# define ROBOPTIM_CORE_IPOPT_HH
# include <coin/IpSmartPtr.hpp>
# include <roboptim-core/solver.hh>
# include <roboptim-core/twice-derivable-function.hh>

/// Ipopt classes.
namespace Ipopt
{
  class TNLP;
  class IpoptApplication;
} // end of namespace Ipopt


namespace roboptim
{
  namespace detail
  {
    /// \internal
    class MyTNLP;
  };

  /// Ipopt based solver.
  class IpoptSolver : public Solver<TwiceDerivableFunction,
                                    const TwiceDerivableFunction*>
  {
  public:
    friend class detail::MyTNLP;
    typedef Solver<TwiceDerivableFunction, const TwiceDerivableFunction*> parent_t;

    /// Constructor.
    explicit IpoptSolver (const problem_t&) throw ();
    /// Destructor.
    virtual ~IpoptSolver () throw ();

    /// Solve the problem.
    virtual void solve () throw ();

    /// Get Ipopt Application object for Ipopt specific tuning.
    virtual Ipopt::SmartPtr<Ipopt::IpoptApplication> getIpoptApplication ()
      throw ();
  private:
    /// Smart pointer to the Ipopt non linear problem description.
    Ipopt::SmartPtr<Ipopt::TNLP> nlp_;
    /// Smtart pointer to the Ipopt application instance.
    Ipopt::SmartPtr<Ipopt::IpoptApplication> app_;
  };

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_IPOPT_HH
