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

# include <vector>
# include <utility>
# include <map>

# include <roboptim/core/twice-differentiable-function.hh>

namespace roboptim
{
  namespace detail
  {
    /// \internal
    /// \brief Copy the content of an Eigen vector into a C array.
    ROBOPTIM_DLLAPI void vector_to_array
    (Function::value_type* dst,
     Function::const_vector_ref src);

    /// \internal
    /// \brief Copy the content of a C array into an Eigen vector.
    ROBOPTIM_DLLAPI void array_to_vector (Function::vector_ref dst,
					  const Function::value_type* src);

    /// \internal
    /// Merge gradients from several functions (each gradient is a line).
    /// The first line of the jacobian is the only one used.
    template <typename T>
    void
    jacobian_from_gradients (DifferentiableFunction::matrix_ref jac,
                             const std::vector<const T*>& c,
                             DifferentiableFunction::const_vector_ref x);
  } // end of namespace detail.

  /// \brief Display a vector.
  template <typename T>
  std::ostream& operator<< (std::ostream&, const std::vector<T>&);

  /// \brief Display a pair.
  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream&, const std::pair<T1, T2>&);

  /// \brief Display a map.
  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream&, const std::map<T1, T2>&);

  /// \brief Display an Eigen object with the appropriate IOFormat.
  template <typename T>
  std::ostream& operator<< (std::ostream&, const Eigen::MatrixBase<T>&);

  /// \brief Convert a sparse matrix into a dense matrix.
  ROBOPTIM_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::matrix_t sparse_to_dense
  (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref m);

  /// \brief Convert a sparse vector into a dense vector.
  ROBOPTIM_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::vector_t sparse_to_dense
  (GenericFunctionTraits<EigenMatrixSparse>::const_gradient_ref v);

  /// \brief Compare sparse vectors (matrices) using both relative and absolute
  /// tolerances.
  /// \see http://stackoverflow.com/a/15052131/1043187
  ROBOPTIM_DLLAPI
  bool allclose
  (const Eigen::SparseMatrix<double>& a,
   const Eigen::SparseMatrix<double>& b,
   double rtol = Eigen::NumTraits<double>::dummy_precision (),
   double atol = Eigen::NumTraits<double>::epsilon ());

  /// \brief Compare dense vectors (matrices) using both relative and absolute
  /// tolerances.
  /// \see http://stackoverflow.com/a/15052131/1043187
  ROBOPTIM_DLLAPI
  bool allclose
  (const Eigen::Ref<const Eigen::MatrixXd>& a,
   const Eigen::Ref<const Eigen::MatrixXd>& b,
   double rtol = Eigen::NumTraits<double>::dummy_precision (),
   double atol = Eigen::NumTraits<double>::epsilon ());

  /// \brief Copy a sparse block into a sparse matrix.
  /// \param matrix matrix to fill.
  /// \param block block to copy.
  /// \param startRow start row of the block.
  /// \param startCol start col of the block.
  /// \param compress whether to compress the sparse matrix at the end.
  template <typename U>
  void copySparseBlock
  (U& matrix,
   const U& block,
   Function::size_type startRow, Function::size_type startCol,
   bool compress = false);

  /// \brief Apply normalize to each element of a matrix.
  inline Function::matrix_t normalize (Function::const_matrix_ref x);

  /// \brief Normalize floating point number output.
  /// Normalization is done on all displayed floating
  /// point numbers to get a consistent output.
  inline double normalize (const double& x);

  /// \brief Apply normalize to each element of a container.
  template <typename T>
  T normalize (const T& x);
} // end of namespace roboptim.

# include <roboptim/core/util.hxx>
#endif //! ROBOPTIM_CORE_UTIL_HH
