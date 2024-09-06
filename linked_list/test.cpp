/*
 Created on 2024-09-02 by TheTepidCoder
 Purpose: To show off how something like
 std::list<T> and std::list<T>::iterator
 might be implemented by hand.
*/
#include <iostream>
#include <cstdint>
#include <format>
#include <iterator>
#include <ostream>
#include <new> //std::nothrow
#include <functional>

template<typename T>
struct node {
  T t_data;
  node *lp_next = nullptr;
  node *lp_prev = nullptr;
};

template<typename T>
class list {
  private:
    node<T> *m_front = nullptr;
    node<T> *m_back = nullptr;
    size_t m_st_count = 0;

  public:

    template<typename K>
    class const_iterator;

    template<typename K>
    class iterator {
      friend class list;
      private:
        node<K> *m_node;
        const node<K> *_get_node ( void ) const noexcept {
          return m_node;
        }
        node<K> *_get_node ( void ) noexcept {
          return m_node;
        }

      public:
        iterator (  ) = delete;
        iterator ( node<K> *n ) : m_node ( n ) {  }
        ~iterator (  ) {  }
        bool done ( void ) const noexcept {
          return m_node == nullptr;
        }
        bool operator!= ( const const_iterator<K> &it ) const noexcept {
          return &(m_node ->t_data) != &(*it);
        }
        bool operator!= ( const iterator<K> &it ) const noexcept {
          return &(m_node ->t_data) != &(*it);
        }
        const K &operator* ( void ) const noexcept {
          return m_node ->t_data;
        }
        const K *operator-> ( void ) const noexcept {
          return &m_node ->t_data;
        }
        //operator++ (++x)
        iterator<K> &operator++() { //pre-increment
          if ( ! m_node ) {
            return *this;
          }
          m_node = m_node ->lp_next;
          return *this;
        }
        //operator++ (x++)
        iterator<K> operator++(int) { //post-increment
          iterator<K> it { m_node };
          if ( ! m_node ) {
            return it;
          }
          m_node = m_node ->lp_next;
          return it;
        }
        //operator-- (--x)
        iterator<K> &operator--() { //pre-decrement
          if ( ! m_node ) {
            return *this;
          }
          m_node = m_node ->lp_prev;
          return *this;
        }
        //operator-- (x--)
        iterator<K> operator--(int) { //post-decrement
          iterator<K> it { m_node };
          if ( ! m_node ) {
            return it;
          }
          m_node = m_node ->lp_prev;
          return it;
        }
    };

    template<typename K>
    class const_iterator {
      private:
        const node<K> *m_node;

      public:
        const_iterator (  ) = delete;
        const_iterator ( const node<K> *n ) : m_node ( n ) {  }
        ~const_iterator (  ) {  }
        bool done ( void ) const noexcept {
          return m_node == nullptr;
        }
        bool operator!= ( const const_iterator<K> &it ) const noexcept {
          return &(m_node ->t_data) != &(*it);
        }
        bool operator!= ( const iterator<K> &it ) const noexcept {
          return &(m_node ->t_data) != &(*it);
        }
        const K &operator* ( void ) const noexcept {
          return m_node ->t_data;
        }
        const K *operator-> ( void ) const noexcept {
          return &m_node ->t_data;
        }
        //operator++ (++x)
        const_iterator<K> &operator++() { //pre-increment
          if ( ! m_node ) {
            return *this;
          }
          m_node = m_node ->lp_next;
          return *this;
        }
        //operator++ (x++)
        const_iterator<K> operator++(int) { //post-increment
          const_iterator<K> it { m_node };
          if ( ! m_node ) {
            return it;
          }
          m_node = m_node ->lp_next;
          return it;
        }
        //operator-- (--x)
        const_iterator<K> &operator--() { //pre-deccrement
          if ( ! m_node ) {
            return *this;
          }
          m_node = m_node ->lp_prev;
          return *this;
        }
        //operator-- (x--)
        const_iterator<K> operator--(int) { //post-decrement
          const_iterator<K> it { m_node };
          if ( ! m_node ) {
            return it;
          }
          m_node = m_node ->lp_prev;
          return it;
        }
    };

    list (  );
    ~list (  );
    list ( const list & );
    list ( list && );
    list &operator= ( const list & );
    list &operator= ( list && );
    bool push_front ( const T &t ) noexcept;
    bool push_back ( const T &t ) noexcept;
    void erase ( const list<T>::iterator<T> &it );

    void pop_front ( void ) noexcept;
    void pop_back ( void ) noexcept;
    void merge_front ( list & ) noexcept;
    void merge_back ( list & ) noexcept;
    T &front ( void ) noexcept;
    T &back ( void ) noexcept;
    size_t count ( void ) const noexcept;
    void clear ( void );

    bool foreach ( std::function<bool(const T&)> fn_callback );

    const_iterator<T> crbegin ( void ) const noexcept;
    const_iterator<T> cbegin ( void ) const noexcept;
    const_iterator<T> cend ( void ) const noexcept;
    iterator<T> rbegin ( void ) noexcept;
    iterator<T> begin ( void ) noexcept;
    iterator<T> end ( void ) noexcept;
};
template<typename T>
list<T>::list (  ) {
}
template<typename T>
list<T>::~list (  ) {
  clear (  );
}

template<typename T>
list<T>::list (
  const list<T> &old
) :
  m_st_count ( 0 )
{
  for (
    list<T>::const_iterator cit = old .cbegin (  );
    cit != old .cend (  );
    cit ++
  ) {
    push_back ( *cit );
  }
}
template<typename T>
list<T>::list ( list<T> &&old ) {
  m_front = old .m_front;
  m_back = old .m_back;
  m_st_count = old .m_st_count;

  old .m_front = nullptr;
  old .m_back = nullptr;
  old .m_st_count = 0;
}
template<typename T>
list<T> &list<T>::operator= ( const list<T> &old ) {
  //Don't allow a list<T> to be copied from itself.
  if ( this == &old ) {
    return *this;
  }
  clear (  );

  for (
    list<T>::const_iterator cit = old .cbegin (  );
    cit != old .cend (  );
    cit ++
  ) {
    push_back ( *cit );
  }

  return *this;
}
template<typename T>
list<T> &list<T>::operator= ( list<T> &&old ) {
  //Don't allow a list<T> to be moved from itself.
  if ( this == &old ) {
    return &this;
  }

  clear (  );

  m_front = old .m_front;
  m_back = old .m_back;
  m_st_count = old .m_st_count;

  old .m_front = nullptr;
  old .m_back = nullptr;
  old .m_st_count = 0;
  return *this;
}
template<typename T>
void list<T>::merge_back ( list<T> &lst ) noexcept {
  if ( &lst == this ) {
    return ;
  }
  m_back ->lp_next = lst .m_front;
  lst .m_front ->lp_prev = m_back;
  m_back = lst .m_back;

  lst .m_back = nullptr;
  lst .m_front = nullptr;
  lst .m_st_count = 0;
}
template<typename T>
void list<T>::merge_front ( list<T> &lst ) noexcept {
  if ( &lst == this ) {
    return ;
  }
  m_front ->lp_prev = lst .m_back;
  lst .m_back ->lp_next = m_front;
  m_front = lst .m_front;

  lst .m_front = nullptr;
  lst .m_back = nullptr;
  lst .m_st_count = 0;
}
template<typename T>
T &list<T>::front ( void ) noexcept {
  return m_front ->t_data;
}
template<typename T>
T &list<T>::back ( void ) noexcept {
  return m_back ->t_data;
}


template<typename T>
list<T>::const_iterator<T> list<T>::crbegin ( void ) const noexcept {
  return list<T>::const_iterator<T> ( m_back );
}
template<typename T>
list<T>::const_iterator<T> list<T>::cbegin ( void ) const noexcept {
  return list<T>::const_iterator<T> ( m_front );
}
template<typename T>
list<T>::const_iterator<T> list<T>::cend ( void ) const noexcept {
//  return list<T>::const_iterator<T> ( m_back );
  return list<T>::const_iterator<T> ( nullptr );
}
template<typename T>
list<T>::iterator<T> list<T>::rbegin ( void ) noexcept {
  return list<T>::iterator<T> ( m_back );
}
template<typename T>
list<T>::iterator<T> list<T>::begin ( void ) noexcept {
  return list<T>::iterator<T> ( m_front );
}
template<typename T>
list<T>::iterator<T> list<T>::end ( void ) noexcept {
  return list<T>::iterator<T> ( nullptr );
}


template<typename T>
bool list<T>::push_front ( const T &t ) noexcept {
  node<T> *n = new (std::nothrow) node<T>;
  if ( ! n ) {
    return false;
  }
  n ->t_data = t;
  n ->lp_next = m_front;
  n ->lp_prev = nullptr;
  if ( m_front ) {
    m_front ->lp_prev = n;
  }
  m_front = n;
  if ( ! m_back ) {
    m_back = m_front;
  }
  m_st_count ++;
  return true;
}
template<typename T>
bool list<T>::push_back ( const T &t ) noexcept {
  node<T> *n = new (std::nothrow) node<T>;
  if ( ! n ) {
    return false;
  }
  n ->t_data = t;
  if ( m_back ) {
    m_back ->lp_next = n;
  }
  n ->lp_prev = m_back;
  n ->lp_next = nullptr;
  m_back = n;
  if ( ! m_front ) {
    m_front = m_back;
  }
  m_st_count ++;
  return true;
}

template<typename T>
void list<T>::erase ( const list<T>::iterator<T> &it ) {
  node<T> *n = const_cast<node<T> *>(it ._get_node (  ));
  if ( n ->lp_prev ) {
    n ->lp_prev ->lp_next = n ->lp_next;
  }
  if ( n ->lp_next ) {
    n ->lp_next ->lp_prev = n ->lp_prev;
  }
  node<T> *next = n ->lp_next;
  node<T> *prev = n ->lp_prev;
  delete n;

  if ( n == m_front ) {
    m_front = next;
  }
  //Note: This is not an else case because the first element of the list could also be
  //the last element of the list, if it's the ONLY element of the list!
  if ( n == m_back ) {
    m_back = prev;
  }

  m_st_count --;
}

template<typename T>
void list<T>::pop_front ( void ) noexcept {
  node<T> *next = m_front ->lp_next;
  if ( m_front ->lp_next ) {
    m_front ->lp_prev = nullptr;
  }
  delete m_front;
  m_front = next;
  m_st_count --;
}

template<typename T>
void list<T>::pop_back ( void ) noexcept {
  node<T> *prev = m_back ->lp_prev;
  if ( m_back ->lp_prev ) {
    m_back ->lp_prev ->lp_next = nullptr;
  }
  delete m_back;
  m_back = prev;
  m_st_count --;
}
template<typename T>
size_t list<T>::count ( void ) const noexcept {
  return m_st_count;
}
template<typename T>
void list<T>::clear ( void ) {
  node<T> *next = m_front, *free_me;
  while ( next ) {
    free_me = next;
    next = next ->lp_next;
    delete free_me;
  }

  m_st_count = 0;
}
template<typename T>
bool list<T>::foreach ( std::function<bool(const T&)> fn_callback ) {
  for (
    node<T> *n = m_front;
    n;
    n = n ->lp_next
  ) {
    if ( ! fn_callback ( n ->t_data ) ) {
      return false;
    }
  }

  return true;
}

struct date_time {
  int i_year = 0;
  int i_month = 0;
  int i_day = 0;
  int i_hour = 0;
  int i_minute = 0;
  int i_second = 0;
};

int main ( void ) {
  std::ostream_iterator<char> cout { std::cout };
  list<int> lst;
  lst .push_back ( 1 );
  lst .push_front ( 0 );
  lst .push_back ( 2 );
  lst .push_back ( 3 );
  lst .push_back ( 4 );
  lst .push_front ( -1 );
  lst .pop_front (  );
  lst .pop_back (  );

  std::cout << "There are " <<
    lst .count (  ) << " elements in the list." <<
    std::endl;

  std::cout << "Testing list<T>::foreach.\n";
  lst .foreach (
    [] ( const int &i ) -> bool {
      std::cout << "Value: " << i << std::endl;
      return true;
    }
  );


/*
//Test deletion in the middle of the list.
{
  list<int>::iterator it = lst .begin (  );
  //0 -> 1 -> 2
  for ( size_t st_i = 0; st_i < 2; st_i ++ ) {
    it ++;
  }
  lst .erase ( it );
}
*/

//Test deletion from the beginning of the list.
{
//  list<int>::iterator it = lst .begin (  );
//  lst .erase ( it );
  lst .erase ( lst .begin (  ) ); //(const list<T>::iterator<T> &) allows RValue.
}

/*
//Test deleting the last element of the list.
{
  list<int>::iterator it = lst .begin (  );
  //0 -> 1 -> 2 -> 3
  for ( size_t st_i = 0; st_i < 3; st_i ++ ) {
    it ++;
  }
  lst .erase ( it );
}
*/

//Prefer cit .is_done().
//  list<T>::const_iterator<T> tmp ( (node<T> *) nullptr );
//  cit!= ( tmp )
//  *tmp
//&(tmp::operator* -->T&) != cit::m_mode::t_data
  std::cout << "Testing const_iterator.\n";
  for (
    list<int>::const_iterator cit = lst .cbegin (  );
    cit != lst .cend (  ); //operator!=(const const_iterator<K> & or const iterator<K> &)
    cit ++ //operator++(int)
  ) {
    std::cout << "Value: " << *cit << std::endl;
  }

  std::cout << "Testing regular iterator.\n";
  for (
    list<int>::iterator cit = lst .begin (  );
    cit != lst .end (  ); //operator!=(const const_iterator<K> & or const iterator<K> &)
    cit ++ //operator++(int)
  ) {
    std::cout << "Value: " << *cit << std::endl;
  }

  list<date_time> lst2;
  lst2 .push_front ( { 2024, 9, 2, 3, 48, 0 } );
  lst2 .push_front ( { 2020, 1, 1, 0, 0, 0 } );
  for (
    list<date_time>::const_iterator cit = lst2 .crbegin (  );
    cit != lst2 .cend (  ); //operator!=(const const_iterator<K> & or const iterator<K> &)
    --cit //operator--()
  ) {
    std::vformat_to (
      cout,
      "Value: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
      std::make_format_args (
        cit ->i_year,
        cit ->i_month,
        cit ->i_day,
        cit ->i_hour,
        cit ->i_minute,
        cit ->i_second
      )
    );
  }


  std::cout << "Copying lst into lst3 (copy constructor).\n";
  list<int> lst3 = lst; //1,2,3 //list<T> (const list<T>&)
  lst3 .foreach ( []( const int &i ) -> bool {  std::cout << "lst3 value: " << i << std::endl; return true; } );

  std::cout << "Copying lst3 into lst4 (copy assignment operator overload).\n";
  list<int> lst4;
  lst4 = lst3; //list<T>::operator=(const list<T>&)
  lst4 .foreach ( []( const int &i ) -> bool {  std::cout << "lst4 value: " << i << std::endl; return true; } );

  std::cout << "Moving lst3 into lst5 (move constructor).\n";
  list<int> lst5 = std::move ( lst3 );
  std::cout << "lst3.count(): " << lst3.count() << "\n";
  lst5 .foreach ( []( const int &i ) -> bool {  std::cout << "lst5 value: " << i << std::endl; return true; } );

  std::cout << "Moving from lst5 into lst6 (move assignment operator overload).\n";
  list<int> lst6;
  lst6 = std::move ( lst5 );
  std::cout << "lst5.count(): " << lst5 .count (  ) << std::endl;

  lst6 .foreach ( []( const int &i ) -> bool {  std::cout << "lst6 value: " << i << std::endl; return true; } );


  std::cout << "lst6.front(): " << lst6 .front (  ) << std::endl;
  std::cout << "lst6.back(): " << lst6 .back(   ) << std::endl;

  //lst6: 1,2,3
  lst6 .push_front ( -99 );
  //lst6: -99, 1, 2, 3
  lst6 .merge_back ( lst );
  //lst6: -99, 1, 2, 3, <--> 1, 2, 3


  //lst6 .merge_front ( lst );
  //lst6: 1, 2, 3, <--> -99, 1, 2, 3
  for (
    list<int>::const_iterator cit = lst6 .cbegin (  );
    ! cit .done (  ); //cit != lst6 .cend (  );
    cit ++
  ) {
    std::cout << "lst6 value: \"" << *cit << "\"\n";
  }

  return 0;
}
