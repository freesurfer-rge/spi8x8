#include <iostream>
#include <sstream>
#include <vector>

#include <pigpiod_if2.h>

const unsigned int maxRow = 8;

void SetRow(const int pi, const int handle,
	    const unsigned int row, const unsigned int value) {
  char buffer[2];

  buffer[1] = row;
  buffer[0] = value;

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
	std::vector<unsigned int> rows(maxRow);
	std::vector<unsigned int> remap { 0, 4, 2, 6, 1, 5, 3, 7};
	
	for(unsigned int i=0; i<maxRow; ++i ) {
	  l >> rows.at(remap.at(i));
	}

	for(unsigned int i=0; i<maxRow; ++i ) {
	  SetRow(pi, handle, i, rows.at(i));
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
    const unsigned int baud = 1 << 22;
    unsigned int flags = 0;

    const unsigned int mode = 0;
    const unsigned int ce0ActiveHigh = 1 << 2;
    const unsigned int auxSPI = 1 << 8;

    const unsigned int copiLSBfirst = 1 << 14;
    const unsigned int wordSize = 16 << 16;

    flags = mode | ce0ActiveHigh | auxSPI | copiLSBfirst | wordSize;

    const int handle = spi_open(piId, spiChannel, baud, flags);
    if( handle < 0 ) {
      std::stringstream msg;
      msg << "spi_open Failed: "
	  << pigpio_error(handle);
      throw std::runtime_error(msg.str());
    }

    RunOnConsole(piId, handle);

    spi_close(piId, handle);
  }
  catch( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }
  

  // ===========================================

  pigpio_stop(piId);
  
  return 0;
}
