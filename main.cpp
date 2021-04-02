#include <iostream>

#include <pigpiod_if2.h>


int main() {
  std::cout << "LED 8x8 SPI Driver" << std::endl;

  const int piId = pigpio_start(nullptr, nullptr);
  if( piId< 0 ) {
    std::cerr << "Could not connect to pigpiod."
	      << "Have you run 'sudo pigpiod' ?"
	      << std::endl;
    return(-1);
  }



  pigpio_stop(piId);
  
  return 0;
}
