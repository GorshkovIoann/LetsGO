#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

using Graph_lib::Address;
using Graph_lib::Point;

struct Chessboard : Graph_lib::Widget
{
  Chessboard(Point xy, Graph_lib::Callback cb_clicked);

  static constexpr int N =
      17;  //= 9 черных клеток, 8 перегородок размером с клетку

  static constexpr int size = N * Cell::size + 2 * 30;

  void attach (Graph_lib::Window& win) override;

  Cell& at (int i, int c);
  bool ko (Cell& c2);
  void check_rules (Cell& c2);
  void move_figure (Cell& c2);

  bool is_black_turn () { return black_turn; }

  void set_black_turn (bool bl_turn) { black_turn = bl_turn; }

  bool death_check (int a, int b, bool black);
  bool check_around_white (int a, int b, bool first);
  bool check_around_black (int a, int b, bool first);
  // void recount ();
  // bool is_on_black_territory (int a, int b);
  // bool is_on_white_territory (int a, int b);
  int get_pass ();

  void reset_pass_color (int i) { pass_text.set_color(i); }

private:
  Graph_lib::Image cal{Point{0, 0}, "rus.png"};

  Graph_lib::Text whit{Graph_lib::Point{40, 20}, "White score = 6.5"};
  Graph_lib::Text pass_text{Graph_lib::Point{270, 20}, "pass"};
  Graph_lib::Text blc{Graph_lib::Point{450, 20}, "BLack score = 0"};
  int white_territory = 0;
  int black_territory = 0;
  bool black_turn = true;
  double white_count = 6.5;
  Cell pass;

  bool first_black_turn = true;
  bool first_white_turn = true;

  double black_count = 0.;
  static constexpr int margin = 30;

  Graph_lib::Vector_ref<Cell> cells;
  Graph_lib::Vector_ref<Cell> to_remove_cells;
  Cell* white_ko_check{nullptr};  // позиция последнего белого камня
  Cell* black_ko_check{nullptr};  // позиция последнего черного камня

  Cell* selected{nullptr};  // activated cell
};

#endif  // #ifndef BOARD_H
