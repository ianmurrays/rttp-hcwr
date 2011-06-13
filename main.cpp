#include <iostream>
#include "hcwr.h"

#define MAX_ITERATIONS 10 // Leave like this
#define MAX_RESTARTS 1000

using namespace std;

HCWR * hcwr;

void capture_sigint(int sig)
{
  cout << endl << "---> Captured SIGINT! Stopping." << endl;
  hcwr->continue_iterating = false;
}

int main (int argc, char * const argv[]) {
  string filename;
  
  if (argc != 2) 
  {
    cout << "Usage: " << argv[0] << " datafile.txt" << endl;
    exit(1);
  }
  
  filename = argv[1];
  
  hcwr = new HCWR(filename, MAX_ITERATIONS, MAX_RESTARTS);
  
  signal(SIGINT, &capture_sigint);
  
  hcwr->start();
  
  delete hcwr;
  
  return 0;
}