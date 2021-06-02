#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

struct mycode_ {
    int Id_Packet;
    int Value;
    std::string Name;
};

int main(int argc, char* argv[])
{
    try
    {
        /*if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }*/

        boost::asio::io_context io_context;
        
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve("127.0.0.1", "daytime");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            mycode_ my;
            
            memcpy(&my, buf.data(), sizeof(my));
            std::cout <<"Packet Id:"<< my.Id_Packet << std::endl << "Packet Name:" << my.Name << std::endl << "Packet Value:" << my.Value << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}