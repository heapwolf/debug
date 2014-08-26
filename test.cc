#include <fstream>
#include <iomanip>
#include <ctime>

#include "./debug.h"

int main() {

  using namespace std;
  stringstream ss;

  //
  // logging to a standard stream... log-level stays
  // in the stream until a new log level is inserted.
  //
  Debug a("test1", Debug::verbose, cout);

  a << a.info << "hello" << endl;
  a << a.warn << "goodbye" << endl;


  //
  // logging to a file...
  //
  ofstream logfile;
  logfile.open("test.log", ios::app);

  Debug b("test2", Debug::verbose, logfile);

  b << b.warn << "hex val: " << hex << 29 << endl;
  logfile.close();


  //
  // format after each endl; convert to json, etc.
  //
  Debug c("test3", Debug::verbose, cout);

  c.format([](auto name, auto level, auto data) {
    return name + ":" + level + ":" + data;
  });

  c << "america is #" << 1 << endl;
}
