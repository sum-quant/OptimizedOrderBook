#include <chrono>
#include <cstdio>
#include <limits>
#include <memory>
#include<string>
#include "itch.h"
#include "order_book.h"
#include<iostream>
using namespace std;

 int mksigned(sprice_t price, char  buy[5])
{
  
   // auto ret = price;
    
    if (strcmp(buy,"SELL"))
          return price;
    else
        return price*-1;
}


int main(int argc, char** argv)
{
    const string infile="orders.csv";
    char action[2];
    char  Side[5];
    order_id_t id;
    timestamp_t  timestamp;
    oid_t  oid=0;
    sprice_t  price;
    qty_t  qty;
    //book_id_t  b_id;
    
 
  size_t npkts = 0;
#define BUILD_BOOK 1
#if !BUILD_BOOK
  size_t nadds(0);
  uint64_t maxoid(0);

#endif
  printf("%lu\n", sizeof(order_book) * order_book::MAX_BOOKS);
  printf("%lu\n", sizeof(order_book) * order_book::MAX_BOOKS);
  FILE* file = fopen(infile.c_str(), "r");
  if (NULL == file)
  {
      fprintf(stderr, "Cannot open file at %s\n", infile.c_str());
      //return -1;
  }
  std::chrono::steady_clock::time_point start;
  
  while (!feof(file))
  {
     /* int nItemsRead = fscanf(file, "%u,%u,%u,%[^,],%[^,],%lu,%lu\n",
          &timestamp,&id,&b_id,&action, &Side, &price, &qty);*/
      int nItemsRead = fscanf(file, "%u,%u,%[^,],%[^,],%lu,%lu\n",
          &timestamp, &id, &action, &Side, &price, &qty);
      if (nItemsRead != 6)
      {
          std::cout << "Bad line" << endl;
      }

      
           if (!strcmp(action,"A"))
               order_book::add_order(id, mksigned(price, Side), qty);
           if (!strcmp(action, "X"))
               order_book::delete_order(id);
           if (!strcmp(action, "M"));
  
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;

  cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
  auto n=order_book::s_levels.m_allocated.size();
  for (int i = 0; i < n; i++)
  {
      auto k = order_book::s_levels.m_allocated[i];
  }
 auto b = order_book::s_levels.m_allocated[1];
 

