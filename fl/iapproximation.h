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

#ifndef IAPPROXIMATION_H
#define IAPPROXIMATION_H

#include "functionbase.h"
#include "functioncontinous.h"
#include "functiondiscrete.h"
#include "functionMixed.h"

#include <vector>

namespace fl{
    namespace Function2D{
        class IApproximation
        {
            public:
                IApproximation(const Function2D::FunctionDiscrete::DomainRange & _range) ; 
                virtual ~IApproximation() ; 
                virtual Function2D::FunctionContinous * approximate() const  = 0 ; 
            protected:
                Function2D::FunctionDiscrete::DomainRange m_range ;
		public:
			static fl::Function2D::FunctionMixed * cropFunction (fl::Function2D::Function2DBase * pFunc, double xstart, double xstop);
        };
    }
}

#endif // IAPPROXIMATION_H
