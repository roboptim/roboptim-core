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
 * \file src/ipopt.hh
 *
 * \brief Declaration of the IpoptSolver class.
 */

#ifndef OPTIMIZATION_IPOPT_HH
# define OPTIMIZATION_IPOPT_HH
# include <IpSmartPtr.hpp>
# include <solver.hh>

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
  class IpoptSolver : public Solver
  {
  public:
    friend class detail::MyTNLP;

    /// Constructor.
    explicit IpoptSolver (const Problem&) throw ();
    /// Destructor.
    virtual ~IpoptSolver () throw ();

    /// Returns the minimum and solve the problem using Ipopt if necessary.
    virtual result_t getMinimum () throw ();

    /// Get Ipopt Application object for Ipopt specific tuning.
    virtual Ipopt::SmartPtr<Ipopt::IpoptApplication> getApplication ()
      throw ();
  private:
    /// Smart pointer to the Ipopt non linear problem description.
    Ipopt::SmartPtr<Ipopt::TNLP> nlp_;
    /// Smtart pointer to the Ipopt application instance.
    Ipopt::SmartPtr<Ipopt::IpoptApplication> app_;
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_IPOPT_HH
