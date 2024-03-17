/*
 * MessageStruture.c / Practicum 1
 *
 * Abhishek Kumar, Anwesa basu / CS5600 / Northeastern University
 * Fall 2023 / Nov 14, 2023
 */
 
#include "MessageStructure.h"
#include "CacheStructure.h"


// Declare cacheArray as a global pointer
cacheStructure** cacheArray;

// initialize the cache before pushing the messages
void initialise_cacheArray() {
    
    cacheArray = (cacheStructure**)malloc(cache_size * sizeof(cacheStructure*));
    
    for (int i = 0; i < cache_size; i++) {
        cacheArray[i] = (cacheStructure*)malloc(sizeof(cacheStructure));
        if (cacheArray[i] == NULL) {
            // Handle memory allocation error
	        return;

        }
        cacheArray[i]->is_occupied = false;
        // Initialize other fields...
    }
}

// de serialized the binary message to string
void parse_message(const char* serialized, message* msg) {
    char buffer[Message_Size];
    const char* separator = ":";
    char* token;
    char* rest;

    // Make a copy of the serialized data to avoid modifying the original string
    strncpy(buffer, serialized, Message_Size);

    // Start tokenizing and parsing
    rest = buffer;

    // unique_id
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) msg->unique_id = atoi(token);

    // time_sent
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) msg->time_sent = atol(token);

    // sender
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) strncpy(msg->sender, token, SENDER_LENGTH);

    // receiver
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) strncpy(msg->receiver, token, RECEIVER_LENGTH);

    // content
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) strncpy(msg->content, token, CONTENT_LENGTH);

    // priority
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) msg->priority = atoi(token);

    // isEncrypted
    token = strtok_r(rest, separator, &rest);
    if (token != NULL) msg->isEncrypted= atoi(token);
}


// This method stores a message into a cache and returns true if it is successful.
// It follows the LRU method for page replacement
bool store_msg_in_cache_lru(message* message) {
    
    int index = -1;
    time_t current_time = time(NULL);
    
    for(int i = 0; i < cache_size; i++) {
        if(cacheArray[i]->is_occupied != true) {
            index = i;
            break;
        }
    }
    // No available slot, perform LRU replacement
    if (index == -1) {

        index = 0;
        // Stores the timestamp of the message which is least recently used.
        struct timeval oldest_time = cacheArray[0]->last_accessed;

        for (int i = 1; i < cache_size; i++) {
        
            if (cacheArray[i]->last_accessed.tv_sec < oldest_time.tv_sec ||
                (cacheArray[i]->last_accessed.tv_sec == oldest_time.tv_sec &&
                 cacheArray[i]->last_accessed.tv_usec < oldest_time.tv_usec)) {

                index = i;
                oldest_time = cacheArray[i]->last_accessed;
            }
        }

        // Release resources of the message in the replaced slot if needed
        //free(cacheArray[index]->messageData);
	}
    
    sprintf(cacheArray[index]->messageData, "%d:%ld:%s:%s:%s:%d:%d:%d", 
        message->unique_id, message->time_sent, message->sender, 
        message->receiver, message->content,message->delivered, message->priority, 
        message->isEncrypted);


    cacheArray[index]->is_occupied = true;
    cacheArray[index]->uniqueId = message->unique_id;
    // Update timestamp
    gettimeofday(&cacheArray[index]->last_accessed, NULL);

    return true;

}

// This method stores a message into a cache and returns true if it is successful.
// It follows the Random Replacement method for page replacement
bool store_msg_in_cache_random_replacement(message* message)
{
    
    int index = -1;

    for(int i = 0; i < cache_size; i++) {
        if(cacheArray[i]->is_occupied != true) {
            index = i;
            break;
        }
    }
    
    // No available slot, perform Random replacement
    if (index == -1) {
        // Generating random number
        index = rand() % cache_size;
        // Release resources of the message in the replaced slot if needed
        //free(cacheArray[index]->messageData);
	}
    
    sprintf(cacheArray[index]->messageData, "%d:%ld:%s:%s:%s:%d:%d:%d", 
        message->unique_id, message->time_sent, message->sender, 
        message->receiver, message->content,message->delivered, message->priority, 
        message->isEncrypted);


    cacheArray[index]->is_occupied = true;
    cacheArray[index]->uniqueId = message->unique_id;

    return true;
}

// This method stores a message into a cache and returns true if it is successful.
// It does not replace any data from the cache
bool store_msg_in_cache_no_replacement(message* message) {
    int index = -1;

    for(int i = 0; i < cache_size; i++) {
        if(cacheArray[i]->is_occupied != true) {
            index = i;
            break;
        }
    }
    
    // No available slot, perform Random replacement
    if (index == -1) {
        return true;
	}
    
    sprintf(cacheArray[index]->messageData, "%d:%ld:%s:%s:%s:%d:%d:%d", 
        message->unique_id, message->time_sent, message->sender, 
        message->receiver, message->content,message->delivered, message->priority, 
        message->isEncrypted);


    cacheArray[index]->is_occupied = true;
    cacheArray[index]->uniqueId = message->unique_id;

    return true;
}



//This method is used to retireve meesage from the cache if the meesage is stored in the cache.
message* retrieve_from_cache(int unique_id) {
    message* found_msg = NULL;
    for(int i =0; i < cache_size; i++) {
        if(cacheArray[i]->is_occupied && cacheArray[i]->uniqueId==unique_id) {

            found_msg = (message*)malloc(sizeof(message));
            
            if (!found_msg)
                return NULL;
                
            //Deserialising the stored message
            parse_message(cacheArray[i]->messageData,found_msg);
        }
    }
    // check if it is a hit or a miss and update the respective counter.
    if (found_msg != NULL) {
        // Cache hit
        cache_hits++;
        
    } else {
        // Cache miss
        cache_misses++;
    }
    return found_msg;
}

//This method is used to retireve meesage from the cache if the meesage is stored in the cache.
void view_current_cache() {
    message* found_msg = NULL;

    for(int i = 0; i < cache_size; i++) {
        
        if(cacheArray[i]->is_occupied != true) {
            break;
        }
        
        found_msg = (message*)malloc(sizeof(message));
        if (!found_msg)
            return;
        //Deserialising the stored message
        parse_message(cacheArray[i]->messageData,found_msg);
        char* c_time_string = ctime(&found_msg->time_sent);
        printf("ID=%d, Sender=%s, Receiver=%s, Content=%s, Time:%s\n",
               found_msg->unique_id, found_msg->sender, found_msg->receiver, found_msg->content, c_time_string);
        free(found_msg);
    }
}

// This method clears the cache. It frees the memory allocated by the cache.
void free_cache() {
    for (int i = 0; i < cache_size; i++) {
        if (cacheArray[i] != NULL) {
            free(cacheArray[i]);
            // Set to NULL after freeing
            cacheArray[i] = NULL; 
        }
    }
    free(cacheArray);
    // Re-initialize cache here if needed
    initialise_cacheArray();
}

// creates a new message with the fields set and returns a dynamically allocated message.
message* create_msg(int unique_id, const char* sender, const char* receiver, const char* content) {
	
	// allocate the memory dynamically
    message* new_msg = (message*)malloc(sizeof(message));

	// empty message
    if (new_msg == NULL) {
        return NULL;
    }

	// set the id
    new_msg->unique_id = unique_id;
    // Current time
    new_msg->time_sent = time(NULL); 
    
    // set the sender name
    strncpy(new_msg->sender, sender, sizeof(new_msg->sender) - 1);
    new_msg->sender[sizeof(new_msg->sender) - 1] = '\0';
    
    // set the receiver name
    strncpy(new_msg->receiver, receiver, sizeof(new_msg->receiver) - 1);
    new_msg->receiver[sizeof(new_msg->receiver) - 1] = '\0';
    
    // set the content
    strncpy(new_msg->content, content, sizeof(new_msg->content) - 1);
    new_msg->content[sizeof(new_msg->content) - 1] = '\0';
    
    // set delivery status
    new_msg->delivered = false;

    return new_msg;
}

// stores the message in a message store on disk
void store_msg_to_disk(message* msg, const char* filename) {
	
	// file to store the message
    FILE* file = fopen(filename, "a");
    
    if (file == NULL) {
        perror("Cannot open the file");
        return;
    }

    fwrite(msg, sizeof(message), 1, file);
    fclose(file);
}

// finds and returns a message identified by its unique id.
message* retrieve_msg_from_disk(int unique_id, const char* filename) {

    FILE* file = fopen(filename, "r");
    
    if (file == NULL) {
        return NULL;
    }

    message msg;
    message* found_msg = NULL;
    
    while (fread(&msg, sizeof(message), 1, file) == 1) {
        if (msg.unique_id == unique_id) {
            found_msg = (message*)malloc(sizeof(message));
            if (found_msg == NULL) {
                fclose(file);
                return NULL;
            }

            memcpy(found_msg, &msg, sizeof(message));
            break;
        }
    }

    fclose(file);
    return found_msg;
}
