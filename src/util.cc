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

#include "debug.hh"

# if defined(__GLIBCXX__) || defined(__GLIBCPP__)
// Include headers for demangling
#  include <cxxabi.h>
# endif // __GLIBCXX__ || __GLIBCPP__

#include <cstring>
#include <algorithm>

#include "roboptim/core/util.hh"

namespace roboptim
{
  namespace detail
  {
    void
    vector_to_array (Function::value_type* dst, Function::const_vector_ref src)
    {
      if (src.size () == 0)
	return;
      Eigen::Map<Eigen::VectorXd>(dst, src.size ()) = src;

      // NaN != NaN, handle this case.
      for (Function::size_type i = 0; i < src.size (); ++i)
	if (src[i] != src[i])
	  assert (dst[i] != dst[i]);
	else
	  assert (dst[i] == src[i]);
    }

    void
    array_to_vector (Function::vector_ref dst, const Function::value_type* src)
    {
      if (dst.size () == 0)
	return;
      dst = Eigen::Map<const Eigen::VectorXd>(src, dst.size ());

      // NaN != NaN, handle this case.
      for (Function::size_type i = 0; i < dst.size (); ++i)
	if (src[i] != src[i])
	  assert (dst[i] != dst[i]);
	else
	  assert (dst[i] == src[i]);
    }
  } // end of namespace detail.

# if defined(__GLIBCXX__) || defined(__GLIBCPP__)
  // Demangling available
  const std::string demangle(const char* name)
  {
    int status = -4;

    char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
    const char* const demangled_name = (status == 0)? res : name;
    std::string ret_val(demangled_name);
    free(res);

    return ret_val;
  }
# else
  // No demangling available
  const std::string demangle(const char* name)
  {
    return std::string(name);
  }
# endif // __GLIBCXX__ || __GLIBCPP__

  GenericFunctionTraits<EigenMatrixDense>::matrix_t sparse_to_dense
  (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref m)
  {
    GenericFunctionTraits<EigenMatrixDense>::matrix_t
      dense(m.rows(), m.cols());
    dense.setZero();

    for (int k = 0; k < m.outerSize(); ++k)
      for (GenericFunctionTraits<EigenMatrixSparse>::matrix_t::InnerIterator
	     it(m,k); it; ++it)
	{
	  dense(it.row(), it.col()) = it.value();
	}
    return dense;
  }

  GenericFunctionTraits<EigenMatrixDense>::vector_t sparse_to_dense
  (GenericFunctionTraits<EigenMatrixSparse>::const_gradient_ref v)
  {
    GenericFunctionTraits<EigenMatrixDense>::vector_t dense(v.size());
    dense.setZero();

    for (GenericFunctionTraits<EigenMatrixSparse>::gradient_t::InnerIterator
           it(v); it; ++it)
      {
	dense(it.index()) = it.value();
      }
    return dense;
  }

  bool allclose
  (const Eigen::Ref<const Eigen::MatrixXd>& a,
   const Eigen::Ref<const Eigen::MatrixXd>& b,
   double rtol,
   double atol)
  {
    assert (a.cols () == b.cols ());
    assert (a.rows () == b.rows ());

    return ((a.derived () - b.derived ()).array ().abs ()
            <= (atol + rtol * b.derived ().array ().abs ())).all ();
  }

  bool allclose
  (const Eigen::SparseMatrix<double>& a,
   const Eigen::SparseMatrix<double>& b,
   double rtol,
   double atol)
  {
    assert (a.cols () == b.cols ());
    assert (a.rows () == b.rows ());
    assert (a.outerSize () == b.outerSize ());

    typedef Eigen::SparseMatrix<double> matrix_t;

    for (int k = 0; k < a.outerSize (); ++k)
      {
	// Iterator over a
	matrix_t::InnerIterator it_a (a.derived (), k);
	// Iterator over b
	matrix_t::InnerIterator it_b (b.derived (), k);

	while (it_a && it_b)
          {
	    assert(it_a.col() == it_b.col());
	    assert(it_a.row() == it_b.row());

	    if (std::abs (it_a.value () - it_b.value ())
		> atol + rtol * std::abs (it_b.value ()))
	      return false;

	    ++it_a;
	    ++it_b;
          }
      }
    return true;
  }

  GenericFunctionTraits<EigenMatrixDense>::const_gradient_ref
  toDense (GenericFunctionTraits<EigenMatrixDense>::const_gradient_ref m)
  {
    return m;
  }

  GenericFunctionTraits<EigenMatrixDense>::gradient_t
  toDense (GenericFunctionTraits<EigenMatrixSparse>::const_gradient_ref m)
  {
    return GenericFunctionTraits<EigenMatrixDense>::gradient_t (m);
  }

  GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref
  toDense (GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref m)
  {
    return m;
  }

  GenericFunctionTraits<EigenMatrixDense>::matrix_t
  toDense (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref m)
  {
    return GenericFunctionTraits<EigenMatrixDense>::matrix_t (m);
  }
} // end of namespace roboptim.
