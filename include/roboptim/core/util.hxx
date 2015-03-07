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
    jacobian_from_gradients (DifferentiableFunction::matrix_ref jac,
                             const std::vector<const T*>& c,
                             DifferentiableFunction::const_vector_ref x)
    {
      for (DifferentiableFunction::matrix_t::Index i = 0; i < jac.rows (); ++i)
        {
          DifferentiableFunction::jacobian_t grad = c[i]->jacobian (x);
          for (DifferentiableFunction::matrix_t::Index j = 0;
	       j < jac.cols (); ++j)
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

  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream& o, const std::map<T1,T2>& m)
  {
    typedef typename std::map<T1,T2>::const_iterator citer_t;

    if (m.empty ())
      return o << "{}";

    citer_t it = m.begin ();
    o << "{" << it->first << ": " << it->second;
    ++it;

    for (; it != m.end (); ++it)
      o << ", " << it->first << ": " << it->second;
    return o << "}";
  }

  template <typename T>
  std::ostream& operator<< (std::ostream& o, const Eigen::MatrixBase<T>& matrix)
  {
    Eigen::IOFormat ioformat (Eigen::StreamPrecision,
                              Eigen::DontAlignCols,
                              ",", ", ", "(", ")", "(", ")");
    ioformat.rowSpacer = "";
    o << "[";

    // Matrix
    if (matrix.cols () == 1 || matrix.rows () == 1)
      {
	// Vector
        ioformat = Eigen::IOFormat (Eigen::StreamPrecision,
                                    Eigen::DontAlignCols,
                                    ",", ",", "", "", "(", ")");
	ioformat.rowSpacer = "";
	o << matrix.size ();
      }
    else
      o << matrix.rows () << "," << matrix.cols ();

    o << "]" << matrix.format (ioformat);
    return o;
  }


  template <typename U>
  void copySparseBlock
  (U& matrix,
   const U& block,
   Function::size_type startRow, Function::size_type startCol,
   bool compress)
  {
    typedef U matrix_t;
    typedef typename U::Index index_t;

    // Make sure that the block fits in the matrix
    assert (startRow + block.rows () <= matrix.rows ());
    assert (startCol + block.cols () <= matrix.cols ());

    // Iterate over outer size
    for (int k = 0; k < block.outerSize (); ++k)
      {
	// Iterator over inner size
	for (typename matrix_t::InnerIterator it (block.derived (), k);
	     it; ++it)
          {
	    matrix.coeffRef (static_cast<index_t> (startRow + it.row ()),
			     static_cast<index_t> (startCol + it.col ()))
	      = it.value ();
          }
      }

    // Compress sparse matrix if asked
    if (compress)
      matrix.makeCompressed ();
  }

  inline double normalize (double x)
  {
      return (std::fabs (x) < 1e-8)? 0:x;
  }

  template <typename T>
  inline T normalize (const T& x)
  {
    return (x.array ().abs () < 1e-8).select (0, x);
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_UTIL_HXX
