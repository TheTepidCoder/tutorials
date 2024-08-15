#include "some_class.h"
#include <iostream>

some_class::some_class (
  int i
) :
  m_i ( i )
{
  std::cout << __PRETTY_FUNCTION__ << " was called with value, " << m_i << ".\n";
}
some_class::~some_class (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called with value, " << m_i << ".\n";
}
some_class::some_class ( const some_class &old ) {
  m_i = old .m_i;
  std::cout << __PRETTY_FUNCTION__ << " was called with value, " << m_i << ".\n";
}
some_class::some_class ( some_class &&old ) {
  m_i = old .m_i;
  std::cout << __PRETTY_FUNCTION__ << " was called with value, " << m_i << ".\n";
}
some_class &some_class::operator= ( const some_class &old ) {
  m_i = old .m_i;
  std::cout << __PRETTY_FUNCTION__ << " was called with value, " << m_i << ".\n";
  return *this;
}
some_class &some_class::operator= ( some_class &&old ) {
  m_i = old .m_i;
  std::cout << __PRETTY_FUNCTION__ << " was called with value, " << m_i << ".\n";
  return *this;
}
