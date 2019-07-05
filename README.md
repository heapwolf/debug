# SYNOPSIS
A small debugging library inspired by [this][0].

# MOTIVATION
I was trying to log  something in C++ and remembered two things... 1. the
only way to log  anything in C++ is with a huge obnoxious framework and 2.
seriously, GTFO.

# USAGE
This module is designed to work with the [`datcxx`][0] build tool. To add this
module to your project us the following command...

```bash
build add heapwolf/debug
```

Construct an instance of `Debug` with a name. The instance will allow you to
toggle the debug output for different parts of your program.

The DEBUG environment variable is used to enable these based on delimited names.

#### COMMAND
```bash
DEBUG=bands ./musicprogram
```

#### CODE
```c++
#include "./deps/heapwolf/debug/index.hxx"

Debug debug("demo");
Debug debug3("demo:beep");

void f3 () {
  Debug debug2("demo:boop");
  debug2("running function f3");
  debug3("running function f3");
}

void f2 () {
  debug("running function f2");
  f3();
}

int main () {
  debug("starting program");
  f2();
  debug("ending program");
  return 0;
}
```

When actively developing an application it can be useful to see when the time
spent between one debug() call and the next. Suppose for example you invoke
debug(...) before requesting a resource, and after as well, the "+NNNms" will
show you how much time was spent between calls.

#### OUTPUT
![example](/docs/screenshot.png)


# TEST

```bash
build test
```


# API

## CONSTRUCTOR

### Debug d(const std::string& name[, std::ostream& stream])
Construct with a name, optionally specify an output stream.

[0]:https://github.com/visionmedia/debug
