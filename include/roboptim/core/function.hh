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

#ifndef ROBOPTIM_CORE_FUNCTION_HH
# define ROBOPTIM_CORE_FUNCTION_HH
# include <cstring>
# include <iomanip>
# include <iostream>
# include <limits>
# include <sstream>
# include <stdexcept>
# include <string>
# include <utility>
# include <vector>

# include <boost/algorithm/string/replace.hpp>
# include <boost/tuple/tuple.hpp>
# include <boost/preprocessor/punctuation/comma.hpp>

# define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
# define EIGEN_RUNTIME_NO_MALLOC
# include <Eigen/Core>
# include <Eigen/Dense>
# include <Eigen/Sparse>

# include <log4cxx/logger.h>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/indent.hh>
# include <roboptim/core/portability.hh>

# define ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(NAME,TYPE)	\
  typedef TYPE NAME##_t;					\
  typedef Eigen::Ref<NAME##_t> NAME##_ref;			\
  typedef const Eigen::Ref<const NAME##_t>& const_##NAME##_ref

# define ROBOPTIM_GENERATE_TYPEDEFS_REF(NAME,TYPE)	\
  typedef TYPE NAME##_t;				\
  typedef NAME##_t& NAME##_ref;				\
  typedef const NAME##_t& const_##NAME##_ref

# define ROBOPTIM_GENERATE_FWD_REFS(NAME)			\
  typedef parent_t::NAME##_t NAME##_t;				\
  typedef parent_t::NAME##_ref NAME##_ref;			\
  typedef parent_t::const_##NAME##_ref const_##NAME##_ref

# define ROBOPTIM_GENERATE_FWD_REFS_(NAME)				\
  typedef typename parent_t::NAME##_t NAME##_t;				\
  typedef typename parent_t::NAME##_ref NAME##_ref;			\
  typedef typename parent_t::const_##NAME##_ref const_##NAME##_ref

# define ROBOPTIM_GENERATE_TRAITS_REFS_(NAME)				\
  typedef typename GenericFunctionTraits<T>::NAME##_t NAME##_t;		\
  typedef typename GenericFunctionTraits<T>::NAME##_ref NAME##_ref;	\
  typedef typename GenericFunctionTraits<T>::const_##NAME##_ref const_##NAME##_ref

# define ROBOPTIM_FUNCTION_FWD_TYPEDEFS(PARENT)	\
  typedef PARENT parent_t;			\
  typedef parent_t::value_type value_type;	\
  typedef parent_t::size_type size_type;	\
  typedef parent_t::names_t names_t;            \
  ROBOPTIM_GENERATE_FWD_REFS(argument);		\
  ROBOPTIM_GENERATE_FWD_REFS(result);		\
  ROBOPTIM_GENERATE_FWD_REFS(vector);		\
  ROBOPTIM_GENERATE_FWD_REFS(matrix)

# define ROBOPTIM_FUNCTION_FWD_TYPEDEFS_(PARENT)	\
  typedef PARENT parent_t;				\
  typedef typename parent_t::value_type value_type;	\
  typedef typename parent_t::size_type size_type;	\
  typedef typename parent_t::names_t names_t;           \
  ROBOPTIM_GENERATE_FWD_REFS_(argument);		\
  ROBOPTIM_GENERATE_FWD_REFS_(result);			\
  ROBOPTIM_GENERATE_FWD_REFS_(vector);			\
  ROBOPTIM_GENERATE_FWD_REFS_(matrix)

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief GenericFunction traits
  ///
  /// This helper class is used to define which types a GenericFunction
  /// should use. The following types are required:
  ///
  /// - value_type scalar type used for matrix coefficients (e.g. double)
  /// - size_type type used for indexing matrices (e.g. std::size_t)
  /// - matrix_t the matrix type (e.g. Eigen::Matrix<double, 1, 2>)
  /// - vector_t the used vector type
  /// - result_t function result type (vector or matrix)
  /// - argument_t function argument type (usually vector)
  template <typename T>
  struct GenericFunctionTraits
  {};

  /// \brief Define an abstract mathematical function (\f$C^0\f$).
  ///
  /// A function is an object that can be evaluated for a given
  /// point.
  ///
  /// \f[ f : x \rightarrow f(x) \f]
  /// \f$x \in \mathbb{R}^n\f$, \f$f(x) \in \mathbb{R}^m\f$ where
  /// \f$n\f$ is the input size and \f$m\f$ is the output size.
  ///
  /// Functions are pure immutable objects: evaluating a function
  /// twice at a given point <b>must</b> give the same result.
  ///
  /// This function is parametrized by the matrix type used in this
  /// function. Currently, dense (which size may be dynamic or
  /// static and sparse Eigen matrices can be used) are supported.
  ///
  /// \tparam T Matrix type
  template <typename T>
  class GenericFunction
  {
  public:
    /// \brief Traits type.
    ///
    /// Represents the matrix type used to store the underlying data. This
    /// can be EigenMatrixDense or EigenMatrixSparse.
    typedef T traits_t;

    /// \brief Values type.
    ///
    /// Represents the numerical type (i.e. float, double, int...)
    /// used for computations.
    typedef typename GenericFunctionTraits<T>::value_type value_type;

    /// \brief Basic vector type.
    ///
    /// This basic vector type is used each time a vector of values
    /// is required.
    ///
    /// \attention It is good practice in RobOptim to rely on this type
    /// when a vector of values is needed instead of relying on a particular
    /// implementation.
    ROBOPTIM_GENERATE_TRAITS_REFS_(vector);

    /// \brief Basic matrix type.
    ///
    /// This basic matrix type is used each time a two dimensional
    /// matrix of values is needed.
    ///
    /// \attention It is good practice in RobOptim to rely on this type
    /// when a matrix of values is needed instead of relying on a particular
    /// implementation.
    ROBOPTIM_GENERATE_TRAITS_REFS_(matrix);

    /// \brief Size type.
    ///
    /// This type is used to represent sizes, indexes, etc.
    typedef typename GenericFunctionTraits<T>::size_type size_type;

    /// \brief Type of a function evaluation result.
    ROBOPTIM_GENERATE_TRAITS_REFS_(result);

    /// \brief Type of a function evaluation argument.
    ROBOPTIM_GENERATE_TRAITS_REFS_(argument);

    /// \brief Type of a function argument name.
    typedef std::string name_t;

    /// \brief Type of a vector of function argument names.
    typedef std::vector<name_t> names_t;

    /// \brief Get the value of the machine epsilon, useful for
    /// floating types comparison.

    /// \return machine epsilon value.
    static value_type epsilon ()
    {
      return std::numeric_limits<value_type>::epsilon ();
    }

    /// \brief Get the value that symbolizes positive infinity.
    /// \return representation of positive infinity in the value type
    static value_type infinity ()
    {
      return std::numeric_limits<value_type>::infinity ();
    }

    /// \name Interval
    /// \{

    /// \brief Interval type (lower, upper).
    /// Use negative or positive infinity to respectively disable the
    /// lower or upper bound.
    typedef std::pair<value_type, value_type> interval_t;

    /// \brief Vector of intervals.
    typedef std::vector<interval_t> intervals_t;

    /// \brief Construct an interval from a lower and upper bound.
    /// \param l lower bound
    /// \param u upper bound
    /// \return interval representing \f$[l, u]\f$
    static interval_t makeInterval (value_type l, value_type u)
    {
      assert (l <= u);
      return std::make_pair (l, u);
    }

    /// \brief Construct an infinite interval.
    /// \return interval representing \f$[-\infty, +\infty]\f$
    static interval_t makeInfiniteInterval ()
    {
      return std::make_pair (-infinity (), infinity  ());
    }

    /// \brief Construct an interval from a lower bound.
    /// \param l lower bound
    /// \return interval representing \f$[l, +\infty]\f$
    static interval_t makeLowerInterval (value_type l)
    {
      return makeInterval (l, infinity  ());
    }

    /// \brief Construct an interval from an upper bound.
    /// \param u upper bound
    /// \return interval representing \f$[-\infty, u]\f$
    static interval_t makeUpperInterval (value_type u)
    {
      return makeInterval (-infinity  (), u);
    }

    /// \brief Get the lower bound of an interval
    /// \param interval accessed interval
    /// \return lower bound of the interval
    static value_type getLowerBound (const interval_t& interval)
    {
      return interval.first;
    }

    /// \brief Get the upper bound of an interval
    /// \param interval accessed interval
    /// \return upper bound of the interval
    static value_type getUpperBound (const interval_t& interval)
    {
      return interval.second;
    }

    /// \}

    /// \name Discrete interval
    /// \{

    /// \brief Types representing a discrete interval.
    /// A discrete interval is a triplet of values:
    /// - lower bound,
    /// - upper bound,
    /// - step.
    typedef boost::tuple<value_type,
			 value_type,
			 value_type> discreteInterval_t;


    /// \brief Construct a discrete interval.
    ///
    /// \param min miminum value of the interval
    /// \param max maxinum value of the interval
    /// \param step discretization step
    static discreteInterval_t makeDiscreteInterval (value_type min,
						    value_type max,
						    value_type step)
    {
      return discreteInterval_t (min, max, step);
    }

    /// \brief Construct a discrete interval.
    ///
    /// \param interval continuous interval
    /// \param step discretization step
    static discreteInterval_t makeDiscreteInterval (interval_t interval,
						    value_type step)
    {
      return discreteInterval_t (getLowerBound (interval),
				 getUpperBound (interval),
				 step);
    }

    /// \brief Get the lower bound of a discrete interval
    ///
    /// \param interval accessed discrete interval
    /// \return lower bound of the discrete interval
    static value_type getLowerBound (const discreteInterval_t& interval)
    {
      return boost::get<0> (interval);
    }

    /// \brief Get the upper bound of a discrete interval
    ///
    /// \param interval accessed discrete interval
    /// \return upper bound of the discrete interval
    static value_type getUpperBound (const discreteInterval_t& interval)
    {
      return boost::get<1> (interval);
    }

    /// \brief Get the upper step of a discrete interval
    ///
    /// \param interval accessed discrete interval
    /// \return upper step of the discrete interval
    static value_type getStep (const discreteInterval_t& interval)
    {
      return boost::get<2> (interval);
    }

    /// \brief Iterate on an interval
    ///
    /// Call the functor to each discretization point of the discrete
    /// interval.
    /// \param interval interval on which the method iterates
    /// \param functor unary function that will be applied
    /// \tparam F functor type (has to satisfy the STL unary function concept)
    template <typename F>
    static void foreach (const discreteInterval_t interval,
			 F functor)
    {
      const value_type delta =
	getUpperBound (interval) - getLowerBound (interval);
      assert (delta >= 0.);
      assert (getStep (interval) > 0.);

      value_type n = std::floor (delta / getStep (interval));

      for (value_type i = 0.; i <= n; i += 1.)
	{
	  value_type t =
	    getLowerBound (interval) + (value_type)i * getStep (interval);
	  if (t > getUpperBound (interval))
	    t = getUpperBound (interval);
	  assert (getLowerBound (interval) <= t
		  && t <= getUpperBound (interval));
	  functor (t);
	}
    }

    /// \brief Iterate on an interval
    ///
    /// Call the functor regularly n times on an interval.
    /// \param interval interval on which the method iterates
    /// \param n number of discretization points
    /// \param functor unary function that will be applied
    /// \tparam F functor type (has to satisfy the STL unary function concept)
    template <typename F>
    static void foreach (const interval_t interval,
			 const size_type n,
			 F functor)
    {
      const value_type delta =
	getUpperBound (interval) - getLowerBound (interval);
      assert (delta >= 0.);

      if (!n)
	return;

      for (size_type i = 0; i < n; ++i)
	{
	  value_type t =
	    getLowerBound (interval)
	    + (value_type)i * (delta / ((value_type)n - 1));
	  if (t > getUpperBound (interval))
	    t = getUpperBound (interval);
	  assert (getLowerBound (interval) <= t
		  && t <= getUpperBound (interval));
	  functor (t);
	}
    }

    /// \}

    /// \brief Check the given result size is valid.
    ///
    /// \param result result that will be checked
    /// \return true if valid, false if not
    bool isValidResult (const_result_ref result) const
    {
      return result.size () == outputSize ();
    }

    /// \brief Return the input size (i.e. argument's vector size).
    ///
    /// \return input size
    typename GenericFunction<T>::size_type inputSize () const
    {
      return inputSize_;
    }

    /// \brief Return the output size (i.e. result's vector size).
    ///
    /// \return input size
    typename GenericFunction<T>::size_type  outputSize () const
    {
      return outputSize_;
    }

    /// \brief Trivial destructor.
    virtual ~GenericFunction ();

    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param argument point at which the function will be evaluated
    /// \return computed result
    result_t operator () (const_argument_ref argument) const
    {
      result_t result (outputSize ());
      result.setZero ();
      (*this) (result, argument);
      return result;
    }

    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param result result will be stored in this vector
    /// \param argument point at which the function will be evaluated
    void operator () (result_ref result, const_argument_ref argument)
      const
    {
      LOG4CXX_TRACE
	(logger, "Evaluating function at point: " << argument);
      assert (argument.size () == inputSize ());
      assert (isValidResult (result));
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      Eigen::internal::set_is_malloc_allowed (false);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      this->impl_compute (result, argument);
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      Eigen::internal::set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      assert (isValidResult (result));
    }

    /// \brief Get function name.
    ///
    /// \return Function name.
    const std::string& getName () const
    {
      return name_;
    }

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize function arity
    /// \param outputSize result size
    /// \param name function's name
    /// \throw std::runtime_error
    GenericFunction (size_type inputSize,
                     size_type outputSize = 1,
                     std::string name = std::string ());


    /// \brief Function evaluation.
    ///
    /// Evaluate the function, has to be implemented in concrete
    /// classes.  \warning Do not call this function directly, call
    /// #operator()(result_ref, const_argument_ref) const
    /// instead.  \param result result will be stored in this vector
    /// \param argument point at which the function will be evaluated
    virtual void impl_compute (result_ref result, const_argument_ref argument)
      const = 0;

  private:
    /// \brief Problem dimension.
    size_type inputSize_;

    /// \brief Result dimension.
    size_type outputSize_;

    /// \brief Function name (for user-friendliness).
    std::string name_;

  protected:
    /// \brief Pointer to function logger (see log4cxx documentation).
    static log4cxx::LoggerPtr logger;
  };

  template <typename T>
  log4cxx::LoggerPtr GenericFunction<T>::logger
  (log4cxx::Logger::getLogger ("roboptim"));

  template <typename T>
  GenericFunction<T>::GenericFunction (size_type inputSize,
                                       size_type outputSize,
                                       std::string name)
    : inputSize_ (inputSize),
      outputSize_ (outputSize),
      name_ (name)
  {
    // Positive size is required.
    assert (inputSize > 0 && outputSize > 0);
  }

  template <typename T>
  GenericFunction<T>::~GenericFunction ()
  {
  }

  template <typename T>
  std::ostream&
  GenericFunction<T>::print (std::ostream& o) const
  {
    if (getName ().empty ())
      return o << "Function";

    std::stringstream ss;
    ss << std::endl;
    char fill = o.fill (' ');
    ss << std::setw ((int)indent (o))
       << ""
       << std::setfill (fill);
    std::string name = getName ();
    boost::algorithm::replace_all (name, "\n", ss.str ());
    return o << name << " (not differentiable)";
  }

  template <typename T>
  std::ostream&
  operator<< (std::ostream& o, const GenericFunction<T>& f)
  {
    return f.print (o);
  }

  /// \brief Trait specializing GenericFunction for Eigen dense matrices.
  template <>
  struct GenericFunctionTraits<EigenMatrixDense>
  {
    /// \brief Matrix storage order.
    static const int StorageOrder = Eigen::RowMajor;

    // For each type, we have:
    //  - type_t:         the type itself
    //  - type_ref:       reference to type object
    //  - const_type_ref: const reference to type object
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (matrix,
     Eigen::Matrix<double BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     StorageOrder>);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (vector,
     Eigen::Matrix<double BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     1>);

    typedef matrix_t::Index size_type;
    typedef matrix_t::Scalar value_type;

    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(result,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(argument,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(gradient,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(jacobian,matrix_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(hessian,matrix_t);
  };

  /// \brief Trait specializing GenericFunction for Eigen sparse matrices.
  template <>
  struct GenericFunctionTraits<EigenMatrixSparse>
  {
    /// \brief Matrix storage order.
    static const int StorageOrder = Eigen::RowMajor;

    // For each type, we have:
    //  - type_t:         the type itself
    //  - type_ref:       reference to type object
    //  - const_type_ref: const reference to type object
    ROBOPTIM_GENERATE_TYPEDEFS_REF
    (matrix,
     Eigen::SparseMatrix<double BOOST_PP_COMMA() StorageOrder>);

    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (vector,
     Eigen::Matrix<double BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     1>);

    typedef matrix_t::Index size_type;
    typedef matrix_t::Scalar value_type;

    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(result,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(argument,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_REF(gradient,Eigen::SparseVector<double>);
    ROBOPTIM_GENERATE_TYPEDEFS_REF(jacobian,matrix_t);
    ROBOPTIM_GENERATE_TYPEDEFS_REF(hessian,matrix_t);
  };

  /// @}


  /// \brief Override operator<< to handle function display.
  ///
  /// \param o output stream used for display
  /// \param f function to be displayed
  /// \return output stream
  template <typename T>
  std::ostream& operator<< (std::ostream& o, const GenericFunction<T>& f);
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_HH
