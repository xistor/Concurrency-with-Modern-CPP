#include <atomic>
#include <thread>
#include <iostream>
#include <mutex>

std::atomic<int> atom{0};
int somethingShared{0};

using namespace std::chrono_literals;

void writeShared(){
  somethingShared = 2011;
  atom.store(2, std::memory_order_release);
}

void readShared(){
  while(!(atom.fetch_sub(1, std::memory_order_acquire) > 0)){
    std::cout <<atom<< std::endl;
    std::this_thread::sleep_for(100ms);
  }
  std::cout << "somethingShared: " << somethingShared << std::endl;
}

int main(){

  std::cout << std::endl;

  
  std::thread t2(readShared);
  std::thread t3(readShared);
  std::thread t1(writeShared);

  t1.join();
  t2.join();
  t3.join();

  std::cout << "atom: " << atom << std::endl;

  std::cout << std::endl;

}