#include "cell.h"
#include "figure.h"

using namespace Graph_lib;

int big_id = 0;

Cell::Cell(Point xy, Callback cb) : Button{xy, size, size, "", cb}
{  // для удобной идентификации клеток
  id = big_id;
  ++big_id;
}

void Cell::attach(Graph_lib::Window& win)
{
  Button::attach(win);
  reset_black_color();
}

void Cell::attach_figure(Figure& ch)
{
  if (fig)
    error("Cell::attach_figure");

  ch.attach(*this);
  fig = &ch;
}

void Cell::detach_figure()
{
  if (!fig)
    error("Cell::detach_figure");

  Figure* p = fig;
  // p->detach();
  delete p;
  fig = nullptr;
}

bool Cell::has_white_figure() const
{
  if (has_figure())
  {
    return !figure().is_black();
  }
  return false;
}

bool Cell::has_black_figure() const
{
  if (has_figure())
  {
    return figure().is_black();
  }
  return false;
}

const Figure& Cell::figure() const
{
  if (!fig)
    error("");

  return *fig;
}

Point Cell::center() const
{
  return {loc.x + width / 2, loc.y + height / 2};
}

void Cell::reset_black_color()
{
  if (!pw)
    error("Cell is not attached to a window");

  pw->color(Color::black);
}
