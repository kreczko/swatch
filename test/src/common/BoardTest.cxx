#include <cstdlib>
#include <iostream>
using namespace std;


class BoardTest {
public:
  BoardTest() {
  
  }
  virtual ~BoardTest() {
    
  }
  
  void run() {
    cout << "aa " << endl;
  }
  
};

/*
 * 
 */
int main(int argc, char** argv) {
  
  BoardTest bt;
  bt.run();

  return 0;
}

