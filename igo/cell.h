#ifndef CELL_H
#define CELL_H 1

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <vector>
using Graph_lib::Point;

struct Figure;

struct Cell : Graph_lib::Button
{
  enum Type
  {
    black,
    white,  // все белые клетки мы в итоге просто скроем

  };

  Cell(Point xy, Graph_lib::Callback cb, Type t);

  void attach (Graph_lib::Window& win) override;

  bool is_black () const { return type == black; }

  void attach_figure (Figure& ch);
  void detach_figure ();

  bool has_figure () const { return fig != nullptr; }

  bool has_white_figure () const;

  bool has_black_figure () const;

  int get_id () { return id; }

  int get_position ()
  {
    return y * 10 + x;
  }  // вернет число:первая цифрв столбец,вторая строка

  const Figure& figure () const;
  Point center () const;

  static constexpr int size = 30;

private:
  int id = 0;
  int x = 0;
  int y = 0;
  Type type;
  Figure* fig{nullptr};
  Graph_lib::Vector_ref<Graph_lib::Rectangle> rectangles;

  void reset_black_color ();
};

#endif  // #ifndef CELL_H
