#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/trim.hpp>

using boost::asio::ip::tcp;

const int max_length = 1024;

class Session {

   public:
      Session(tcp::socket& iSocket)
      : mSocket(iSocket)
      {
      }

      void run()
      {
         while(true)
         {
            std::string line = nextLine();
            std::cout << ">" << line << std::endl;
            if(line.empty()) {
               std::cout << "Send Answer" << std::endl;
               std::string data=
                        "HTTP/1.1 200 OK\n"
                        "Content-Type: text/html\n"
                        "\n"
                        "<!DOCTYPE html><!DOCTYPE html>\n"
                        "<html><head>\n"
                        "<title>TinyBit</title>\n"
                        "</head><body>\n"
                        "<h1>BITtaylor @ tinybit</h1>\n"
                        "<p><a href='/on/9'>Turn on Led 1</a></p>\n"
                        "</body></html>\n"
                        "\n";
               mSocket.send(boost::asio::buffer(data));
               mSocket.shutdown(boost::asio::socket_base::shutdown_both);
               mSocket.close();
               return;
            }
         }

      }

      std::string nextLine()
      {
         std::string line;
         char c = getc();
         while (c != '\n')
         {
            if (c != '\r')
            {
               line.append(1,c);
            }
            c = getc();
         }
         return line;
      }


      char getc()
      {
         size_t length = 0;
         char data[1];
         while (length < 1)
         {
            boost::system::error_code error;
            length = mSocket.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
            {
               return '\n';
            }
            else if (error)
            {
               throw boost::system::system_error(error);
            }
         }
         return data[0];
      }


   private:
      tcp::socket& mSocket;
};

class WebServer {

   public:
      WebServer(unsigned short port)
      : mAcceptor(mIoService,tcp::endpoint(tcp::v4(), port))
      {
      }

      void run()
      {
         while (true)
         {
            tcp::socket socket(mIoService);
            mAcceptor.accept(socket);
            socket.set_option(boost::asio::socket_base::linger(false,0));
            std::cout << "New Session" << std::endl;
            Session session(socket);
            session.run();
            std::cout << "End Session" << std::endl;
         }
      }

   private:
      boost::asio::io_service mIoService;
      tcp::acceptor mAcceptor;
};









int main(int argc, char* argv[])
{
   try
   {
      WebServer webServer(8080);
      webServer.run();
   } catch (std::exception& e)
   {
      std::cerr << "Exception: " << e.what() << "\n";
   }

   return 0;
}
