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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>
# include <roboptim/core/function.hh>

# include <vector>

# define EIGEN_YES_I_KNOW_SPARE_MODULE_IS_NOT_STABLE_YET
# include <Eigen/Core>
# include <Eigen/Sparse>

# include <roboptim/core/function.hh>
# include <roboptim/core/visualization/fwd.hh>
# include <roboptim/core/visualization/gnuplot-commands.hh>

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


    /// \brief Gnuplot script
    ///
    /// This class gathers Gnuplot commands
    /// to build a complete Gnuplot script.
    /// Gnuplot commands can be inserted through
    /// this object through the \c << operator
    /// and this object can be put into an output stream
    /// using the \c << operator.
    class ROBOPTIM_DLLAPI Gnuplot
    {
    public:
      ~Gnuplot () throw ();

      /// \brief Instanciate a Gnuplot without setting a term.
      /// \param with_header whether to print the header or not
      /// \return Gnuplot instance
      static Gnuplot make_gnuplot (bool with_header = true) throw ()
      {
	return Gnuplot (with_header);
      }

      /// \brief Instanciate a Gnuplot suitable for interactive use.
      ///
      /// This initializes a persistent Gnuplot instance which are
      /// suitable for user interaction.
      /// \param with_header whether to print the header or not
      /// \return Gnuplot instance
      static Gnuplot make_interactive_gnuplot (bool with_header = true) throw ()
      {
	Gnuplot gp(with_header);
	gp.push_command (gnuplot::set ("terminal", "wxt persist"));
	return gp;
      }

      /// \brief Add a new Gnuplot command to the script.
      /// \param cmd command that will be pushed
      void push_command (gnuplot::Command cmd) throw ();


      /// \brief Display the Gnuplot script on the specified output stream.
      ///
      /// \param o output stream used for display
      /// \return output stream
      std::ostream& print (std::ostream&) const throw ();

      Gnuplot& operator << (gnuplot::Command) throw ();

    protected:
      /// \brief Default constructor can not be called directly.
      ///
      /// Use of the named constructor (see static methods) to
      /// instantiate this class.
      explicit Gnuplot (bool with_header = true) throw ();
    private:
      /// \brief Vector of commands.
      std::vector<gnuplot::Command> commands_;
    };

    /// Example shows simple Gnuplot visualization.
    /// \example visualization-gnuplot-simple.cc

    /// Example shows function display with Gnuplot.
    /// \example visualization-gnuplot-function.cc

    /// \brief Override operator<< to handle Gnuplot command insertion.
    ///
    /// \tparam Gnuplot command type
    /// \param gp Gnuplot script that will receive the new command
    /// \param t new Gnuplot command.
    /// \return modified Gnuplot script
    template <typename T>
    Gnuplot& operator<< (Gnuplot& gp, T t)
    {
      gp.push_command (t ());
      return gp;
    }

    /// @}

    /// \brief Override operator<< to handle Gnuplot script display.
    ///
    /// \param o output stream used for display
    /// \param gp Gnuplot script to be displayed
    /// \return output stream
    ROBOPTIM_DLLAPI std::ostream& operator<< (std::ostream&, const Gnuplot& gp);
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
