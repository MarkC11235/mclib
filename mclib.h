#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

// DEBUG ==========================================================================================
#define NO_COLOR "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

#define ERROR(msg, ...)                                                                    \
    do {                                                                                   \
        printf(RED"%s:%d   ERROR: "NO_COLOR msg "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        exit(1);                                                                           \
    } while(0) 

#define TODO(msg, ...)                                                          \
    do {                                                                        \
        printf("%s:%d   TODO: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__);   \
        exit(1);                                                                \
    } while(0) 

#define INFO(msg, ...)                                                          \
    do {                                                                        \
        printf("%s:%d   INFO: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__);   \
    } while(0) 
// ================================================================================================


// CONSTANTS ====================================================================================== 
#define PI 3.14159
#define e 2.71828
// ================================================================================================

// BOOL ===========================================================================================
#define bool int
#define false 0
#define true 1
// ================================================================================================

// Types ==========================================================================================
#define u8 uint8_t
#define u32 uint32_t
#define u64 uint64_t
#define i64 int64_t
#define f32 float
// ================================================================================================

// CONVERSIONS ====================================================================================
#define DEG2RAD(deg) ((deg) * (PI/180))
#define RAD2DEG(rad) ((rad) * (180/PI)) 
// ================================================================================================

// LIST ===========================================================================================
// must have allocated memory > 0 for the items or else many of these macros will segfault
// this happens because in some macros I dereference items in the list (usually to extract the type, so that the user doesn't constantly need to pass the type in functions 
// figured this is ok since it doesn't really make sense to have a list that can't contain anything

#define def_list(type)  \
    typedef struct {    \
        type* items;    \
        uint64_t count; \
        uint64_t cap;   \
    } type##_list

#define list(type) type##_list

#define lcreate(type, size)                            \
    {                                                  \
        .items = (type*) calloc(size, sizeof(type)),   \
        .count = 0,                                    \
        .cap = size                                    \
    } 

#define lexpand(list, size) \
    (list).cap = (size);                                                                 \
    (list).items = (typeof((list).items)) realloc((list).items, (size) * sizeof(typeof((list).items[0])))

#define ldestroy(list) free((list).items)

#define lpush(list, item)                       \
    do {                                        \
        if((list).count >= (list).cap){             \
            lexpand(list, (list).count * 1.5);    \
        }                                       \
        (list).items[(list).count++] = (item);      \
    } while(0) 

#define lpushv(list, ...)                                            \
    do {                                                             \
        typeof((list).items[0]) _tmp[] = { __VA_ARGS__ };             \
        for (u64 _pushv_iter = 0; _pushv_iter < sizeof(_tmp)/sizeof(_tmp[0]); _pushv_iter++)\
            lpush((list), _tmp[_pushv_iter]);                                  \
    } while (0)

#define lpop(list, var_name) \
    do{ \
        if((list).count <= 0){ \
            ERROR("Cannot pop from an empty list"); \
        } \
        else {  \
            var_name = (list).items[((list).count - 1)]; \
            (list).count--;                            \
        } \
    } while(0) 

#define lget(list, index, var_name)  \
    do{ \
        if(index >= (list).count){ \
            ERROR("Out of range at index %" PRIu64 , index); \
        } \
        else {  \
            var_name = (list).items[index]; \
        } \
    } while(0) 

// the item that the list stores must have a field named name
#define lgetbyname(list, n, var_name)               \
    do{                                             \
        bool found_ = false;                        \
        foreach(item_, list, item_iter_,            \
            if(strcmp(item_.name, n) == 0){         \
                lget(list, item_iter_, var_name);   \
                found_ = true;                      \
                break;                              \
            }                                       \
        );                                          \
        if(!found_){                                \
            ERROR("Item with name: %s, was not found\n", n); \
        }                                                    \
    } while(0) 


#define lgetref(list, index, var_name)  \
    do{ \
        if(index >= (list).count){ \
            ERROR("Out of range at index %" PRIu64 , index); \
        } \
        else {  \
            var_name = &((list).items[index]); \
        } \
    } while(0) 


#define lset(list, index, item) \
    do{ \
        if(index >= (list).count){ \
            ERROR("Out of range at index %" PRIu64, index); \
        } \
        else {  \
            (list).items[index] = item; \
        } \
    } while(0) 


#define ITEM_TO_STR_BUF_LEN 20
#define itostr(buf, i) snprintf(buf, sizeof(char) * ITEM_TO_STR_BUF_LEN, "%d", i)
#define ftostr(buf, f) snprintf(buf, sizeof(char) * ITEM_TO_STR_BUF_LEN, "%f", f)
#define ctostr(buf, c) snprintf(buf, sizeof(char) * ITEM_TO_STR_BUF_LEN, "%c", c)

#define lprint(list, item_to_str)                               \
    do {                                                        \
        printf("[ ");                                           \
        char buf[ITEM_TO_STR_BUF_LEN];                          \
        for(uint64_t i = 0; i < (list).count - 1; i++){                \
            item_to_str(buf, (list).items[i]);                    \
            printf("%s, ", buf);                                \
        }                                                       \
        item_to_str(buf, (list).items[list.count - 1]);           \
        printf("%s ]\n", buf);                                  \
    } while(0)

// make the foreach loops accept the iter variable name, so that these loops can be nested
#define foreach(item, list, _iter_name, ...)                                   \
    do {                                                                        \
        for(uint64_t _iter_name = 0; _iter_name < (list).count; _iter_name++){  \
            typeof(*((list).items)) item = (list).items[_iter_name];              \
            __VA_ARGS__                                                                \
        }                                                                       \
    } while(0)

// ==================================================================================================


// String ===========================================================================================
#define def_string def_list(char)
def_string;
#define string list(char)
#define screate(size) lcreate(char, size)
#define sdestroy(str) ldestroy(str)

#define sappend(str, newstr) \
    do {                     \
        for(int i = 0; (newstr)[i] != '\0'; i++){ \
            lpush(str, (newstr)[i]); \
        }   \
    } while(0)

#define sprint(str)                         \
    do {                                    \
        if((str).count < (str).cap) {         \
            (str).items[(str).count] = '\0';  \
        }                                   \
        else{                               \
            lexpand(str, (str).count + 1);  \
            (str).items[(str).count] = '\0';            \
        }                                   \
        printf("%s\n", (str).items);         \
    } while(0)

// ==================================================================================================

#endif // COMMON_H


