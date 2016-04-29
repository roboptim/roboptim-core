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
# include <iostream>
# include <string>
# include <vector>

# include <boost/static_assert.hpp>
# include <boost/tuple/tuple.hpp>
# include <boost/preprocessor/punctuation/comma.hpp>

# define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET

# include <roboptim/core/alloc.hh>
# include <Eigen/Core>
# include <Eigen/Dense>
# include <Eigen/Sparse>

# include <log4cxx/logger.h>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/indent.hh>
# include <roboptim/core/portability.hh>

# include <roboptim/core/detail/utility.hh>

// Generate Eigen::Ref types for a given data type.
# define ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(NAME,TYPE)	\
  typedef TYPE NAME##_t;					\
  typedef Eigen::Ref<NAME##_t> NAME##_ref;			\
  typedef const Eigen::Ref<const NAME##_t>& const_##NAME##_ref

// Generate Eigen::Ref types for a given data type. Specialized version for row
// vectors with the proper stride, no matter what the storage order is.
# define ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF_VEC(NAME,TYPE)	\
  typedef TYPE NAME##_t;					\
  typedef Eigen::Ref<NAME##_t, 0, detail::row_vector_stride<StorageOrder>::type> NAME##_ref;			\
  typedef const Eigen::Ref<const NAME##_t, 0, detail::row_vector_stride<StorageOrder>::type>& const_##NAME##_ref

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

# define ROBOPTIM_GENERATE_TRAITS_REFS_T(NAME,TRAITS)				\
  typedef typename GenericFunctionTraits<TRAITS>::NAME##_t NAME##_t;		\
  typedef typename GenericFunctionTraits<TRAITS>::NAME##_ref NAME##_ref;	\
  typedef typename GenericFunctionTraits<TRAITS>::const_##NAME##_ref const_##NAME##_ref

# define ROBOPTIM_FUNCTION_FWD_TYPEDEFS(PARENT)	\
  typedef PARENT parent_t;			\
  typedef parent_t::value_type value_type;	\
  typedef parent_t::size_type size_type;	\
  typedef parent_t::names_t names_t;            \
  ROBOPTIM_GENERATE_FWD_REFS(argument);		\
  ROBOPTIM_GENERATE_FWD_REFS(result);		\
  ROBOPTIM_GENERATE_FWD_REFS(vector);		\
  ROBOPTIM_GENERATE_FWD_REFS(rowVector);		\
  ROBOPTIM_GENERATE_FWD_REFS(matrix)

# define ROBOPTIM_FUNCTION_FWD_TYPEDEFS_(PARENT)	\
  typedef PARENT parent_t;				\
  typedef typename parent_t::value_type value_type;	\
  typedef typename parent_t::size_type size_type;	\
  typedef typename parent_t::names_t names_t;           \
  ROBOPTIM_GENERATE_FWD_REFS_(argument);		\
  ROBOPTIM_GENERATE_FWD_REFS_(result);			\
  ROBOPTIM_GENERATE_FWD_REFS_(vector);			\
  ROBOPTIM_GENERATE_FWD_REFS_(rowVector);			\
  ROBOPTIM_GENERATE_FWD_REFS_(matrix)

# define ROBOPTIM_DEFINE_FLAG_TYPE()		\
  typedef unsigned int flag_t

# define ROBOPTIM_ADD_FLAG(FLAG)				\
  ROBOPTIM_DEFINE_FLAG_TYPE();					\
 /* /// \brief Get the type-checking flag*/			\
 virtual flag_t getFlags() const				\
 {								\
   return flags;						\
 }								\
 /* /// \brief Flag representing the Roboptim Function type*/	\
 static const flag_t flags = parent_t::flags|FLAG


// Default storage order = ColMajor
# ifndef ROBOPTIM_STORAGE_ORDER
#  define ROBOPTIM_STORAGE_ORDER ColMajor
# endif //! ROBOPTIM_STORAGE_ORDER

BOOST_STATIC_ASSERT_MSG (Eigen::ROBOPTIM_STORAGE_ORDER == Eigen::ColMajor \
                         || Eigen::ROBOPTIM_STORAGE_ORDER == Eigen::RowMajor, \
                         "Wrong storage order provided by ROBOPTIM_STORAGE_ORDER.");

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Default matrix storage order.
  static const int StorageOrder = Eigen::ROBOPTIM_STORAGE_ORDER;

  /// \brief Bit flags describing the type of function.
  enum FunctionFlag
    {
      ROBOPTIM_IS_FUNCTION              = 1 << 0,
      ROBOPTIM_IS_DIFFERENTIABLE        = 1 << 1,
      ROBOPTIM_IS_TWICE_DIFFERENTIABLE  = 1 << 2,
      ROBOPTIM_IS_QUADRATIC             = 1 << 3,
      ROBOPTIM_IS_NUMERIC_QUADRATIC     = 1 << 4,
      ROBOPTIM_IS_LINEAR                = 1 << 5,
      ROBOPTIM_IS_NUMERIC_LINEAR        = 1 << 6,
      ROBOPTIM_IS_POLYNOMIAL            = 1 << 7,
      ROBOPTIM_IS_CONSTANT              = 1 << 8
    };

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
  class ROBOPTIM_CORE_DLLAPI GenericFunction
  {
  public:
    ROBOPTIM_DEFINE_FLAG_TYPE();

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

    /// \brief Basic (column) vector type.
    ///
    /// This basic vector type is used each time a vector of values
    /// is required.
    ///
    /// \attention It is good practice in RobOptim to rely on this type
    /// when a vector of values is needed instead of relying on a particular
    /// implementation.
    ROBOPTIM_GENERATE_TRAITS_REFS_(vector);

    /// \brief Row vector type.
    ///
    /// This basic vector type is used each time a row vector of values
    /// is required (e.g. gradients).
    ROBOPTIM_GENERATE_TRAITS_REFS_(rowVector);

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
    static value_type epsilon ();

    /// \brief Get the value that symbolizes positive infinity.
    /// \return representation of positive infinity in the value type
    static value_type infinity ();

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
    static interval_t makeInterval (value_type l, value_type u);

    /// \brief Construct an infinite interval.
    /// \return interval representing \f$[-\infty, +\infty]\f$
    static interval_t makeInfiniteInterval ();

    /// \brief Construct an interval from a lower bound.
    /// \param l lower bound
    /// \return interval representing \f$[l, +\infty]\f$
    static interval_t makeLowerInterval (value_type l);

    /// \brief Construct an interval from an upper bound.
    /// \param u upper bound
    /// \return interval representing \f$[-\infty, u]\f$
    static interval_t makeUpperInterval (value_type u);

    /// \brief Get the lower bound of an interval
    /// \param interval accessed interval
    /// \return lower bound of the interval
    static value_type getLowerBound (const interval_t& interval);

    /// \brief Get the upper bound of an interval
    /// \param interval accessed interval
    /// \return upper bound of the interval
    static value_type getUpperBound (const interval_t& interval);

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
						    value_type step);

    /// \brief Construct a discrete interval.
    ///
    /// \param interval continuous interval
    /// \param step discretization step
    static discreteInterval_t makeDiscreteInterval (interval_t interval,
						    value_type step);

    /// \brief Get the lower bound of a discrete interval
    ///
    /// \param interval accessed discrete interval
    /// \return lower bound of the discrete interval
    static value_type getLowerBound (const discreteInterval_t& interval);

    /// \brief Get the upper bound of a discrete interval
    ///
    /// \param interval accessed discrete interval
    /// \return upper bound of the discrete interval
    static value_type getUpperBound (const discreteInterval_t& interval);

    /// \brief Get the upper step of a discrete interval
    ///
    /// \param interval accessed discrete interval
    /// \return upper step of the discrete interval
    static value_type getStep (const discreteInterval_t& interval);

    /// \brief Cast function to ExpectedType.
    /// \tparam ExpectedType type we want to cast the function into.
    /// \param check check that the cast is valid, and throws if it is not.
    template <class ExpectedType>
    ExpectedType* castInto (bool check = false);

    /// \brief Cast function to ExpectedType (const).
    /// \tparam ExpectedType type we want to cast the function into.
    /// \param check check that the cast is valid, and throws if it is not.
    template <class ExpectedType>
    const ExpectedType* castInto (bool check = false) const;

    /// \brief Fonction type checking.
    ///
    /// \tparam ExpectedType type the function could be compatible with
    template <class ExpectedType>
    bool asType() const;

    /// \brief Iterate on an interval
    ///
    /// Call the functor to each discretization point of the discrete
    /// interval.
    /// \param interval interval on which the method iterates
    /// \param functor unary function that will be applied
    /// \tparam F functor type (has to satisfy the STL unary function concept)
    template <typename F>
    static void foreach (const discreteInterval_t interval,
			 F functor);

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
			 F functor);

    /// \}

    /// \brief Check the given result size is valid.
    ///
    /// \param result result that will be checked
    /// \return true if valid, false if not
    bool isValidResult (const_result_ref result) const;

    /// \brief Return the input size (i.e. argument's vector size).
    ///
    /// \return input size
    typename GenericFunction<T>::size_type inputSize () const;

    /// \brief Return the output size (i.e. result's vector size).
    ///
    /// \return output size
    typename GenericFunction<T>::size_type outputSize () const;

    /// \brief Trivial destructor.
    virtual ~GenericFunction ();

    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param argument point at which the function will be evaluated
    /// \return computed result
    result_t operator () (const_argument_ref argument) const;

    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param result result will be stored in this vector
    /// \param argument point at which the function will be evaluated
    void operator () (result_ref result, const_argument_ref argument)
      const;

    /// \brief Get function name.
    ///
    /// \return Function name.
    const std::string& getName () const;

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

    /// \brief Flag representing the Roboptim Function type
    static const flag_t flags = ROBOPTIM_IS_FUNCTION;

    /// \brief Get the type-checking flag
    virtual flag_t getFlags () const;

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

  /// \brief Trait specializing GenericFunction for Eigen dense matrices.
  template <>
  struct ROBOPTIM_CORE_DLLAPI GenericFunctionTraits<EigenMatrixDense>
  {
    /// \brief Matrix storage order.
    static const int StorageOrder = roboptim::StorageOrder;

    /// \brief Value type.
    typedef double value_type;

    // For each type, we have:
    //  - type_t:         the type itself
    //  - type_ref:       reference to type object
    //  - const_type_ref: const reference to type object

    // Matrix types
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (matrix,
     Eigen::Matrix<value_type BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     StorageOrder>);

    // Vector types
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (vector,
     Eigen::Matrix<value_type BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     1>);

    // Row vector types
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (rowVector,
     Eigen::Matrix<value_type BOOST_PP_COMMA()
     1 BOOST_PP_COMMA()
     Eigen::Dynamic>);

    typedef matrix_t::Index size_type;

    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(result,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(argument,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF_VEC(gradient,rowVector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(jacobian,matrix_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(hessian,matrix_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(derivative,vector_t);
  };

  /// \brief Trait specializing GenericFunction for Eigen sparse matrices.
  template <>
  struct ROBOPTIM_CORE_DLLAPI GenericFunctionTraits<EigenMatrixSparse>
  {
    /// \brief Matrix storage order.
    static const int StorageOrder = roboptim::StorageOrder;

    /// \brief Value type.
    typedef double value_type;

    // For each type, we have:
    //  - type_t:         the type itself
    //  - type_ref:       reference to type object
    //  - const_type_ref: const reference to type object

    // Matrix types
    ROBOPTIM_GENERATE_TYPEDEFS_REF
    (matrix,
     Eigen::SparseMatrix<value_type BOOST_PP_COMMA() StorageOrder>);

    // Vector types
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (vector,
     Eigen::Matrix<value_type BOOST_PP_COMMA()
     Eigen::Dynamic BOOST_PP_COMMA()
     1>);

    // Row vector types
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF
    (rowVector,
     Eigen::Matrix<value_type BOOST_PP_COMMA()
     1 BOOST_PP_COMMA()
     Eigen::Dynamic>);

    typedef matrix_t::Index size_type;

    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(result,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(argument,vector_t);
    ROBOPTIM_GENERATE_TYPEDEFS_REF(gradient,Eigen::SparseVector<value_type \
                                   BOOST_PP_COMMA() Eigen::RowMajor>);
    ROBOPTIM_GENERATE_TYPEDEFS_REF(jacobian,matrix_t);
    ROBOPTIM_GENERATE_TYPEDEFS_REF(hessian,matrix_t);
    ROBOPTIM_GENERATE_TYPEDEFS_EIGEN_REF(derivative,vector_t);
  };

  /// @}


  /// \brief Override operator<< to handle function display.
  ///
  /// \param o output stream used for display
  /// \param f function to be displayed
  /// \return output stream
  template <typename T>
  std::ostream& operator<< (std::ostream& o, const GenericFunction<T>& f);

  /// \brief Cast a shared_ptr of function to a different function type.
  /// This throws if the cast is not possible.
  ///
  /// \tparam U type of the origin function.
  /// \tparam V new function type.
  /// \param u input shared_ptr to a function.
  /// \return shared_ptr cast to the appropriate type.
  template <typename U, typename V>
  boost::shared_ptr<V> castInto (boost::shared_ptr<U>& u);

  /// \brief Cast a shared_ptr of function to a different function type.
  /// This throws if the cast is not possible. Const version.
  ///
  /// \tparam U type of the origin function.
  /// \tparam V new function type.
  /// \param u input shared_ptr to a function.
  /// \return shared_ptr cast to the appropriate type.
  template <typename U, typename V>
  const boost::shared_ptr<V> castInto (const boost::shared_ptr<U>& u);
} // end of namespace roboptim

# include <roboptim/core/function.hxx>

#endif //! ROBOPTIM_CORE_FUNCTION_HH
