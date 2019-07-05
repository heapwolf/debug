#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <variant>
#include <tuple>
#include <chrono>
#include <map>
#include <regex>

class Debug {
  using clock_t = std::chrono::high_resolution_clock;
  using duration_t = clock_t::duration;
  using Str = std::string;

  Str name = "";
  bool enabled = false;
  std::ostream& output = std::cerr;

  inline static std::map<Str, Str> colors;
  inline static int colorIndex = 0;

  const Str RESET = "\033[0m";

  clock_t::time_point start;

  void init () {
    const auto envp = std::getenv("DEBUG");

    if (envp != nullptr) {
      auto env = Str(envp);

      const auto res = replace(env, "*", "(.*)");

      if (!std::regex_match(this->name, std::regex(res))) {
        return; // the env var does not contain the name
      }
    } else {
      return;
    }

    this->enabled = true;

    start = clock_t::now();

    if (Debug::colorIndex == 255) {
      Debug::colorIndex = 0;
    }

    const auto n = std::to_string(Debug::colorIndex++);
    Debug::colors[this->name] = "\033[38;5;" + n + "m";
  }

  public:
    Debug () { init(); }

    Debug (const Str& str)
      : name(str) { init(); };

    Debug (const Str& str, std::ostream& out)
      : name(str), output(out) { init(); };

    Str ms() const {
      using namespace std::literals;
      const auto delta = (clock_t::now() - start) / 1ms;
      const auto sign = Debug::colors[this->name] + " +";

      return sign + std::to_string(delta) + "ms" + RESET;
    }

    Str replace(Str s, const Str& a, const Str& b) {
      Str::size_type pos = 0;

      while ((pos = s.find(a, pos)) != Str::npos) {
        s.replace(pos, a.length(), b);
        pos += b.length();
      }

      return s;
    }

    template <typename ...Args> 
    void operator() (Args&&... args) {
      if (!this->enabled) {
        return;
      }

      //
      // Print out the name of this instance.
      //
      const auto color = Debug::colors[this->name];
      this->output << color << this->name << RESET << " ";

      //
      // Put all the args and their types into a container
      //
      std::tuple<Args...> v { args... };

      //
      // apply() the parameters to a callable function and
      // expand them, use a expanded list to print them.
      //
      auto size = std::tuple_size<decltype(v)>::value - 1;

      auto print = [this, &size](auto&&... args) {
        (
          (
            this->output
            << args
            << (size-- ? " " : ms())
          ), ...
        );
      };

      std::apply(print, v);

      //
      // Print EOL.
      //
      this->output << std::endl;

      //
      // Reset the timer.
      //
      start = clock_t::now();
    }
};

#endif
