/*
 * MessageStruture.h / Practicum 1
 *
 * Abhishek Kumar, Anwesa basu / CS5600 / Northeastern University
 * Fall 2023 / Nov 14, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#ifndef MESSAGESTRUCTURE_H
#define MESSAGESTRUCTURE_H


#define SENDER_LENGTH 50
#define RECEIVER_LENGTH 50
#define CONTENT_LENGTH 1024


// Data structure for a "message".
typedef struct {
    int unique_id;
    time_t time_sent;
    char sender[SENDER_LENGTH];
    char receiver[RECEIVER_LENGTH];
    char content[CONTENT_LENGTH];
    bool delivered;
    int priority;
    bool isEncrypted;
} message;

//method signatures to be used
message* create_msg(int unique_id, const char* sender, const char* receiver, const char* content);

void store_msg_to_disk(message* msg, const char* filename);

message* retrieve_msg_from_disk(int unique_id, const char* filename);

bool store_msg_in_cache_lru(message* message);

bool store_msg_in_cache_random_replacement(message* message);

bool store_msg_in_cache_no_replacement(message* message);

message* retrieve_from_cache(int unique_id);

#endif
