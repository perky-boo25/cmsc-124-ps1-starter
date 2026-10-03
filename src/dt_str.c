/*
 * dt_str.c: Length-carrying strings for Unit 5, Section B.
 *
 * A C string is a null-terminated character sequence stored in an array.
 * An array expression usually converts to a pointer to its first character.
 * strlen reads only through the first zero byte.
 * A pointer does not store the array capacity.
 *
 * This type stores the length and capacity with the bytes. dt_str_len reads a
 * field. A zero byte is data. Append operations use the stored capacity.
 *
 * An implementation can store a final zero byte after the data.
 * The public interface requires callers to use dt_str_len.
 */

#include "dt.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct dt_str {
    char  *bytes;
    size_t length;
    size_t capacity;
};

/*
 * dt_str_new copies the first `length` bytes. A zero byte is data. The function
 * returns NULL when allocation or size representation fails.
 */
dt_str *dt_str_new(const char *bytes, size_t length)
{
    /* TODO: Reject SIZE_MAX because the buffer needs one terminator byte.
       Allocate the handle and buffer. Copy `length` bytes with memcpy.
       Store the length. Return NULL if an allocation fails.
       dt_str_new("hello", 5)  -> a string whose dt_str_len is 5
       dt_str_new("a\0b", 3)   -> a string whose dt_str_len remains 3
       cases/normal/string_building.case, cases/capacity/embedded_zero_byte.case */
    //(void)bytes;
    //(void)length;
    //return NULL;

    // length + 1 would overflow, since we need one extra byte for '\0'
    if (length == SIZE_MAX) {
        return NULL;
    }

    // malloc() - used to allocate a block of memory of the specific size
    // created a memory space for s to store the dtr_str struct
    dt_str *s = malloc(sizeof(dt_str));

    // if no memory was allocated, then return NULL
    if (s == NULL){
        return NULL;
    }

    // added 1 because the '\0' needs its own space 
    // even though it is not part of the length
    s->bytes = malloc(length + 1);


    // free() - deallocates prev allocated memory to be reused by the system
    // for the bytes that are not allocated, free the memory
    if(s->bytes == NULL){
        free(s);
        return NULL;
    }

    // memcopy() - copies a specified number of bytes from one memory space to another
    // copy length bytes so an embedded '\0' is still treated as part of the data
    if(length > 0 && bytes != NULL){
        memcpy(s->bytes, bytes, length);
    }

    // length points to the first space after the copied bytes, put '\0'
    // this is to properly terminate the buffer
    s->bytes[length] = '\0';

    // i stored the length separately so it doesn't depend on '\0' 
    // length counts actual data dytes , including '\0'
    s->length = length;

    // add 1 because the capacity also includes the space for '\0'
    s->capacity = length + 1;

    return s;
}

/*
 * dt_str_free releases the buffer and handle. It accepts NULL.
 */
void dt_str_free(dt_str *s)
{
    /* TODO: Release the buffer. Then release the handle. Accept NULL.
       dt_str_free(s)     -> the buffer and the handle are both released
       dt_str_free(NULL)  -> returns, having done nothing */
    (void)s;
}

/*
 * dt_str_len returns the stored byte count in constant time.
 */
size_t dt_str_len(const dt_str *s)
{
    /* TODO: Return the stored length. Do not scan the bytes.
       after `str new greeting "hello"` then `str append greeting ", world"`:
         dt_str_len(greeting) -> 12
       cases/normal/string_building.case */
    (void)s;
    return 0;
}

/*
 * dt_str_bytes returns the string bytes. Internal storage can include a final
 * zero byte. Callers must use dt_str_len with this pointer.
 */
const char *dt_str_bytes(const dt_str *s)
{
    /* TODO: Return the buffer. The caller uses it with dt_str_len.
       after `str new s "a\0b"`:
         dt_str_bytes(s) -> the three bytes 'a', 0, 'b'
         dt_str_len(s)   -> 3, the required read length
       cases/capacity/embedded_zero_byte.case */
    (void)s;
    return "";
}

/*
 * dt_str_append adds `length` bytes and grows the buffer when necessary. It
 * returns DT_ERR_CAPACITY when allocation or size representation fails.
 * The function does not change the string after a failure.
 */
dt_status dt_str_append(dt_str *s, const char *bytes, size_t length)
{
    /* TODO: Check that the new length and terminator fit in size_t.
       Grow the buffer before you copy the bytes.
       Prevent unsigned wrap during capacity growth.
       Geometric growth makes repeated append operations efficient.
       s holds "hello": dt_str_append(s, ", world", 7) -> DT_OK, len is now 12
       an allocation failure                           -> DT_ERR_CAPACITY, s unchanged
       cases/normal/string_building.case, cases/capacity/string_growth.case */
    //(void)s;
    //(void)bytes;
    //(void)length;
    //return DT_ERR_CAPACITY;

    // * TO DO: remove before submitting; nong ja ang summary for append 
    // basically, gin-check ko anay nong if possible pa mag-add ang new data without exceeding the maximum size, para sure nga indi mag-overflow
    // then I check if the current storage is enough, if not, gin-expand ko siya by making the space bigger, then gin-copy ko ang new data after the existing data
    // after that, gin-update ko ang total amount of data and placed the ending marker right after it para properly terminated gihapon ang string.
    // * end deletion here, ako lang nong madelete. thanks

    // i subtract the current length and 1 first to check how much space is still available without overflow
    size_t available = SIZE_MAX - s->length -1;
    if(length > available) {
        return DT_ERR_CAPACITY;
    }

    // I add the new length to the current length after checking for overflow,
    // so the result is safe to use.
    size_t new_length = s->length + length;

    // add one for the buffer ('\0')
    size_t required_capacity = new_length + 1;

    
    // realloc() - resizes an already allocated block of memory

    // i double the space instead of adding only what is needed, 
    // so repeated small appends won't keep copying the whole string
    if(required_capacity > s->capacity) {
        size_t new_capacity = s->capacity == 0 ? 16 : s->capacity*2;
        if(new_capacity < required_capacity) {
            new_capacity = required_capacity;
        }

        // temporary pointer for realloc. If the allocation fails, 
        // the original string memory is left perfectly valid and untouched
        char *new_bytes = realloc(s->bytes, new_capacity);
        if (new_bytes == NULL) {
            return DT_ERR_CAPACITY;
        }
        s->bytes = new_bytes;
        s->capacity = new_capacity;
    }

    // started copying at the current length so the new bytes will be added after the old ones
    if(length > 0 && bytes != NULL){
        memcpy(s->bytes + s->length, bytes, length);
    }

    // update length after copying
    s->length = new_length;

    // i put the '\0' after the new length, so buffer properly terminates
    s->bytes[s->length] = '\0';

    return DT_OK;
}

/*
 * dt_str_substr builds a new string from length bytes at start.
 * It returns DT_ERR_RANGE when the requested range exceeds the source.
 * It returns DT_ERR_CAPACITY after an allocation failure.
 * The function does not change the source string.
 */
dt_status dt_str_substr(const dt_str *s, size_t start, size_t length, dt_str **out)
{
    /* TODO: Return DT_ERR_RANGE when the requested range exceeds the source.
       Two size_t values can wrap. First compare start with the source length.
       Then compare length with the remaining length.
       s holds "hello" (length 5):
         dt_str_substr(s, 3, 2, &out)  -> DT_OK, *out is "lo"
         dt_str_substr(s, 5, 0, &out)  -> DT_OK, *out is a valid empty string
         dt_str_substr(s, 3, 5, &out)  -> DT_ERR_RANGE, *out untouched
       an allocation failure           -> DT_ERR_CAPACITY, *out untouched
       cases/boundary/substr_exact_end.case, cases/boundary/substr_past_end.case */

    //(void)s;
    //(void)start;
    //(void)length;
    //(void)out;
    //return DT_ERR_RANGE;

    // i check first if the starting position is still within the string
    if(start > s->length){
        return DT_ERR_RANGE;
    }

    // i subtract the starting position from the total length to check if the requested length still fits
    // i do this instead of adding them first to avoid possible overflow
    if(length > s->length - start) {
        return DT_ERR_RANGE;
    }

    // i create the new string starting from the given position and copy only the requested length
    dt_str *piece = dt_str_new(s->bytes + start, length);

    // if no memory was allocated for the new string, return a capacity error
    if(piece == NULL){
        return DT_ERR_CAPACITY;
    }

    *out = piece;
    return DT_OK;
}

/*
 * dt_str_eq reports whether both strings hold the same bytes.
 * The stored lengths let the comparison include embedded zero bytes.
 */
bool dt_str_eq(const dt_str *a, const dt_str *b)
{
    /* TODO: Compare the lengths first. Then use memcmp.
       strcmp ends at an embedded zero byte and can report unequal data as equal.
       "world" and "world"  -> true
       "hello" and "world"  -> false
       "a\0b" and "a"       -> false because their lengths are 3 and 1
       cases/normal/string_building.case, cases/capacity/embedded_zero_byte.case */
    //(void)a;
    //(void)b;
    //return false;

    // if both lengths are not equal then it str itself isn't equal
    if(a->length != b->length) {
        return false;
    }

    // they would not pass this point if the lengths aren't eq
    // this would mean that both are empty strings
    if (a->length == 0) {
        return true;
    }

    return memcmp(a->bytes, b->bytes, a->length) == 0;
}
