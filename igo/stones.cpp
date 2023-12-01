#include "stones.h"
#include <iostream>
#include <vector>
using namespace Graph_lib;

Stones::Stones(Point xy)
    : Window{xy, Chessboard::size, Chessboard::size, "go_game"},
      board{Point{0, 0}, cb_clicked}
{
  size_range(Chessboard::size, Chessboard::size, Chessboard::size,
             Chessboard::size);  // fixed window size
  board.attach(*this);
}

void Stones::clicked(Cell& c2)
{

  if (c2.has_figure())
  {
    return;
  }
  if (c2.get_id() == board.get_pass())
  {
    ++pass_count;
    board.set_black_turn(!board.is_black_turn());
    board.reset_pass_color(board.is_black_turn() ? 56
                                                 : 255);  // черный белый
    if (pass_count == 2)
    {
      exit(0);
    }
    return;
  }
  if (board.is_black_turn())
  {

    pass_count = 0;
    board.set_black_turn(false);
    stones.push_back(new BlackStone{*this});
    if (!board.ko(c2))
    {
      c2.attach_figure(stones[stones.size() - 1]);
      board.check_rules(c2);
    }
  }
  else
  {
    pass_count = 0;
    board.set_black_turn(true);
    stones.push_back(new WhiteStone{*this});
    if (!board.ko(c2))
    {
      c2.attach_figure(stones[stones.size() - 1]);
      board.check_rules(c2);
    }
  }
  board.reset_pass_color(board.is_black_turn() ? 56 : 255);
  Fl::redraw();
}