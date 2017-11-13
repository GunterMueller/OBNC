#!/bin/sh

# Copyright 2017 Karl Landstrom <karl@miasap.se>
#
# This file is part of obnc-libstd.
#
# obnc-libstd is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# obnc-libstd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with obnc-libstd.  If not, see <http://www.gnu.org/licenses/>.

set -e

expectedOutput="a
abc
-1
 -1
0
1
37
-1.000000E+00
0.000000E+00
  0.000000E+00
1.000000E+00
3.700000E+01
3.700000E-01"

./ErrTest 2>&1 >/dev/null | while IFS= read line; do
	expectedLine="$(echo "$expectedOutput" | head -n 1)"
	if [ "$line" != "$expectedLine" ]; then
		echo "test failed: output: \"$line\", expected output: \"$expectedLine\"" >&2
		exit 1
	fi
	expectedOutput="$(echo "$expectedOutput" | tail -n +2)"
done
