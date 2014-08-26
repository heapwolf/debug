# SYNOPSIS
A quick and **SIMPLE** header only debug module. I was trying to log 
something in C++ and remembered two things... 1. the only way to log 
anything in C++ is with a huge obnoxious framework and 2. wtf.

# USAGE
```cpp
  Debug d("name", Debug::verbose, cout);

  d << d.info << "danzig" << endl;
  d << d.warn << "is basically a midget" << endl;
```

# API

## CONSTRUCTOR

### Debug d(name, level, stream)
Create an instance. The first argument is the `name` of the instance.
The second is the level. The third is the output stream, which could
be any writable stream. great. now you can go back to what you were
actually working on.

## INSTANCE MEMBERS

### error, warn, info, verbose
Various levels. Just pass these into the stream when you want to change
the level. The default level is `verbose`.

### name
The name of the current stream.

### level
The current level of the stream.

## STATIC MEMBERS

### quiet, error, warn, info, verbose
Various level options for passing to the constructor.

## INSTANCE METHODS

### format(lambda)
If you want to call a common function each time a log is written to the 
stream. just supply a callback. Of course you could do something more 
interesting here, like turn it into [`JSON`](https://github.com/dropbox/json11)!

```cpp
  d.format([](auto name, auto level, auto data) {
    return name + ":" + level + ":" + data;
  });
```

