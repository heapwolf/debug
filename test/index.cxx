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
    auto expected = "foo 2 hello x 1.1\n";

    t->equal(actual, expected, "output matches");
    t->end();
  });
}
