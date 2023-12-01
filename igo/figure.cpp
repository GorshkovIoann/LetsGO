#include "figure.h"

using namespace Graph_lib;

Stone::Stone(Graph_lib::Window& win) : Circle{Point{0, 0}, r}
{
  set_style(Line_style{Line_style::solid, 2});
  win.attach(*this);
}

void Stone::attach(const Cell& c)
{
  if (is_attached())
    error("Stone::attach: already attached");
  cell = &c;
  move(c.center().x - center().x, c.center().y - center().y);
}

namespace {
const Color white{255};
const Color black{56};
}  // namespace

WhiteStone::WhiteStone(Graph_lib::Window& win) : Stone{win}
{
  set_color(black);
  set_fill_color(white);
}

BlackStone::BlackStone(Graph_lib::Window& win) : Stone{win}
{
  set_color(white);
  set_fill_color(black);
}