#include <iostream>

class first {
  private:
  public:
    first (  );
    ~first (  );
};
first::first (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
first::~first (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
class second {
  private:
  public:
    second (  );
    ~second (  );
};
second::second (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
second::~second (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
class assign_me {
  private:
  public:
    assign_me (  );
    ~assign_me (  );
    assign_me ( const int & );
    assign_me &operator= ( const int & );
};
assign_me::assign_me (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
assign_me::~assign_me (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
assign_me::assign_me ( const int &rhs ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
  operator= ( rhs );
}
assign_me &assign_me::operator= ( const int &rhs ) {
  std::cout << __PRETTY_FUNCTION__ <<
    " was called with value " << rhs << "." << std::endl;
  return *this; //this = assign_me* -->assign_me -->assign_me &
}

class parent {
  private:
    first m_first;
    second m_second;
    assign_me m_assign_me = 1; //copy constructor

  public:
    parent (  );
    //This constructor shows that assigning a value in the
    //body of a constructor doesn't prevent the default
    //member value from being assigned.
    parent ( const int & );
    ~parent (  );
};
//If someone constructors our object with an integer like
//this: parent p ( 10 );
parent::parent ( const int &x ) {
  m_assign_me = 2;
}
parent::parent (
) :
  m_first {  },
  m_second {  },
  m_assign_me { 2 }
{
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}
parent::~parent (  ) {
  std::cout << __PRETTY_FUNCTION__ << " was called." << std::endl;
}

int main ( void ) {
//  assign_me am = 2;

  parent p;
//  parent ( 1 );

  return 0;
}
