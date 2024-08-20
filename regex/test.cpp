#include <iostream>
#include <regex> //std::regex, std::regex_match, std::regex_seach, std::smatch
#include <string>

int main ( void ) {
  //This is only a [test].
  std::string str_regex = R"#(\[([^\]]+)\].*\[([^\]]+)\])#";
  std::string str_haystack = R"(This is only a [test]trash1 [best] garbage
pre [rest] mid [quest] post.)";
  std::regex re_bracket_contents {
    str_regex,
    std::regex_constants::ECMAScript | //egrep
    std::regex_constants::icase
  };
/*
  std::smatch matches;
  std::regex_search (
    str_haystack,
    matches,
    re_bracket_contents
  );
  for ( const auto &match : matches ) {
    std::cout << "Capture group: \"" << match << "\"\n";
  }
  std::cout << "First capture group: \"" << matches [ 1 ] .str (  ) << "\"\n";
*/

  for (
    std::sregex_iterator it {
      str_haystack .begin (  ),
      str_haystack .end (  ),
      re_bracket_contents
    }, it_end;
    it != it_end;
    it ++
  ) {
    //std::smatch
    for ( const auto &match : *it ) {
      std::cout << "Capture group: \"" << match << "\"\n";
    }
    //std::cout << "First capture group: \"" <<
    //  (*it) [ 1 ] .str() << "\"\n";
  }

  return 0;
}
