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

./InputTest >/dev/null &
pid="$!"
wid="$(xdotool search --sync --onlyvisible --pid "$pid")"

xdotool key --window "$wid" k
xdotool key --window "$wid" space
xdotool key --window "$wid" dollar

#save mouse position (X, Y and SCREEN)
eval "$(xdotool getmouselocation --shell)"

xdotool mousemove --window "$wid" 0 0 click 3
xdotool mousemove --window "$wid" 0 0 click 2
xdotool mousemove --window "$wid" 0 0 click 1

#restore mouse position
xdotool mousemove --screen $SCREEN $X $Y

