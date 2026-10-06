# ANALYSIS

Authored by:

- Percie Louise Y. Samaniego (`@perky-boo25`)
- Dan Gabriel P. Guevara (`@d4landan`)

# Question 1

> Pick 3 of the 10 categories. For each, pick a language that gives it to you for free and say what that language pays for it. "Python has dictionaries" isn't an answer. What does Python's dictionary cost in memory or in speed compared to what you built, and where would you notice?

---

## a. Maps

Well, as you stated an example sir, we will use dictionaries to represent maps. We have already discussed in CMSC 11 that dictionaries are a set of key-value pairs we can store and retrieve value using its corresponding key as also stated in the Python’s docs. In our C implementation, we had to build the map ourselves by creating buckets, with the given hash function, linked-list entries for handling collisions, and an array for keeping track of the order. Compared to that, Python already gives us dict, so we can simply create something like {“Bucket”: 12} and access the value using its key without implementing all those parts ourselves. The tradeoff is that although Python saves us time and effort of implementing the data structure, the dictionary uses memory and processing to store and manage those key-value pairs. So like in this case, what Python gives us for free is the already implemented map, what we pay for is the resources used by that implementation.

---

## b. Arrays

Java gives array bounds checking for free, unlike C where we have to explicitly code the checking ourselves. As we experienced in CMSC 22, every time we access an array in Java, it checks if the index is within the valid range, and if it is not, it throws an exception instead of accessing an invalid memory location and this is also stated in Java’s documentation Chapter 10. In our implementation for the arrays, we had to store its length and lower bound, then create an index_to_offset() function that checks if the index is below the lower bound or already past the length before allowing get or set to access the element. This shows what Java is doing for us automatically and it has to keep enough information to know the array’s bounds and perform a check before an invalid access is allowed. The tradeoff is that this safety can add a small runtime cost because the index needs to be checked, while the array also needs to keep its length. In C, we could avoid this automatic check when using normal arrays, but then an invalid index error can access memory outside the array and it can result in undefined behavior. We specifically had to return an error when the check failed instead of allowing the access to happen. So, from my experience, Java’s “free” bounds checking is really a convenience that moves work from the programmer to the language/runtime. Java pays the small checking overhead and memory needed for the array’s metadata, while C makes us pay for the safety ourselves in code.

---

## c. Integers

Well, another Python implementation that gives it to us for “free” is their built-ins for integers. We have already been using Python since CMSC 11, and we still use it in some of our subjects and projects up until now, so we are already used to simply doing arithmetic with integers without really thinking about their limits. In C, however, we have to use a fixed-size integer such as long long means that we have to manually check for overflow before performing the operation. For example in a case where addition ( a + b ) checks long long maximum value and minimum value minus b first, while multiplication requires several checks due to having a lot of edge cases that depend on the sign of the two values or the 0 value. Python gives us a much more convenient integer type because it can represent integers larger than the fixed range of C long long, so we do not have to write these overflow checks ourselves. The tradeoff is that Python has to use additional memory when the integer becomes very large, since it cannot simply keep everything within a fixed-size integer. For us, this is something we actually experience using Python because we normally just write the arithmetic operation and let Python handle the integer for us, whereas C we have to think about what happens when the value reaches its limit.

---

# Question 2

> You wrote the tag check in dt_value_as_int by hand. Some languages don't let you. They make the tagged union a language construct, so the compiler writes the check for you, refuses to compile a read that skips it, and refuses to compile a set of cases that misses one. Rust's enum and match work this way, and so do ML's datatypes and Swift's enumerations with associated values. What does the C version let you do that a compiler enforcing the check wouldn't, and is any of it worth wanting?

In the tag checking, we had to manually make sure that we are reading a value as the correct type. C gives us freedom to access different parts of a union, but also means that we are responsible for making sure that we are accessing it correctly. In Rust, I noticed that the language is more strict about this in the enums and pattern matching, so the compiler can help make sure that the different possible types or cases are handled properly. In C, we can directly access a different union member even though the current tag does not match, while Rust would prevent this through its type system and pattern matching. For me, the difference is that C gives us more freedom to provide more protection. I think the extra freedom we have in C can be useful when we need more control on what we are doing but for the kind of programming we usually do, I find the compiler checking in Rust more convenient because it catches things I might otherwise have to check.

---

# References

1. *Chapter 10. Arrays*. (2026, September 15). <https://docs.oracle.com/javase/specs/jls/se19/html/jls-10.html>
2. Python Documentation. (n.d.) <https://docs.python.org/3/builtins/stdtypes.html#numeric-types-int-float-complex>
3. Python Documentation. (n.d.) <https://docs.python.org/3/builtins/stdtypes.html#mapping-types-dict>

