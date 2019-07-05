#include "../index.hxx"

Debug debug("demo");
Debug debug3("demo:beep");

int f3 () {
  Debug debug2("demo:boop");
  debug2("running function f3");
  debug3("running function f3");
  return 0;
}


int f2 () {
  debug("running function f2");
  return f3();
}

int main () {
  debug("starting program");
  f2();
  debug("ending program");
  return 0;
}
