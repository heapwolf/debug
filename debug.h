#ifndef __DEBUG__
#define __DEBUG__

#include <iostream>
#include <sstream>
#include <functional>

class Debug : public std::ostream {

  private:
    typedef std::function
    <std::string (const std::string &name, const std::string &level, const std::string &data)> Formatter;

    Formatter formatter;
    bool hasFormatter = false;
  
    class Buffer : public std::stringbuf {

      public:
        Debug *debug;
        Buffer(std::ostream &str) : output(str) {}

        int sync () {
          static const char * labels[4] = { "error", "warn", "info", "verbose" };

          if (debug->currentLevel <= debug->maxLevel) {
            if (debug->hasFormatter) {

              std::string name = debug->name;
              std::string level = labels[debug->currentLevel - 1];

              output 
                << debug->formatter(name, level, str());
            }
            else {
              output 
                << debug->name << " "
                << labels[debug->currentLevel - 1] << " "
                << str();
            }
          }
          str("");
          output.flush();
          return 0;
        }

      private:
        std::ostream &output;
    };

    Buffer buffer;

  public:
    typedef enum { quiet, error, warn, info, verbose } level;

    level currentLevel = verbose;
    level maxLevel = verbose;
    std::string name;

    void format(const Formatter &f) {
      hasFormatter = true;
      formatter = f;
    }

    Debug(const std::string &name, level maxLevel, std::ostream& stream) :
      std::ostream(&buffer), 
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

