/**
 * @file   doctest-grading-extensions.h
 * @brief  Useful utility functions and hacks to aid in using Doctest
 *         unit testing framework in a program grading environment.
 * @author Jason L Causey
 * @version 0.1
 *
 * See "README.md" for detailed usage information.
 *
 * @copyright 2023-2024 Jason L Causey, Arkansas State University
 *
 * MIT License (https://opensource.org/licenses/MIT)
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifndef DOCTEST_GRADING_EXTENSIONS_H
#define DOCTEST_GRADING_EXTENSIONS_H

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#ifdef MAIN_FILE  /// Define `MAIN_FILE` with the name of the file containing `main()` to
                  /// deactivate it.
#define main __original_main
#include MAIN_FILE
#undef main
#endif

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#define ANSI_TXT_GRN "\033[0;32m"  /// ANSI code macro for Green output color.
#define ANSI_TXT_BLU "\033[0;34m"  /// ANSI code macro for Blue output color.
#define ANSI_TXT_MGT "\033[0;35m"  /// ANSI code macro for Magenta output color.
#define ANSI_TXT_DFT "\033[0;0m"   /// ANSI code macro for Console default output color.

#define __epsilon \
    1e-5  /// Tolerance macro used for determining "nearly equal" for floating-point
          /// comparisons.

std::mt19937 __rng;  /// Random number generator used in this library.

/**
 * @brief Shuffle the contents of a STL-style container in-place.
 * @remark The shuffle is deterministic, so it will always shuffle the same way
 *         given the same original container.
 * @param  std_container  The original STL-style container to shuffle in-place.
 */
auto __shuffle = []( auto& std_container ) {
    __rng.seed( 42 );
    std::shuffle( std_container.begin(), std_container.end(),
                  __rng );  // Shuffle the values
};
/**
 * @brief Shuffle the contents of a range within an STL-style container in-place.
 * @remark The shuffle is deterministic, so it will always shuffle the same way
 *         given the same original container and range.
 * @param  range_begin  Iterator pointing to the beginning of the range to shuffle
 * @param  range_end    Iterator pointing to the end of the range to shuffle
 */
auto __shuffle_range = []( auto& range_begin, auto& range_end ) {
    __rng.seed( 42 );
    std::shuffle( range_begin, range_end, __rng );  // Shuffle the values
};

/**
 * @brief Generate a random integer in the range [`lower_bound`, `upper_bound`).
 * @param lower_bound  The lower bound of the integer range to generate.
 * @param upper_bound  The upper bound (non-inclusive) of the integer range to generate.
 * @return Random integer in the range [`lower_bound`, `upper_bound`)
 */
auto __rand_int = []( int lower_bound, int upper_bound ) {
    return (int)( rand() % ( upper_bound - lower_bound ) + lower_bound );
};

/**
 * @brief Generate a random double in the range [`lower_bound`, `upper_bound`).
 * @param lower_bound  The lower bound of the double range to generate.
 * @param upper_bound  The upper bound (non-inclusive) of the double range to generate.
 * @return Random double in the range [`lower_bound`, `upper_bound`)
 */
auto __rand_double
    = []( double lower_bound = 0.0, double upper_bound = ( 100 + __epsilon ) ) {
          return ( (double)__rand_int( lower_bound * 100, upper_bound * 100 ) / 100.0 );
      };

/**
 * @brief Generate a random string of a given length.
 * @param length The number of characters in the resulting string.
 * @return A random string with `length` characters (all printable characters).
 *
 */
auto __rand_string = []( int length = 10 ) {
    const static char valid_chars[]{
        "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ1234567890~!@#$%^&*-+_=?" };
    std::string result;
    for ( int i = 0; i < length; ++i ) { result += valid_chars[__rand_int( 0, 66 )]; }
    return result;
};

/**
 * @brief Get the elapsed time between to measurements, in (fractional) seconds.
 *
 * @tparam CLOCK  A type representing a clock (see `std::chrono`).
 * @param b       Clock time at the beginning of measurement.
 * @param e       Clock time at the end of measurement.
 * @return        Seconds elapsed between `b` and `e`.
 */
template <typename CLOCK>
double __get_elapsed_seconds( CLOCK b, CLOCK e ) {
    std::chrono::duration<double> elapsed_time;
    elapsed_time = e - b;
    return elapsed_time.count();
};

/**
 * @brief          Determine how many seconds are spent in a function call.
 *
 * @tparam FTYPE   A type representing a function.
 * @param f        The function to profile.
 * @return         Seconds elapsed during the call of function `f`.
 */
template <typename FTYPE>
double __profile( FTYPE f ) {
    std::chrono::high_resolution_clock::time_point begin, end;
    begin = std::chrono::high_resolution_clock::now();  // record start time
    f();
    end = std::chrono::high_resolution_clock::now();  // record start time
    return __get_elapsed_seconds( begin, end );
};

/**
 * @brief   Trims whitespace from beginning and end of a string.
 * @remark Based on https://stackoverflow.com/a/1798170
 *
 * @param str           The original string.
 * @param whitespace    What whitespace should be removed?
 * @return              `str` with `whitespace` removed
 */
std::string __s_trim_ws( const std::string& str, const std::string& whitespace = " \t" ) {
    const auto strBegin = str.find_first_not_of( whitespace );
    if ( strBegin == std::string::npos ) return std::string{ "" };  // no content

    const auto strEnd   = str.find_last_not_of( whitespace );
    const auto strRange = strEnd - strBegin + 1;

    return str.substr( strBegin, strRange );
}

/**
 * @brief  Condenses long runs of whitespace to a single space (or user-specified token).
 * @remark Based on https://stackoverflow.com/a/1798170
 *
 * @param str           The original string
 * @param fill          String that will replace removed whitespace
 * @param whitespace    Whitespace characters to condense
 * @return              Processed `str` is returned.
 */
std::string __s_reduce_ws( const std::string& str, const std::string& fill = " ",
                           const std::string& whitespace = " \t\r\n" ) {
    // trim first
    auto result = __s_trim_ws( str, whitespace );

    // replace sub ranges
    auto beginSpace = result.find_first_of( whitespace );
    while ( beginSpace != std::string::npos ) {
        const auto endSpace = result.find_first_not_of( whitespace, beginSpace );
        const auto range    = endSpace - beginSpace;

        result.replace( beginSpace, range, fill );

        const auto newStart = beginSpace + fill.length();
        beginSpace          = result.find_first_of( whitespace, newStart );
    }

    return result;
}

/**
 * @brief       Removes all whitespace from a string.
 *
 * @param str   The original string.
 * @return      `str` edited to remove all whitespace
 */
std::string __s_remove_ws( const std::string& str ) {
    std::string result;
    for ( char ch : str )
        if ( !isspace( ch ) ) result.push_back( ch );
    return result;
}

/**
 * @brief  Condenses or removes whitespace and punctuation in a string.
 *
 * @param str           The original string
 * @param keep_punct    Set to true to keep punctuation, or false to remove it.
 * @param keep_space    Set to true to collapse whitespace, or false to remove it
 * completely.
 * @return              Processed `str` is returned.
 */
std::string __s_collapse( const std::string& s, bool keep_punct = true,
                          bool keep_space = true ) {
    std::string tmp;
    if ( keep_space )
        tmp = __s_reduce_ws( s );
    else
        tmp = __s_remove_ws( s );

    std::string out;
    for ( auto c : tmp )
        if ( keep_punct || !ispunct( c ) ) out += c;
    return out;
}

/**
 * @brief   Condenses or removes whitespace and punctuation in each line of a multiline
 * string.
 *
 * @param s             The original string
 * @param keep_punct    Set to true to retain punctuation, or false to remove it.
 * @return              Processed `s` is returned.
 */
std::string __s_multiline_collapse( const std::string& s, bool keep_punct = true ) {
    std::istringstream _iss{ s };
    std::string        out;
    for ( std::string line; getline( _iss, line ); ) {
        std::string tmp = __s_reduce_ws( line );
        std::string line_out;
        for ( auto c : tmp )
            if ( keep_punct || !ispunct( c ) ) line_out += c;
        if ( line_out != std::string{ "\n" } ) out += line_out;
    }
    return out;
}

/**
 * @brief       Get an all-lowercase version of a string.
 *
 * @param s     The original string
 * @return      The lowercase version of `s`.
 */
std::string __s_lowercase( std::string s ) {
    std::transform( s.begin(), s.end(), s.begin(), ::tolower );
    return s;
}

/**
 * @brief               Collapses a stream (see @ref __s_collapse)
 *
 * @param strm          Stream containing original output.
 * @param keep_punct    Set to true to retain punctuation, or false to remove it.
 * @return              String representing contents of `strm` with whitespace/punctuation
 * collapsed.
 */
std::string __collapse( const std::ostringstream& strm, bool keep_punct = true ) {
    return __s_collapse( strm.str(), keep_punct );
}

/**
 * @brief               Collapses each line in a multiline stream (see @ref
 * __s_multiline_collapse)
 *
 * @param strm          Stream containing original output
 * @param keep_punct    Set to true to retain punctuation, or false to remove it.
 * @return std::string  String representing contents of `strm` with whitespace/punctuation
 * collapsed.
 */
std::string __multiline_collapse( const std::ostringstream& strm,
                                  bool                      keep_punct = true ) {
    return __s_multiline_collapse( strm.str(), keep_punct );
}

/**
 * @brief       Convert a value to a string (if it supports stream insertion).
 * @param value The value to convert to a string.
 * @return      String representation of `value`.
 */
auto __str = []( const auto& value ) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
};

/**
 * @brief Determine if a string contains a second string.
 * @param haystack The source string (to find the needle within).
 * @param needle   The target string (to try to find in the haystack).
 * @return         true if `needle` is found, false otherwise
 */
auto __str_contains = []( const std::string& haystack, const std::string& needle ) {
    return haystack.find( needle ) != std::string::npos;
};

/**
 * @brief Determine if a STL-style container contains a value.
 * @param c_haystack    The source STL-style container (to find the needle within).
 * @param value_needle  The target value (to try to find in the haystack).
 * @return              true if `needle` is found, false otherwise
 */
auto __contains = []( const auto& c_haystack, const auto& value_needle ) {
    return std::find( c_haystack.begin(), c_haystack.end(), value_needle )
           != c_haystack.end();
};

/**
 * @brief Capture output destined to `stdout` (i.e. capture `cout`) and convert it to a
 * string.
 * @details Use `begin()` to begin capturing, then perform statements that might produce
 * output. Then, use `end()` to stop capturing and get the captured output as a
 * `std::string`.
 * @remark Idea came from https://stackoverflow.com/a/4191318
 */
class __capture_stdout {
public:
    __capture_stdout() = default;
    /**
     * @brief Begin capturing stdout.
     */
    void begin_capture() {
        oss.clear();
        oss.str( "" );
        p_cout_streambuf = std::cout.rdbuf();
        std::cout.rdbuf( oss.rdbuf() );
    }
    /**
     * @brief Begin capturing stdout. Alias for @ref `begin_capture`.
     */
    void begin() { begin_capture(); }
    /**
     * @brief Begin capturing stdout. Alias for @ref `begin_capture`.
     */
    void start() { begin_capture(); }
    /**
     * @brief Stop capturing stdout and return all captured output as a `std::string`.
     * @return All captured output is returned.
     */
    std::string end_capture() {
        if ( p_cout_streambuf ) {
            std::cout.rdbuf( p_cout_streambuf );  // restore
            p_cout_streambuf = nullptr;
        }
        std::string result = oss.str();
        oss.clear();
        oss.str( "" );
        return result;
    }
    /**
     * @brief Stop capturing stdout and return all captured output as a `std::string`.
     * Alias for @ref `end_capture`.
     */
    std::string end() { return end_capture(); }
    ~__capture_stdout() { end_capture(); }

private:
    std::ostringstream oss;
    std::streambuf*    p_cout_streambuf = nullptr;
};

/**
 * @brief Allows stdin to be temporarily replaced by an istringstream object
 *        or string so that you can simulate input when a function/method
 *        only reads from stdin.
 * @remark  This idea came from Benjamin van den Hout's blog
 *          https://smork.info/blog/posts/100126-131539/
 */
class __simulate_stdin {
public:
    __simulate_stdin() = default;

    /**
     * @brief Begin simulating stdin, given an istringstream to use for input.
     * @param input_strm  The istringstream object to use for input instead of stdin.
     */
    void begin_simulation( const std::istringstream& input_strm ) {
        p_sim_streambuf = input_strm.rdbuf();
        std::cin.rdbuf( p_sim_streambuf );
    }
    /**
     * @brief Begin simulating stdin, given a std::string to use for input.
     * @param input_str  The string to use for input instead of stdin.
     */
    void begin_simulation( const std::string& input_str ) {
        iss.clear();
        iss.str( input_str );
        begin_simulation( iss );
    }
    /**
     * @brief Begin simulating stdin, given an istringstream to use for input. (Alias of
     * begin_simulation.)
     * @param input_strm  The istringstream object to use for input instead of stdin.
     */
    void start( const std::istringstream& input_strm ) { begin_simulation( input_strm ); }
    /**
     * @brief Begin simulating stdin, given a std::string to use for input. (Alias of
     * begin_simulation.)
     * @param input_strm  The string to use for input instead of stdin.
     */
    void start( const std::string& input_str ) { begin_simulation( input_str ); }
    /**
     * @brief Begin simulating stdin, given an istringstream to use for input. (Alias of
     * begin_simulation.)
     * @param input_strm  The istringstream object to use for input instead of stdin.
     */
    void begin( const std::istringstream& input_strm ) { begin_simulation( input_strm ); }
    /**
     * @brief Begin simulating stdin, given a std::string to use for input. (Alias of
     * `begin_simulation`.)
     * @param input_strm  The string to use for input instead of stdin.
     */
    void begin( const std::string& input_str ) { begin_simulation( input_str ); }
    /**
     * @brief Stop simulating stdin and restore the normal stdin stream.
     */
    void end_simulation() {
        if ( p_sim_streambuf ) {
            std::cin.rdbuf( p_cin_streambuf );  // restore
            p_sim_streambuf = nullptr;
        }
    }
    /**
     * @brief Stop simulating stdin and restore the normal stdin stream. (Alias of
     * `end_simulation`)
     */
    void end() { end_simulation(); }
    ~__simulate_stdin() { end_simulation(); }

private:
    std::istringstream    iss;
    std::streambuf* const p_cin_streambuf = std::cin.rdbuf();
    std::streambuf*       p_sim_streambuf = nullptr;
};

#endif
