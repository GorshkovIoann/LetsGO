#include <iostream>

#include "board.h"
#include <vector>

using namespace Graph_lib;
std::vector<int> from_null_to_line;

Cell::Type type_of_cell (int i, int j)
{
  if (i % 2 == 0)
    return (j % 2 == 0) ? Cell::black : Cell::white;
  else
    return Cell::white;
}

Board::Board(Point xy, Callback cb_clicked)
    : Widget{xy, size, size, "RusGame", nullptr},
      pass{Point(270, 0), cb_clicked, Cell::black}
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
    {  // заполняется по строкам начиная с нижней левой
       // if(i%2==0 && j%2 == 0)
      cells.push_back(new Cell{
          Point{margin + j * Cell::size, margin + (N - 1 - i) * Cell::size},
          cb_clicked, type_of_cell(i, j)});
    }
}

void Board::attach(Graph_lib::Window& win)
{
  int x = 1;
  int y = 1;
  for (int i = 0; i < cells.size(); ++i)  // крепим клетки
  {
    if (cells[i].is_black())
    {
      cells[i].set_position(y, x);
      ++x;
      if (x > 9)
      {
        ++y;
        x = 1;
      }
      std::cout << x << y << std::endl;
    }
    win.attach(cells[i]);
  }

  win.attach(cal);  // картинка поля

  win.attach(whit);  // добавим счет
  win.attach(blc);

  win.attach(pass);  // добавим пасс
  win.attach(pass_text);
}

bool Board::ko(Cell& c2)
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

int Board::get_pass() { return pass.get_id(); }

void Board::check_rules(Cell& c2)
{
  // std::cout << c2.get_id();

  int a = c2.get_y();
  int b = c2.get_x();
  std::cout << a << b;

  /*for (int i = 1; i <= 9; ++i)
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
   */
  if (black_turn)
  {  // был ход белых
    check_around_black(a, b);
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
    check_around_white(a, b);
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

bool Board::check(int a, int b)
{
  bool del = true;
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

  if ((a - 1 > 0 &&
       (at(a - 1, b).has_white_figure() == at(a, b).has_white_figure())) &&
      std::count(nums.begin(), nums.end(), (a - 1) * 10 + b) == 0)
  {
    del = check(a - 1, b) && del;
    std::cout << "utop" << a << b << std::endl;
  }

  if ((a + 1 <= 9 &&
       (at(a + 1, b).has_white_figure() == at(a, b).has_white_figure())) &&
      std::count(nums.begin(), nums.end(), (a + 1) * 10 + b) == 0)
  {

    del = del && check(a + 1, b);
    std::cout << "udown" << a << b << std::endl;
  }

  if ((b + 1 <= 9 &&
       (at(a, b + 1).has_black_figure() != at(a, b).has_white_figure())) &&
      std::count(nums.begin(), nums.end(), (a) * 10 + b + 1) == 0)
  {

    del = del && check(a, b + 1);
    std::cout << "uright" << a << b << std::endl;
  }

  if ((b - 1 > 0 &&
       (at(a, b - 1).has_black_figure() != at(a, b).has_white_figure())) &&
      std::count(nums.begin(), nums.end(), (a) * 10 + b - 1) == 0)
  {

    del = del && check(a, b - 1);
    std::cout << "uleft" << a << b << std::endl;
  }
  if (del)
  {
    to_remove_cells.push_back(at(a, b));
    return true;
  }
  return false;
}

void Board::check_around_black(int a, int b)
{

  std::cout << "first_black";
  if (a - 1 > 0 && at(a - 1, b).has_black_figure())
  {
    if (check(a - 1, b))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++white_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
  if (a + 1 < 10 && at(a + 1, b).has_black_figure())
  {
    if (check(a + 1, b))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++white_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
  if (b + 1 < 10 && at(a, b + 1).has_black_figure())
  {
    if (check(a, b + 1))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++white_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
  if (b - 1 > 0 && at(a, b - 1).has_black_figure())
  {
    if (check(a, b - 1))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++white_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
}

bool Board::death_check(int a, int b, bool black)
{
  // 2-проверкасуицида
  if (black)
    if (check(a, b))
    {
      black_turn = !black_turn;
      return true;
    }
  if (!black)
    if (check(a, b))
    {
      black_turn = !black_turn;
      return true;
    }
  return false;
}

void Board::check_around_white(int a, int b)
{

  std::cout << "first_w";
  if (a - 1 > 0 && at(a - 1, b).has_white_figure())
  {
    if (check(a - 1, b))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++black_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
  if (a + 1 < 10 && at(a + 1, b).has_white_figure())
  {
    if (check(a + 1, b))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++black_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
  if (b + 1 < 10 && at(a, b + 1).has_white_figure())
  {
    if (check(a, b + 1))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++black_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }

  if (b - 1 > 0 && at(a, b - 1).has_white_figure())
  {
    if (check(a, b - 1))
      for (int i = 0; i < to_remove_cells.size(); ++i)
      {
        ++black_count;
        to_remove_cells[i].detach_figure();
      }
    nums.clear();
    to_remove_cells.clear();
  }
}

void Board::check_num_w(int i, int j, int group_num)
{
  if (copy2[i][j] == 0)
  {
    copy2[i][j] = group_num;
    if (i - 1 >= 0)
      check_num_w(i - 1, j, group_num);
    if (i + 1 < 9)
      check_num_w(i + 1, j, group_num);
    if (j - 1 >= 0)
      check_num_w(i, j - 1, group_num);
    if (j + 1 < 9)
      check_num_w(i, j + 1, group_num);
  }
}

void Board::check_num_bl(int i, int j, int group_num)
{
  if (copy1[i][j] == 0)
  {
    copy1[i][j] = group_num;
    if (i - 1 >= 0)
      check_num_bl(i - 1, j, group_num);
    if (i + 1 < 9)
      check_num_bl(i + 1, j, group_num);
    if (j - 1 >= 0)
      check_num_bl(i, j - 1, group_num);
    if (j + 1 < 9)
      check_num_bl(i, j + 1, group_num);
  }
}

void Board::check_w_line(int i, int j, int group_num)
{
  from_null_to_line.push_back(i * 10 + j);
  copy2[i][j] = 3;
  if (i - 1 >= 0)
  {
    if (copy2[i - 1][j] == 1)
    {
      check_w_line(i - 1, j, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i - 1][j]) == 0)
    {
      territory.push_back(copy2[i - 1][j]);
    }
  }

  if (i + 1 < 9)
  {
    if (copy2[i + 1][j] == 1)
    {
      check_w_line(i + 1, j, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i + 1][j]) == 0)
    {
      territory.push_back(copy2[i + 1][j]);
    }
  }

  if (j - 1 >= 0)
  {
    if (copy2[i][j - 1] == 1)
    {
      check_w_line(i, j - 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i][j - 1]) == 0)
    {
      territory.push_back(copy2[i][j - 1]);
    }
  }

  if (j + 1 < 9)
  {
    if (copy2[i][j + 1] == 1)
    {
      check_w_line(i, j + 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i][j + 1]) == 0)
    {
      territory.push_back(copy2[i][j + 1]);
    }
  }
  if (i - 1 >= 0 && j - 1 >= 0)
  {
    if (copy2[i - 1][j - 1] == 1)
    {
      check_w_line(i - 1, j - 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i - 1][j - 1]) == 0)
    {
      territory.push_back(copy2[i - 1][j - 1]);
    }
  }

  if (i + 1 < 9 && j + 1 < 9)
  {
    if (copy2[i + 1][j + 1] == 1)
    {
      check_w_line(i + 1, j + 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i + 1][j + 1]) == 0)
    {
      territory.push_back(copy2[i + 1][j + 1]);
    }
  }
  if (j - 1 >= 0 && i + 1 < 9)
  {
    if (copy2[i + 1][j - 1] == 1)
    {
      check_w_line(i + 1, j - 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i + 1][j - 1]) == 0)
    {
      territory.push_back(copy2[i + 1][j - 1]);
    }
  }
  if (j + 1 < 9 && i - 1 >= 0)
  {
    if (copy2[i + 1][j] == 1)
    {
      check_w_line(i - 1, j + 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy2[i - 1][j + 1]) == 0)
    {
      territory.push_back(copy2[i - 1][j + 1]);
    }
  }
}

void Board::check_bl_line(int i, int j, int group_num)
{
  from_null_to_line.push_back(i * 10 + j);
  copy1[i][j] = 4;
  if (i - 1 >= 0)
  {
    if (copy1[i - 1][j] == 2)
    {
      check_bl_line(i - 1, j, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i - 1][j]) == 0)
    {
      territory.push_back(copy1[i - 1][j]);
    }
  }

  if (i + 1 < 9)
  {
    if (copy1[i + 1][j] == 2)
    {
      check_bl_line(i + 1, j, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i + 1][j]) == 0)
    {
      territory.push_back(copy1[i + 1][j]);
    }
  }

  if (j - 1 >= 0)
  {
    if (copy1[i][j - 1] == 2)
    {
      check_bl_line(i, j - 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i][j - 1]) == 0)
    {
      territory.push_back(copy1[i][j - 1]);
    }
  }

  if (j + 1 < 9)
  {
    if (copy1[i][j + 1] == 2)
    {
      check_bl_line(i, j + 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i][j + 1]) == 0)
    {
      territory.push_back(copy1[i][j + 1]);
    }
  }
  if (i - 1 >= 0 && j - 1 >= 0)
  {
    if (copy1[i - 1][j - 1] == 2)
    {
      check_bl_line(i - 1, j - 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i - 1][j - 1]) == 0)
    {
      territory.push_back(copy1[i - 1][j - 1]);
    }
  }

  if (i + 1 < 9 && j + 1 < 9)
  {
    if (copy1[i + 1][j + 1] == 2)
    {
      check_bl_line(i + 1, j + 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i + 1][j + 1]) == 0)
    {
      territory.push_back(copy1[i + 1][j + 1]);
    }
  }
  if (j - 1 >= 0 && i + 1 < 9)
  {
    if (copy1[i + 1][j - 1] == 2)
    {
      check_bl_line(i + 1, j - 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i + 1][j - 1]) == 0)
    {
      territory.push_back(copy1[i + 1][j - 1]);
    }
  }
  if (j + 1 < 9 && i - 1 >= 0)
  {
    if (copy1[i + 1][j] == 2)
    {
      check_bl_line(i - 1, j + 1, group_num);
    }
    else if (std::count(territory.begin(), territory.end(),
                        copy1[i - 1][j + 1]) == 0)
    {
      territory.push_back(copy1[i - 1][j + 1]);
    }
  }
}

void Board::recount()
{

  int group_num = 5;
  int bl_num = 100;
  int w_num = 200;
  std::cout << "recount";
  int num_cells[9][9];
  for (int x = 0; x < cells.size(); ++x)
  {
    if (cells[x].is_black())
    {
      if (cells[x].has_black_figure())
      {
        num_cells[cells[x].get_y() - 1][cells[x].get_x() - 1] = 2;
      }
      if (cells[x].has_white_figure())
      {
        num_cells[cells[x].get_y() - 1][cells[x].get_x() - 1] = 1;
      }
      if (!cells[x].has_figure())
        num_cells[cells[x].get_y() - 1][cells[x].get_x() - 1] = 0;
    }
  }
  // forblack

  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      // std::cout << num_cells[i][j] << std::endl;
      copy1[i][j] = num_cells[i][j];
      if (copy1[i][j] == 1)
        copy1[i][j] = 0;
    }
  }

  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      check_num_bl(i, j, group_num);
      group_num++;
    }
  }
  /*for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      std::cout << copy1[i][j] << std::endl;
    }
  }*/
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      if (copy1[i][j] == 2)
      {
        check_bl_line(i, j, bl_num);
        if (territory.size() > 2)
        {
          for (int x : from_null_to_line)
          {
            copy1[x / 10][x % 10] = bl_num;
          }
          for (int x : territory)
          {
            std::cout << x << std::endl;
            if (x < 100)
              bl_territory.push_back(x);
          }
        }
        from_null_to_line.clear();
        territory.clear();
        bl_num++;
      }
    }
  }
  // for white

  group_num = 5;
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      copy2[i][j] = num_cells[i][j];
      if (copy2[i][j] == 2)
        copy2[i][j] = 0;
    }
  }

  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      check_num_w(i, j, group_num);
      group_num++;
    }
  }

  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      if (copy2[i][j] == 1)
      {
        check_w_line(i, j, w_num);
        if (territory.size() > 2)
        {
          for (int x : from_null_to_line)
          {
            copy2[x / 10][x % 10] = w_num;
          }
          for (int x : territory)
          {
            std::cout << x << std::endl;
            if (x < 100)
              w_territory.push_back(x);
          }
        }
        from_null_to_line.clear();
        territory.clear();
        w_num++;
      }
    }
  }
  // общая проверка
  std::cout << std::endl;
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      std::cout << copy1[i][j] << std::endl;
    }
  }
  std::cout << std::endl;
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      std::cout << copy2[i][j] << std::endl;
    }
  }
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      if (copy2[i][j] > 99)
      {
        int t = copy1[i][j];
        for (int i = 0; i < 9; ++i)
        {
          for (int j = 0; j < 9; ++j)
          {
            if (copy1[i][j] == t)
              copy1[i][j] = 99;
          }
        }
      }
      if (copy1[i][j] > 99)
      {
        int t = copy2[i][j];
        for (int i = 0; i < 9; ++i)
        {
          for (int j = 0; j < 9; ++j)
          {
            if (copy2[i][j] == t)
              copy2[i][j] = 99;
          }
        }
      }
    }
  }
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      if (copy1[i][j] > 99 || copy2[i][j] > 99)
      {
      }
      else if (copy1[i][j] == 4)
      {
        check_along_black(i, j);
      }
      else if (copy2[i][j] == 3)
      {
        check_along_white(i, j);
      }
      else
      {
        if (std::count(w_territory.begin(), w_territory.end(),
                       copy2[i][j]) > 0 &&
            std::count(bl_territory.begin(), bl_territory.end(),
                       copy1[i][j]) == 0)
        {
          white_count++;
        }
        if (std::count(w_territory.begin(), w_territory.end(),
                       copy2[i][j]) == 0 &&
            std::count(bl_territory.begin(), bl_territory.end(),
                       copy1[i][j]) > 0)
        {
          black_count++;
        }
      }
    }
  }
  std::ostringstream oss;
  oss << "White score = " << white_count;
  whit.set_label(oss.str());
  std::ostringstream oss2;
  oss2 << "Black score = " << black_count;
  blc.set_label(oss2.str());
}

bool Board::check_along_black(int i, int j)
{
  std::cout << "strt";
  copy1[i][j] = 2;
  bool flg = false;
  if (i - 1 >= 0)
  {
    if (copy1[i - 1][j] == 4)
    {
      if (check_along_black(i - 1, j))
      {
        flg = true;
      }
    }
    std::cout << copy1[i - 1][j] << std::endl;
    if (copy1[i - 1][j] >= 5 && copy1[i - 1][j] < 100)
    {
      std::cout << "!" << std::endl;
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i - 1][j]) > 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i - 1][j]) == 0)
      {
        flg = true;
      }
    }
  }

  if (i + 1 <= 9)
  {
    if (copy1[i + 1][j] == 4)
    {
      if (check_along_black(i + 1, j))
      {
        flg = true;
      }
    }
    std::cout << copy1[i + 1][j] << std::endl;
    if (copy1[i + 1][j] >= 5 && copy1[i + 1][j] < 100)
    {
      std::cout << "!" << std::endl;
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i + 1][j]) > 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i + 1][j]) == 0)
      {
        flg = true;
      }
    }
  }

  if (j - 1 >= 0)
  {
    if (copy1[i][j - 1] == 4)
    {
      if (check_along_black(i, j - 1))
      {
        flg = true;
      }
    }
    if (copy1[i][j - 1] >= 5 && copy1[i][j - 1] < 100)
    {
      std::cout << "!" << std::endl;
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i][j - 1]) > 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i][j - 1]) == 0)
      {
        flg = true;
      }
    }
  }

  if (j + 1 <= 9)
  {
    if (copy1[i][j + 1] == 4)
    {
      if (check_along_black(i, j + 1))
      {
        flg = true;
      }
    }
    if (copy1[i][j + 1] >= 5 && copy1[i][j + 1] < 100)
    {
      std::cout << "!" << std::endl;
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i][j + 1]) > 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i][j + 1]) == 0)
      {

        flg = true;
      }
    }
  }
  if (flg == true)
  {
    white_count = white_count + 2;
  }
  return flg;
}

bool Board::check_along_white(int i, int j)
{
  copy2[i][j] = 1;
  bool flg = false;
  if (i - 1 >= 0)
  {
    if (copy2[i - 1][j] == 3)
    {
      if (check_along_white(i - 1, j))
      {

        flg = true;
      }
    }
    if (copy2[i - 1][j] >= 5 && copy2[i - 1][j] < 100)
    {
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i - 1][j]) == 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i - 1][j]) > 0)
      {

        flg = true;
      }
    }
  }

  if (i + 1 <= 9)
  {
    if (copy2[i + 1][j] == 3)
    {
      if (check_along_white(i + 1, j))
      {

        flg = true;
      }
    }
    if (copy2[i + 1][j] >= 5 && copy2[i + 1][j] < 100)
    {
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i + 1][j]) == 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i + 1][j]) > 0)
      {

        flg = true;
      }
    }
  }

  if (j - 1 >= 0)
  {
    if (copy2[i][j - 1] == 3)
    {
      if (check_along_white(i, j - 1))
      {

        flg = true;
      }
    }
    if (copy2[i][j - 1] >= 5 && copy2[i][j - 1] < 100)
    {
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i][j - 1]) == 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i][j - 1]) > 0)
      {
        flg = true;
      }
    }
  }

  if (j + 1 <= 9)
  {
    if (copy2[i][j + 1] == 3)
    {
      if (check_along_white(i, j + 1))
      {
        flg = true;
      }
    }
    if (copy2[i][j + 1] >= 5 && copy2[i][j + 1] < 100)
    {
      if (std::count(w_territory.begin(), w_territory.end(),
                     copy2[i][j + 1]) == 0 &&
          std::count(bl_territory.begin(), bl_territory.end(),
                     copy1[i][j + 1]) > 0)
      {

        flg = true;
      }
    }
  }
  if (flg == true)
  {
    black_count = black_count + 2;
  }
  return flg;
}

Cell& Board::at(int i, int c)  // строка и столбец
{
  // first cell has index 1
  if (i < 1 || 9 < i)
    error("Board::at:out of range");

  if (c < 1 || 9 < c)
    error("Board::at:out of range");

  return cells[(i - 1) * 2 * N + (c - 1) * 2];
}
