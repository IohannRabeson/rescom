#include "rescom.hpp"

#include <cassert>

int main() {
    auto const& slot = rescom::getResource("test.txt");

    assert( std::string_view(slot.buffer, slot.size) == "Hello world!" );

    return 0;
}

