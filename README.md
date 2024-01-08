# doctext-extended.h

A single-header unit testing framework that adds some helpful utility functions to the outstanding [Doctest](https://github.com/doctest/doctest) unit testing framework.

These extensions primarily focus on doing things that are common when testing student programs for grading, but may be useful in other circumstances as well.  Examples include generating random data, capturing `stdout`, and manipulating strings or streams to a canonical form with less stylistic variation.  Plus, this header allows you to deactivate an existing `main()` function so that it doesn't interfere with the Doctest framework as the primary program entry-point.

## Installation

The file "_doctest-extended.h_" is the only thing you need.  You can use it in one of two ways:

1. **Install in a standard location.**  Just copy the header to a directory that is part of your _include path_.  For example, on Linux/UNIX/MacOS, you might place it in `/usr/local/include`.  Or, you might put it in your user directory, e.g. `~/.local/include`, assuming you have that directory in your _include path_.  You can now just include the framework with `#include <doctest-extended.h>`.
3. **Include using a relative path.** If you copy the header to an arbitrary directory, you can include it by supplying the path, e.g. `#include "path/to/doctest-extended.h"`.

## Documentation for Doctest

A good starting point for learning Doctest is <https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md>

This library always assumes you want Doctest to write the `main()` function for you, so `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` is defined in the header.

## Deactivating an existing `main()` function
This library provides a means (via a nasty macro trick) to disable an existing `main` function, if one is present.  To do this, define the macro `MAIN_FILE` with the name of the file containing `main()`, _**prior**_ to including this header.  
For example, if the main program is named "_student\_program.cpp_", you could test it by adding the following code at the top of your unit test driver code:

```cpp
#define MAIN_FILE "student_program.cpp"
#include <doctest-extended.h>
// ... your testing code below ...
```

If you want to call the original `main()` function, you can do so by calling `__original_main()` instead.  (Notice the two leading underscores.)  This can be useful for simple programs where the functionality you are testing occurs in `main()`.

## Small Example
A small example is available in the "_example_" directory.  There is a program "_example.cpp_" that has two functions and a main function that require testing.  The unit tests are in the file "_test\_driver.cpp_".  If you compile "_test\_driver.cpp_", then run the resulting executable, you should see output like the text in "_doctest\_results.txt_".

The example assumes you have installed "_doctest-extended.h_" in a standard include directory.  If you have not, edit the `#include` directive in "_test\_driver.cpp_" to use a relative path (with quotes instead of angle brackets).

## API Reference for the Extensions to Doctest

The extensions to Doctest provided by the header "doctest-extended.h" are detailed here.

You will notice that functions and classes defined as extensions here are named with two leading underscores (e.g. `__capture_stdout`).  This help avoid collisions with the code you are interacting with, while still being relatively easy to type.  Most macros are not prefixed in this way, but are fully uppercased instead (note that `__epsilon` is the exception).

---

| Macros          | Descriptions                                                                     |
|-----------------|----------------------------------------------------------------------------------|
| `MAIN_FILE`     | Define MAIN_FILE with the name of the file containing main() to.                 |
| `ANSI_TXT_GRN`  | ANSI terminal code for Green text.                                               |
| `ANSI_TXT_BLU ` | ANSI terminal code for Blue text.                                                |
| `ANSI_TXT_MGT`  | ANSI terminal code for Magenta text.                                             |
| `ANSI_TXT_DFT`  | ANSI terminal code for Default text color.                                       |
| `__epsilon`     | A small tolerance value used for determine "closeness" of floating-point values. |


| Functions                                                                                                                   | Descriptions                                                                        |
|-----------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| `template< typename CLOCK >  double  __get_elapsed_seconds (CLOCK b, CLOCK e)`                                              | Get the elapsed time between to measurements, in (fractional) seconds.              |
| `template< typename FTYPE >    double  __profile (FTYPE f)`                                                                 | Determine how many seconds are spent in a function call.                            |
| `std::string  __s_trim_ws (const std::string &str, const std::string &whitespace=" \t")`                                    | Trims whitespace from beginning and end of a string.                                |
| `std::string  __s_reduce_ws (const std::string &str, const std::string &fill=" ", const std::string &whitespace=" \t\r\n")` | Condenses long runs of whitespace to a single space (or user-specified token).      |
| `std::string  __s_remove_ws (const std::string &str)`                                                                       | Removes all whitespace from a string.                                               |
| `std::string  __s_collapse (const std::string &s, bool keep_punct=true, bool keep_space=true)`                              | Condenses or removes whitespace and punctuation in a string.                        |
| `std::string  __s_multiline_collapse (const std::string &s, bool keep_punct=true)`                                          | Condenses or removes whitespace and punctuation in each line of a multiline string. |
| `std::string  __s_lowercase (std::string s)`                                                                                | Get an all-lowercase version of a string.                                           |
| `std::string  __collapse (const std::ostringstream &strm, bool keep_punct=true)`                                            | Collapses a stream (see `__s_collapse`)                                             |
| `std::string  __multiline_collapse (const std::ostringstream &strm, bool keep_punct=true)`                                  | Collapses each line in a multiline stream (see `__s_multiline_collapse`)            |
| `void  __shuffle (auto& std_container)`                                                                                     | Shuffle the contents of a STL-style container in-place.                             |
| `void  __shuffle_range (auto& begin, auto& end)`                                                                            | Shuffle the contents of a range within an STL-style container in-place.             |
| `int  __rand_int( int lower_bound, int upper_bound )`                                                                       | Generate a random integer in the range [lower_bound, upper_bound).                  |
| `double  __rand_double( double lower_bound, double upper_bound )`                                                           | Generate a random double in the range [lower_bound, upper_bound).                   |
| `std::string  __rand_string( int length = 10 )`                                                                             | Generate a random string of a given length.                                         |
| `std::string  __str( const auto& value )`                                                                                   | Convert a value to a string (if it supports stream insertion).                      |
| `bool  __str_contains(const std::string& haystack, const std::string& needle)`                                              | Determine if a string contains a second string.                                     |
| `bool  __contains(const auto& c_haystack, const auto& value_needle)`                                                        | Determine if a STL-style container contains a value.                                |

| Classes                                                | Descriptions                                                                          |
|--------------------------------------------------------|---------------------------------------------------------------------------------------|
| `class`[`__capture_stdout`](#class____capture__stdout) | 
Capture output destined to `stdout` (i.e. capture `cout`) and convert it to a string. |
| `class`[`__simulate_stdin`](#class____simulate_stdin) | Allows stdin to be temporarily replaced by an `istringstream` object or `string` so that you can simulate input when a function/method only reads from stdin. |

## Detailed Descriptions

### `template< typename CLOCK >  double  __get_elapsed_seconds (CLOCK b, CLOCK e)` 
Get the elapsed time between to measurements, in (fractional) seconds.

##### Template Parameters
`CLOCK``	A type representing a clock (see `std::chrono`).

##### Parameters
`b`	Clock time at the beginning of measurement.

`e`	Clock time at the end of measurement.

##### Returns
Seconds elapsed between `b` and `e`. 

### `template< typename FTYPE >    double  __profile (FTYPE f)` 
Determine how many seconds are spent in a function call.

##### Template Parameters
`FTYPE`	A type representing a function.

##### Parameters
`f`	The function to profile.

##### Returns
Seconds elapsed during the call of function `f`. 

### `std::string  __s_trim_ws (const std::string &str, const std::string &whitespace=" \t")` 
Trims whitespace from beginning and end of a string.
(Based on https://stackoverflow.com/a/1798170)

##### Parameters
`str`	The original string.

`whitespace`	List of the whitespace characters to remove.

##### Returns
`str` with whitespace removed 


### `std::string  __s_reduce_ws (const std::string &str, const std::string &fill=" ", const std::string &whitespace=" \t\r\n")` 
Condenses long runs of whitespace to a single space (or user-specified token).
(Based on https://stackoverflow.com/a/1798170)

##### Parameters
`str`	The original string

`fill`	String that will replace removed whitespace

`whitespace`	Whitespace characters to condense

##### Returns
Processed `str` is returned. 


### `std::string  __s_remove_ws (const std::string &str)` 
Removes all whitespace from a string.

##### Parameters
`str`	The original string.

##### Returns
`str` edited to remove all whitespace 


### `std::string  __s_collapse (const std::string &s, bool keep_punct=true, bool keep_space=true)` 
Condenses or removes whitespace and punctuation in a string.
##### Parameters
`str`	The original string

`keep_punct`	Set to true to keep punctuation, or false to remove it.

`keep_space`	Set to true to collapse whitespace, or false to remove it completely.

##### Returns
Processed `str` is returned.


### `std::string  __s_multiline_collapse (const std::string &s, bool keep_punct=true)` 
Condenses or removes whitespace and punctuation in each line of a multiline string.

##### Parameters
`s`	The original string

`keep_punct`	Set to true to retain punctuation, or false to remove it.

##### Returns
Processed `s` is returned. 


### `std::string  __s_lowercase (std::string s)` 
Get an all-lowercase version of a string.

##### Parameters
`s`	The original string.

##### Returns
The lowercase version of `s`. 


### `std::string  __collapse (const std::ostringstream &strm, bool keep_punct=true)` 
Collapses a stream (see `__s_collapse`) 

##### Parameters
`strm`	Stream containing original output.
`keep_punct`	Set to true to retain punctuation, or false to remove it.

##### Returns
String representing contents of `strm` with whitespace/punctuation collapsed. 


### `std::string  __multiline_collapse (const std::ostringstream &strm, bool keep_punct=true)` 
Collapses each line in a multiline stream (see `__s_multiline_collapse`) 

##### Parameters
`strm`	Stream containing original output
`keep_punct`	Set to true to retain punctuation, or false to remove it.

##### Returns
`std::string` representing contents of strm with whitespace/punctuation collapsed. 


### `void  __shuffle (auto& std_container)` 
Shuffle the contents of a STL-style container in-place.

The shuffle is deterministic, so it will always shuffle the same way given the same original container. 

##### Parameters
`std_container`	The original STL-style container to shuffle in-place. 


### `void  __shuffle_range (auto& begin, auto& end)`
Shuffle the contents of a range within an STL-style container in-place.

The shuffle is deterministic, so it will always shuffle the same way given the same original container and range. 

##### Parameters
`range_begin`	Iterator pointing to the beginning of the range to shuffle.
`range_end`	Iterator pointing to the end of the range to shuffle.


### `int  __rand_int( int lower_bound, int upper_bound )` 
Generate a random integer in the range [lower_bound, upper_bound).

##### Parameters
`lower_bound`	The lower bound of the integer range to generate.
    
`upper_bound`	The upper bound (non-inclusive) of the integer range to generate.

##### Returns
Random integer in the range [`lower_bound`, `upper_bound`).


### `double  __rand_double( double lower_bound, double upper_bound )` 
Generate a random double in the range [lower_bound, upper_bound).

##### Parameters
`lower_bound`	The lower bound of the double range to generate.
    
`upper_bound`	The upper bound (non-inclusive) of the double range to generate.

##### Returns
Random double in the range [`lower_bound`, `upper_bound`) 


### `std::string  __rand_string( int length = 10 )` 
Generate a random string of a given length.

##### Parameters
`length`	The number of characters in the resulting string.

##### Returns
A random string with `length` characters (all printable characters). 


### `std::string  __str( const auto& value )` 
Convert a value to a string (if it supports stream insertion). 

##### Parameters
`value`	The value to convert to a string.

##### Returns
String representation of `value`. 


### `bool  __str_contains(const std::string& haystack, const std::string& needle)` 
Determine if a string contains a second string.

##### Parameters
`haystack`	The source string (to find the needle within).

`needle`	The target string (to try to find in the haystack).

##### Returns
true if `needle` is found, false otherwise 


### `bool  __contains(const auto& c_haystack, const auto& value_needle)` 
Determine if a STL-style container contains a value. 

##### Parameters
`c_haystack`	The source STL-style container (to find the needle within).

`value_needle`	The target value (to try to find in the haystack).

##### Returns
true if needle is found, false otherwise 

---

### `class` `__capture_stdout`

Capture output destined to `stdout` (i.e. capture `cout`) and convert it to a string.

Use `begin()` to begin capturing, then perform statements that might produce output. Then, use `end()` to stop capturing and get the captured output as a `std::string`.

(Thanks to Tony Delroy https://stackoverflow.com/a/4191318 for the idea.)

#### Methods

###### `void`[`begin_capture`](#class____capture__stdout_1a1b01e7fdddf0ee8fdbad69324081d930)`()`

Begin capturing stdout.

###### `void`[`begin`](#class____capture__stdout_1a0041bd73bf3aec923224229a0c51392b)`()`

Begin capturing stdout. Alias for `begin_capture`.

###### `void`[`start`](#class____capture__stdout_1ae7021dcc14904629df9ade7cf920155e)`()`

Begin capturing stdout. Alias for `begin_capture`.

###### `std::string`[`end_capture`](#class____capture__stdout_1a148a30385db8a70347bcf0b23737c971)`()`

Stop capturing stdout and return all captured output as a `std::string`.

**Returns**: All captured output is returned as a `string`.

###### `std::string`[`end`](#class____capture__stdout_1abd332dfee71f871b83e102371cd8528f)`()`

Stop capturing stdout and return all captured output as a `std::string`.
Alias for `end_capture`.

**Returns**: All captured output is returned as a `string`.


###  `class __simulate_stdin` 
Allows stdin to be temporarily replaced by an `istringstream` object or `string` so that you can simulate input when a function/method only reads from stdin.

Use `begin()` to begin simulating given a `string` or `istringstream`, then perform statements that expect to read from standard input. Then, use `end()` to stop simulating and return standard input to the original condition.

This idea came from Benjamin van den Hout's blog https://smork.info/blog/posts/100126-131539/

#### Methods

##### `void begin_simulation( const std::istringstream& input_strm )`
Begin simulating stdin, given an `istringstream` to use for input.

###### Parameters
`input_strm`  The `istringstream` object to use for input instead of stdin.

##### `void begin_simulation( const std::string& input_str )`
Begin simulating stdin, given a `std::string` to use for input.

###### Parameters
`input_str`  The string to use for input instead of stdin.

##### `void start( const std::istringstream& input_strm )`
Begin simulating stdin, given an `istringstream` to use for input. (Alias for `begin_simulation`.)

###### Parameters
`input_strm`  The `istringstream` object to use for input instead of stdin.

##### `void start( const std::string& input_str )`
Begin simulating stdin, given a `std::string` to use for input. (Alias for `begin_simulation`.)

###### Parameters
`input_str`  The string to use for input instead of stdin.

##### `void begin( const std::istringstream& input_strm )`
Begin simulating stdin, given an `istringstream` to use for input. (Alias for `begin_simulation`.)

###### Parameters
`input_strm`  The `istringstream` object to use for input instead of stdin.


##### `void begin( const std::string& input_str )`
Begin simulating stdin, given a `std::string` to use for input. (Alias for `begin_simulation`.)

###### Parameters
`input_str`  The string to use for input instead of stdin.


##### `void end_simulation()`
Stops simulating stdin, and returns stdin to the original configuration.

##### `void end()`
Stops simulating stdin, and returns stdin to the original configuration. (Alias for `end_simulation`.)
