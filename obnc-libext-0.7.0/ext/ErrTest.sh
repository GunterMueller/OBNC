#!/bin/sh

# Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>
#
# This file is part of obnc-libext.
#
# obnc-libext is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# obnc-libext is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with obnc-libext.  If not, see <http://www.gnu.org/licenses/>.

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
 00000000
 00000001
 DEADBEEF
-1.000000E+00
0.000000E+00
  0.000000E+00
1.000000E+00
3.700000E+01
3.700000E-01"

expectedOutput1="a
abc
-2147483648
-1
 -1
0
1
37
2147483647
 00000000
 00000001
 DEADBEEF
-1.000000E+000
0.000000E+000
 0.000000E+000
1.000000E+000
3.700000E+001
3.700000E-001"

IFS='
'
i=1
for line in $(./ErrTest 2>&1); do
	expectedLine="$(echo "$expectedOutput" | awk -v i=$i 'NR == i')"
	expectedLine1="$(echo "$expectedOutput1" | awk -v i=$i 'NR == i')"
	if [ "$line" != "$expectedLine" ] && [ "$line" != "$expectedLine1" ]; then
		echo "test failed: output: \"$line\", expected output: \"$expectedLine\" or \"$expectedLine1\"" >&2
		exit 1
	fi
	i="$(expr "$i" + 1)"
done
