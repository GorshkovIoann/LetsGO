#include <iostream>

#include "board.h"
#include <vector>

using namespace Graph_lib;
std::vector<int> nums;
std::vector<int> nums2;

Cell::Type type_of_cell (int i, int j)
{
  if (i % 2 == 0)
    return (j % 2 == 0) ? Cell::black : Cell::white;
  else
    return Cell::white;
}

Chessboard::Chessboard(Point xy, Callback cb_clicked)
    : Widget{xy, size, size, "RusGame", nullptr},
      pass{Point(270, 0), cb_clicked, Cell::black}
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
    {  // заполняется по строкам начиная с нижней левой
      cells.push_back(new Cell{
          Point{margin + j * Cell::size, margin + (N - 1 - i) * Cell::size},
          cb_clicked, type_of_cell(i, j)});
    }
}

void Chessboard::attach(Graph_lib::Window& win)
{
  for (int i = 0; i < cells.size(); i = i + 2)  // крепим клетки
  {
    win.attach(cells[i]);
  }

  win.attach(cal);  // картинка поля

  win.attach(whit);  // добавим счет
  win.attach(blc);

  win.attach(pass);  // добавим пасс
  win.attach(pass_text);
}

bool Chessboard::ko(Cell& c2)
{
  if (black_turn && first_white_turn)
  {
    white_ko_check = &c2;
    first_white_turn = false;
    return false;
  }
  if (!black_turn && first_black_turn)
  {
    black_ko_check = &c2;
    first_black_turn = false;
    return false;
  }
  if ((black_turn && &c2 == white_ko_check) ||
      (!black_turn && &c2 == black_ko_check))
  {
    black_turn = !black_turn;
    std::cout << "ko";
    return true;
  }
  if (black_turn)
    white_ko_check = &c2;
  if (!black_turn)
    black_ko_check = &c2;
  return false;
}

int Chessboard::get_pass() { return pass.get_id(); }

void Chessboard::check_rules(Cell& c2)
{
  // std::cout << c2.get_id();

  int a = 0;
  int b = 0;
  for (int i = 1; i <= 9; ++i)
  {
    for (int c = 1; c <= 9; ++c)
    {
      // std::cout << at(i, c).get_id();

      if (at(i, c).get_id() == c2.get_id())
      {
        a = i;
        b = c;
        std::cout << 1;
      }
    }
  }
  if (black_turn)
  {  // был ход белых
    check_around_black(a, b, true);
    nums.clear();
    to_remove_cells.clear();
    if (death_check(a, b, true))
    {
      c2.detach_figure();
    }
    nums.clear();
    to_remove_cells.clear();
  }
  else if (!black_turn)
  {  // был ход черных
    check_around_white(a, b, true);
    nums.clear();
    to_remove_cells.clear();
    if (death_check(a, b, false))
    {

      c2.detach_figure();
    }
    nums.clear();
    to_remove_cells.clear();
  }
  // recount();
  std::ostringstream oss;
  oss << "White score = " << white_count + white_territory;
  whit.set_label(oss.str());
  std::ostringstream oss2;
  oss2 << "Black score = " << black_count + black_territory;
  blc.set_label(oss2.str());
}

bool Chessboard::check_around_black(int a, int b, bool first)
{
  if (first)
  {
    std::cout << "first_black";
    if (a - 1 > 0 && at(a - 1, b).has_black_figure())
    {
      if (check_around_black(a - 1, b, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++white_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
    if (a + 1 < 10 && at(a + 1, b).has_black_figure())
    {
      if (check_around_black(a + 1, b, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++white_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
    if (b + 1 < 10 && at(a, b + 1).has_black_figure())
    {
      if (check_around_black(a, b + 1, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++white_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
    if (b - 1 > 0 && at(a, b - 1).has_black_figure())
    {
      if (check_around_black(a, b - 1, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++white_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
  }

  bool left = false;
  bool right = false;
  bool top = false;
  bool down = false;
  if (!first)
  {
    std::cout << "black";
    nums.push_back(a * 10 + b);

    if (a - 1 > 0 && !at(a - 1, b).has_figure())
      return false;
    if (a + 1 <= 9 && !at(a + 1, b).has_figure())
      return false;
    if (b + 1 <= 9 && !at(a, b + 1).has_figure())
      return false;
    if (b - 1 > 0 && !at(a, b - 1).has_figure())
      return false;

    if (a - 1 == 0 || (a - 1 > 0 && at(a - 1, b).has_white_figure()) ||
        std::count(nums.begin(), nums.end(), (a - 1) * 10 + b))
    {
      top = true;
      std::cout << "top" << a << b << std::endl;
    }

    if ((a - 1 > 0 && at(a - 1, b).has_black_figure()) &&
        std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) == 0)
    {
      if (check_around_black(a - 1, b, false))
        top = true;
      std::cout << "utop" << a << b << std::endl;
    }

    if (a + 1 == 10 || (a + 1 <= 9 && at(a + 1, b).has_white_figure()) ||
        std::count(nums.begin(), nums.end(), (a + 1) * 10 + b))
    {
      down = true;
      std::cout << "down" << a << b << std::endl;
    }
    if ((a + 1 <= 9 && at(a + 1, b).has_black_figure()) &&
        std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) == 0)
    {

      if (check_around_black(a + 1, b, false))
        down = true;
      std::cout << "udown" << a << b << std::endl;
    }

    if (b + 1 == 10 || (b + 1 <= 9 && at(a, b + 1).has_white_figure()) ||
        std::count(nums.begin(), nums.end(), (a) * 10 + b + 1))
    {
      right = true;
      std::cout << "right" << a << b << std::endl;
    }
    if ((b + 1 <= 9 && at(a, b + 1).has_black_figure()) &&
        std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) == 0)
    {

      if (check_around_black(a, b + 1, false))
        right = true;
      std::cout << "uright" << a << b << std::endl;
    }
    if (b - 1 == 0 || (b - 1 > 0 && at(a, b - 1).has_white_figure()) ||
        std::count(nums.begin(), nums.end(), (a) * 10 + b - 1))
    {
      left = true;
      std::cout << "left" << a << b << std::endl;
    }

    if ((b - 1 > 0 && at(a, b - 1).has_black_figure()) &&
        std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) == 0)
    {

      if (check_around_black(a, b - 1, false))
        left = true;
      std::cout << "uleft" << a << b << std::endl;
    }
    if (left && right && top && down)
    {
      to_remove_cells.push_back(at(a, b));
      return true;
    }
    return false;
  }
  return false;
}

bool Chessboard::death_check(int a, int b, bool black)
{
  // 2-проверкасуицида
  if (black)
    if (check_around_white(a, b, false))
    {
      black_turn = !black_turn;
      return true;
    }
  if (!black)
    if (check_around_black(a, b, false))
    {
      black_turn = !black_turn;
      return true;
    }
  return false;
}

bool Chessboard::check_around_white(int a, int b, bool first)
{
  if (first)
  {
    std::cout << "first_w";
    if (a - 1 > 0 && at(a - 1, b).has_white_figure())
    {
      if (check_around_white(a - 1, b, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++black_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
    if (a + 1 < 10 && at(a + 1, b).has_white_figure())
    {
      if (check_around_white(a + 1, b, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++black_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
    if (b + 1 < 10 && at(a, b + 1).has_white_figure())
    {
      if (check_around_white(a, b + 1, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++black_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }

    if (b - 1 > 0 && at(a, b - 1).has_white_figure())
    {
      if (check_around_white(a, b - 1, false))
        for (int i = 0; i < to_remove_cells.size(); ++i)
        {
          ++black_count;
          to_remove_cells[i].detach_figure();
        }
      nums.clear();
      // to_remove_cells.clear();
    }
  }

  bool left = false;
  bool right = false;
  bool top = false;
  bool down = false;
  if (!first)
  {
    std::cout << "w";
    nums.push_back(a * 10 + b);

    if (a - 1 > 0 && !at(a - 1, b).has_figure())
      return false;
    if (a + 1 <= 9 && !at(a + 1, b).has_figure())
      return false;
    if (b + 1 <= 9 && !at(a, b + 1).has_figure())
      return false;
    if (b - 1 > 0 && !at(a, b - 1).has_figure())
      return false;

    if (a - 1 == 0 || (a - 1 > 0 && at(a - 1, b).has_black_figure()) ||
        std::count(nums.begin(), nums.end(), (a - 1) * 10 + b))
      top = true;
    if ((a - 1 > 0 && at(a - 1, b).has_white_figure()) &&
        std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) == 0)
    {
      if (check_around_white(a - 1, b, false))
        top = true;
    }
    if (a + 1 == 10 || (a + 1 <= 9 && at(a + 1, b).has_black_figure()) ||
        std::count(nums.begin(), nums.end(), (a + 1) * 10 + b))
      down = true;
    if ((a + 1 <= 9 && at(a + 1, b).has_white_figure()) &&
        std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) == 0)
    {

      if (check_around_white(a + 1, b, false))
        down = true;
    }
    if (b + 1 == 10 || (b + 1 <= 9 && at(a, b + 1).has_black_figure()) ||
        std::count(nums.begin(), nums.end(), (a) * 10 + b + 1))
      right = true;
    if ((b + 1 <= 9 && at(a, b + 1).has_white_figure()) &&
        std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) == 0)
    {

      if (check_around_white(a, b + 1, false))
        right = true;
    }
    if (b - 1 == 0 || (b - 1 > 0 && at(a, b - 1).has_black_figure()) ||
        std::count(nums.begin(), nums.end(), (a) * 10 + b - 1))
      left = true;

    if ((b - 1 > 0 && at(a, b - 1).has_white_figure()) &&
        std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) == 0)
    {

      if (check_around_white(a, b - 1, false))
        left = true;
    }
    if (left && right && top && down)
    {
      to_remove_cells.push_back(at(a, b));

      return true;
    }
    return false;
  }
  return false;
}

/*void Chessboard::recount()
  std::cout << "recount";
  int around = 0;
  bool left = false;
  bool right = false;
  bool top = false;
  bool down = false;
  for (int x = 0; x < cells.size(); ++x)
  {
    int a = 0;
    int b = 0;
    for (int i = 1; i <= 9; ++i)
    {
      for (int c = 1; c <= 9; ++c)
      {

        if (at(i, c).get_id() == cells[x].get_id())
        {
          a = i;
          b = c;
        }
      }
    }
    std::cout << "start_enum";
    nums.push_back(a * 10 + b);
    if (a - 1 == 0 || at(a - 1, b).has_black_figure() ||
        is_on_black_territory(a - 1, b))
    {
      std::cout << "start_enum1";
      top = true;
      if ((a - 1 != 0) && (at(a - 1, b).has_black_figure() ||
                           is_on_black_territory(a - 1, b)))
      {
        ++around;
        std::cout << "start_enum.0";
      }
      std::cout << "start_enum.0";
    }
    if (a + 1 == 10 || at(a + 1, b).has_black_figure() ||
        is_on_black_territory(a + 1, b))
    {
      std::cout << "start_enum2";
      down = true;
      if ((a + 1 != 10) && (at(a + 1, b).has_black_figure() ||
                            is_on_black_territory(a + 1, b)))
        ++around;
    }
    if (b + 1 == 10 || at(a, b + 1).has_black_figure() ||
        is_on_black_territory(a, b + 1))
    {
      right = true;
      if ((b + 1 != 10) && (at(a, b + 1).has_black_figure() ||
                            is_on_black_territory(a, b + 1)))
        ++around;
    }
    if (b - 1 == 0 || at(a, b - 1).has_black_figure() ||
        is_on_black_territory(a, b - 1))
    {
      left = true;
      if ((a - 1 != 0) && (at(a, b - 1).has_black_figure() ||
                           is_on_black_territory(a, b - 1)))
        ++around;
    }

    if (left && right && top && down && around >= 2)
    {
      if (at(a, b).has_white_figure())
      {
        black_territory = black_territory + 2;
        std::cout << "+2b";
      }
      else
      {
        black_territory = black_territory + 1;
        std::cout << "+1b";
      }
    }
    nums.clear();
    nums2.clear();
    around = 0;
    std::cout << "en_b";

    nums.push_back(a * 10 + b);
    // проверка принадлежности белой территории
    if (a - 1 == 0 || at(a - 1, b).has_white_figure() ||
        is_on_white_territory(a - 1, b))
    {
      std::cout << "start_enum1";
      top = true;
      if ((a - 1 != 0) && (at(a - 1, b).has_white_figure() ||
                           is_on_white_territory(a - 1, b)))
      {
        ++around;
        std::cout << "start_enum.0on_wite";
      }
      std::cout << "start_enum.on_wht";
    }
    if (a + 1 == 10 || at(a + 1, b).has_white_figure() ||
        is_on_white_territory(a + 1, b))
    {
      std::cout << "start_enum2";
      down = true;
      if ((a + 1 != 10) && (at(a + 1, b).has_white_figure() ||
                            is_on_white_territory(a + 1, b)))
        ++around;
    }
    if (b + 1 == 10 || at(a, b + 1).has_white_figure() ||
        is_on_white_territory(a, b + 1))
    {
      right = true;
      if ((b + 1 != 10) && (at(a, b + 1).has_white_figure() ||
                            is_on_white_territory(a, b + 1)))
        ++around;
    }
    if (b - 1 == 0 || at(a, b - 1).has_white_figure() ||
        is_on_white_territory(a, b - 1))
    {
      left = true;
      if ((a - 1 != 0) && (at(a, b - 1).has_white_figure() ||
                           is_on_white_territory(a, b - 1)))
        ++around;
    }

    if (left && right && top && down && around >= 2)
    {
      if (at(a, b).has_black_figure())
        white_territory = white_territory + 2;
      else
        white_territory = white_territory + 1;
    }
    std::cout << "en_w";
    nums.clear();
    nums2.clear();
    around = 0;
  }
  std::ostringstream oss;
  oss << "White score = " << white_count + white_territory;
  whit.set_label(oss.str());
  std::ostringstream oss2;
  oss2 << "Black score = " << black_count + black_territory;
  blc.set_label(oss2.str());
  white_territory = 0;
  black_territory = 0;
}
*/

/*bool Chessboard::is_on_black_territory(int a, int b)  // не смотреть
{
  int around = 0;
  std::cout << "bl_t";
  bool left = false;
  bool right = false;
  bool top = false;
  bool down = false;
  nums.push_back(a * 10 + b);
  if (a - 1 == 0 ||
      std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) ||
      at(a - 1, b).has_black_figure() || is_on_black_territory(a - 1, b))
  {
    std::cout << "start_enum1";
    top = true;
    if ((a - 1 != 0) &&
        (at(a - 1, b).has_black_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a - 1) * 10 + b) == 0 &&
          is_on_black_territory(a - 1, b)) ||
         (std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) == 0 &&
          is_on_black_territory(a - 1, b))))
    {
      ++around;
      std::cout << "start_enum.0on_blc";
    }
    std::cout << "start_enum.0on_blc";
  }
  if (a + 1 == 10 ||
      std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) ||
      at(a + 1, b).has_black_figure() || is_on_black_territory(a + 1, b))
  {
    std::cout << "start_enum2";
    down = true;
    if ((a + 1 != 10) &&
        (at(a + 1, b).has_black_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a + 1) * 10 + b) == 0 &&
          is_on_black_territory(a - 1, b)) ||
         (std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) == 0 &&
          is_on_black_territory(a + 1, b))))

      ++around;
  }
  if (b + 1 == 10 ||
      std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) ||
      at(a, b + 1).has_black_figure() || is_on_black_territory(a, b + 1))
  {
    right = true;
    if ((b + 1 != 10) &&
        (at(a, b + 1).has_black_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a) * 10 + b + 1) == 0 &&
          is_on_black_territory(a, b + 1)) ||
         (std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) == 0 &&
          is_on_black_territory(a, b + 1))))

      ++around;
  }
  if (b - 1 == 0 ||
      std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) ||
      at(a, b - 1).has_black_figure() || is_on_black_territory(a, b - 1))
  {
    left = true;
    if ((b - 1 != 0) &&
        (at(a, b - b).has_black_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a) * 10 + b - 1) == 0 &&
          is_on_black_territory(a, b - 1)) ||
         (std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) == 0 &&
          is_on_black_territory(a, b - 1))))

      ++around;
  }

  if (left && right && top && down && around >= 2)
  {
    nums2.push_back(a * 10 + b);
    return true;
  }
  return false;
}
*/
/*bool Chessboard::is_on_white_territory(int a, int b)  // не смотреть
{
  int around = 0;
  std::cout << "bl_t";
  bool left = false;
  bool right = false;
  bool top = false;
  bool down = false;
  nums.push_back(a * 10 + b);
  if (a - 1 == 0 ||
      std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) ||
      at(a - 1, b).has_white_figure() || is_on_white_territory(a - 1, b))
  {
    std::cout << "start_enum1";
    top = true;
    if ((a - 1 != 0) &&
        (at(a - 1, b).has_white_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a - 1) * 10 + b) == 0 &&
          is_on_white_territory(a - 1, b)) ||
         (std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) == 0 &&
          is_on_white_territory(a - 1, b))))
    {
      ++around;
      std::cout << "start_enum.0on_blc";
    }
    std::cout << "start_enum.0on_blc";
  }
  if (a + 1 == 10 ||
      std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) ||
      at(a + 1, b).has_white_figure() || is_on_white_territory(a + 1, b))
  {
    std::cout << "start_enum2";
    down = true;
    if ((a + 1 != 10) &&
        (at(a + 1, b).has_white_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a + 1) * 10 + b) == 0 &&
          is_on_white_territory(a - 1, b)) ||
         (std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) == 0 &&
          is_on_white_territory(a + 1, b))))

      ++around;
  }
  if (b + 1 == 10 ||
      std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) ||
      at(a, b + 1).has_white_figure() || is_on_white_territory(a, b + 1))
  {
    right = true;
    if ((b + 1 != 10) &&
        (at(a, b + 1).has_white_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a) * 10 + b + 1) == 0 &&
          is_on_white_territory(a, b + 1)) ||
         (std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) == 0 &&
          is_on_white_territory(a, b + 1))))

      ++around;
  }
  if (b - 1 == 0 ||
      std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) ||
      at(a, b - 1).has_white_figure() || is_on_white_territory(a, b - 1))
  {
    left = true;
    if ((b - 1 != 0) &&
        (at(a, b - b).has_white_figure() ||
         (std::count(nums2.begin(), nums2.end(), (a) * 10 + b - 1) == 0 &&
          is_on_white_territory(a, b - 1)) ||
         (std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) == 0 &&
          is_on_white_territory(a, b - 1))))

      ++around;
  }

  if (left && right && top && down && around >= 2)
  {
    nums2.push_back(a * 10 + b);
    return true;
  }
  return false;
}
*/
Cell& Chessboard::at(int i, int c)  // строка и столбец
{
  // first cell has index 1
  if (i < 1 || 9 < i)
    error("Chessboard::at:out of range");

  if (c < 1 || 9 < c)
    error("Chessboard::at:out of range");

  return cells[(i - 1) * 2 * N + (c - 1) * 2];
}
