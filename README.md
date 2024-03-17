# Cached Messaging Simulator


Instructions for Running the Program:-

## Compiling the Program**

First, you need to compile the program using the provided Makefile. In your terminal, navigate to the directory containing the Makefile and source files, then run the following command:

      -> make
   
This command will compile the program according to the rules specified in the Makefile and create an executable named prog.

## Modifying the Makefile for Different Cache Policies**

Our code takes 2 command line arguments. 
    a. 1st argumentis the cache size which should be an integer
    b. The second argument is the replacement technique which can be "LRU" or "Random" 
    
    Our Makefile currently has a way to run the program with cache size 5 with the LRU (Least Recently Used) cache replacement policy. To change the cache policy to "Random Replacement", we need to modify the "run" in the Makefile.

For example, to change the cache size to "4" and the replacement technique to "Random Replacement", edit the Makefile as follows:
    
    run:
	    ./$(TARGET) 4 Random

*** Note:- if we want to run the program directly without modifying the Makefile each time, we can skip modifying the Makefile and proceed to the next step.


## Running the Program

After compiling and modifying the Makefile (if needed), we can run the program with the chosen cache replacement technique. Execute the following command in your terminal:

      -> make run
      
This command will run the program with the cache replacement technique specified in the Makefile (LRU or Random).

## Running the Program Directly (Without Modifying Makefile)

If you prefer to run the program directly without modifying the Makefile each time for different cache policies, you can execute the program with the desired replacement technique as a command-line argument. For example:

To run with LRU policy:
        ./prog 5 LRU

To run with Random policy:
        ./prog 4 Random



## Code Explanation and flow of the program(documenatation):

    a. Initialize a file to store message in the disk.
    b. Create 8 messages and allocate memory.
    c. Store message in the cache according to the selected replacement technique and also store in the disk.
    d. Print the existing cache data.
    e. Do the Evaluation and check the cache metrics for 1000 random message. If the message is found in the cache, it is a hit. If the message is not found, it is a miss and we put this missed data in the cache using appropriate technique. Then we calculate the hit ratio by => total number of hits / total attempts made (1000)
    
    f. Print cache metrics.
    g. Print final available cache data.
    h. Free all allocated memory
    i. Test all possible use cases
    
    
## Cache Metrics Observation
    
    It is my general obserevation observation that the hit ratio ranges from 0.60 to 0.65 for 1000 random generated message with 8 messages in the disk and cache size 5, using LRU replacement technique.
    
    It is my general obserevation observation that the hit ratio also ranges from 0.60 to 0.65 for 1000 random generated message with 8 messages in the disk and cache size 5, using Random replacement technique.
    
    To evaluate the cache metrics for 1000 random message. We are checking if the random generated message exists in the cache. If the message is found in the cache, it is a hit. If the message is not found, it is a miss and we put this missed data in the cache using appropriate technique. Then we calculate the hit ratio by => total number of hits / total attempts made (1000)



## Assumptions:-

***Note:- We have also added the explanation in our code:- CacheStructure.h ******

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


 

 
 




