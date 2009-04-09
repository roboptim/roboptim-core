// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \brief Declaration of the IpoptSolver class.
 */

#ifndef OPTIMIZATION_IPOPT_HH
# define OPTIMIZATION_IPOPT_HH
# include <coin/IpSmartPtr.hpp>
# include <liboptimization/solver.hh>
# include <liboptimization/twice-derivable-function.hh>

/// Ipopt classes.
namespace Ipopt
{
  class TNLP;
  class IpoptApplication;
}; // end of namespace Ipopt


namespace optimization
{
  namespace detail
  {
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
    virtual Ipopt::SmartPtr<Ipopt::IpoptApplication> getApplication ()
      throw ();
  private:
    /// Smart pointer to the Ipopt non linear problem description.
    Ipopt::SmartPtr<Ipopt::TNLP> nlp_;
    /// Smtart pointer to the Ipopt application instance.
    Ipopt::SmartPtr<Ipopt::IpoptApplication> app_;
  };

}; // end of namespace optimization

#endif //! OPTIMIZATION_IPOPT_HH
