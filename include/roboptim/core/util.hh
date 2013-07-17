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

#ifndef ROBOPTIM_CORE_UTIL_HH
# define ROBOPTIM_CORE_UTIL_HH
# include <roboptim/core/fwd.hh>
# include <roboptim/core/portability.hh>

# include <roboptim/core/twice-differentiable-function.hh>

namespace roboptim
{
  namespace detail
  {
    /// \internal
    /// \brief Copy the content of a uBLAS vector into a C array.
    ROBOPTIM_DLLAPI void vector_to_array
    (Function::value_type* dst,
     const Function::vector_t& src);

    /// \internal
    /// \brief Copy the content of a C array into a uBLAS vector.
    ROBOPTIM_DLLAPI void array_to_vector (Function::vector_t& dst,
					  const Function::value_type* src);

    /// \internal
    /// Merge gradients from several functions (each gradient is a line).
    /// The first line of the jacobian is the only one used.
    template <typename T>
    void
    jacobian_from_gradients (typename DifferentiableFunction::matrix_t& jac,
                             const std::vector<const T*>& c,
                             const DifferentiableFunction::vector_t& x);
  } // end of namespace detail.

  /// \brief Display a vector.
  template <typename T>
  std::ostream& operator<< (std::ostream&, const std::vector<T>&);

  /// \brief Display a pair.
  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream&, const std::pair<T1, T2>&);

  /// \brief Display an Eigen object with the appropriate IOFormat.
  template <typename T>
  std::ostream& operator<< (std::ostream&, const Eigen::MatrixBase<T>&);

  /// \brief Convert a sparse matrix into a dense matrix.
  ROBOPTIM_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::matrix_t sparse_to_dense
  (const GenericFunctionTraits<EigenMatrixSparse>::matrix_t&);

  /// \brief Convert a sparse vector into a dense vector.
  ROBOPTIM_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::vector_t sparse_to_dense
  (const GenericFunctionTraits<EigenMatrixSparse>::gradient_t& v);

  /// \brief Compare dense vectors (matrices) using both relative and absolute
  /// tolerances.
  /// \see http://stackoverflow.com/a/15052131/1043187
  template<typename DerivedA, typename DerivedB>
  bool allclose
  (const Eigen::DenseBase<DerivedA>& a,
   const Eigen::DenseBase<DerivedB>& b,
   const typename DerivedA::RealScalar& rtol
   = Eigen::NumTraits<typename DerivedA::RealScalar>::dummy_precision(),
   const typename DerivedA::RealScalar& atol
   = Eigen::NumTraits<typename DerivedA::RealScalar>::epsilon());

  /// \brief Compare sparse vectors (matrices) using both relative and absolute
  /// tolerances.
  /// \see http://stackoverflow.com/a/15052131/1043187
  template<typename DerivedA, typename DerivedB>
  bool allclose
  (const Eigen::SparseMatrixBase<DerivedA>& a,
   const Eigen::SparseMatrixBase<DerivedB>& b,
   const typename DerivedA::RealScalar& rtol
   = Eigen::NumTraits<typename DerivedA::RealScalar>::dummy_precision(),
   const typename DerivedA::RealScalar& atol
   = Eigen::NumTraits<typename DerivedA::RealScalar>::epsilon());

} // end of namespace roboptim.

# include <roboptim/core/util.hxx>
#endif //! ROBOPTIM_CORE_UTIL_HH
