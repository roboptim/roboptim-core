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

#ifndef ROBOPTIM_CORE_DIFFERENTIABLE_FUNCTION_HXX
# define ROBOPTIM_CORE_DIFFERENTIABLE_FUNCTION_HXX
# include <boost/algorithm/string/replace.hpp>

# include <roboptim/core/indent.hh>
# include <roboptim/core/util.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  template <typename T>
  GenericDifferentiableFunction<T>::GenericDifferentiableFunction
  (size_type inputSize, size_type outputSize, std::string name)
    : GenericFunction<T> (inputSize, outputSize, name)
  {
  }

  template <>
  inline void
  GenericDifferentiableFunction<EigenMatrixSparse>::impl_jacobian
  (jacobian_ref jacobian, const_argument_ref argument)
    const
  {
    typedef Eigen::Triplet<value_type> triplet_t;
    std::vector<triplet_t> coefficients;

    for (jacobian_t::Index i = 0; i < this->outputSize (); ++i)
      {
        gradient_t grad = gradient (argument, i);
        for (gradient_t::InnerIterator it (grad); it; ++it)
          {
            const jacobian_t::Index
              idx = static_cast<const jacobian_t::Index> (it.index ());
            coefficients.push_back
              (triplet_t (i, idx, it.value ()));
          }
      }

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    bool cur_malloc_allowed = is_malloc_allowed ();
    set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

    jacobian.setFromTriplets (coefficients.begin (), coefficients.end ());

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
  }


  template <typename T>
  void
  GenericDifferentiableFunction<T>::impl_jacobian (jacobian_ref jacobian,
						   const_argument_ref argument)
    const
  {
    for (typename jacobian_t::Index i = 0; i < this->outputSize (); ++i)
       gradient (jacobian.row (i), argument, i);
  }

  template <typename T>
  std::ostream&
  GenericDifferentiableFunction<T>::print (std::ostream& o) const
  {
    if (this->getName ().empty ())
      return o << "Differentiable function";

    std::stringstream ss;
    ss << std::endl;
    char fill = o.fill (' ');
    ss << std::setw ((int)indent (o))
       << ""
       << std::setfill (fill);
    std::string name = this->getName ();
    boost::algorithm::replace_all (name, "\n", ss.str ());

    return o << name << " (differentiable function)";
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_DIFFERENTIABLE_FUNCTION
