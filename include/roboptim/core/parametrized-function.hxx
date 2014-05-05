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

#ifndef ROBOPTIM_CORE_PARAMETRIZED_FUNCTION_HXX
# define ROBOPTIM_CORE_PARAMETRIZED_FUNCTION_HXX

namespace roboptim
{
  template <typename F>
  ParametrizedFunction<F>::ParametrizedFunction (size_type inputSize,
						 size_type functionInputSize,
						 size_type functionOutputSize)
    : inputSize_ (inputSize),
      functionInputSize_ (functionInputSize),
      functionOutputSize_ (functionOutputSize)
  {
  }

  template <typename F>
  typename ParametrizedFunction<F>::result_t
  ParametrizedFunction<F>::operator () (const argument_t& argument)
    const
  {
    assert (argument.size () == inputSize ());
    return impl_compute (argument);
  }

  template <typename F>
  typename ParametrizedFunction<F>::size_type
  ParametrizedFunction<F>::inputSize () const
  {
    return inputSize_;
  }

  template <typename F>
  typename ParametrizedFunction<F>::size_type
  ParametrizedFunction<F>::functionInputSize () const
  {
    return functionInputSize_;
  }


  template <typename F>
  typename ParametrizedFunction<F>::size_type
  ParametrizedFunction<F>::functionOutputSize () const
  {
    return functionOutputSize_;
  }

  template <typename F>
  std::ostream&
  ParametrizedFunction<F>::print (std::ostream& o) const
  {
    o << "Parametrized function";
    return o;
  }


  template <typename F>
  std::ostream& operator<< (std::ostream& o, const ParametrizedFunction<F>& f)
  {
    return f.print (o);
  }

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_PARAMETRIZED_FUNCTION_HXX
