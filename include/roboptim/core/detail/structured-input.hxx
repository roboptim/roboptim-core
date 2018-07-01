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

#ifndef ROBOPTIM_CORE_DETAIL_STRUCTURED_INPUT_HXX
# define ROBOPTIM_CORE_DETAIL_STRUCTURED_INPUT_HXX

namespace roboptim
{
  namespace detail
  {
    // This is the only way we found to get a submatrix from a Sparse matrix
    // while still granting us writing rights to said submatrix.
    // This will become useless when Eigen will let us write in .block()
    // obtained from Sparse matrices.
    template<typename Derived>
    Eigen::Block<Derived>
    getBlock (Eigen::SparseMatrixBase<Derived>& m, int colStart, int colLength)
    {
      return Eigen::Block<Derived> (m.derived (), 0, colStart, m.rows (), colLength);
    }

    template<typename U>
    void StructuredInput<U>::addBlock(size_t size)
    {
      size_t startingIndex = 0;

      if (BlockProvider::blocks.size() != 0)
	{
	  std::pair<size_t, size_t> lastPair = BlockProvider::blocks[BlockProvider::blocks.size()-1];
	  startingIndex = lastPair.first + lastPair.second;
	}

      BlockProvider::blocks.push_back(std::make_pair(startingIndex, size));
    }

    template<typename U>
    size_t StructuredInput<U>::getNumBlocks() const
    {
      return BlockProvider::blocks.size();
    }

    template<typename U>
    typename StructuredInput<U>::ConstSegment StructuredInput<U>::getInputBlock(typename U::const_argument_ref input, size_t blockInd) const
    {
      assert(blockInd < BlockProvider::blocks.size());

      const std::pair<size_t, size_t>& block = this->BlockProvider::blocks[blockInd];
      return input.segment(static_cast<typename U::argument_t::Index>(block.first),
                           static_cast<typename U::argument_t::Index>(block.second));
    }

    template<typename FuncType>
    typename StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixDense>::JacBlock
    StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixDense>::getJacobianBlock
    (typename StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixDense>::InputJacBlock jacobian,
     size_t blockInd)
      const
    {
      assert(blockInd < blocks.size());

      return jacobian.middleCols(static_cast<int>(blocks[blockInd].first), static_cast<int>(blocks[blockInd].second));
    }

    template<typename FuncType>
    typename StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixSparse>::JacBlock
    StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixSparse>::getJacobianBlock
    (typename StructuredInputJacobianInternal<FuncType, roboptim::EigenMatrixSparse>::InputJacBlock jacobian,
     size_t blockInd)
      const
    {
      assert(blockInd < blocks.size());

      return getBlock(jacobian, static_cast<int>(blocks[blockInd].first), static_cast<int>(blocks[blockInd].second));
    }

  } // end of namespace detail.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_DETAIL_STRUCTURED_INPUT_HXX
