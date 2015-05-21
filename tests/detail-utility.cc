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

#define EIGEN_RUNTIME_NO_MALLOC
#include <Eigen/Core>

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

namespace detail
{
  using namespace Eigen;
  using namespace ::roboptim::detail;

  template <int SO>
  struct TestRowVectorStride
  {
    typedef Matrix<double, Dynamic, Dynamic, SO> matrix_t;
    typedef Matrix<double, 1, Dynamic> rowVector_t;
    typedef Ref<rowVector_t, 0, typename row_vector_stride<SO>::type> rowVector_ref;

    void foo (rowVector_ref v)
    {
      v.fill (0.);
    }

    void run ()
    {
      matrix_t m (2, 2);
      m << 1., 2., 3., 4.;

      set_is_malloc_allowed (false);
      foo (m.row (1));
      set_is_malloc_allowed (true);

      BOOST_CHECK (!m.row (0).isZero ());
      BOOST_CHECK (m.row (1).isZero ());
    }
  };
} // end of namespace detail


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
    typedef is_same<const Eigen::Ref<const Eigen::Vector3d>&,
                    const_ref<Eigen::Vector3d>::type> predicate3_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
  }

  // Test row_vector_stride.
  {
    detail::TestRowVectorStride<Eigen::ColMajor> testCol;
    testCol.run ();

    detail::TestRowVectorStride<Eigen::RowMajor> testRow;
    testRow.run ();
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

  // Test is_compatible_list.
  {
    typedef vector<QuadraticFunction, DifferentiableFunction> testSeq1_t;
    typedef vector<Function, QuadraticFunction> testSeq2_t;
    typedef vector<SparseFunction, QuadraticFunction> testSeq3_t;

    typedef is_compatible_list<testSeq1_t, testSeq1_t> predicate1_t;
    typedef is_compatible_list<testSeq1_t, testSeq2_t> predicate2_t;
    typedef not_<is_compatible_list<testSeq2_t, testSeq1_t> > predicate3_t;

    typedef not_<is_compatible_list<testSeq1_t, testSeq3_t> > predicate4_t;
    typedef not_<is_compatible_list<testSeq2_t, testSeq3_t> > predicate5_t;
    typedef not_<is_compatible_list<testSeq3_t, testSeq1_t> > predicate6_t;
    typedef not_<is_compatible_list<testSeq3_t, testSeq2_t> > predicate7_t;

    TEST_PREDICATE (predicate1_t);
    TEST_PREDICATE (predicate2_t);
    TEST_PREDICATE (predicate3_t);
    TEST_PREDICATE (predicate4_t);
    TEST_PREDICATE (predicate5_t);
    TEST_PREDICATE (predicate6_t);
    TEST_PREDICATE (predicate7_t);
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
