#include <chrono>
#include <cstdio>
#include <limits>
#include <memory>
#include<string>
#include "itch.h"
#include "order_book.h"
#include<iostream>
using namespace std;
/*
template <itch_t __code>
class PROCESS
{
 public:
  static itch_message<__code> read_from(buf_t *__buf)
  {
    uint16_t const msglen = be16toh(*(uint16_t *)__buf->get(0));
    __buf->advance(2);
    assert(msglen == netlen<__code>);

    __buf->ensure(netlen<__code>);
    itch_message<__code> ret = itch_message<__code>::parse(__buf->get(0));
    __buf->advance(netlen<__code>);
    return ret;
  }
}*/;
//enum class BUY_SELL : char { BUY = 'B', SELL = 'S' };
//enum class timestamp_t : uint64_t {};
//enum class oid_t : uint64_t {};
//enum class price_t : uint32_t {};
//enum class qty_t : uint32_t {};


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
#else
  // order_book::oid_map.max_load_factor(0.5);
 // order_book::oid_map.reserve(order_id_t(1));  // the first number
                                                           // is the empirically
                                                           // largest oid seen.
                                                           // multiply by 2 for
                                                           // good measure

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

      //switch (action) {
      //case ('A'): 
      //{
      //    if (!npkts) {
      //        start = std::chrono::steady_clock::now();
      //        ++npkts;
       //}

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
 


//      case (itch_t::ADD_ORDER_MPID): {
//        auto const pkt = PROCESS<itch_t::ADD_ORDER_MPID>::read_from(&buf);
//#if BUILD_BOOK
//        order_book::add_order(
//            order_id_t(pkt.add_msg.oid), book_id_t(pkt.add_msg.stock_locate),
//            mksigned(pkt.add_msg.price, pkt.add_msg.buy), pkt.add_msg.qty);
//#else
//        ++nadds;
//#endif
//        break;
//      }
//      case (itch_t::EXECUTE_ORDER): {
//        auto const pkt = PROCESS<itch_t::EXECUTE_ORDER>::read_from(&buf);
//#if BUILD_BOOK
//        order_book::execute_order(order_id_t(pkt.oid), pkt.qty);
//#endif
//        break;
//      }
//      case (itch_t::EXECUTE_ORDER_WITH_PRICE): {
//        auto const pkt =
//            PROCESS<itch_t::EXECUTE_ORDER_WITH_PRICE>::read_from(&buf);
//#if BUILD_BOOK
//        order_book::execute_order(order_id_t(pkt.exec.oid), pkt.exec.qty);
//#endif
//        break;
//      }
//      case (itch_t::REDUCE_ORDER): {
//        auto const pkt = PROCESS<itch_t::REDUCE_ORDER>::read_from(&buf);
//#if BUILD_BOOK
//        order_book::cancel_order(order_id_t(pkt.oid), pkt.qty);
//#endif
//        break;
//      }
//      case (itch_t::DELETE_ORDER): {
//        auto const pkt = PROCESS<itch_t::DELETE_ORDER>::read_from(&buf);
//#if BUILD_BOOK
//        order_book::delete_order(order_id_t(pkt.oid));
//#endif
//        break;
//      }
//      case (itch_t::REPLACE_ORDER): {
//        auto const pkt = PROCESS<itch_t::REPLACE_ORDER>::read_from(&buf);
//#if BUILD_BOOK
//        order_book::replace_order(order_id_t(pkt.oid),
//                                  order_id_t(pkt.new_order_id), pkt.new_qty,
//                                  mksigned(pkt.new_price, BUY_SELL::BUY));
//#endif
//        // actually it will get re-signed inside. code smell
//        break;
//      }
//      default: {
//        printf("Uh oh bad code %d\n", char(msgtype));
//        assert(false);
//        break;
//      }
//    }
 //}
 fclose;
 
 // cout << (start - end);
 /* size_t nanos =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  printf("%lu packets in %lu nanos , %.2f nanos per packet \n", npkts, nanos,
         nanos / (double)npkts);*/
  return 0;
}

//#include <iostream>
//#include<vector>
//using namespace std;
//typedef uint16_t  book_id_t;
//typedef uint32_t level_id_t;
//typedef uint32_t order_id_t;
//typedef char BUY_SELL;
//typedef uint64_t timestamp_t;
//typedef uint64_t oid_t;
//typedef uint32_t price_t;
//typedef uint32_t qty_t;
//
//typedef struct order {
//    qty_t m_qty;
//    level_id_t level_idx;
//    book_id_t book_idx;
//} order_t;
//
//template <class T>
//class oidmap
//{
//public:
//    std::vector<T> m_data;
//    size_t m_size;
//    void reserve(order_id_t const oid)
//    {
//        size_t const idx = size_t(oid);
//        if (idx >= m_data.size()) {
//            m_data.resize(idx + 1);
//        }
//    }
//};
//
