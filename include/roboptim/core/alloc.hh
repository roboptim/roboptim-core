// Copyright (C) 2015 by Félix Darricau, AIST, CNRS, EPITA
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

#ifndef ROBOPTIM_CORE_ALLOC_HH
# define ROBOPTIM_CORE_ALLOC_HH

# define EIGEN_RUNTIME_NO_MALLOC
# include <Eigen/Core>

# include <roboptim/core/sys.hh>

namespace roboptim
{
  /// \brief Update the static variable used for Eigen::set_is_malloc_allowed.
  ROBOPTIM_DLLAPI
  bool is_malloc_allowed_update (bool update = false, bool new_value = false);

  /// \brief Manage the calls to Eigen::set_is_malloc_allowed.
  /// \param allow whether to allow dynamic allocations.
  inline bool set_is_malloc_allowed (bool allow)
  {
    is_malloc_allowed_update (true, allow);

    return Eigen::internal::set_is_malloc_allowed (allow);
  }

  /// \brief Whether dynamic allocation is allowed.
  inline bool is_malloc_allowed ()
  {
    return is_malloc_allowed_update (false);
  }
}

#endif //! ROBOPTIM_CORE_ALLOC_HH
