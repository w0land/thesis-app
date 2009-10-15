/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef CONVOLUTIONOPERATION_H
#define CONVOLUTIONOPERATION_H

#include "ioperation.h"
#include <QDebug>
template<class T>
QDebug operator<<(QDebug dbg, const std::vector<T> & vec ) 
{
    dbg.nospace() << "[" ; 
    foreach ( T type, vec ) {
        dbg.nospace() << type << ", " ; 
    }
    dbg.nospace() << "]" ; 
    return dbg.space();
}

class ConvolutionOperation : public IOperation
{
    public:
        ConvolutionOperation( ) ;
        virtual ~ConvolutionOperation() {}
        virtual fl::FunctionBase* calculate() = 0 ; 
        virtual void addFunction ( fl::FunctionBase* pPtr ) ;
};

#endif // CONVOLUTIONOPERATION_H