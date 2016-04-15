// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

# include <stdexcept>
# include <typeinfo>

# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  namespace detail
  {
    template <typename T>
    void
    jacobian_from_gradients (Function::matrix_ref jac,
                             const std::vector<const T*>& c,
                             Function::const_vector_ref x)
    {
      for (Function::matrix_t::Index i = 0; i < jac.rows (); ++i)
        {
          Function::matrix_t grad = c[i]->jacobian (x);
          for (Function::matrix_t::Index j = 0;
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
      return o << "empty vector";

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

  template <typename T>
  std::string typeString ()
  {
    // TODO: use Boost.TypeIndex (Boost >= 1.56.0) instead?
    return demangle (typeid (T).name ());
  }

  template <typename M, typename B>
  void copySparseBlock
  (M& matrix,
   const B& block,
   Function::size_type startRow, Function::size_type startCol,
   bool compress)
  {
    typedef M matrix_t;
    typedef typename M::Index index_t;

    // Make sure that the block fits in the matrix
    ROBOPTIM_ASSERT (startRow + block.rows () <= matrix.rows ());
    ROBOPTIM_ASSERT (startCol + block.cols () <= matrix.cols ());

    // Iterate over outer size
    for (int k = 0; k < block.outerSize (); ++k)
      {
	// Iterator over inner size
	for (typename matrix_t::InnerIterator it (block.derived (), k);
	     it; ++it)
          {
	    ROBOPTIM_ASSERT_MSG (startRow + it.row () < matrix.rows (),
                                 startRow + it.row () << " ≥ "
                                 << matrix.rows ());
	    ROBOPTIM_ASSERT_MSG (startCol + it.col () < matrix.cols (),
                                 startCol + it.col () << " ≥ "
                                 << matrix.cols ());
	    matrix.coeffRef (static_cast<index_t> (startRow + it.row ()),
			     static_cast<index_t> (startCol + it.col ()))
	      = it.value ();
          }
      }

    // Compress sparse matrix if asked
    if (compress)
      matrix.makeCompressed ();
  }

  template <typename M, typename B>
  void updateSparseBlock
  (M& m, const B& b,
   Function::size_type startRow, Function::size_type startCol)
  {
    typedef M matrix_t;
    typedef B block_t;
    typedef typename M::Index index_t;

    // Make sure that the block fits in the matrix
    ROBOPTIM_ASSERT (startRow + b.rows () <= m.rows ());
    ROBOPTIM_ASSERT (startCol + b.cols () <= m.cols ());
# if __cplusplus < 201103L || !defined (__GXX_EXPERIMENTAL_CXX0X__)
    ROBOPTIM_ASSERT ( ROBOPTIM_UNUSED int (M::IsRowMajor) == int (B::IsRowMajor));
# else
    ROBOPTIM_ASSERT_MSG (int (M::IsRowMajor) == int (B::IsRowMajor),
                         "You should use the same storage order");
# endif

    // Iterate over outer size
    index_t startRow_ = static_cast<index_t> (startRow);
    index_t startCol_ = static_cast<index_t> (startCol);
    index_t outer_start = (matrix_t::IsRowMajor)? startRow_ : startCol_;
    for (index_t k = 0; k < b.outerSize (); ++k)
      {
        // Get iterator to first matrix element in the block
        typename matrix_t::InnerIterator m_it (m, outer_start + k);
        typename block_t::InnerIterator b_it (b, k);

        if (!(b_it))
          continue;

        // TODO: find if there's a better way to find the position of the
        // iterator
        while ((m_it) && ((matrix_t::IsRowMajor)?
                        (m_it.col () < startCol_) : (m_it.row () < startRow_)))
	  {
	    ++m_it;
	  }

        if (!m_it)
          throw std::runtime_error
            ("sparse matrix structure mismatch in updateSparseBlock");

	// Iterator over inner size
	for (; b_it && m_it; ++b_it, ++m_it)
          {
            ROBOPTIM_ASSERT (m_it.row () == startRow_ + b_it.row ());
            ROBOPTIM_ASSERT (m_it.col () == startCol_ + b_it.col ());

            m_it.valueRef () = b_it.value ();
          }
      }
  }

  inline double normalize (double x, double eps)
  {
      return (std::fabs (x) < eps)? 0:x;
  }

  template <typename T>
  inline T normalize (const T& x, double eps)
  {
    return (x.array ().abs () < eps).select (0, x);
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_UTIL_HXX
