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

#ifndef ROBOPTIM_CORE_UTIL_HXX
# define ROBOPTIM_CORE_UTIL_HXX
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  namespace detail
  {
    template <typename T>
    void
    jacobian_from_gradients (typename DifferentiableFunction::matrix_t& jac,
                             const std::vector<const T*>& c,
                             const DifferentiableFunction::vector_t& x)
    {
      for (unsigned i = 0; i < jac.rows (); ++i)
        {
          DifferentiableFunction::jacobian_t grad = c[i]->jacobian (x);
          for (unsigned j = 0; j < jac.cols (); ++j)
            jac (i, j) = grad(0, j);
        }
    }
  } // end of namespace detail.

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

  /// \brief Display an Eigen object with the appropriate IOFormat.
  template <typename T>
  std::ostream& operator<< (std::ostream& o, const Eigen::MatrixBase<T>& matrix)
  {
    Eigen::IOFormat ioformat (Eigen::StreamPrecision,
			      0, ",", ", ", "(", ")", "(", ")");
    ioformat.rowSpacer = "";
    o << "[";

    // Matrix
    if (matrix.cols () == 1 || matrix.cols () == 1)
      {
	// Vector
	ioformat = Eigen::IOFormat (Eigen::StreamPrecision,
				    0, ",", ",", "", "", "(", ")");
	ioformat.rowSpacer = "";
	o << matrix.size ();
      }
    else
      o << matrix.rows () << "," << matrix.cols ();

    o << "]" << matrix.format (ioformat);
    return o;
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_UTIL_HXX
