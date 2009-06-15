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
# include <boost/numeric/ublas/io.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/variant.hpp>
# include <boost/variant/get.hpp>
# include <boost/variant/apply_visitor.hpp>

# include <roboptim/core/indent.hh>
# include <roboptim/core/util.hh>

namespace roboptim
{
  template <typename F, typename CLIST>
  Problem<F, CLIST>::Problem (const function_t& f) throw ()
    : function_ (f),
      startingPoint_ (),
      constraints_ (),
      bounds_ (),
      argumentBounds_ (f.inputSize ()),
      scales_ (),
      argumentScales_ (f.inputSize ())
  {
    // Check that in the objective function m = 1 (R^n -> R).
    assert (f.outputSize () == 1);

    // Initialize bound.
    std::fill (argumentBounds_.begin (), argumentBounds_.end (),
	       Function::makeInfiniteInterval ());
    // Initialize scale.
    std::fill (argumentScales_.begin (), argumentScales_.end (), 1.);
  }

  template <typename F, typename CLIST>
  Problem<F, CLIST>::~Problem () throw ()
  {
  }

  // Copy constructor.
  template <typename F, typename CLIST>
  Problem<F, CLIST>::Problem (const Problem<F, CLIST>& pb) throw ()
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (pb.constraints_),
      bounds_ (pb.bounds_),
      argumentBounds_ (pb.argumentBounds_),
      scales_ (pb.scales_),
      argumentScales_ (pb.argumentScales_)
  {
  }

  // Copy constructor (convert from another class of problem).
  template <typename F, typename CLIST>
  template <typename F_, typename CLIST_>
  Problem<F, CLIST>::Problem (const Problem<F_, CLIST_>& pb) throw ()
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (),
      bounds_ (pb.bounds_),
      argumentBounds_ (pb.argumentBounds_),
      scales_ (pb.scales_),
      argumentScales_ (pb.argumentScales_)
  {
    // Check that F is a subtype of F_.
    BOOST_STATIC_ASSERT((boost::is_base_of<F, F_>::value));

    //FIXME: check that CLIST is a MPL vector of Function's sub-classes.

    std::copy (pb.constraints_.begin (), pb.constraints_.end (),
               constraints_.begin ());
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::function_t&
  Problem<F, CLIST>::function () const throw ()
  {
    return function_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::constraints_t&
  Problem<F, CLIST>::constraints () const throw ()
  {
    return constraints_;
  }

  template <typename F, typename CLIST>
  template <typename C>
  void
  Problem<F, CLIST>::addConstraint (boost::shared_ptr<C> x,
				    interval_t b,
				    value_type s)
    throw (std::runtime_error)
  {
    //FIXME: check that C is in CLIST.

    // Check that the pointer is not null.
    assert (!!x.get ());
    assert (b.first <= b.second);
    constraints_.push_back (boost::static_pointer_cast<C> (x));
    bounds_.push_back (b);
    scales_.push_back (s);
  }

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::startingPoint_t&
  Problem<F, CLIST>::startingPoint () throw ()
  {
    return startingPoint_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::startingPoint_t&
  Problem<F, CLIST>::startingPoint () const throw ()
  {
    return startingPoint_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::intervals_t&
  Problem<F, CLIST>::bounds () const throw ()
  {
    return bounds_;
  }

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::intervals_t&
  Problem<F, CLIST>::argumentBounds () throw ()
  {
    return argumentBounds_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::intervals_t&
  Problem<F, CLIST>::argumentBounds () const throw ()
  {
    return argumentBounds_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::scales_t&
  Problem<F, CLIST>::scales () const throw ()
  {
    return scales_;
  }

  template <typename F, typename CLIST>
  typename Problem<F, CLIST>::scales_t&
  Problem<F, CLIST>::argumentScales () throw ()
  {
    return argumentScales_;
  }

  template <typename F, typename CLIST>
  const typename Problem<F, CLIST>::scales_t&
  Problem<F, CLIST>::argumentScales () const throw ()
  {
    return argumentScales_;
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
		       int i) :
	problem_ (problem),
	o_ (o),
	i_ (i)
      {}

      template <typename U>
      void operator () (const U&)
      {
	assert (problem_.constraints ().size () - i_ > 0);
	using namespace boost;
        o_ << iendl << incindent
	   << "Constraint " << i_ << incindent << iendl
	   << "Bounds: " << problem_.bounds ()[i_] << iendl
	   << "Scales: " << problem_.scales ()[i_] << iendl;

	if (problem_.startingPoint ())
	  {
	    U g = get<U> (problem_.constraints ()[i_]);
	    o_ << "Initial value: "
	       << (*g) (*problem_.startingPoint ()) << iendl;
	  }
	o_ << decindent << decindent;
      }
    private:
      const P& problem_;
      std::ostream& o_;
      int i_;
    };
  } // end of namespace detail.

  template <typename F, typename CLIST>
  std::ostream&
  Problem<F, CLIST>::print (std::ostream& o) const throw ()
  {
    using namespace boost;

    o << "Problem:" << incendl;
    // Function.
    o << function () << iendl;

    // Arguments' bounds.
    o << "Argument's bounds: " << argumentBounds () << iendl;
    // Arguments' scales.
    o << "Argument's scales: " << argumentScales () << iendl;

    // Constraints.
    if (constraints ().empty ())
      o << "No constraints.";
    else
      o << "Number of constraints: " << constraints ().size ();

    for (unsigned i = 0; i < this->constraints ().size (); ++i)
      {
	detail::printConstraint<Problem<F, CLIST> > pc (o, *this, i);
	boost::apply_visitor (pc, this->constraints ()[i]);
      }

    // Starting point.
    if (startingPoint_)
      {
	o << iendl << "Starting point: " << *startingPoint_
	  << iendl << "Starting value: " << function () (*startingPoint_);
      }
    else
      o << iendl << "No starting point.";

    // Infinity.
    o << iendl << "Infinity value (for all functions): " << Function::infinity ();
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
