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

#ifndef ROBOPTIM_CORE_PROBLEM_HXX
# define ROBOPTIM_CORE_PROBLEM_HXX
# include <algorithm>
# include <stdexcept>

# include <boost/format.hpp>
# include <boost/variant.hpp>
# include <boost/variant/get.hpp>
# include <boost/variant/apply_visitor.hpp>

# include <roboptim/core/indent.hh>
# include <roboptim/core/terminal-color.hh>
# include <roboptim/core/util.hh>
# include <roboptim/core/detail/utility.hh>

namespace roboptim
{

  //
  // General template implementation
  //
  template <typename T>
  Problem<T>::Problem (const function_t& f)
    : function_ (f),
      startingPoint_ (),
      constraints_ (),
      boundsVect_ (),
      argumentBounds_ (static_cast<std::size_t> (f.inputSize ())),
      scalingVect_ (),
      argumentScaling_ (static_cast<std::size_t> (f.inputSize ())),
      argumentNames_ ()
  {
    // Initialize bound.
    std::fill (argumentBounds_.begin (), argumentBounds_.end (),
               function_t::makeInfiniteInterval ());
    // Initialize scaling.
    std::fill (argumentScaling_.begin (), argumentScaling_.end (), 1.);
  }

  template <typename T>
  Problem<T>::~Problem ()
  {
  }

  // Copy constructor.
  template <typename T>
  Problem<T>::Problem (const Problem<T>& pb)
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (pb.constraints_),
      boundsVect_ (pb.boundsVect_),
      argumentBounds_ (pb.argumentBounds_),
      scalingVect_ (pb.scalingVect_),
      argumentScaling_ (pb.argumentScaling_),
      argumentNames_ (pb.argumentNames_)
  {
  }

  template <typename T>
  const typename Problem<T>::function_t&
  Problem<T>::function () const
  {
    return function_;
  }

  template <typename T>
  const typename Problem<T>::constraints_t&
  Problem<T>::constraints () const
  {
    return constraints_;
  }

  template <typename T>
  void
  Problem<T>::addConstraint (boost::shared_ptr<GenericFunction<T> > x,
				    interval_t b,
				    value_type s)
  {
    if (x->inputSize () != this->function ().inputSize ())
      throw std::runtime_error ("Invalid constraint (wrong input size)");
    if (x->outputSize () != 1)
      throw std::runtime_error
	("Invalid constraint (output size is not equal to one)");

    // Check that the pointer is not null.
    assert (!!x.get ());
    assert (b.first <= b.second);
    constraints_.push_back (x);
    intervals_t bounds;
    bounds.push_back (b);
    boundsVect_.push_back (bounds);
    scaling_t scaling;
    scaling.push_back (s);
    scalingVect_.push_back (scaling);
  }

  template <typename T>
  void
  Problem<T>::addConstraint (boost::shared_ptr<GenericFunction<T> > x,
				    intervals_t b,
				    scaling_t s)
  {
    if (!x)
      throw std::runtime_error
	("Failed to add constraint: null shared pointer");

    if (x->inputSize () != this->function ().inputSize ())
      {
	boost::format fmt
	  ("Failed to add constraint '%s': invalid input size "
	   "(%d, expected size is %d)");
	fmt
	  % x->getName ()
	  % x->inputSize ()
	  % this->function ().inputSize ();
	throw std::runtime_error (fmt.str ());
      }
    if (x->outputSize () != static_cast<typename function_t::size_type> (b.size ()))
      {
	boost::format fmt
	  ("Failed to add constraint '%s': interval vector size is invalid "
	   "(%d, expected size is %d)");
	fmt
	  % x->getName ()
	  % s.size ()
	  % x->outputSize ();
	throw std::runtime_error (fmt.str ());
      }
    if (x->outputSize () != static_cast<typename function_t::size_type> (s.size ()))
      {
	boost::format fmt
	  ("Failed to add constraint '%s': scaling vector size is invalid "
	   "(%d, expected size is %d)");
	// Ignore some irrelevant exceptions
	fmt.exceptions (boost::io::all_error_bits ^ (boost::io::too_many_args_bit
						     | boost::io::too_few_args_bit
						     | boost::io::bad_format_string_bit));
	fmt
	  % x->getName ()
	  % b.size ()
	  % x->outputSize ();
	throw std::runtime_error (fmt.str ());
      }

    // Check that the pointer is not null.
    assert (!!x.get ());
    constraints_.push_back (x);

    // Check that the bounds are correctly defined.
    for (std::size_t i = 0; i < static_cast<std::size_t> (x->outputSize ());
         ++i)
      {
        ROBOPTIM_DEBUG_ONLY(const interval_t& interval = b[i]);
	assert (interval.first <= interval.second);
      }

    boundsVect_.push_back (b);
    scalingVect_.push_back (s);
  }

  template <typename T>
  typename Problem<T>::startingPoint_t&
  Problem<T>::startingPoint ()
  {
    if (startingPoint_ && startingPoint_->size ()
	!= this->function ().inputSize ())
      throw std::runtime_error ("Invalid starting point (wrong size)");
    return startingPoint_;
  }

  template <typename T>
  const typename Problem<T>::startingPoint_t&
  Problem<T>::startingPoint () const
  {
    if (startingPoint_ && startingPoint_->size ()
	!= this->function ().inputSize ())
      throw std::runtime_error ("Invalid starting point (wrong size)");
    return startingPoint_;
  }

  template <typename T>
  const typename Problem<T>::intervalsVect_t&
  Problem<T>::boundsVector () const
  {
    return boundsVect_;
  }

  template <typename T>
  typename Problem<T>::intervals_t&
  Problem<T>::argumentBounds ()
  {
    return argumentBounds_;
  }

  template <typename T>
  const typename Problem<T>::intervals_t&
  Problem<T>::argumentBounds () const
  {
    return argumentBounds_;
  }

  template <typename T>
  const typename Problem<T>::scalingVect_t&
  Problem<T>::scalingVector () const
  {
    return scalingVect_;
  }

  template <typename T>
  const typename Problem<T>::scalesVect_t&
  Problem<T>::scalesVector () const
  {
    return scalingVector ();
  }

  template <typename T>
  typename Problem<T>::scaling_t&
  Problem<T>::argumentScaling ()
  {
    return argumentScaling_;
  }

  template <typename T>
  const typename Problem<T>::scaling_t&
  Problem<T>::argumentScaling () const
  {
    return argumentScaling_;
  }

  template <typename T>
  typename Problem<T>::scales_t&
  Problem<T>::argumentScales ()
  {
    return argumentScaling ();
  }

  template <typename T>
  const typename Problem<T>::scales_t&
  Problem<T>::argumentScales () const
  {
    return argumentScaling ();
  }

  template <typename T>
  typename Problem<T>::names_t&
  Problem<T>::argumentNames ()
  {
    return argumentNames_;
  }

  template <typename T>
  const typename Problem<T>::names_t&
  Problem<T>::argumentNames () const
  {
    return argumentNames_;
  }

  namespace detail
  {
    template <typename T>
    std::ostream&
    impl_print (std::ostream& o, Eigen::Ref<T> t)
    {
      return o << t;
    }

    template <typename T>
    std::ostream&
    impl_print (std::ostream& o, const T* t)
    {
      return o << *t;
    }

    template <typename T>
    std::ostream&
    impl_print (std::ostream& o, const T& t)
    {
      return o << t;
    }
  }

  namespace detail
  {
    template <typename P>
    struct printConstraint : public boost::static_visitor<void>
    {
      printConstraint (std::ostream& o,
		       const P& problem,
		       std::size_t i) :
	problem_ (problem),
	o_ (o),
	i_ (i)
      {}

      template <typename U>
      void operator () (const U& constraint)
      {
	assert (problem_.constraints ().size () - i_ > 0);
	using namespace boost;
        o_ << iendl << incindent
           << "Constraint " << i_ << incindent
           << iendl << *constraint
           << iendl << "Bounds: " << problem_.boundsVector ()[i_]
           << iendl << "Scaling: " << problem_.scalingVector ()[i_];

	if (problem_.startingPoint ())
	  {
	    typename P::function_t::argument_t x0 = *problem_.startingPoint ();
	    U g = get<U> (problem_.constraints ()[i_]);
            typename P::function_t::result_t x = (*g) (x0);
	    bool satisfied = true;
            o_ << iendl << "Initial value: ";
	    o_ << "[" << x.size () << "](";
            for (typename P::size_type j = 0; j < x.size (); ++j)
	      {
                if (x[j] < P::function_t::
		    getLowerBound ((problem_.boundsVector ()
				    [i_])
				   [static_cast<std::size_t> (j)])
                    || x[j] > P::function_t::
		    getUpperBound ((problem_.boundsVector ()
				    [i_])
				   [static_cast<std::size_t> (j)]))
		  {
		    satisfied = false;
		    o_ << fg::fail << x[j];
		  }
		else
		  o_ << fg::ok << x[j];

		o_ << fg::reset;
		if (j < x.size () - 1)
		  o_ << ", ";
	      }
	    o_ << ")";
	    if (!satisfied)
	      o_ << " (constraint not satisfied)";
            o_ << fg::reset;
	  }
        o_ << decindent << decindent;
      }
    private:
      const P& problem_;
      std::ostream& o_;
      std::size_t i_;
    };
  } // end of namespace detail.

  template <typename T>
  std::ostream&
  Problem<T>::print (std::ostream& o) const
  {
    using namespace boost;

    o << "Problem:" << incendl;
    // Function.
    o << this->function () << iendl;

    // Arguments bounds.
    o << "Arguments bounds: " << this->argumentBounds () << iendl;
    // Arguments scaling.
    o << "Arguments scaling: " << this->argumentScaling () << iendl;
    // Arguments names.
    if (!this->argumentNames ().empty ())
      o << "Arguments names: " << this->argumentNames () << iendl;

    // Constraints.
    if (this->constraints ().empty ())
      o << fg::ok << "No constraints." << fg::reset;
    else
      o << "Number of constraints: " << this->constraints ().size ();

    for (unsigned i = 0; i < this->constraints ().size (); ++i)
      {
	detail::printConstraint<Problem<T> > pc (o, *this, i);
	try
	  {
	    boost::apply_visitor (pc, this->constraints ()[i]);
	  }
	catch (const boost::bad_get& e)
	  {
	    o << "failed to print constraints (boost::bad_get)" << iendl;
	  }
      }

    // Starting point.
    if (startingPoint_)
      {
	o << iendl << "Starting point: "
	  << "[" << startingPoint_->size () << "](";
	for (typename GenericFunction<T>::vector_t::Index i = 0;
	     i < startingPoint_->size (); ++i)
	  {
	    if (i > 0)
	      o << ",";
	    std::size_t i_ = static_cast<std::size_t> (i);
	    if (GenericFunction<T>::getLowerBound
		(this->argumentBounds ()[i_]) <= (*startingPoint_)[i] &&
		(*startingPoint_)[i] <= GenericFunction<T>::getUpperBound
		(this->argumentBounds ()[i_]))
	      o << fg::ok << (*startingPoint_)[i];
	    else
	      o << fg::fail << (*startingPoint_)[i];
	    o << fg::reset;
	  }
	typename GenericFunction<T>::argument_t x0 = *startingPoint_;
	o << ")" << iendl << "Starting value: "
	  << this->function () (x0);
      }
    else
      o << iendl << fg::warn << "No starting point." << fg::reset;

    // Infinity.
    o << iendl << "Infinity value (for all functions): "
      << function_t::infinity ();
    return o << decindent;
  }

  template <typename T>
  std::ostream&
  operator<< (std::ostream& o, const Problem<T>& pb)
  {
    return pb.print (o);
  }
} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_PROBLEM_HH
