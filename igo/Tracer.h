#pragma once

#include <iostream>
#include <string>
#define TRACE(name)                                                        \
  Tracer TR { (name, __FILE__, __LINE__) }

class Tracer
{
public:
  Tracer(const char* func_name, const char* file, int line);
  ~Tracer();

private:
  static int level;
  std::string func;

  std::string indent () const;

  // disallow copying
  Tracer(const Tracer&) = delete;
  Tracer& operator= (const Tracer&) = delete;
};