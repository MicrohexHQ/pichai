/* Test code for pichai.
This file is part of pichai.

pichai is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

pichai is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with pichai; see the file LICENSE.  If not see
<http://www.gnu.org/licenses/>.  */
#include "platform.h"
int main(){
    platformBasedDisplayBegin();
    platformBasedDisplayBackground(0x001F);
    platformBasedSerialWrite('P');
    return 0;
}
