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

