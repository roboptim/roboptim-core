// Copyright (C) 2015 by Benjamin Chrétien, CNRS-UM LIRMM.
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

#include "shared-tests/fixture.hh"

#include <boost/make_shared.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/result-analyzer.hh>

using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
                          ::roboptim::EigenMatrixSparse> functionTypes_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Define f(x) = x * x
template <typename T>
struct Square : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericDifferentiableFunction<T>);

  explicit Square (size_type n)
    : GenericDifferentiableFunction<T> (n, 1, "x * x")
  {
  }

  void impl_compute (result_ref res,
                     const_argument_ref x) const
  {
    res[0] = x.dot (x);
  }

  void impl_gradient (gradient_ref grad,
                      const_argument_ref x,
                      size_type = 0) const
  {
    grad = 2. * x;
  }
};

template <>
void Square<EigenMatrixSparse>::impl_gradient
(gradient_ref grad, const_argument_ref x, size_type) const
{
  grad = x.sparseView ();
  grad *= 2.;
}


template <typename T>
class NullSolver : public Solver<T>
{
public:
  typedef Solver<T> parent_t;
  typedef typename parent_t::problem_t problem_t;
  typedef typename problem_t::function_t::argument_t argument_t;

  NullSolver (const problem_t& pb)
    : parent_t (pb)
  {
  }

  ~NullSolver ()
  {
  }

  void solve ()
  {
    (*output) << "solve ()" << std::endl;

    SolverError error ("the null solver always fails.");
    error.lastState () = Result (1, 1);

    parent_t::result_ = error;
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (result_analyzer, T, functionTypes_t)
{
  output = retrievePattern ("result-analyzer");

  typedef NullSolver<T> solver_t;
  typedef typename solver_t::problem_t problem_t;

  typedef typename problem_t::function_t      function_t;
  typedef typename function_t::argument_t     argument_t;
  typedef typename problem_t::intervals_t     intervals_t;
  typedef typename problem_t::scaling_t       scaling_t;

  typedef Square<T>                           square_t;

  // For shared_ptr constructor
  boost::shared_ptr<square_t>
    f = boost::make_shared<square_t> (2);

  problem_t pb (f);
  argument_t x (2);
  x << 0., 0.;
  pb.startingPoint () = x;

  typename square_t::names_t names (2);
  names[0] = "x₀";
  names[1] = "x₁";
  pb.argumentNames () = names;

  for (std::size_t i = 0; i < 2; ++i)
    pb.argumentBounds ()[i] = function_t::makeInterval (-1., 3.);

  boost::shared_ptr<square_t>
    cstr = boost::make_shared<square_t>  (2);
  intervals_t intervals (1);
  scaling_t scaling (1, 1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeInterval (0., 9.);
  pb.addConstraint (cstr, intervals, scaling);

  // A = {g}, but ∂g = 0
  {
    Result res (pb.function ().inputSize (), pb.constraintsOutputSize ());
    res.x = (argument_t (2) << 0., 0.).finished ();
    res.lambda = (argument_t (4) << 0., 0., 0., 1.).finished ();

    ResultAnalyzer<T> ra (pb, res);

    (*output) << "x = " << res.x << std::endl;
    (*output) << "λ = " << res.lambda << std::endl;

    typename ResultAnalyzer<T>::LICQData licq = ra.checkLICQ ();
    BOOST_CHECK (!licq);
    (*output) << licq << std::endl;

    typename ResultAnalyzer<T>::KKTData kkt = ra.checkKKT ();
    BOOST_CHECK (kkt);
    (*output) << kkt << std::endl;

    typename ResultAnalyzer<T>::NullGradientData ngd = ra.checkNullGradient ();
    BOOST_CHECK (!ngd);
    (*output) << ngd << std::endl;
  }

  // A = {x,g}, but gradients linearly dependent
  {
    Result res (pb.function ().inputSize (), pb.constraintsOutputSize ());
    res.x = (argument_t (2) << 0., 3.).finished ();
    res.lambda = (argument_t (4) << 1., 1., -1., 1.).finished ();

    ResultAnalyzer<T> ra (pb, res);

    (*output) << "x = " << res.x << std::endl;
    (*output) << "λ = " << res.lambda << std::endl;

    typename ResultAnalyzer<T>::LICQData licq = ra.checkLICQ ();
    BOOST_CHECK (!licq);
    (*output) << licq << std::endl;

    typename ResultAnalyzer<T>::KKTData kkt = ra.checkKKT ();
    BOOST_CHECK (!kkt);
    (*output) << kkt << std::endl;

    typename ResultAnalyzer<T>::NullGradientData ngd = ra.checkNullGradient ();
    BOOST_CHECK (ngd);
    (*output) << ngd << std::endl;
  }

  // TODO: A = {g}

  // A = {x}
  {
    Result res (pb.function ().inputSize (), pb.constraintsOutputSize ());
    res.x = (argument_t (2) << -1., 1.).finished ();
    res.lambda = (argument_t (4) << 1., 1., -1., 1.).finished ();

    ResultAnalyzer<T> ra (pb, res);

    (*output) << "x = " << res.x << std::endl;
    (*output) << "λ = " << res.lambda << std::endl;

    typename ResultAnalyzer<T>::LICQData licq = ra.checkLICQ ();
    BOOST_CHECK (licq);
    (*output) << licq << std::endl;

    typename ResultAnalyzer<T>::KKTData kkt = ra.checkKKT ();
    BOOST_CHECK (!kkt);
    (*output) << kkt << std::endl;

    typename ResultAnalyzer<T>::NullGradientData ngd = ra.checkNullGradient ();
    BOOST_CHECK (ngd);
    (*output) << ngd << std::endl;
  }

  (*output) << pb << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
