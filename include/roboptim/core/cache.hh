// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
//
// This file is part of the roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ROBOPTIM_CORE_CACHE_HH
# define ROBOPTIM_CORE_CACHE_HH

# include <list>
# include <vector>
# include <boost/unordered_map.hpp>

namespace roboptim
{
  /// \brief LRU (Least Recently Used) cache.
  /// \tparam K type for keys.
  /// \tparam V type for values.
  /// Some of the ideas used here come from
  /// Tim Day's "LRU cache implementation in C++"
  /// (http://timday.bitbucket.org/lru.html)
  template <typename K, typename V>
  class LRUCache
  {
  public:
    /// \brief Type of keys.
    typedef K key_t;

    /// \brief Type of values.
    typedef V value_t;

    typedef std::list<key_t> keyTracker_t;
    typedef std::vector<value_t> valuePool_t;

    /// \brief Map from key to iterator in the value pool.
    typedef boost::unordered_map
    <key_t, typename valuePool_t::iterator> map_t;

    typedef typename map_t::const_iterator const_iterator;
    typedef typename map_t::iterator       iterator;

  public:
    /// \brief Constructor.
    /// Note: all the memory is allocated in the constructor.
    /// \param size maximum size of the cache.
    LRUCache (size_t size = 10);

    /// \brief Destructor.
    virtual ~LRUCache ();

    /// \brief Size of the cache.
    size_t size () const;

    /// \brief Change the size of the cache.
    void resize (size_t size);

    /// \brief Find an element in the cache.
    const_iterator find (const K& key) const;

    /// \brief Iterator to the beginning of the cache.
    iterator begin ();

    /// \brief Iterator to the end of the cache.
    iterator end ();

    /// \brief Iterator to the beginning of the cache.
    const_iterator cbegin () const;

    /// \brief Iterator to the end of the cache.
    const_iterator cend () const;

    /// \brief Access a cached element.
    /// \param key key to the element.
    /// \return reference to the element.
    V& operator [] (const K& key);

    /// \brief Insert a value into the cache.
    /// \param key key of the element.
    /// \param value value of the element.
    void insert (const K& key, const V& value);

    /// \brief Clear the cache.
    void clear ();

    /// \brief Display the cache on the specified output stream.
    virtual std::ostream& print (std::ostream&) const;

  private:
    /// \brief Allocate memory based on the cache's size.
    void allocate ();

    /// \brief Update an existing cached value.
    void update (iterator iter, const V& value);

    /// \brief Update an existing cached value.
    V& update (iterator iter);

    /// \brief Insert a value into the cache.
    V& insert (const K& key);

    /// \brief Notice the tracker that the key was used.
    void bump (const K& key);

    /// \brief Find Least Recently Used element.
    const_iterator findLRU () const;

  private:
    /// \brief Size of the cache.
    size_t size_;

    /// \brief Tracker for the least recently used elements.
    keyTracker_t tracker_;

    /// \brief Map containing iterators to cached values.
    map_t map_;

    /// \brief Pool of values to prevent reallocations.
    valuePool_t pool_;
  };

  template <typename K, typename V>
  std::ostream&
  operator<< (std::ostream& o, const LRUCache<K,V>& cache);

} // end of namespace roboptim

# include <roboptim/core/cache.hxx>

#endif //! ROBOPTIM_CORE_CACHE_HH
