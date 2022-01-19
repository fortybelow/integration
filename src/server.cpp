#include <iostream>
#include <boost/asio.hpp>
#include "server.hpp"

namespace nstd
{
    void hexdump(void* pointer, int length, const std::string& prefix)
    {
        unsigned char* buffer = (unsigned char*)pointer;

        for (int i = 0, j = 0; i < length; i += 16)
        {
            std::clog << prefix;
            printf("%06x: ", i);

            for (j = 0; j < 16; j++)
                if (i + j < length)
                    printf("%02x ", buffer[i + j]);
                else
                    printf("   ");

            printf(" ");

            for (j = 0; j < 16; j++)
                if (i + j < length)
                    printf("%c", isprint(buffer[i + j]) ? buffer[i + j] : '.');

            printf("\n");
        }
    }
}

namespace net
{
    int run(int argc, char *argv[])
    {
        try
        {
            if (argc != 4)
            {
                std::cerr << "Usage: server <port> <cert> <priv-key>\n";
                return 1;
            }

            boost::asio::io_context io_context;

            net::server serve(io_context, std::atoi(argv[1]), argv[2], argv[3]);

            io_context.run();
        }
        catch (std::exception &e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }

        return 0;
    }
}
