/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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

using namespace Magick;

// from import.cc
#include "importnode.h"

#include "module.h"
#include "polyset.h"
#include "context.h"
#include "builtin.h"
#include "dxfdata.h"
#include "dxftess.h"
#include "printutils.h"
#include "handle_dep.h" // handle_dep()

#ifdef ENABLE_CGAL
#include "cgalutils.h"
#endif

#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
#include <boost/assign/std/vector.hpp>
using namespace boost::assign; // bring 'operator+=()' into scope
#include "boosty.h"


PolySet * readPolySetFromImage( const Filename filename, bool center=false, double scale=1.0, int convexity=2 )
{
    handle_dep(filename);
    PolySet *p = new PolySet();
    p->convexity = convexity;

    int lines = 0, columns = 0;
    boost::unordered_map<std::pair<int,int>,double> data;
    // boost::unordered_map<std::pair<int,int>,double> data_alpha;
    double min_val = 0;

    bool isImage;
    Image image;
    Blob blob;
    try {
        image.read( filename );
        PRINTB("read: loaded image '%s'",filename );
        isImage=true;
    } catch( Exception &error_ ) {
        PRINTB("read: %s",error_.what() );
        isImage=false;
    }

    if(isImage) {
        PRINTB("read: image '%s'; large images may crash OpenSCAD; use scale as needed.",filename);
        lines = image.baseRows();
        columns = image.baseColumns();
        PRINTB("       : lines = %d",lines);
        PRINTB("       : columns = %d",columns);
        PRINTB("       : scale =  %d",scale);
        image.scale(Geometry(scale*columns,scale*lines));
        image.write( &blob );
        image.read(blob);
        lines = image.baseRows();
        columns = image.baseColumns();
        PRINTB("       : lines (scaled) = %d",lines);
        PRINTB("       : columns (scaled) = %d",columns);
        // Gray Scale pixel shade range is 0.0 - 1.0
        ColorGray thisPixel;
        for( int ix =0; ix<columns; ix++) {
            for( int jy=0;jy<lines;jy++) {
                thisPixel=image.pixelColor(ix,jy);
                double v=thisPixel.shade();
                // double a=thisPixel.alpha();
                data[std::make_pair(lines-jy-1,ix)]=v;
                // data_alpha[std::make_pair(lines-jy-1,ix)]=a;
                min_val = std::min(v-0.01, min_val);
            }
        }
    } else {
        return NULL;
    }

    double ox = center ? -(columns-1)/2.0 : 0;
    double oy = center ? -(lines-1)/2.0 : 0;

    for (int i = 1; i < lines; i++)
    for (int j = 1; j < columns; j++)
    {
        double v1 = data[std::make_pair(i-1, j-1)];
        double v2 = data[std::make_pair(i-1, j)];
        double v3 = data[std::make_pair(i, j-1)];
        double v4 = data[std::make_pair(i, j)];
        double vx = (v1 + v2 + v3 + v4) / 4;

        p->append_poly();
        p->append_vertex(ox + j-1, oy + i-1, v1);
        p->append_vertex(ox + j, oy + i-1, v2);
        p->append_vertex(ox + j-0.5, oy + i-0.5, vx);

        p->append_poly();
        p->append_vertex(ox + j, oy + i-1, v2);
        p->append_vertex(ox + j, oy + i, v4);
        p->append_vertex(ox + j-0.5, oy + i-0.5, vx);

        p->append_poly();
        p->append_vertex(ox + j, oy + i, v4);
        p->append_vertex(ox + j-1, oy + i, v3);
        p->append_vertex(ox + j-0.5, oy + i-0.5, vx);

        p->append_poly();
        p->append_vertex(ox + j-1, oy + i, v3);
        p->append_vertex(ox + j-1, oy + i-1, v1);
        p->append_vertex(ox + j-0.5, oy + i-0.5, vx);
    }
    for (int i = 1; i < lines; i++)
    {
        p->append_poly();
        p->append_vertex(ox + 0, oy + i-1, min_val);
        p->append_vertex(ox + 0, oy + i-1, data[std::make_pair(i-1, 0)]);
        p->append_vertex(ox + 0, oy + i, data[std::make_pair(i, 0)]);
        p->append_vertex(ox + 0, oy + i, min_val);

        p->append_poly();
        p->insert_vertex(ox + columns-1, oy + i-1, min_val);
        p->insert_vertex(ox + columns-1, oy + i-1, data[std::make_pair(i-1, columns-1)]);
        p->insert_vertex(ox + columns-1, oy + i, data[std::make_pair(i, columns-1)]);
        p->insert_vertex(ox + columns-1, oy + i, min_val);
    }

    for (int i = 1; i < columns; i++)
    {
        p->append_poly();
        p->insert_vertex(ox + i-1, oy + 0, min_val);
        p->insert_vertex(ox + i-1, oy + 0, data[std::make_pair(0, i-1)]);
        p->insert_vertex(ox + i, oy + 0, data[std::make_pair(0, i)]);
        p->insert_vertex(ox + i, oy + 0, min_val);

        p->append_poly();
        p->append_vertex(ox + i-1, oy + lines-1, min_val);
        p->append_vertex(ox + i-1, oy + lines-1, data[std::make_pair(lines-1, i-1)]);
        p->append_vertex(ox + i, oy + lines-1, data[std::make_pair(lines-1, i)]);
        p->append_vertex(ox + i, oy + lines-1, min_val);
    }

    p->append_poly();
    for (int i = 0; i < columns-1; i++)
        p->insert_vertex(ox + i, oy + 0, min_val);
    for (int i = 0; i < lines-1; i++)
        p->insert_vertex(ox + columns-1, oy + i, min_val);
    for (int i = columns-1; i > 0; i--)
        p->insert_vertex(ox + i, oy + lines-1, min_val);
    for (int i = lines-1; i > 0; i--)
        p->insert_vertex(ox + 0, oy + i, min_val);

    return p;
}

PolySet * readPolySetFromSTL( const Filename filename, int convexity)
{
	PolySet *p = NULL;

		handle_dep(filename);
		std::ifstream f(filename.c_str(), std::ios::in | std::ios::binary);
		if (!f.good()) {
			PRINTB("WARNING: Can't open import file '%s'.", filename);
			return p;
		}

		p = new PolySet();

		boost::regex ex_sfe("solid|facet|endloop");
		boost::regex ex_outer("outer loop");
		boost::regex ex_vertex("vertex");
		boost::regex ex_vertices("\\s*vertex\\s+([^\\s]+)\\s+([^\\s]+)\\s+([^\\s]+)");

		char data[5];
		f.read(data, 5);
		if (!f.eof() && !memcmp(data, "solid", 5)) {
			int i = 0;
			double vdata[3][3];
			std::string line;
			std::getline(f, line);
			while (!f.eof()) {
				
				std::getline(f, line);
				boost::trim(line);
				if (boost::regex_search(line, ex_sfe)) {
					continue;
				}
				if (boost::regex_search(line, ex_outer)) {
					i = 0;
					continue;
				}
				boost::smatch results;
				if (boost::regex_search(line, results, ex_vertices)) {
					try {
						for (int v=0;v<3;v++) {
							vdata[i][v] = boost::lexical_cast<double>(results[v+1]);
						}
					}
					catch (boost::bad_lexical_cast &blc) {
						PRINTB("WARNING: Can't parse vertex line '%s'.", line);
						i = 10;
						continue;
					}
					if (++i == 3) {
						p->append_poly();
						p->append_vertex(vdata[0][0], vdata[0][1], vdata[0][2]);
						p->append_vertex(vdata[1][0], vdata[1][1], vdata[1][2]);
						p->append_vertex(vdata[2][0], vdata[2][1], vdata[2][2]);
					}
				}
			}
		}
		else
		{
			f.ignore(80-5+4);
			while (1) {
#ifdef _MSC_VER
#pragma pack(push,1)
#endif
				struct {
					float i, j, k;
					float x1, y1, z1;
					float x2, y2, z2;
					float x3, y3, z3;
					unsigned short acount;
				}
#ifdef __GNUC__
				__attribute__ ((packed))
#endif
				stldata;
#ifdef _MSC_VER
#pragma pack(pop)
#endif

				f.read((char*)&stldata, sizeof(stldata));
				if (f.eof()) break;
				p->append_poly();
				p->append_vertex(stldata.x1, stldata.y1, stldata.z1);
				p->append_vertex(stldata.x2, stldata.y2, stldata.z2);
				p->append_vertex(stldata.x3, stldata.y3, stldata.z3);
			}
		}
	if (p) p->convexity = convexity;
	return p;
}


PolySet * readPolySetFromDXF( const Filename filename, std::string layername, double origin_x, double origin_y, double scale, double fn, double fs, double fa, int convexity)
{
    PolySet *p = NULL;
    p = new PolySet();
	DxfData dd(fn, fs, fa, filename, layername, origin_x, origin_y, scale);
	p->is2d = true;
	dxf_tesselate(p, dd, 0, true, false, 0);
	dxf_border_to_ps(p, dd);
	if (p) p->convexity = convexity;
	return p;
}

