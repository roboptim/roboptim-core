
// Copyright (c) 2011 CNRS
// Authors: Florent Lamiraux
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

#include "roboptim/core/sum-of-c1-squares.hh"

namespace roboptim {
  bool operator!= (const Function::vector_t& x1,
		   const Function::vector_t& x2)
  {
    if (x1.size() != x2.size()) return true;
    for (Function::size_type i=0; i<x1.size(); i++) {
      if (x1(i) != x2(i)) return true;
    }
    return false;
  }

  SumOfC1Squares::SumOfC1Squares (const boost::shared_ptr<DerivableFunction>&
				  function,
				  const std::string& name) throw () :
    DerivableFunction(function->inputSize(), 1, name),
    baseFunction_ (function)
  {
    value_.resize (function->outputSize());
    gradient_.resize (function->inputSize());
    x_.resize (function->inputSize());
    x_.clear();
    (*baseFunction_) (value_, x_);
  }
    
  SumOfC1Squares::SumOfC1Squares (const SumOfC1Squares& src) throw ():
    DerivableFunction(src.inputSize(), 1, src.getName()),
    baseFunction_ (src.baseFunction_), x_ (src.x_),
    value_ (src.value_),
    gradient_ (src.gradient_)
  {
  }

  SumOfC1Squares::~SumOfC1Squares () throw ()
  {
  }

  const boost::shared_ptr<const DerivableFunction>& SumOfC1Squares::
  baseFunction () const
  {
    return baseFunction_;
  }

  void SumOfC1Squares::
  impl_compute(result_t &result, const argument_t &x) const throw ()
  {
    computeFunction (x);
    value_t sumSquares = 0;
    for (size_t i = 0; i < value_.size(); i++) {
      value_t y = value_[i];
      sumSquares += y*y;
    }
    result[0] = sumSquares;
  }

  void SumOfC1Squares::
  impl_gradient(gradient_t& gradient, const argument_t& x,
		size_type row) const throw ()
  {
    row = row; // keep unused variable warning silent.

    assert (row == 0);
    computeFunction (x);
    gradient.clear ();
    for (size_t i = 0; i < value_.size(); i++) {
      value_t y = value_[i];
      baseFunction_->gradient(gradient_, x, i);
      gradient += 2*y*gradient_;
    }
  }

  void SumOfC1Squares::computeFunction (const argument_t x) const
  {
    if (x != x_) {
      x_ = x;
      (*baseFunction_) (value_, x_);
    }
  }
} // namespace roboptim
