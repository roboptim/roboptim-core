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

#ifndef ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
# define ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH

# include <roboptim/core/result.hh>
# include <roboptim/core/config.hh>
# include <roboptim/core/deprecated.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Warnings have been merged to Result to simplify the API.
  /// To prevent issues with existing visitors, we cannot use a simple
  /// typedef such as:
  // ROBOPTIM_CORE_DEPRECATED typedef Result ResultWithWarnings;
  ///
  /// \deprecated Deprecated since version 3.3. Use Result instead.
  class ROBOPTIM_CORE_DEPRECATED ROBOPTIM_CORE_DLLAPI ResultWithWarnings : public Result
  {
  public:
    /// \brief Import size type from Result class.
    typedef Result::size_type size_type;

    explicit ResultWithWarnings (const size_type inputSize,
                                 const size_type outputSize = 1);
    ~ResultWithWarnings ();
  };
  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
