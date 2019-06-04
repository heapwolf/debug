#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <variant>
#include <tuple>

class Debug {
  std::string name = "";
  std::ostream& output = std::cout;

  public:

    Debug ();

    Debug (const std::string& str)
      : name(str) {};

    Debug (const std::string& str, std::ostream& out)
      : name(str), output(out) {};

    template <typename ...Args> 
    void operator() (Args&&... args) {
      auto envp = std::getenv("DEBUG");

      if (envp != nullptr) {
        auto env = std::string(envp);

        if (env.find(this->name) == std::string::npos) {
          return; // the env var does not contain the name
        }
      }

      //
      // Print out the name of this instance.
      //
      this->output << this->name << " ";

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
        ((this->output << args << (size-- == 0 ? "" : " ")), ...);
      };

      std::apply(print, v);

      //
      // Print EOL.
      //
      this->output << std::endl;
    }
};

#endif
