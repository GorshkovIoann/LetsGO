#include "Tracer.h"
int Tracer::level = 0;

Tracer::Tracer(const char* func_name, const char* file, int line)
{
  ++level;
  func = func_name;
  std::cout << "In->Func: " << func << ", File: " << file << ", line "
            << line << ",lvl:" << level << std::endl;
}

Tracer::~Tracer()
{
  ++level;
  std::cout << "Out->Func: " << func << ",lvl:" << level << std::endl;
}