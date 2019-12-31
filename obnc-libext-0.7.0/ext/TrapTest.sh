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

output="$(./TrapTest)"
if [ "$output" != hello ]; then
	echo "$0 failed: output: \"$output\", expected output: \"hello\"" >&2
	exit 1
fi
