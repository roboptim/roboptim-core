// Copyright (C) 2014 by Benjamin Chretien, CNRS-LIRMM.
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

#include <boost/mpl/equal.hpp>
#include <boost/type_traits/is_same.hpp>

#include <roboptim/core/fwd.hh>
#include <roboptim/core/n-times-derivable-function.hh>
#include <roboptim/core/detail/utility.hh>

using namespace roboptim;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

// Test a predicate at compile-time and run-time.
#define TEST_PREDICATE(PRED) \
  BOOST_MPL_ASSERT ((PRED)); \
  BOOST_CHECK ((PRED::type::value));


BOOST_AUTO_TEST_CASE (detail_utility)
{
  using namespace boost;
  using namespace boost::mpl;
  using namespace ::roboptim::detail;

  // Test add_shared_ptr.
  {
    typedef vector<int, std::string, bool> testSeq_t;
    typedef add_shared_ptr<testSeq_t>::type testSeq_ptr;
    typedef vector<shared_ptr<int>, shared_ptr<std::string>, shared_ptr<bool> >
      expectedTestSeq_ptr;
    typedef boost::mpl::equal<testSeq_ptr, expectedTestSeq_ptr> predicate_t;

    TEST_PREDICATE (predicate_t);
  }

  // Test shared_ptr_variant.
  {
    typedef vector<int, std::string, bool> testVector_t;
    typedef shared_ptr_variant<testVector_t>::type testVariant_ptr;
    typedef variant<shared_ptr<int>, shared_ptr<std::string>, shared_ptr<bool> >
      expectedTestVariant_ptr;
    typedef boost::mpl::equal<testVariant_ptr, expectedTestVariant_ptr> predicate_t;

    TEST_PREDICATE (predicate_t);
  }

  // Test is_eigen_type.
  {
    typedef is_eigen_type<Eigen::MatrixXd> predicate1_t;
    typedef is_eigen_type<Eigen::Vector3d> predicate2_t;

    typedef not_<is_eigen_type<std::string> > predicate3_t;
    typedef not_<is_eigen_type<int> > predicate4_t;
    typedef not_<is_eigen_type<float> > predicate5_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
    TEST_PREDICATE (predicate4_t);
    TEST_PREDICATE (predicate5_t);
  }

  // Test const_ref.
  {
    typedef is_same<const float&, const_ref<float>::type> predicate1_t;
    typedef is_same<const std::vector<int>&,
                    const_ref<std::vector<int> >::type> predicate2_t;
    typedef is_same<const Eigen::Ref<const Eigen::Vector3d>,
                    const_ref<Eigen::Vector3d>::type> predicate3_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
  }

  // Test contains_base_of.
  {
    typedef vector<Function, DifferentiableFunction> testSeq_t;
    typedef contains_base_of<testSeq_t, QuadraticFunction> predicate1_t;
    typedef not_<contains_base_of<testSeq_t, std::string> > predicate2_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
  }

  // Test get_descendant.
  {
    typedef is_same<get_descendant<DifferentiableFunction, Function>::type,
                    DifferentiableFunction> predicate1_t;
    typedef is_same<get_descendant<Function, DifferentiableFunction>::type,
                    DifferentiableFunction> predicate2_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
  }

  // Test check_constraint_type.
  {
    typedef vector<QuadraticFunction, DifferentiableFunction> testSeq_t;
    typedef check_constraint_type<LinearFunction, testSeq_t> predicate1_t;
    typedef not_<check_constraint_type<testSeq_t, Function> > predicate2_t;
    typedef not_<check_constraint_type<testSeq_t, std::string> > predicate3_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
  }

  // Test cast_constraint_type.
  {
    typedef vector<QuadraticFunction, DifferentiableFunction> testSeq_t;
    typedef is_same<cast_constraint_type<LinearFunction, testSeq_t>::type,
                    QuadraticFunction> predicate1_t;
    typedef is_same<cast_constraint_type<QuadraticFunction, testSeq_t>::type,
                    QuadraticFunction> predicate2_t;
    typedef is_same<cast_constraint_type<DifferentiableFunction, testSeq_t>::type,
                    DifferentiableFunction> predicate3_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
  }

  // Test derives_from_function.
  {
    typedef derives_from_function<Function> predicate1_t;
    typedef derives_from_function<DifferentiableSparseFunction> predicate2_t;
    typedef derives_from_function<QuadraticFunction> predicate3_t;

    typedef not_<derives_from_function<std::string> > predicate4_t;
    typedef not_<derives_from_function<float> > predicate5_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
    TEST_PREDICATE (predicate4_t);
    TEST_PREDICATE (predicate5_t);
  }

  // Test derives_from_differentiable_function.
  {
    typedef derives_from_differentiable_function<DifferentiableFunction> predicate1_t;
    typedef derives_from_differentiable_function<DifferentiableSparseFunction> predicate2_t;
    typedef derives_from_differentiable_function<QuadraticFunction> predicate3_t;

    typedef not_<derives_from_differentiable_function<std::string> > predicate4_t;
    typedef not_<derives_from_differentiable_function<SparseFunction> > predicate5_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
    TEST_PREDICATE (predicate4_t);
    TEST_PREDICATE (predicate5_t);
  }

  // Test derives_from_twice_differentiable_function.
  {
    typedef derives_from_twice_differentiable_function
             <TwiceDifferentiableFunction> predicate1_t;
    typedef derives_from_twice_differentiable_function
             <TwiceDifferentiableSparseFunction> predicate2_t;
    typedef derives_from_twice_differentiable_function
             <QuadraticFunction> predicate3_t;

    typedef not_<derives_from_twice_differentiable_function
                 <std::string> > predicate4_t;
    typedef not_<derives_from_twice_differentiable_function
                 <DifferentiableSparseFunction> > predicate5_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
    TEST_PREDICATE (predicate4_t);
    TEST_PREDICATE (predicate5_t);
  }

  // Test derives_from_ntimes_derivable_function.
  {
    typedef derives_from_ntimes_derivable_function
             <NTimesDerivableFunction<2> > predicate1_t;
    typedef derives_from_ntimes_derivable_function
             <NTimesDerivableFunction<3> > predicate2_t;
    typedef derives_from_ntimes_derivable_function
             <NTimesDerivableFunction<4> > predicate3_t;

    typedef not_<derives_from_ntimes_derivable_function
                 <std::string> > predicate4_t;
    typedef not_<derives_from_ntimes_derivable_function
                 <TwiceDifferentiableFunction> > predicate5_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
    TEST_PREDICATE (predicate4_t);
    TEST_PREDICATE (predicate5_t);
  }
}

BOOST_AUTO_TEST_SUITE_END ()
