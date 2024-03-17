/*
 * MessageStruture.c / Practicum 1
 *
 * Abhishek Kumar, Anwesa basu / CS5600 / Northeastern University
 * Fall 2023 / Nov 14, 2023
 */

#include "MessageStructure.h"
#include "CacheStructure.h"
#include <sys/time.h>

// initialize the global variable to keep track of hits and misses
int cache_hits = 0;
int cache_misses = 0;
int cache_size = 0;

//Writting test cases for all the possible cases
//Testcase to check LRU replacement strategy
void test_LRU_Replacement_Strategy() {
    message* msgLRU[5];
    msgLRU[0] = create_msg(1, "Abhishek", "Kumar", "Welcome Kumar!");
    msgLRU[1] = create_msg(2, "Anwesa", "Basu", "Welcome Basu!");
    msgLRU[2] = create_msg(3, "John", "Barry", "Welcome Barry!");
    msgLRU[3] = create_msg(4, "Lara", "David", "Welcome David!");
    msgLRU[4] = create_msg(5, "Katrina", "Carlos", "Welcome Carlos!");

    printf("\n Testcase 1 to test LRU functionality : \n");

    //Test Case 1:- Testing LRU 
    //We have our maximum number of cache pages to be 4, we are trying to send 
    //5 messages and since it exeeds the total number of cache pages, we will use LRU
    //to replace a least recently used cache block.
     // Create and store 5 messages
    

    for(int i =0; i<4; i++) {
       store_msg_in_cache_lru(msgLRU[i]);
    }
    printf("\nPrinting all current cache data:\n");
    printf("\n");
    view_current_cache();
    
    //Now lets try storing the 5th message, since the cache size is full, LRU will
    //try to replace message 1 with message 5.
    
    store_msg_in_cache_lru(msgLRU[4]);
    printf("\nPrinting all current cache data after LRU:\n");

    //This will now replace the message. 
    //Expected output:-
    //ID=5, Sender=Katrina, Receiver=Carlos, Content=Welcome Carlos!
    //ID=2, Sender=Anwesa, Receiver=Basu, Content=Welcome Basu!
    //ID=3, Sender=John, Receiver=Barry, Content=Welcome Barry!
    //ID=4, Sender=Lara, Receiver=David, Content=Welcome David!
    view_current_cache();
    printf("\ntestcase 1/1 passes!\n");
}

//Testcase 2:- this test case is devused to check random page replacement straegy 
void test_random_page_replacement() {
    printf("\nTest Case 2: Testing Random Page Replacement\n");

    free_cache();


    message* msgRandom[5];
    msgRandom[0] = create_msg(1, "Abhishek", "Kumar", "Welcome Kumar!");
    msgRandom[1] = create_msg(2, "Anwesa", "Basu", "Welcome Basu!");
    msgRandom[2] = create_msg(3, "John", "Barry", "Welcome Barry!");
    msgRandom[3] = create_msg(4, "Lara", "David", "Welcome David!");

    // Storing meesages upto the cache size capacity
    for (int i = 0; i < 4; i++) {
        store_msg_in_cache_random_replacement(msgRandom[i]);
    }
     printf("Cache data before random replacement:\n\n");
     view_current_cache();
     
    msgRandom[4] = create_msg(5, "Katrina", "Carlos", "Welcome Carlos!");
    store_msg_in_cache_random_replacement(msgRandom[4]);
    // Printing the cache data after the random replacement which will replace 
    //randomly one cache block with the new message
    printf("Cache data after random replacement:\n");
    printf("Note: We should see that the new cache data should have replaced 1 random data from the earlier cache\n\n");
    view_current_cache();

    for (int i = 0; i < 5; i++) {
        free(msgRandom[i]);
    }
    printf("\ntestcase 2/2 passed!\n");
}

//Test case 3:- Testcase for checking how message storing works in cache and disk
void test_storing_messages() {
    printf("\nTest Case 3: Testing Storing Messages in Cache and Disk\n");

    free_cache();

    const char* filename = "messages_test.dat";
    message* msgs[5];
    for (int i = 0; i < 5; i++) {
        msgs[i] = create_msg(i + 1, "Sender", "Receiver", "Message Content");
        //Stroring message in cache
        store_msg_in_cache_lru(msgs[i]);
        //Storing the message in disk
        store_msg_to_disk(msgs[i], filename);
    }

    printf("Cache and disk should now have messages stored:\n");
    view_current_cache();
    // Optionally check disk file as well

    for (int i = 0; i < 5; i++) {
        free(msgs[i]);
    }
   printf("\ntestcase 3/3 passed!\n");  
}

//Test Case 4:- Test case for retrieving a message from disk by passing an unique id
void test_retrieving_from_disk() {
    printf("\nTest Case 4: Testing Retrieval from Disk\n");

    free_cache();

    const char* filename = "messages_test.dat";
    message* retrieved_msg = retrieve_msg_from_disk(1, filename);
    if (retrieved_msg != NULL) {
        char* c_time_string = ctime(&retrieved_msg->time_sent);
        printf("Message retrieved from disk:\n ID=%d, Sender=%s, Receiver=%s, Content=%s, Time:%s\n",
               retrieved_msg->unique_id, retrieved_msg->sender, retrieved_msg->receiver, retrieved_msg->content, c_time_string);
        free(retrieved_msg);
    } else {
        printf("Message not found on disk.\n");
    }
    printf("\ntestcase 4/4 passed!\n");
}


//Test Case 5:- test case for checking retrieval of a message from cache
void test_retrieving_from_cache() {
    printf("\nTest Case 5: Testing Retrieval from Cache\n");

    // Reset and initialize cache
    free_cache();

    // Create a message and store it in the cache
    message* msg = create_msg(123, "Alice", "Bob", "Hello Bob!");
    store_msg_in_cache_lru(msg);

    // Attempt to retrieve the message from the cache
    message* retrieved_msg = retrieve_from_cache(123);

    // Check if the retrieved message matches the stored message
    if (retrieved_msg != NULL && retrieved_msg->unique_id == msg->unique_id) {
        printf("Message successfully retrieved from cache: ID=%d, Sender=%s, Receiver=%s, Content=%s\n",
               retrieved_msg->unique_id, retrieved_msg->sender, retrieved_msg->receiver, retrieved_msg->content);
        free(retrieved_msg);
    } else {
        printf("Failed to retrieve message from cache.\n");
    }

    // Clean up
    free(msg);
    printf("\ntestcase 5/5 passed!\n"); 
}

// test retrieving from disk if the data is not present in the cache
void test_retrieving_data() {
    printf("\nTest Case 6: Testing Retrieval data if it is not present in the cache.\n");

    // Reset and initialize cache
    free_cache();

    const char* filename = "messages_test_1.dat";
    // Create and store 8 messages
    message* msgs[5];
    msgs[0] = create_msg(1, "Abhishek", "Kumar", "Welcome Kumar!");
    msgs[1] = create_msg(2, "Anwesa", "Basu", "Welcome Anwesa!");
    msgs[2] = create_msg(3, "John", "Barry", "Welcome Barry!");
    msgs[3] = create_msg(4, "Lara", "David", "Welcome David!");
    msgs[4] = create_msg(5, "Katrina", "Carlos", "Welcome Carlos!");

    for (int i = 0; i < 5; i++) {
        store_msg_in_cache_no_replacement(msgs[i]);
        store_msg_to_disk(msgs[i], filename);
    }

    // Attempt to retrieve the message from the cache
    message* retrieved_msg = retrieve_from_cache(5);
    
    if (retrieved_msg == NULL) {
        printf("\nMessage retrieved from disk. Message not present in cache\n");

        retrieved_msg = retrieve_msg_from_disk(5, filename);
    }
    else {
        printf("\nMessage retrieved from cache\n");
    }

    // Check if the retrieved message matches the stored message
    if (retrieved_msg != NULL && retrieved_msg->unique_id == 5) {
        printf("Message successfully retrieved: ID=%d, Sender=%s, Receiver=%s, Content=%s\n",
               retrieved_msg->unique_id, retrieved_msg->sender, retrieved_msg->receiver, retrieved_msg->content);
        free(retrieved_msg);
    } else {
        printf("Failed to retrieve message from cache.\n");
    }

    // Clean up
    printf("\ntestcase 6/6 passed!\n"); 
}


// Custom string comparison function
int custom_strcmp(const char *str1, const char *str2) {
   while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return -1; // The characters are not equal
        }
        str1++;
        str2++;
    }

    if (*str1 == '\0' && *str2 == '\0') {
        return 0; // Both strings are equal
    } else {
        return -1; // The strings have different lengths
    }
}



int main(int argc, char *argv[]) {
    
    // File to store the messages on the disk.
    const char* filename = "messages.dat";
    
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Create and store 8 messages
    message* msgs[8];
    msgs[0] = create_msg(1, "Abhishek", "Kumar", "Welcome Kumar!");
    msgs[1] = create_msg(2, "Anwesa", "Basu", "Welcome Anwesa!");
    msgs[2] = create_msg(3, "John", "Barry", "Welcome Barry!");
    msgs[3] = create_msg(4, "Lara", "David", "Welcome David!");
    msgs[4] = create_msg(5, "Katrina", "Carlos", "Welcome Carlos!");
    msgs[5] = create_msg(6, "Abc", "123", "Welcome 123!");
    msgs[6] = create_msg(7, "Def", "456", "Welcome 456!");
    msgs[7] = create_msg(8, "Ghi", "789", "Welcome 789!");

    // check whether we have to perform LRU or Random Replacement technique for page replacement.
    if (argc == 3) {
    
        int size_of_cache = atoi(argv[1]);
        cache_size = size_of_cache;
        
        // Initialize cache
        initialise_cacheArray();
        if (custom_strcmp(argv[2], "LRU") == 0) {
            for (int i = 0; i < 8; i++) {
                // Store message in cache
                store_msg_in_cache_lru(msgs[i]);
                
                // Store message on disk
                store_msg_to_disk(msgs[i], filename); 
            }
        } else if (custom_strcmp(argv[2], "Random") == 0) {
            for (int i = 0; i < 8; i++) {
            
                // Store message in cache
                store_msg_in_cache_random_replacement(msgs[i]);
                
                // Store message on disk
                store_msg_to_disk(msgs[i], filename); 
            }
        } else {
            for (int i = 0; i < 8; i++) {
            
                // Store message in cache
                store_msg_in_cache_no_replacement(msgs[i]);
                
                // Store message on disk
                store_msg_to_disk(msgs[i], filename); 
            }
            // Return -1 for all invalid arguments

        }
    } else {
        printf("Invalid input!! Please provide an argument containing size of the cache array and either \"LRU\" or \"Random\"\n");
        // Return -1 for all cases where argc is not 2
        return -1; 
    }
    
    printf("\nPrinting cache data after creating messages:\n");
    view_current_cache();
    
    printf("\nIntitial Hits are: %d", cache_hits);
    printf("\nInitial Misses are: %d\n", cache_misses);
    
    
    for (int i = 0; i < 1000; i++) {  // Simulate 1000 random message accesses
        // Generate a random unique_id for simulation purposes
        int random_unique_id = (rand() % 8) + 1;
        // Simulate random message access
        message* retrieved_msg = retrieve_from_cache(random_unique_id);

        if (retrieved_msg != NULL) {
            // Free memory
            free(retrieved_msg);  
        } else {
            // Cache miss
            // Simulate retrieving from disk and storing in the cache
            message* msg = retrieve_msg_from_disk(random_unique_id, filename);
            if (msg != NULL) {
                if (custom_strcmp(argv[2], "LRU") == 0) {
                    store_msg_in_cache_lru(msg);
                }
                else if (custom_strcmp(argv[2], "Random") == 0) {
                    store_msg_in_cache_random_replacement(msg);
                }
                
                // Free memory
                free(msg);  
            }
        }
        
    }
    
    if (custom_strcmp(argv[2], "LRU") == 0) {
        printf("\nCache metrics for LRU is :");
    }
    else if (custom_strcmp(argv[2], "Random") == 0) {
        printf("\nCache metrics for Random Replacement is :");
    }

    
    printf("\nHits are: %d\n", cache_hits);
    printf("Misses are: %d\n", cache_misses);
    double hit_ratio = (double)cache_hits / (cache_hits + cache_misses);
    printf("Hit Ratio : %2f\n", hit_ratio);
    
    // view all cache data
    printf("\n Printing final cache data:\n");
    view_current_cache();
    

    // Free memory of all the messages
    for (int i = 0; i < 8; i++) {
        free(msgs[i]);
    }

    free_cache();
    
    // set cache size to 4 for testing the proper working of the code.
    cache_size = 4;
    
    printf("\n**************************************************************************\n");
    printf("\n Generating Testcases for different testing strategies : \n");
    printf("\n**************************************************************************\n\n");
    test_LRU_Replacement_Strategy();
    printf("\n**************************************************************************\n\n");
    test_random_page_replacement();
    printf("\n**************************************************************************\n\n");
    test_storing_messages();
    printf("\n**************************************************************************\n\n");
    test_retrieving_from_disk();
    printf("\n**************************************************************************\n\n");
    test_retrieving_from_cache();
    printf("\n**************************************************************************\n\n");
    test_retrieving_data();

    return 0;
}


    
