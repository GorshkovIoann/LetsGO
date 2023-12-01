#ifndef GRAPH_SHAPES_H
#define GRAPH_SHAPES_H

#include "Graph.h"
namespace Graph_lib
{

    struct Arc2 : Ellipse
    {
        Arc2(const Point &center, int w, int h, double angle1, double angle2);

        void set_start_angle(double a) { angle_start = a; }
        void set_end_angle(double a) { angle_end = a; }
        double get_start_angle() const { return angle_start; }
        double get_end_angle() const { return angle_end; }

    protected:
        void draw_lines() const override;

    private:
        double angle_start;
        double angle_end;
    };

}

#endif