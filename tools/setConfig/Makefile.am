# Copyright (C) 2013 Calpont Corp.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2 of
# the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

# $Id: Makefile.am 333 2009-04-03 20:35:04Z rdempsey $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = $(idb_cppflags)
AM_CFLAGS = $(idb_cflags)
AM_CXXFLAGS = $(idb_cxxflags)
AM_LDFLAGS = $(idb_ldflags)
bin_PROGRAMS = setConfig
setConfig_SOURCES = main.cpp
setConfig_CPPFLAGS = $(idb_common_includes) $(AM_CPPFLAGS)
setConfig_LDFLAGS = $(idb_common_ldflags) $(idb_common_libs) $(idb_oam_libs) $(idb_exec_libs) $(idb_brm_libs) $(AM_LDFLAGS)

dist_bin_SCRIPTS = configxml.sh

test:

coverage:

leakcheck:

docs:

bootstrap: install-data-am

