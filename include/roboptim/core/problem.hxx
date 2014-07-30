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
# include <boost/mpl/count_if.hpp>
# include <boost/mpl/greater.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/variant.hpp>
# include <boost/variant/get.hpp>
# include <boost/variant/apply_visitor.hpp>

# include <roboptim/core/indent.hh>
# include <roboptim/core/terminal-color.hh>
# include <roboptim/core/util.hh>

namespace roboptim
{
  namespace detail
  {
    using namespace boost;
    using namespace boost::mpl;

    /// \brief Whether a sequence of types contains a base of a given type.
    /// \tparam Sequence sequence of types.
    /// \tparam Type type.
    template<typename Sequence, typename Type>
    struct contains_base_of
    : greater<int_<count_if<Sequence, is_base_of<_, Type> >::value>, int_<0> >
    {};
  } // end of namespace detail.

  //
  // Template specialization for problem without constraint
  //
  template <typename F>
  Problem<F, boost::mpl::vector <> >::Problem (const function_t& f)
    : function_ (f),
      startingPoint_ (),
      argumentBounds_ (static_cast<size_t> (f.inputSize ())),
      argumentScales_ (static_cast<size_t> (f.inputSize ())),
      argumentNames_ ()
  {
    // Check that in the objective function m = 1 (R^n -> R).
    assert (f.outputSize () == 1);

    // Initialize bound.
    std::fill (argumentBounds_.begin (), argumentBounds_.end (),
               function_t::makeInfiniteInterval ());
    // Initialize scale.
    std::fill (argumentScales_.begin (), argumentScales_.end (), 1.);
  }

  // Copy constructor.
  template <typename F>
  Problem<F, boost::mpl::vector <> >::Problem
  (const Problem<F, boost::mpl::vector <> >& pb) :
    function_ (pb.function_),
    startingPoint_ (pb.startingPoint_),
    argumentBounds_ (pb.argumentBounds_),
    argumentScales_ (pb.argumentScales_),
    argumentNames_ (pb.argumentNames_)
  {
  }

  // Copy constructor (convert from another class of problem).
  template <typename F>
  template <typename F_>
  Problem<F, boost::mpl::vector<> >::Problem
  (const Problem<F_, boost::mpl::vector<> >& pb)
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      argumentBounds_ (pb.argumentBounds_),
      argumentScales_ (pb.argumentScales_),
      argumentNames_ (pb.argumentNames_)
  {
    // Check that F is a subtype of F_.
    BOOST_MPL_ASSERT_MSG((boost::is_base_of<F, F_>::value),
                         SAME_COST_FUNCTION_TYPE_EXPECTED, (F&, F_&));
  }

  template <typename F>
  Problem<F, boost::mpl::vector<> >::~Problem ()
  {
  }

  template <typename F>
  const typename Problem<F, boost::mpl::vector<> >::function_t&
  Problem<F, boost::mpl::vector<> >::function () const
  {
    return function_;
  }

  template <typename F>
  typename Problem<F, boost::mpl::vector<> >::startingPoint_t&
  Problem<F, boost::mpl::vector<> >::startingPoint ()
  {
    if (startingPoint_ && startingPoint_->size ()
	!= this->function ().inputSize ())
      assert (0 && "Invalid starting point (wrong size)");
    return startingPoint_;
  }

  template <typename F>
  const typename Problem<F, boost::mpl::vector<> >::startingPoint_t&
  Problem<F, boost::mpl::vector<> >::startingPoint () const
  {
    if (startingPoint_ && startingPoint_->size ()
	!= this->function ().inputSize ())
      assert (0 && "Invalid starting point (wrong size)");
    return startingPoint_;
  }

  template <typename F>
  std::ostream&
  operator<< (std::ostream& o,
	      const Problem<F, boost::mpl::vector<> > & pb)
  {
    return pb.print (o);
  }

  template <typename F>
  std::ostream&
  Problem<F, boost::mpl::vector<> >::
  print (std::ostream& o) const
  {
    using namespace boost;

    o << "Problem:" << incendl;
    // Function.
    o << this->function () << iendl;

    // Arguments bounds.
    o << "Arguments bounds: " << this->argumentBounds () << iendl;
    // Arguments scales.
    o << "Arguments scales: " << this->argumentScales () << iendl;
    // Arguments names.
    if (!this->argumentNames ().empty ())
      o << "Arguments names: " << this->argumentNames () << iendl;

    // Starting point.
    if (startingPoint_)
      {
	o << iendl << "Starting point: "
	  << "[" << startingPoint_->size () << "](";
	for (typename F::vector_t::Index i = 0;
	     i < startingPoint_->size (); ++i)
	  {
	    if (i > 0)
	      o << ",";
	    std::size_t i_ = static_cast<std::size_t> (i);
	    if (F::getLowerBound
		(this->argumentBounds ()[i_]) <= (*startingPoint_)[i] &&
		(*startingPoint_)[i] <= F::getUpperBound
		(this->argumentBounds ()[i_]))
	      o << fg::ok << (*startingPoint_)[i] << fg::reset;
	    else
	      o << fg::fail << (*startingPoint_)[i] << fg::reset;
	  }
	o << ")" << iendl << "Starting value: "
	  << this->function () (*startingPoint_);
      }
    else
      o << iendl << fg::warn << "No starting point." << fg::reset;

    // Infinity.
    o << iendl << "Infinity value (for all functions): "
      << function_t::infinity ();
    return o << decindent;
  }

  template <typename F>
  typename Problem<F, boost::mpl::vector<> >::intervals_t&
  Problem<F, boost::mpl::vector<> >::argumentBounds ()
  {
    return argumentBounds_;
  }

  template <typename F>
  const typename Problem<F, boost::mpl::vector<> >::intervals_t&
  Problem<F, boost::mpl::vector<> >::argumentBounds () const
  {
    return argumentBounds_;
  }

  template <typename F>
  typename Problem<F, boost::mpl::vector<> >::scales_t&
  Problem<F, boost::mpl::vector<> >::argumentScales ()
  {
    return argumentScales_;
  }

  template <typename F>
  const typename Problem<F, boost::mpl::vector<> >::scales_t&
  Problem<F, boost::mpl::vector<> >::argumentScales () const
  {
    return argumentScales_;
  }

  template <typename F>
  typename Problem<F, boost::mpl::vector<> >::names_t&
  Problem<F, boost::mpl::vector<> >::argumentNames ()
  {
    return argumentNames_;
  }

  template <typename F>
  const typename Problem<F, boost::mpl::vector<> >::names_t&
  Problem<F, boost::mpl::vector<> >::argumentNames () const
  {
    return argumentNames_;
  }

  //
  //
  // General template implementation
  //
  template <typename F, typename CLIST>
  Problem<F, CLIST>::Problem (const function_t& f)
    : function_ (f),
      startingPoint_ (),
      constraints_ (),
      boundsVect_ (),
      argumentBounds_ (static_cast<std::size_t> (f.inputSize ())),
      scalesVect_ (),
      argumentScales_ (static_cast<std::size_t> (f.inputSize ())),
      argumentNames_ ()
  {
    // Initialize bound.
    std::fill (argumentBounds_.begin (), argumentBounds_.end (),
               function_t::makeInfiniteInterval ());
    // Initialize scale.
    std::fill (argumentScales_.begin (), argumentScales_.end (), 1.);
  }

  template <typename F, typename CLIST>
  Problem<F, CLIST>::~Problem ()
  {
  }

  // Copy constructor.
  template <typename F, typename CLIST>
  Problem<F, CLIST>::Problem (const Problem<F, CLIST>& pb)
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (pb.constraints_),
      boundsVect_ (pb.boundsVect_),
      argumentBounds_ (pb.argumentBounds_),
      scalesVect_ (pb.scalesVect_),
      argumentScales_ (pb.argumentScales_),
      argumentNames_ (pb.argumentNames_)
  {
  }

  // Copy constructor (convert from another class of problem).
  template <typename F, typename CLIST>
  template <typename F_, typename CLIST_>
  Problem<F, CLIST>::Problem (const Problem<F_, CLIST_>& pb)
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (),
      boundsVect_ (pb.boundsVect_),
      argumentBounds_ (pb.argumentBounds_),
      scalesVect_ (pb.scalesVect_),
      argumentScales_ (pb.argumentScales_),
      argumentNames_ (pb.argumentNames_)
  {
    using namespace boost;

    // Check that F is a subtype of F_.
    BOOST_MPL_ASSERT_MSG((is_base_of<F, F_>::value),
                         INCOMPATIBLE_TYPES_FOR_COST, (F&, F_&));

    // Check that CLIST_ is a subset of CLIST (i.e. all the functions
    // of CLIST_ derive from functions of CLIST).
    // The algorithm is as follows:
    // (0) outer_ok = true
    // (1) for c_ in FLIST_:
    // (2)   inner_ok = false
    // (3)   for c in FLIST:
    // (4)     if c is_base_of c_:
    // (5)       inner_ok = true
    // (6)   if not inner_ok:
    // (7)     outer_ok = false
    // (8) return outer_ok
    BOOST_MPL_ASSERT_MSG(
      (mpl::fold<CLIST_, // (1)
                 mpl::bool_<true>, // (0)
                 mpl::if_< // (6)
                   detail::contains_base_of<CLIST,mpl::_2>, // (2,3,4,5)
                   mpl::_1,
                   mpl::bool_<false> // (7)
                 >
       >::type::value), // (8)
       INCOMPATIBLE_TYPES_IN_LIST, (CLIST_, CLIST));

    // Copy constraints.
    std::copy (pb.constraints_.begin (), pb.constraints_.end (),
               constraints_.begin ());
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::function_t&
  Problem<F, CLIST>::function () const
  {
    return function_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::constraints_t&
  Problem<F, CLIST>::constraints () const
  {
    return constraints_;
  }

// Verify that C is in CLIST or derives from one of its elements.
#define ASSERT_CONSTRAINT_TYPE(C,CLIST)                       \
  BOOST_MPL_ASSERT_MSG(                                       \
    (boost::mpl::fold<CLIST,                                  \
                      boost::mpl::bool_<false>,               \
                      boost::mpl::if_<                        \
                        boost::is_base_of<boost::mpl::_2, C>, \
                        boost::mpl::bool_<true>,              \
                        boost::mpl::_1>                       \
     >::type::value),                                         \
     CONSTRAINT_TYPE_IS_NOT_VALID, (C&, CLIST))

  template <typename F, typename CLIST>
  template <typename C>
  void
  Problem<F, CLIST>::addConstraint (boost::shared_ptr<C> x,
				    interval_t b,
				    value_type s)
  {
    // Check that C is in CLIST.
    ASSERT_CONSTRAINT_TYPE (C,CLIST);

    if (x->inputSize () != this->function ().inputSize ())
      throw std::runtime_error ("Invalid constraint (wrong input size)");
    if (x->outputSize () != 1)
      throw std::runtime_error
	("Invalid constraint (output size is not equal to one)");

    // Check that the pointer is not null.
    assert (!!x.get ());
    assert (b.first <= b.second);
    constraints_.push_back (boost::static_pointer_cast<C> (x));
    intervals_t bounds;
    bounds.push_back (b);
    boundsVect_.push_back (bounds);
    scales_t scales;
    scales.push_back (s);
    scalesVect_.push_back (scales);
  }

  template <typename F, typename CLIST>
  template <typename C>
  void
  Problem<F, CLIST>::addConstraint (boost::shared_ptr<C> x,
				    intervals_t b,
				    scales_t s)
  {
    // Check that C is in CLIST.
    ASSERT_CONSTRAINT_TYPE (C,CLIST);

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
	  ("Failed to add constraint '%s': scale vector size is invalid "
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
    constraints_.push_back (boost::static_pointer_cast<C> (x));

    // Check that the bounds are correctly defined.
    for (std::size_t i = 0; i < static_cast<std::size_t> (x->outputSize ());
         ++i)
      {
        ROBOPTIM_DEBUG_ONLY(const interval_t& interval = b[i]);
	assert (interval.first <= interval.second);
      }

    boundsVect_.push_back (b);
    scalesVect_.push_back (s);
  }

#undef ASSERT_CONSTRAINT_TYPE

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::startingPoint_t&
  Problem<F, CLIST>::startingPoint ()
  {
    if (startingPoint_ && startingPoint_->size ()
	!= this->function ().inputSize ())
      throw std::runtime_error ("Invalid starting point (wrong size)");
    return startingPoint_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::startingPoint_t&
  Problem<F, CLIST>::startingPoint () const
  {
    if (startingPoint_ && startingPoint_->size ()
	!= this->function ().inputSize ())
      throw std::runtime_error ("Invalid starting point (wrong size)");
    return startingPoint_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::intervalsVect_t&
  Problem<F, CLIST>::boundsVector () const
  {
    return boundsVect_;
  }

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::intervals_t&
  Problem<F, CLIST>::argumentBounds ()
  {
    return argumentBounds_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::intervals_t&
  Problem<F, CLIST>::argumentBounds () const
  {
    return argumentBounds_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::scalesVect_t&
  Problem<F, CLIST>::scalesVector () const
  {
    return scalesVect_;
  }

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::scales_t&
  Problem<F, CLIST>::argumentScales ()
  {
    return argumentScales_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::scales_t&
  Problem<F, CLIST>::argumentScales () const
  {
    return argumentScales_;
  }

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::names_t&
  Problem<F, CLIST>::argumentNames ()
  {
    return argumentNames_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::names_t&
  Problem<F, CLIST>::argumentNames () const
  {
    return argumentNames_;
  }

  namespace detail
  {
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
           << iendl << "Scales: " << problem_.scalesVector ()[i_];

	if (problem_.startingPoint ())
	  {
	    U g = get<U> (problem_.constraints ()[i_]);
            typename P::vector_t x = (*g) (*problem_.startingPoint ());
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

  template <typename F, typename CLIST>
  std::ostream&
  Problem<F, CLIST>::print (std::ostream& o) const
  {
    using namespace boost;

    o << "Problem:" << incendl;
    // Function.
    o << this->function () << iendl;

    // Arguments bounds.
    o << "Arguments bounds: " << this->argumentBounds () << iendl;
    // Arguments scales.
    o << "Arguments scales: " << this->argumentScales () << iendl;
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
	detail::printConstraint<Problem<F, CLIST> > pc (o, *this, i);
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
	for (typename F::vector_t::Index i = 0;
	     i < startingPoint_->size (); ++i)
	  {
	    if (i > 0)
	      o << ",";
	    std::size_t i_ = static_cast<std::size_t> (i);
	    if (F::getLowerBound
		(this->argumentBounds ()[i_]) <= (*startingPoint_)[i] &&
		(*startingPoint_)[i] <= F::getUpperBound
		(this->argumentBounds ()[i_]))
	      o << fg::ok << (*startingPoint_)[i];
	    else
	      o << fg::fail << (*startingPoint_)[i];
	    o << fg::reset;
	  }
	o << ")" << iendl << "Starting value: "
	  << this->function () (*startingPoint_);
      }
    else
      o << iendl << fg::warn << "No starting point." << fg::reset;

    // Infinity.
    o << iendl << "Infinity value (for all functions): "
      << function_t::infinity ();
    return o << decindent;
  }

  template <typename F, typename CLIST>
  std::ostream&
  operator<< (std::ostream& o, const Problem<F, CLIST>& pb)
  {
    return pb.print (o);
  }
} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_PROBLEM_HH
