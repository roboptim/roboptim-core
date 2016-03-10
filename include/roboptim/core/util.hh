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
# include <string>

# ifdef __linux__
#  include <fenv.h>
#  define ROBOPTIM_HAS_FENV_H
# endif //! __linux__

// WARNING: careful with circular includes
# include <roboptim/core/function.hh>

namespace roboptim
{
  namespace detail
  {

# ifdef ROBOPTIM_HAS_FENV_H
    typedef fenv_t fenv_t;
# else //! ROBOPTIM_HAS_FENV_H
    typedef void* fenv_t;
# endif //! ROBOPTIM_HAS_FENV_H

    /// \internal
    /// \brief Copy the content of an Eigen vector into a C array.
    ROBOPTIM_CORE_DLLAPI void vector_to_array
    (Function::value_type* dst,
     Function::const_vector_ref src);

    /// \internal
    /// \brief Copy the content of a C array into an Eigen vector.
    ROBOPTIM_CORE_DLLAPI void array_to_vector (Function::vector_ref dst,
					  const Function::value_type* src);

    /// \internal
    /// Merge gradients from several functions (each gradient is a line).
    /// The first line of the jacobian is the only one used.
    template <typename T>
    void
    jacobian_from_gradients (Function::matrix_ref jac,
                             const std::vector<const T*>& c,
                             Function::const_vector_ref x);

    /// \brief Helper class used to disable floating-point exceptions.
    /// Simply create an object in a scope where the exceptions should be
    /// disabled.
    class ROBOPTIM_CORE_DLLAPI DisableFPE
    {
    public:
      /// \brief Constructor, disabling floating-point exceptions and storing
      /// the floating-point environment.
      DisableFPE ();

      /// \brief Destructor, restoring the floating-point environment.
      ~DisableFPE ();

    private:
      /// \brief Floating-point exception environment.
      ::roboptim::detail::fenv_t fenv_;
    };

    /// \brief Custom deleter that does not delete anything.
    /// This can be used when creating a shared_ptr from a reference, although
    /// this should be used with **great** care...
    template <typename T>
    struct NoopDeleter
    {
      inline void operator() (const T*) const {}
    };
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

  /// \brief Demangle (if available).
  ROBOPTIM_CORE_DLLAPI const std::string demangle(const char* name);

  /// \brief Return a string describing the type of T.
  template <typename T>
  std::string typeString ();

  /// \brief Convert a sparse matrix into a dense matrix.
  ROBOPTIM_CORE_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::matrix_t sparse_to_dense
  (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref m);

  /// \brief Convert a sparse vector into a dense vector.
  ROBOPTIM_CORE_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::vector_t sparse_to_dense
  (GenericFunctionTraits<EigenMatrixSparse>::const_gradient_ref v);

  /// \brief Convert an input gradient to a dense gradient (e.g. for printing).
  /// \param g input gradient.
  ROBOPTIM_CORE_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::gradient_t toDense
  (GenericFunctionTraits<EigenMatrixSparse>::const_gradient_ref g);

  /// \brief Convert an input matrix to a dense matrix (e.g. for printing).
  /// \param m input matrix.
  ROBOPTIM_CORE_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::matrix_t toDense
  (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref m);

  /// \brief Convert an input matrix to a dense matrix (e.g. for printing).
  /// \param m input matrix.
  /// Note: since the input is a dense matrix, we just return it.
  ROBOPTIM_CORE_DLLAPI
  GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref toDense
  (GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref m);

  /// \brief Compare sparse vectors (matrices) using both relative and absolute
  /// tolerances.
  /// \see http://stackoverflow.com/a/15052131/1043187
  ROBOPTIM_CORE_DLLAPI
  bool allclose
  (const Eigen::SparseMatrix<double>& a,
   const Eigen::SparseMatrix<double>& b,
   double rtol = Eigen::NumTraits<double>::dummy_precision (),
   double atol = Eigen::NumTraits<double>::epsilon ());

  /// \brief Compare dense vectors (matrices) using both relative and absolute
  /// tolerances.
  /// \see http://stackoverflow.com/a/15052131/1043187
  ROBOPTIM_CORE_DLLAPI
  bool allclose
  (const Eigen::Ref<const Eigen::MatrixXd>& a,
   const Eigen::Ref<const Eigen::MatrixXd>& b,
   double rtol = Eigen::NumTraits<double>::dummy_precision (),
   double atol = Eigen::NumTraits<double>::epsilon ());

  /// \brief Copy a sparse block into a sparse matrix. This function involves
  /// filling a vector of triplets, so this should be avoided in critical
  /// sections.
  /// \param m matrix to fill.
  /// \param b block to copy to m.
  /// \param startRow start row of the block in m where b will be copied.
  /// \param startCol start col of the block in m where b will be copied.
  /// \param compress whether to compress the sparse matrix at the end.
  template <typename U>
  void copySparseBlock
  (U& m, const U& b,
   Function::size_type startRow, Function::size_type startCol,
   bool compress = false);

  /// \brief Update a sparse block of a sparse matrix.
  /// This function expects the sparse matrix to have its structure set
  /// already, thus it can easily iterate over the values and copy them without
  /// any extra allocation. If that's not the case, the behavior is undefined.
  /// \param m sparse matrix to update.
  /// \param b sparse block to copy to m.
  /// \param startRow start row of the block in m where b will be copied.
  /// \param startCol start col of the block in m where b will be copied.
  /// \tparam M matrix type.
  /// \tparam B block type.
  /// \throw std::runtime_error if the sparse pattern does not match.
  template <typename M, typename B>
  void updateSparseBlock
  (M& m, const B& b,
   Function::size_type startRow, Function::size_type startCol);

  /// \brief Apply normalize to a scalar.
  inline double normalize (double x, double eps = 1e-8);

  /// \brief Apply normalize to each element of an Eigen vector.
  template <typename T>
  inline T normalize (const T& x, double eps = 1e-8);

  /// \brief Split a string on a given delimiter.
  /// \param s string to split.
  /// \param d delimiter used.
  /// \return vector of tokens.
  ROBOPTIM_CORE_DLLAPI
  std::vector<std::string> split (const std::string& s, char d);
} // end of namespace roboptim.

# include <roboptim/core/util.hxx>
#endif //! ROBOPTIM_CORE_UTIL_HH
