/*      ----------------------------------------------------------------------------
        FILE:                   const.h
        PROJECT:                pinguino - http://www.pinguino.cc/
        PURPOSE:                definition of constants
        PROGRAMERS:             regis blanchot <rblanchot@gmail.com>
        FIRST RELEASE:          2010
        ----------------------------------------------------------------------------
        CHANGELOG:
        Sept 2011 - Marcus Fazzi - UBW32 and Emperor board revised
        17 mar. 2012 [hgmvanbeek@gmail.com] - added support for PIC32_PINGUINO_MICRO
        ----------------------------------------------------------------------------
        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with this library; if not, write to the Free Software
        Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
        --------------------------------------------------------------------------*/

#ifndef __CONST_H
    #define __CONST_H
    // Tell the compiler the next function must be MIPS32.
    // Typically needed by all functions such as interrupt handlers
    // which cannot be MIPS16 functions when -mips16 option is enabled.
    // Usage : void MIPS32 myfunction(...)
    #define MIPS32          __attribute__((noinline,nomips16))


    #ifndef OUTPUT
        #define OUTPUT      0
    #endif

#endif /* __CONST_H */
