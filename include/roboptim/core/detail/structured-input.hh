// Copyright (C) 2015 by Grégoire Duchemin, AIST, CNRS, EPITA
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

#ifndef ROBOPTIM_CORE_DETAIL_STRUCTURED_INPUT_HH
# define ROBOPTIM_CORE_DETAIL_STRUCTURED_INPUT_HH
# include <boost/mpl/assert.hpp>
# include <boost/type_traits.hpp>

# include <vector>

# include <roboptim/core/fwd.hh>

namespace roboptim
{
  namespace detail
  {
    /// \brief Gives access to a std::vector of std::pair<size_t, size_t> representing blocks of input
    /// Those blocks are needed even if the function is not differentiable,
    /// making it easier for us to define it in a exterior class to then use it transparently.
    struct BlockProvider
    {
      /// \brief stores the blocks defined by the function as a pair of integers.
      /// The first element is the first index of the block, the second element
      /// repersetn the total size of the block.
      std::vector<std::pair<size_t, size_t> > blocks;
    };

    /// \brief This class gives access to the getJacobianBlock() method, which implementation
    /// is to be spacialized according to the type of matrix handled by the function.
    template<typename FuncType, typename MatType>
    struct StructuredInputJacobianInternal : public BlockProvider
    {};

    /// \brief DenseMatrix specialization of the getJacobianBlock() method provider.
    template<typename FuncType>
    struct StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixDense> : public BlockProvider
    {
      /// \brief return type of the getJacobianBlock() method
      typedef typename FuncType::jacobian_ref JacBlock;
      /// \brief input type of the jacobian given by the user
      typedef typename FuncType::jacobian_ref InputJacBlock;

      /// \brief retrieve a specific block of the jacobian given as an input
      /// \param jacobian the jacobian to operate upon
      /// \param blockInd the index of the block to be retrieved
      JacBlock getJacobianBlock(InputJacBlock jacobian, size_t blockInd) const;
    };

    /// \brief SparseMatrix specialization of the getJacobianBlock() method provider.
    template<typename FuncType>
    struct StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixSparse> : public BlockProvider
    {
      /// \brief return type of the getJacobianBlock() method
      // Why is the InputPanel set to false ? According to the documentation, the
      // InputPanel parameter allows eigen to check if aligend access to the memory is feasible.
      // In the Sparse specialization of the BlockImpl class, we can see that if it is
      // set to false, it will use the default template implementation, which is explicitly
      // made read-only. Therefore, we should want it to be set to true, so as to enable
      // the read-access we want for the returned block of the jacobian matrix.
      //
      // ...only, the compiler expects a inputpanel value of 0 (false), here, and actually tells
      // us, for a value of one, that there is no way of writing anythig to the extracted
      // submatrix.
      //
      // This may be due to some inheritance shenanigans we missed in the documentation that
      // reverse the meaning of the InputPanel attribute, but for lack of a better explanation,
      // we just use the magical value "false" here. Everything compiles and works in both
      // RowMajor and ColMajor mode, so we get the result we expected.
      typedef typename Eigen::Block<typename FuncType::jacobian_t, Eigen::Dynamic, Eigen::Dynamic, false> JacBlock;
      /// \brief input type of the jacobian given by the user
      typedef typename FuncType::jacobian_ref InputJacBlock;

      /// \brief retrieve a specific block of the jacobian given as an input
      /// \param jacobian the jacobian to operate upon
      /// \param blockInd the index of the block to be retrieved
      JacBlock getJacobianBlock(InputJacBlock jacobian, size_t blockInd) const;
    };

    /// \brief Provides utility methods to describe the input format of a function
    ///
    /// Inheriting from this class will allow a function to define "blocks" of input.
    /// You can then retrieve a specific block from the whole input, or the specific
    /// part of the jacobian depending on this block.
    ///
    /// To inherit from this class, please specify as the template argument the roboptim
    /// function type of the function (Function, DifferentiableFunction, LinearFunction,
    /// et cetera)
    template<typename FuncType>
    class StructuredInput
    // This boost::conditional allows us to not define the getJacobianBlock method
    // if the function is not differentiable.
      : public boost::conditional<
      boost::is_base_of<roboptim::GenericDifferentiableFunction<typename FuncType::traits_t>, FuncType>::value,
      StructuredInputJacobianInternal<FuncType, typename FuncType::traits_t>,
      BlockProvider>
    ::type
    {
    public:
      /// \brief Adds a new block of input to the function
      /// \param size size of the block to add
      void addBlock(size_t size);
      /// \brief Returns the number of blocks defined by the function
      size_t getNumBlocks() const;

      /// \brief return type of the getInputBlock() method
      typedef typename Eigen::Ref<const typename FuncType::argument_t>::ConstSegmentReturnType ConstSegment;

      /// \brief Reads a specified block of data from an input argument
      ///
      /// \param input the input data to be read
      /// \param blockInd the index of the block to be read
      ConstSegment getInputBlock(typename FuncType::const_argument_ref input, size_t blockInd) const;
    };

  } // end of namespace detail.
} // end of namespace roboptim.

# include <roboptim/core/detail/structured-input.hxx>

#endif //! ROBOPTIM_CORE_DETAIL_STRUCTURED_INPUT_HH
