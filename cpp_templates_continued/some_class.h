#pragma once
#ifndef SOME_CLASS_HEADER
#define SOME_CLASS_HEADER

class some_class {
  private:
    int m_i = 0;

  public:
    some_class (  ) = delete; //default constructor disallowed
    some_class ( int i ); //constructor
    ~some_class (  ); //destructor
    some_class ( const some_class &old ); //copy constructor
    some_class ( some_class &&old ); //move constructor

    some_class &operator= ( const some_class &old ); //copy assignment
    some_class &operator= ( some_class &&old ); //move assignment
};

#endif
