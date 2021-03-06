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

#ifndef FL_FUNCTION2D_POLYMONIALAPPROXIMATIO_H
#define FL_FUNCTION2D_POLYMONIALAPPROXIMATIO_H
#include "iapproximation.h"

namespace fl{
    namespace Function2D{
        class PolymonialApproximation : public IApproximation
        {
            public:    
                PolymonialApproximation( int m, const Function2D::FunctionDiscrete::DomainRange & _range ) ; 
                virtual FunctionContinous* approximate() const ;
            private:
				PolymonialApproximation ( const PolymonialApproximation & rhs );
				PolymonialApproximation & operator=( const PolymonialApproximation & rhs );
                int _m ;
        };
    }
}

#endif // FL_FUNCTION2D_POLYMONIALAPPROXIMATIO_H
