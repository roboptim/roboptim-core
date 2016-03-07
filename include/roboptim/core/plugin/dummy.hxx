// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
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

#ifndef ROBOPTIM_CORE_PLUGIN_DUMMY_HXX
# define ROBOPTIM_CORE_PLUGIN_DUMMY_HXX

namespace roboptim
{
  template <typename T>
  GenericDummySolver<T>::GenericDummySolver (const problem_t& pb)
    : parent_t (pb)
  {
    this->parameters_["dummy-parameter"].description = "dummy parameter";
    this->parameters_["dummy-parameter"].value = 42.;

    this->parameters_["dummy-parameter2"].description = "yet another dummy parameter";
    this->parameters_["dummy-parameter2"].value = 3;

    this->parameters_["dummy-parameter3"].description = "just a dummy key";
    this->parameters_["dummy-parameter3"].value = std::string ("...and a dummy value!");

    Parameter::vector_t v (4);
    v << 1., 2., 3., 4.;
    this->parameters_["dummy-parameter4"].value = v;

    this->parameters_["dummy-parameter5"].description = "dummy boolean";
    this->parameters_["dummy-parameter5"].value = false;
  }

  template <typename T>
  GenericDummySolver<T>::~GenericDummySolver ()
  {
  }

  template <typename T>
  void GenericDummySolver<T>::solve ()
  {
    this->result_ = SolverError ("The dummy solver always fail.");
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PLUGIN_DUMMY_HXX
