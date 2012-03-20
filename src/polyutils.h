#ifndef POLYUTILS_H_
#define POLYUTILS_H_

/* PolySet Utility Functions */
#include "function.h"
#include "expression.h"
#include "context.h"
#include "builtin.h"
#include <sstream>
#include <ctime>
#include "mathc99.h"
#include <algorithm>
#include "stl-utils.h"
#include "printutils.h"
#include <Eigen/Core>
#include "handle_dep.h"
#include <Magick++.h>
#include <string>
#include <BGLMesh3d.hh>

using namespace Magick;



PolySet * readPolySetFromImage( const Filename, bool, double, int );

PolySet * readPolySetFromRiseGroundBase( const Filename, bool, double, int );

BGL::Mesh3d * readMesh3dFromSTL( const Filename );

PolySet * createPolySetFromMesh3d( BGL::Mesh3d * );

PolySet * readPolySetFromSTL( const Filename , int );

PolySet * readPolySetFromDXF( const Filename , std::string , double , double , double , double , double , double , int );

#endif
