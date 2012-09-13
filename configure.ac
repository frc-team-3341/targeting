#    This file is part of FRC Team 3341 Aimer.
#
#    FRC Team 3341 Aimer is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    FRC Team 3341 Aimer is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with FRC Team 3341 Aimer.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.65])
AC_INIT([FRC Team 3341 Aimer], [1.0], [software@wvrobotics.com], [frc-team-3341-aimer], [http://wvrobotics.com/])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE([dist-xz])

AC_LANG([C++])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

BOOST_REQUIRE([1.34.1])
BOOST_ASIO

AC_CHECK_HEADERS([sys/socket.h netinet/in.h netdb.h getopt.h opencv2/core/core.hpp opencv2/imgproc/imgproc.hpp opencv2/highgui/highgui.hpp])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
