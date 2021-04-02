#include <iostream>
#include <sstream>

#include <pigpiod_if2.h>

const unsigned int maxRow = 8;

void SetRow(const int pi, const int handle,
	    const unsigned int row, const unsigned int value) {
  char buffer[2];

  buffer[0] = row;
  buffer[1] = value;

  const int result = spi_write(pi, handle, buffer, 2);
  if( result != 2 ) {
    std::stringstream msg;
    msg << "spi_write Failed: "
	  << pigpio_error(result);
    throw std::runtime_error(msg.str());
  }
}


void RunOnConsole(const int pi, const int handle) {
  std::cout << "Entering main loop" << std::endl;
  bool done = false;
  
  while( !done ) {
    std::string inputLine;
    
    std::getline( std::cin, inputLine );
    if( inputLine == "q" ) {
      std::cout << "Received quit" << std::endl;
      done = true;
    } else {
      try {
	std::stringstream l(inputLine);
	for( unsigned int i=0; i<maxRow; i++ ) {
	  unsigned int nxtValue;
	  l >> nxtValue;
	  
	  SetRow(pi, handle, i, nxtValue);
	}
      }
      catch( std::exception& e ) {
        std::cerr << e.what() << std::endl;
        continue;
      } 
    }
  }
}


int main() {
  std::cout << "LED 8x8 SPI Driver" << std::endl;

  const int piId = pigpio_start(nullptr, nullptr);
  if( piId< 0 ) {
    std::cerr << "Could not connect to pigpiod."
	      << "Have you run 'sudo pigpiod' ?"
	      << std::endl;
    return(-1);
  }

  // ===========================================

  try {
    const unsigned int spiChannel = 0;
    const unsigned int baud = 32768;
    unsigned int flags = 0;

    const unsigned int ce0ActiveHigh = 1 << 2;
    const unsigned int auxSPI = 1 << 8;

    const unsigned int copiLSBfirst = 1 << 14;

    flags = ce0ActiveHigh | auxSPI | copiLSBfirst;

    const int handle = spi_open(piId, spiChannel, baud, flags);
    if( handle < 0 ) {
      std::stringstream msg;
      msg << "spi_open Failed: "
	  << pigpio_error(handle);
      throw std::runtime_error(msg.str());
    }

    RunOnConsole(pi, handle)

    spi_close(piId, handle);
  }
  catch( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }
  

  // ===========================================

  pigpio_stop(piId);
  
  return 0;
}
