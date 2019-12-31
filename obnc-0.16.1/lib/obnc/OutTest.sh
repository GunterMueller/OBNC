#!/bin/sh

# Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>
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
-32768
-1
 -1
0
1
37
32767
 0000
 0001
 BEEF
-1.000000E+00
0.000000E+00
  0.000000E+00
1.000000E+00
3.700000E+01
3.700000E-01"

expectedOutput1="a
abc
-32768
-1
 -1
0
1
37
32767
 00000000
 00000001
 0000BEEF
-1.000000E+000
0.000000E+000
 0.000000E+000
1.000000E+000
3.700000E+001
3.700000E-001"

expectedOutput2="a
abc
-32768
-1
 -1
0
1
37
32767
 0000000000000000
 0000000000000001
 000000000000BEEF
-1.000000E+000
0.000000E+000
 0.000000E+000
1.000000E+000
3.700000E+001
3.700000E-001"

IFS='
'
i=1
for line in $(./OutTest); do
	expectedLine="$(echo "$expectedOutput" | head -n "$i" | tail -n 1)"
	expectedLine1="$(echo "$expectedOutput1" | head -n "$i" | tail -n 1)"
	expectedLine2="$(echo "$expectedOutput2" | head -n "$i" | tail -n 1)"
	if [ "$line" != "$expectedLine" ] && [ "$line" != "$expectedLine1" ] && [ "$line" != "$expectedLine2" ]; then
		echo "test failed: output: \"$line\", expected output: \"$expectedLine\" or \"$expectedLine1\"" >&2
		exit 1
	fi
	i="$(expr "$i" + 1)"
done
