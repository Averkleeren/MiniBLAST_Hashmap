#ifndef hashy_h_
#define hashy_h_

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

typedef std::pair<std::string, std::vector<int>> KeyType;
template < class HashFunc >
class hashy {

private:
  // HASH SET REPRESENTATION
  std::vector< std::list<KeyType> > m_table;  // actual table
  HashFunc m_hash;                            // hash function
  unsigned int m_size;                        // number of keys
  double resize;


//_____________________________________________________________________
//ITERATOR IMPLEMENATATION
public:

//iterator class
class iterator {
  public:
    friend class hashy;   // allows access to private variables

  private:
    typedef typename std::list<KeyType>::iterator hash_list_itr;
    // ITERATOR REPRESENTATION
    hashy* m_hs;          
    int m_index;               // current index in the hash table
    hash_list_itr m_list_itr;  // current iterator at the current index

  private:
    // private constructors for use by the hashy only
    iterator(hashy * hs) : m_hs(hs), m_index(-1) {}
    iterator(hashy* hs, int index, hash_list_itr loc)
      : m_hs(hs), m_index(index), m_list_itr(loc) {}

  public:
    // Ordinary constructors & assignment operator
    iterator() : m_hs(0), m_index(-1)  {}
    iterator(iterator const& itr)
      : m_hs(itr.m_hs), m_index(itr.m_index), m_list_itr(itr.m_list_itr) {}
    iterator&  operator=(const iterator& old) {
      m_hs = old.m_hs;
      m_index = old.m_index; 
      m_list_itr = old.m_list_itr;
      return *this;
    }

    // The dereference operator need only worry about the current
    // list iterator, and does not need to check the current index.
    const KeyType& operator*() const { return *m_list_itr; }

    // The comparison operators must account for the list iterators
    // being unassigned at the end.
    friend bool operator== (const iterator& lft, const iterator& rgt)
    { return lft.m_hs == rgt.m_hs && lft.m_index == rgt.m_index && 
	(lft.m_index == -1 || lft.m_list_itr == rgt.m_list_itr); }
    friend bool operator!= (const iterator& lft, const iterator& rgt)
    { return lft.m_hs != rgt.m_hs || lft.m_index != rgt.m_index || 
	(lft.m_index != -1 && lft.m_list_itr != rgt.m_list_itr); }
    // increment and decrement
    iterator& operator++() { 
      this->next();
      return *this;
    }
    iterator operator++(int) {
      iterator temp(*this);
      this->next();
      return temp;
    }
    iterator & operator--() { 
      this->prev();
      return *this;
    }
    iterator operator--(int) {
      iterator temp(*this);
      this->prev();
      return temp;
    }

  private:
    // Find the next entry in the table
    void next() {
      ++ m_list_itr;  // next item in the list

      // If we are at the end of this list
      if (m_list_itr == m_hs->m_table[m_index].end()) {
        // Find the next non-empty list in the table
        for (++m_index; 
             m_index < int(m_hs->m_table.size()) && m_hs->m_table[m_index].empty();
             ++m_index) {}
        
        // If one is found, assign the m_list_itr to the start
        if (m_index != int(m_hs->m_table.size()))
          m_list_itr = m_hs->m_table[m_index].begin();
        
        // Otherwise, we are at the end
        else
          m_index = -1;
      }
    }

    // Find the previous entry in the table
    void prev() {
      // If we aren't at the start of the current list, just decrement
      // the list iterator
      if (m_list_itr != m_hs->m_table[m_index].begin())
	m_list_itr -- ;

      else {
        // Otherwise, back down the table until the previous
        // non-empty list in the table is found
        for (--m_index; m_index >= 0 && m_hs->m_table[m_index].empty(); --m_index) {}

        // Go to the last entry in the list.
        m_list_itr = m_hs->m_table[m_index].begin();
        hash_list_itr p = m_list_itr; ++p;
        for (; p != m_hs->m_table[m_index].end(); ++p, ++m_list_itr) {}
      }
    }
  };
//_____________________________________________________________________________________
//END OF ITERATOR


public:
    typedef typename std::list<KeyType>::iterator hash_list_itr;
    typedef typename std::list<KeyType>::const_iterator const_hash_list_itr;
    // HASH SET IMPLEMENTATION
    hashy(unsigned int init_size = 100, double re = 0.5) : m_table(init_size), m_size(0), resize(re) {}

    // Copy constructor just uses the member function copy constructors.
    hashy(const hashy<HashFunc>& old) 
    : m_table(old.m_table), m_size(old.m_size) {}

    ~hashy() {}

    hashy& operator=(const hashy<HashFunc>& old) {
    if (&old != this) {
        this->m_table = old.m_table;
        this->m_size = old.m_size;
        this->m_hash = old.m_hash;
    }
    return *this;
    }

    unsigned int size() const { return m_size; }
    void insert(const std::string& key, const int position) {
      // resize check
      unsigned int threshold = (unsigned int)floor(resize*m_table.size());
      if (m_size >= threshold) this->resize_table(2 * m_table.size());
      
      // find index
      unsigned int index = m_hash(key) % m_table.size();
      
      // use linear search to find available
      hash_list_itr p = m_table[index].begin();
      while ((*p).first != key && p != m_table[index].end()) { 
          p++;
      }
      
      // find a empty slot.
      if (p == m_table[index].end()) {
          std::pair<std::string, std::vector<int>> entry = std::make_pair(key, std::vector<int>());
          entry.second.push_back(position);
          m_table[index].push_back(entry);
          m_size += 1;
      }
      else{
        (*p).second.push_back(position);
      }
    }
      
    
    
    std::vector<int> starts(const std::string &stream) {
      std::vector<int> positions;
      unsigned int hash_value = m_hash(stream);
      unsigned int index = hash_value % m_table.size();
      hash_list_itr p = m_table[index].begin();
      for(;p != m_table[index].end(); p++){
        if((*p).first == stream){
          return (*p).second;
        }
      }
      return positions;
    }

    std::string rest(const std::string stream, int location) {
      const char fill[4] = {'a', 't', 'c', 'g'};
      for(int i = 0; i<4; i++){
        if(find(std::make_pair(stream +fill[i], location)) != end()){
          return stream + fill[i];
        }
      }
    }

    // Find the key, using hash function, indexing and list find
    bool find(const KeyType& key){
      unsigned int hash_value = m_hash(key.first);
      unsigned int index = hash_value % m_table.size();
      hash_list_itr p = std::find(m_table[index].begin(),
          m_table[index].end(), key);
      if (p == m_table[index].end())
        return false;
      else
        return true;
    }
    // Erase the key 
    int erase(const KeyType& key) {
      // Find the key and use the erase iterator function.
      iterator p = find(key);
      if (p == end())
        return 0;
      else {
        erase(p);
        return 1;
      }
    }

    // Erase at the iterator
    void erase(iterator p) {
      m_table[ p.m_index ].erase(p.m_list_itr);
    }

    // Find the first entry in the table and create an associated iterator
    iterator begin(){

      for (unsigned int i = 0; i < m_table.size(); ++i) {
        if (!m_table[i].empty()) {
          return iterator(this, i, m_table[i].begin());
        }
      }
      return end();
    }

    // Create an end iterator.
    iterator end(){
      iterator p(this);
      p.m_index = -1;
      return p;
    }
    
    //print
    void print(std::ostream& ostr){
      for (unsigned int i=0; i<m_table.size(); ++i) {
        ostr << i << ": ";
        for (typename std::list<KeyType>::iterator p = m_table[i].begin(); p != m_table[i].end(); ++p)
        ostr << ' ' << (p)->first;
        ostr << std::endl;
      }
    }
    
    

    
  private:

    void resize_table(unsigned int new_size) {

      // Create a new table with the new capacity
      std::vector< std::list<KeyType> > new_table(new_size);

      // For each element in the old table, insert it into the new table based on its new hash value
      for (iterator itr = begin(); itr != end(); ++itr) {
        unsigned int new_index = m_hash((*itr).first) % new_size;
        new_table[new_index].push_back(*itr);
      }

      // Set the hash function and size of the new table to match the old table
      m_table = new_table;
      m_hash = HashFunc();
      m_size = size();

      // Delete the old table
      new_table.clear();

    }
    




};

#endif