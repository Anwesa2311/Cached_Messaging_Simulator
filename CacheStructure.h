/*
 * CacheStructure.h / Practicum 1
 *
 * Abhishek Kumar, Anwesa basu / CS5600 / Northeastern University
 * Fall 2023 / Nov 14, 2023
 */
 
 
#define Message_Size 1024

typedef struct
{
    
    int uniqueId;
    bool is_occupied;
    char messageData[Message_Size];
    //time_t last_accessed;
    struct timeval last_accessed;


} cacheStructure;

//global variable to store cache size, hits and misses
extern int cache_size;
extern int cache_hits;
extern int cache_misses;

//method signatures to be used
void initialise_cacheArray();
void view_current_cache();
void free_cache();


/*

Strategy used for caching mechanism: -
 
We have used a fixed size array named cacheArray of cacheStructure to store the messages. If the number of messages exceeds the total size of the array, we use a page replacement policy.

We have implemented below methods for the same:

    1. store_msg_in_cache_lru:- This method checks if the size exceeds the capacity to store new messages, it then replaces the least   recently used block. We are using last_accessed to store the timestamp of each access to a cache block and the based on the least recently accessed time stamp, we are replacing the blocks.
    
    2. store_msg_in_cache_random_replacement:- This method performs a random operation and generates a random number which can be considered as an index of the array to replace it with the new content.
    
    
Below are the reason why we chose the above strategy:-
    Advantages:-
        1. Direct Access:- We will be requiring o(1) time to access element from a cache. So, it is fast and efficient. ( Since, in array it takes constant time to access one element based on its index)
        
        2. Ease of Implementation/ Maintenance: - The use of array reduces the complex code structure and makes the code simpler. The fixed size array makes the implementation straightforward and easier. It also makes debugging easier compared to more complex data structures.
        3. Well suited for predictable cache size:- We have been given an option to select a fixed size for our cache. In this scenario, where data set isn't excessively large, our approach is more appropriate. For applications where the cache size is relatively small or medium, this approach is quite effective. It provides a good balance between performance and complexity.
        4. Performance tuning:- Since, the performance of the cache is more deterministic in nature, as the time complexity for all the operations are predictable, we can make the performance tuning more straightforward.
 
 
Alternative Approach:-
    1. An alternative approach would be using HashMap for fast lookup and LinkedList to maintain the order of the elements.

Reason for not choosing the approach:-

    1. This approach may require more dynamic memory allocation and that can be more resource intensive than fixed size array.
    2. If, the cache size is predictable and small, then the benefits of a hash table's faster lookup might not be as effective, which makes the simpler array-based approach the winner.
    3. The usage of two data structures and maintaining them can be more complex and may require potential more development time and debugging issues.
 
*/
 
 
