#ifndef STONES_H
#define STONES_H 1
#include "board.h"
#include "cell.h"
#include "figure.h"

#include <Graph_lib/GUI.h>

struct Stones : Graph_lib::Window
{
  Stones(Graph_lib::Point xy);

private:
  int pass_count;
  Chessboard board;

  Graph_lib::Vector_ref<Figure> stones;

  static void cb_clicked (Address, Address widget)
  {
    auto& btn = Graph_lib::reference_to<Cell>(widget);
    dynamic_cast<Stones&>(btn.window()).clicked(btn);
  }

  void clicked (Cell& c);
};

#endif