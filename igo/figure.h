#ifndef FIGURE_H
#define FIGURE_H 1
#include "cell.h"
#include <Graph_lib/Graph.h>

struct Figure
{
  virtual bool is_black () const = 0;
  virtual void attach (const Cell& c) = 0;

  bool is_attached () const
  {
    return cell != nullptr;
  }  // вероятно не нужно

  void detach () { cell = nullptr; }  // вероятно не нужно

  virtual ~Figure() = default;

  void set_xy (int a, int b)
  {
    x = a;
    y = b;
  }

  int get_x () { return x; }

  int get_y () { return y; }

protected:
  int x = 0;
  int y = 0;
  const Cell* cell{nullptr};
};

struct Stone : Figure, Graph_lib::Circle
{
  Stone(Graph_lib::Window& win);
  void attach (const Cell& c) override;
  // void draw_lines () const override;

private:
  static constexpr int r = 1.5 * Cell::size / 2;
  static constexpr double f = 0.50;
};

struct WhiteStone : Stone
{
  WhiteStone(Graph_lib::Window& win);

  virtual bool is_black () const override { return false; }
};

struct BlackStone : Stone
{
  BlackStone(Graph_lib::Window& win);

  virtual bool is_black () const override { return true; }
};
#endif