// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
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

#ifndef ROBOPTIM_CORE_VISUALIZATION_UTIL_HH
# define ROBOPTIM_CORE_VISUALIZATION_UTIL_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>
# include <roboptim/core/function.hh>

# include <vector>

# define EIGEN_YES_I_KNOW_SPARE_MODULE_IS_NOT_STABLE_YET
# include <Eigen/Core>
# include <Eigen/Sparse>

# include <roboptim/core/function.hh>
# include <roboptim/core/visualization/fwd.hh>

namespace roboptim
{
  namespace visualization
  {
    /// \addtogroup roboptim_visualization
    /// @{

    /// \brief Normalize floating point number output.
    /// Normalization is done on all displayed floating
    /// point numbers to get a consistent output.
    double normalize (const double& x);

    /// \brief Apply normalize to each element of a matrix.
    Eigen::MatrixXd
    normalize (const Eigen::MatrixXd& x);


    /// \brief Apply normalize to each element of a container.
    template <typename T>
    T normalize (const T& x);

    inline double
    normalize (const double& x)
    {
      if (std::fabs (x) < 1e-8)
	return 0.;
      return x;
    }

    inline Eigen::MatrixXd
    normalize (const Eigen::MatrixXd& x)
    {
      Eigen::MatrixXd res (x.rows (), x.cols ());
      for (Function::size_type i = 0; i < x.rows (); ++i)
	for (Function::size_type j = 0; j < x.cols (); ++j)
	  res (i, j) = normalize (x (i, j));
      return res;
    }

    template <typename T>
    T
    normalize (const T& x)
    {
      T res (x.size ());
      for (Function::size_type i = 0; i < x.size (); ++i)
	res[i] = normalize (x[i]);
      return res;
    }

  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_UTIL_HH
