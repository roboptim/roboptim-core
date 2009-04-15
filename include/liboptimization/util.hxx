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
 * \brief Useful procedures (templated) implementation.
 */
#ifndef OPTIMIZATION_UTIL_HXX
# define OPTIMIZATION_UTIL_HXX
# include <boost/variant/apply_visitor.hpp>

# include <liboptimization/function.hh>

namespace optimization
{
  namespace detail
  {
    template <typename T>
    void
    jacobian_from_gradients (DerivableFunction::matrix_t& jac,
                             const std::vector<const T*>& c,
                             const DerivableFunction::vector_t& x)
    {
      for (unsigned i = 0; i < jac.size1 (); ++i)
        {
          DerivableFunction::jacobian_t grad = c[i]->jacobian (x);
          for (unsigned j = 0; j < jac.size2 (); ++j)
            jac (i, j) = grad(0, j);
        }
    }


    struct GenVariantPrintVisitor : public boost::static_visitor<void>
    {
      GenVariantPrintVisitor (std::ostream& o)
        : o_ (o)
      {}

      template <typename T>
      void operator () (const T& value)
      {
        o_ << value;
      }

      template <typename T>
      void operator () (const T* value)
      {
        if (!value)
          o_ << "Null pointer";
        else
          o_ << *value;
      }

    private:
      std::ostream& o_;
    };

  }; // end of namespace detail.

  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream& o, const boost::variant<T1, T2>& var)
  {
    detail::GenVariantPrintVisitor pv (o);
    boost::apply_visitor (pv, var);
    return o;
  }

  template <typename T>
  std::ostream& operator<< (std::ostream& o, const std::vector<T>& vect)
  {
    typedef typename std::vector<T>::const_iterator citer_t;

    if (vect.empty ())
        return o << "Empty vector";

    citer_t it = vect.begin ();
    o << *it;
    ++it;

    for (; it != vect.end (); ++it)
      o << ", " << *it;
    return o;
  }

  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream& o, const std::pair<T1, T2>& p)
  {
    return o << "(" << p.first << ", " << p.second << ")";
  }

}; // end of namespace optimization.

#endif //! OPTIMIZATION_UTIL_HXX
