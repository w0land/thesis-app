/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef FUNCTIONDISCRETE_H
#define FUNCTIONDISCRETE_H
#include <vector>

#include "function2dbase.h"
#include "function3dbase.h"
#include "functioncontinous.h"

#include <boost/tuple/tuple.hpp>

namespace fl{
    namespace Function2D{

        class FunctionDiscrete : public fl::Function2D::Function2DBase
        {
            public:
                enum ApproximationType{
                    eWielAlg=0
                };
                typedef std::pair<double,double> Point ; 
                typedef std::vector<Point> DomainRange ;
                typedef DomainRange::iterator DomainRangeIterator  ;
                
                FunctionDiscrete(const std::vector< double >& _x, const std::vector< double >& _y, const std::string& _functionName = "unnamed");
                virtual ~FunctionDiscrete();                
                virtual double eval( double point, bool * pCorrect ) const;
                
                virtual double integrate(double start, double stop,double dStep ) const ;
                
                virtual double max() const ;
                virtual double min() const ;
                
                virtual fl::Function2D::FunctionContinous * approximate() const ; 
                virtual double xStartWhereIntegratingMakesSense() const ;
                virtual double xStopWhereIntegratingMakesSense() const ;
                
                double xMin() const ; 
                double xMax() const ;
				
				const DomainRange & range() const { return m_xy;}

            private:
                mutable DomainRange m_xy ; 
        };
    }
    namespace Function3D{

        class FunctionDiscrete : public fl::Function3D::Function3DBase 
        {
            typedef boost::tuple<double,double,double> EntityType ; 
            typedef std::vector<EntityType> DomainRange ;
            typedef DomainRange::iterator DomainRangeIterator ;

			public:
				FunctionDiscrete ( const std::vector<double>& _x, const std::vector<double> & _y, const std::vector<double> & _z ,const std::string & _functionName );
				virtual ~FunctionDiscrete() {} 
				virtual double max() const ; 
				virtual double min() const ; 
				virtual double eval(double x,double y ) ; 
			private:
				mutable DomainRange m_xyz ; 

        };
    }
}

#endif // FUNCTIONDISCRETE_H
