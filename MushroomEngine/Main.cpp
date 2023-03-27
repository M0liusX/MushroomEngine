#pragma warning(disable : 26812) // Warning C26812: Prefer 'enum class' over 'enum' (Enum.3)

#include "MeAppBase.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
   me::MeAppBase app{};

   try {
      app.run();
   }
   catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}