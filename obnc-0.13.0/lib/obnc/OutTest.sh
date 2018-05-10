#!/bin/sh

# Copyright (C) 2017, 2018 Karl Landstrom <karl@miasap.se>
#
# This file is part of OBNC.
#
# OBNC is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# OBNC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with OBNC.  If not, see <http://www.gnu.org/licenses/>.

set -e

expectedOutput="a
abc
-2147483648
-1
 -1
0
1
37
2147483647
-1.000000E+00
0.000000E+00
  0.000000E+00
1.000000E+00
3.700000E+01
3.700000E-01"

./OutTest | while IFS= read line; do
	expectedLine="$(echo "$expectedOutput" | head -n 1)"
	if [ "$line" != "$expectedLine" ]; then
		echo "test failed: output: \"$line\", expected output: \"$expectedLine\"" >&2
		exit 1
	fi
	expectedOutput="$(echo "$expectedOutput" | tail -n +2)"
done
