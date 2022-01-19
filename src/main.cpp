#include "server.hpp"
#include "math.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    //shifting(argc, argv);
    
    // e.g. 8080 server.cert server.pem
    return net::run(argc, argv);
}
