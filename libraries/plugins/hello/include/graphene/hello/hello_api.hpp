
#pragma once

#include <fc/api.hpp>
#include <string>
#include <graphene/app/database_api.hpp>

using namespace std;

/*
 * We need this so we can refer to graphene::app::application without including the entire header
 */
namespace graphene { namespace app {
class application;
} }

namespace graphene { namespace wallet {
        class wallet_api;
    } }

namespace graphene {
    namespace app {
        class database_api_impl;
    }
}

/*
 * Our basic namespace
 */
namespace graphene { namespace hello {

namespace detail {
 /* The "detail" is used to namespace the actual implemention and separated it
  * from the interface
  */
 class hello_api_impl;
}

class hello_api
{
   public:
      /**
       * The API requires a constructor which takes app.
       */
      hello_api( graphene::app::application& app );

      /**
       * Called immediately after the constructor. If the API class uses enable_shared_from_this,
       * shared_from_this() is available in this method, which allows signal handlers to be registered
       * with app::connect_signal()
       */
      void on_api_startup();

      // TODO: Add API methods here
      uint32_t hello();
      std::string hello_denis();
      std::string hello_transfer(std::string from, std::string to, std::string amount,
                                 std::string asset_symbol);

   private:
      /*
       * The implementation instance is stored privately in `my`
       */
      std::shared_ptr< detail::hello_api_impl > my;
};

} }

/*
 * Reflection for C++ allowing automatic serialization in Json & Binary of C++
 * Structs 
 */
FC_API( graphene::hello::hello_api,
    (hello)
    (hello_denis)
    (hello_transfer)
)
