#pragma once
#include <vector>
#include <type_traits> 
#include<assert.h>
using namespace std;


typedef uint16_t  book_id_t;
typedef uint32_t level_id_t;
typedef uint32_t order_id_t;
typedef char BUY_SELL;
typedef uint64_t timestamp_t;
typedef uint64_t oid_t;
typedef uint32_t price_t;
typedef int qty_t;
typedef int sprice_t;


// TODO replace casts with following:
#define MKPRIMITIVE(__x) ((std::underlying_type<decltype(__x)>::type)__x)

//#define TRACE 1

constexpr bool is_power_of_two(uint64_t n)
{  // stolen from linux header
  return (n != 0 && ((n & (n - 1)) == 0));
}

//enum class sprice_t : int{};

bool constexpr is_bid(sprice_t const x) { return int32_t(x) >= 0; }
#define MEMORY_DEFS    \
  using __ptr = ptr_t; \
  
  


template <class B>
class k
{
public:
    using size_t__ = typename  underlying_type<B>::type;
    
};
template <class T, typename ptr_t, size_t SIZE_HINT>
class pool
{
 public:
 MEMORY_DEFS;
 typedef ptr_t size_t__;
  std::vector<T> m_allocated;
  std::vector<ptr_t> m_free;
  pool() { m_allocated.reserve(SIZE_HINT); }
  pool(size_t reserve_size) { m_allocated.reserve(reserve_size); }
  T *get(ptr_t idx) { return &m_allocated[size_t__(idx)]; }
  T &operator[](ptr_t idx) { return m_allocated[size_t__(idx)]; }
#define ALLOC_INVARIANT \
  (m_free_size >= 0) /* aka can't free more than has been allocated */
  __ptr alloc(void)
  {
    if (m_free.empty()) {
      auto ret = __ptr(m_allocated.size());
      m_allocated.push_back(T());
      return ret;
    } else {
      auto ret = __ptr(m_free.back());
      m_free.pop_back();
      return ret;
    }
  }
  void free(__ptr idx) { m_free.push_back(idx); }
#undef ALLOC_INVARIANT
};
class level
{
 public:
  sprice_t m_price;
  qty_t m_qty;
  level(sprice_t __price, qty_t __qty) : m_price(__price), m_qty(__qty) {}
  level() {}
};


typedef struct order {
  qty_t m_qty;
  level_id_t level_idx;
  //book_id_t book_idx;
} order_t;

class price_level
{
 public:
  price_level() {}
  price_level(sprice_t __price, level_id_t __ptr)
      : m_price(__price), m_ptr(__ptr)
  {
  }
  sprice_t m_price;
  level_id_t m_ptr;
};

template <class T>
class oidmap
{
 public:
  std::vector<T> m_data;
  size_t m_size;
  void reserve(order_id_t const oid)
  {
    size_t const idx = size_t(oid);
    if (idx >= m_data.size()) {
      m_data.resize(idx + 1);
    }
  }
  T &operator[](order_id_t const oid)
  {
    size_t const idx = size_t(oid);
    return m_data[idx];
  }
  T *get(order_id_t const oid)
  {
    size_t const idx = size_t(oid);
    return &m_data[idx];
  }
};

bool operator>(price_level a, price_level b)
{
  return int32_t(a.m_price) > int32_t(b.m_price);
}

struct order_id_hash {
  size_t operator()(order_id_t const id) const { return size_t(id); }
};

//qty_t operator+(qty_t const a, qty_t const b)
//{
//  return qty_t((a) + (b));
//}

class order_book
{
 public:
  static constexpr size_t MAX_BOOKS = 1 << 14;
  static constexpr size_t NUM_LEVELS = 1 << 20;
  static order_book *s_books;  // can we allocate this on the stack?
  static oidmap<order_t> oid_map;
  using level_vector = pool<level, level_id_t, NUM_LEVELS>;
 using sorted_levels_t = std::vector<price_level>;
  // A global allocator for all the price levels allocated by all the books.
  static level_vector s_levels;
  std::vector<price_level> m_bids;
  std::vector<price_level> m_offers;
  using level_ptr_t = level_vector::__ptr;

  static void add_order(order_id_t const oid,
                        sprice_t const price, qty_t  qty)
  {
#if TRACE
    printf("ADD %lu, %u, %d, %u", oid, book_idx, price, qty);
#endif  // TRACE
    oid_map.reserve(oid);
    order *order = oid_map.get(oid);
    order->m_qty = qty;
    //order->book_idx = book_idx;

    s_books->ADD_ORDER(order, oid,price, qty);
#if TRACE
    auto lvl = oid_map[oid].level_idx;
    printf(", %u, %u \n", lvl, s_books[size_t(book_idx)].s_levels[lvl].m_qty);
#endif  // TRACE
  }
  void ADD_ORDER(order_t* order, order_id_t const oid, sprice_t  price, qty_t & qty)
  {
      auto* sorted_levels = price>0? &m_bids : &m_offers;
     
      if (is_bid(price))
      {
          auto it = m_offers.rbegin();
          while (qty != 0 && it != m_offers.rend())
          {
              if (price >= it->m_price)
              {
                  execute_order(oid, (s_levels[it->m_ptr].m_qty));
              }
              it++;

          }

      }
      else
      {
          price = price > 0 ? price : price * -1;
          auto it = m_bids.begin();

          while (qty != 0 && it != m_bids.end())
          {

              if (price <= it->m_price)
              {
                  
                  execute_order(oid, (s_levels[it->m_ptr].m_qty));
              }
              it++;

          }
      }
      if (order->m_qty)
      {
          qty = order->m_qty;
          auto insertion_point = sorted_levels->begin();
          bool found = false;
          while (insertion_point != sorted_levels->end()) {
              price_level& curprice = *insertion_point;
              if (curprice.m_price == price) {
                  order->level_idx = curprice.m_ptr;
                  found = true;
                  break;
              }
              else if (price > curprice.m_price) {
                  // insertion pt will be -1 if price < all prices
                  break;
              }
              insertion_point++;
          }
          if (!found) {
              order->level_idx = s_levels.alloc();
              s_levels[order->level_idx].m_qty = qty_t(0);
              s_levels[order->level_idx].m_price = price;
              price_level const px(price, order->level_idx);
              // ++insertion_point;
              sorted_levels->insert(insertion_point, px);
          }
          s_levels[order->level_idx].m_qty = s_levels[order->level_idx].m_qty + qty;
      }
  }

  static void delete_order(order_id_t const oid)
  {
#if TRACE
    printf("DELETE %lu\n", oid);
#endif  // TRACE
    order_t *order = oid_map.get(oid);
    s_books->DELETE_ORDER(order);
  }
  static void cancel_order(order_id_t const oid, qty_t const qty)
  {
#if TRACE
    printf("REDUCE %lu, %u\n", oid, qty);
#endif  // TRACE
    order_t *order = oid_map.get(oid);
    s_books->REDUCE_ORDER(order, qty);
  }
  // shared between cancel(aka partial cancel aka reduce) and execute
  void REDUCE_ORDER(order_t *order, qty_t  qty)
  {
    auto tmp0 = (s_levels[order->level_idx].m_qty);
   tmp0 -= order->m_qty;
    s_levels[order->level_idx].m_qty = qty_t(tmp0);

  int  tmp = (order->m_qty);
    tmp -= (qty);
    order->m_qty = tmp > 0 ? tmp : 0;
   // order->m_qty = qty_t(tmp);
  }
  // shared between delete and execute
  void DELETE_ORDER(order_t *order)
  {
   // assert((s_levels[order->level_idx].m_qty) >=
          // (order->m_qty));
    auto tmp = (s_levels[order->level_idx].m_qty);
    tmp -= (order->m_qty);
    s_levels[order->level_idx].m_qty = qty_t(tmp)>0?tmp:0;
    if (qty_t(0) == s_levels[order->level_idx].m_qty) {
      // DELETE_SORTED([order->level_idx].price);
      sprice_t price = s_levels[order->level_idx].m_price;
     // sorted_levels_t *sorted_levels = is_bid(price) ? &m_bids : &m_offers;
      sorted_levels_t* sorted_levels = &m_bids;

      auto it = sorted_levels->begin();
      while (it!= sorted_levels->end()) {
        if (it->m_price == price)
        {
          sorted_levels->erase(it);
          break;
        }
        it++;
      }
      s_levels.free(order->level_idx);
    }
  }
  static void execute_order(order_id_t const oid, qty_t & qty)
  {
#if TRACE
    printf("EXECUTE %lu %u\n", oid, qty);
#endif  // TRACE
    order_t *order = oid_map.get(oid);
    order_book *book = s_books;

    if (qty == order->m_qty) {
      book->DELETE_ORDER(order);
    } else {
      book->REDUCE_ORDER(order, qty);
    }
  }
  static void replace_order(order_id_t const old_oid, order_id_t const new_oid,
                            qty_t const new_qty, sprice_t new_price)
  {
#if TRACE
    printf("REPLACE %lu %lu %d %u\n", old_oid, new_oid, new_price, new_qty);
#endif  // TRACE
    order_t *order = oid_map.get(old_oid);
   // order_book *book = &s_books[order->book_idx];
   // bool const bid = is_bid(book->s_levels[order->level_idx].m_price);
  //  book->DELETE_ORDER(order);
   // if (!bid)
    {
      new_price = sprice_t (-new_price);
    }
//book->add_order(new_oid, order->book_idx, new_price, new_qty);
  }
};

//oidmap<order_t> oid_map; 
///
oidmap<order_t> order_book::oid_map = oidmap<order_t>();
order_book* order_book::s_books = new order_book();
order_book::level_vector order_book::s_levels = level_vector();
