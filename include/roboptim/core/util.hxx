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

  template <typename T>
  std::ostream& operator<< (std::ostream& o, const Eigen::MatrixBase<T>& matrix)
  {
    Eigen::IOFormat ioformat (Eigen::StreamPrecision,
                              Eigen::DontAlignCols,
                              ",", ", ", "(", ")", "(", ")");
    ioformat.rowSpacer = "";
    o << "[";

    // Matrix
    if (matrix.cols () == 1 || matrix.cols () == 1)
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

  template<typename DerivedA, typename DerivedB>
  bool allclose
  (const Eigen::DenseBase<DerivedA>& a,
   const Eigen::DenseBase<DerivedB>& b,
   const typename DerivedA::RealScalar& rtol,
   const typename DerivedA::RealScalar& atol)
  {
    assert(a.cols() == b.cols());
    assert(a.rows() == b.rows());

    return ((a.derived() - b.derived()).array().abs()
            <= (atol + rtol * b.derived().array().abs())).all();
  }

  template<typename DerivedA, typename DerivedB>
  bool allclose
  (const Eigen::SparseMatrixBase<DerivedA>& a,
   const Eigen::SparseMatrixBase<DerivedB>& b,
   const typename DerivedA::RealScalar& rtol,
   const typename DerivedA::RealScalar& atol)
  {
    assert(a.cols() == b.cols());
    assert(a.rows() == b.rows());
    assert(a.outerSize() == b.outerSize());

    for (int k = 0; k < a.outerSize(); ++k)
      {
	// Iterator over a
	typename DerivedA::InnerIterator it_a(a.derived(),k);
	// Iterator over b
	typename DerivedB::InnerIterator it_b(b.derived(),k);

	while (it_a && it_b)
          {
	    assert(it_a.col() == it_b.col());
	    assert(it_a.row() == it_b.row());

	    if(fabs(it_a.value() - it_b.value())
	       > atol + rtol * fabs(it_b.value()))
	      return false;

	    ++it_a;
	    ++it_b;
          }
      }
    return true;
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_UTIL_HXX
