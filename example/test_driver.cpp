#define MAIN_FILE "example.cpp"
#include <doctest-extended.h>

TEST_CASE( "get_name - input" ) {
    auto output_snapshot = __capture_stdout{};  // suppress output
    auto input_driver    = __simulate_stdin{};  // simulate input
    input_driver.begin( "John Doe" );
    output_snapshot.begin();
    std::string name = get_name();
    input_driver.end();
    output_snapshot.end();
    REQUIRE( name == "John Doe" );
}

TEST_CASE( "get_name - prompt" ) {
    auto output_snapshot = __capture_stdout{};  // capture output
    auto input_driver    = __simulate_stdin{};  // satisfy input
    output_snapshot.begin();
    input_driver.begin( "John Doe" );
    std::string name = get_name();
    input_driver.end();
    auto prompt_text = output_snapshot.end();
    REQUIRE( __s_lowercase( __s_collapse( prompt_text, false, false ) )
             == "enteryourname" );
}

TEST_CASE( "check name" ) {
    std::string name_OK  = "Bob";
    std::string name_bad = "";
    REQUIRE( check_name( name_OK ) == true );
    REQUIRE( check_name( name_bad ) == false );
}

TEST_CASE( "main - gets and uses name" ) {
    auto input_driver    = __simulate_stdin{};  // simulate input
    auto output_snapshot = __capture_stdout{};  // capture output
    input_driver.begin( "John Doe" );
    output_snapshot.begin();
    __original_main();
    input_driver.end();
    auto results = output_snapshot.end();
    REQUIRE( __str_contains( results, "John Doe" ) == true );
}
