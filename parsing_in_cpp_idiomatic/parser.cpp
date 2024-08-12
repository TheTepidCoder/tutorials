#include <iostream> //std::cout / std::cin
#include <fstream> //std::fstream (instead of FILE *, fopen, fclose, fread, fseek, ftell)
#include <string> //std::string, std::wstring
#include <map> //std::map<K, V> --> std::map<std::string, std::string>

int main (  ) {
  std::fstream fs { "load_me.txt", std::fstream::in | std::fstream::binary }; //std::ios_base::in (std::ifstream) std::ofstream
  if ( ! fs .is_open (  ) ) {
    std::cerr << "Error: Couldn't open the file.\n" << std::endl;
    return 1;
  }

  std::string str_line;
  std::map<std::string, std::string> m_map;
  //std::getline ( std::cin, str_line );
  size_t st_line_number = 1;
  while ( true ) {
    if ( ! std::getline ( fs, str_line ) ) {
      if ( fs .bad (  ) ) {
        std::cerr << "Error: Unable to read a line from the file." << std::endl;
        return 1;
      }

      break;
    }

    size_t st_offset = str_line .find ( '=' );
    if ( st_offset == std::string::npos ) {
      std::cerr <<
        "Warning: Malformed line in configuration file on line " <<
        st_line_number << ": \"" << str_line << "\"" << std::endl;

      continue;
    }

    std::string str_key, str_value;

    //width=100
    str_key = str_line .substr ( 0, st_offset );
    str_value = str_line .substr ( st_offset + 1 );

    m_map [ str_key ] = str_value;

    st_line_number ++;
  }

  //current newest
  //for ( const auto &[k, v] : m_map ) {
  //  std::cout << "\"" << k << "\"" << " => \"" << v << "\"" << std::endl;
  //}

  //older
  //for ( const auto &pair : m_map ) {
  //  std::cout << "\"" << pair.first << "\"" << " => \"" << pair.second << "\"" << std::endl;
  //}

  //even older
  for (
    std::map<std::string, std::string>::const_iterator it_pair = m_map .cbegin (  );
    it_pair != m_map .cend (  );
    it_pair ++
  ) {
    std::cout << "\"" <<
      it_pair ->first << "\"" << " => \"" <<
      it_pair ->second << "\"" << std::endl;
  }

  return 0;
}
