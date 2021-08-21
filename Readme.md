This is a very fast implementation of the order book offering fast updates and O(1) access to any price level (to get the price is a single dereference, the aggregate quantity is another dereference). It only calculates the aggregate quantities at each price and does not track the queue depth for each order.

Among other things, there is no hashtable or trees used, only <vector>s and C arrays, and no allocation beyond what <vector> does. For description and implementation 
  
  Unordered_map(Hashing) vs Vector:
  
  Order ID used as key created by hash function that points to bucket in which value exists, Millions of order ID as key are assigned with dynamically scattered memory.
  Hash keys are forwared iterator pointers.
  
  With Vector memory for Millions of ID are reserved which are contingeous , thus are cache efficient and provide locality of reference.
  However memory consumed are more as compared to hashmap , since each index has a memory of data type of it.
  
  Optimization:
  
  Each Order ID has a level_ID that map to price that is provided with level value from the memory pool( starting from 0) , thus Orders are close to each other when added and indexed.
  Insertion is always at the end of level index. provide better TLB and cache.
  
  Deleted level index and cached and pooled further.
  
  Sorted_levels are of buy and sell vector in the order of price/time.
  
  Deletion from the sorted level vector are O(n) operation.
  
  Addition and indexing are O(1) operation with better TLBS and cache efficinecy.
  
