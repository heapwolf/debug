#ifndef __DEBUG__
#define __DEBUG__

#include <iostream>
#include <sstream>
#include <functional>

using namespace std;
class Debug : public std::ostream {

  private:
    typedef function
    <string (string name, string level, string data)> Formatter;

    Formatter formatter;
    bool hasFormatter = false;
  
    const char* labels[4] = { "error", "warn", "info", "verbose" };
 
    class Buffer : public stringbuf {

      public:
        Debug *debug;
        Buffer(ostream &str) : output(str) {}

        int sync () {
          if (debug->currentLevel <= debug->maxLevel) {
            if (debug->hasFormatter) {

              string name = debug->name;
              string level = debug->labels[debug->currentLevel - 1];

              output 
                << debug->formatter(name, level, str());
            }
            else {
              output 
                << debug->name << " "
                << debug->labels[debug->currentLevel - 1] << " "
                << str();
            }
          }
          str("");
          output.flush();
          return 0;
        }

      private:
        ostream &output;
    };

    Buffer buffer;

  public:
    typedef enum { quiet, error, warn, info, verbose } level;

    level currentLevel = verbose;
    level maxLevel = verbose;
    string name = "";

    void format(Formatter f) {
      hasFormatter = true;
      formatter = f;
    }

    Debug(string name, level maxLevel, ostream& stream) :
      ostream(&buffer), 
      maxLevel(maxLevel), 
      name(name), 
      buffer(stream) {
        buffer.debug = this;
      }
};

inline Debug &operator << (Debug &d, const Debug::level &level) {
  d.currentLevel = level;
  return d;
}

#endif

