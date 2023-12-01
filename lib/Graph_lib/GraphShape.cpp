#include "GraphShape.h"

namespace Graph_lib
{
    Arc2::Arc2(const Point &center, int w, int h, double angle1, double angle2)
        : Ellipse{center, w, h}, angle_start{angle1}, angle_end{angle2}
    {
    }

    void Arc2::draw_lines() const
    {
        if (fill_color().visibility()) // fill
        {
            fl_color(fill_color().as_int());
            fl_pie(point(0).x, point(0).y, 2 * major() - 1, 2 * minor() - 1, angle_start, angle_end);
            fl_color(color().as_int()); // reset color
        }

        if (color().visibility())
            fl_arc(point(0).x, point(0).y, 2 * major() - 1, 2 * minor() - 1, angle_start, angle_end);
    }

    // Box2::Box2(const Point &center, int w, int h, int radius);
    // Box2::Box2(const Point &p1, const Point &p2, int r) : Rectangle
    // {
    //     Point{c.x - w / 2, c.y + h / 2},
    //         Point { c.x + w / 2, c.y - h / 2 }
    // }

} // namespace Graph_lib
