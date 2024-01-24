#include "address.hh"
#include "socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

using namespace std;

void get_URL( const string& host, const string& path )
{
  auto addr = Address(host, "http");
  auto client = TCPSocket();
  client.connect(addr);
  auto write_line = [&](string_view s) {
    client.write(s);
    client.write("\r\n");
  };

  write_line("GET " + path + " HTTP/1.1");
  write_line("Host: " + host);
  write_line("Connection: close");
  write_line("");
  string buffer;
  while (!client.eof()) {
    client.read(buffer);
    cout << buffer;
  }

  client.close();
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
