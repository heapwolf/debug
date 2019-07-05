#include <sstream>

#include "../deps/heapwolf/cxx-tap/index.hxx"
#include "../index.hxx"

int main() {
  TAP::Test t;

  t.test("sanity", [&](auto t) {
    t->ok(true, "true is true");
    t->end();
  });

  t.test("constructor no env var", [&](auto t) {
    putenv("DEBUG=");
    std::stringstream ss;
    Debug debug("foo", ss);
    debug(2, 1, 'x');

    auto actual = ss.str();

    t->equal(actual, "", "output is empty");
    t->end();
  });

  t.test("constructor", [&](auto t) {
    putenv("DEBUG=foo");

    std::stringstream ss;
    Debug debug("foo", ss);
    debug(2, "hello", 'x', 1.1);

    auto actual = ss.str();

    t->ok(actual.find("foo") != std::string::npos, "output matches");
    t->ok(actual.find("hello x 1.1") != std::string::npos, "output matches");

    t->end();
  });

  t.test("time", [&](auto t) {
    putenv("DEBUG=foo:*:bazz");
    using namespace std::chrono_literals;

    std::stringstream ss;
    Debug x("foo:bar:bazz", ss);
    Debug y("foo:boop:bazz", ss);
    Debug z("foo:boop:nope", ss);

    for (int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(0.01s);
      x("test", rand(), "x=" + std::to_string(i));
      y("test", "y=" + std::to_string(i));
      z("test", "z=" + std::to_string(i));
    }

    auto actual = ss.str();

    for (int i = 0; i < 10; i++) {
      t->ok(actual.find("x=" + std::to_string(i)) != std::string::npos, "output matches");
      t->ok(actual.find("y=" + std::to_string(i)) != std::string::npos, "output matches");
      t->ok(actual.find("z=" + std::to_string(i)) == std::string::npos, "output matches");
    }

    t->end();
  });
}
