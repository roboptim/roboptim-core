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
# include <boost/unordered_map.hpp>

# include <roboptim/core/detail/utility.hh>

namespace roboptim
{
  /// \brief LRU (Least Recently Used) cache.
  ///
  /// Note that the cache is unidirectional, i.e. the map does not store the
  /// actual keys, since this was designed with large vectors in mind.
  ///
  /// \tparam K type for keys.
  /// \tparam V type for values.
  ///
  /// Some of the ideas used here come from
  /// Tim Day's "LRU cache implementation in C++"
  /// (http://timday.bitbucket.org/lru.html)
  template <typename K, typename V, typename H = boost::hash<K> >
  class LRUCache
  {
  public:
    /// \brief Type of keys.
    typedef K key_t;

    /// \brief Type of const reference to key.
    typedef typename detail::const_ref<key_t>::type const_key_ref;

    /// \brief Type of values.
    typedef V value_t;

    /// \brief Type of const reference to key.
    typedef typename detail::const_ref<value_t>::type const_value_ref;

    /// \brief Hasher type.
    typedef H hasher_t;

    /// \brief Hash type used by the Boost map.
    typedef std::size_t hash_t;

    /// \brief Pool of values stored as std::vector.
    /// The container is properly aligned if an Eigen type is used.
    typedef typename detail::aligned_vector_type<value_t>::type valuePool_t;

    /// \brief List used to track key usage.
    /// Note: we use hashes rather than vectors to prevent costly
    /// allocations.
    typedef std::list<hash_t> keyTracker_t;

    /// \brief Key type for the underlying map.
    typedef hash_t mapKey_t;
    typedef detail::const_ref<mapKey_t>::type const_mapKey_ref;

    /// \brief Map from map's key to iterator in the value pool.
    typedef boost::unordered_map
    <mapKey_t, typename valuePool_t::iterator> map_t;

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
    const_iterator find (const_key_ref key) const;

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
    V& operator [] (const_key_ref key);

    /// \brief Insert a value into the cache.
    /// \param key key of the element.
    /// \param value value of the element.
    void insert (const_key_ref key, const_value_ref value);

    /// \brief Clear the cache.
    void clear ();

    /// \brief Display the cache on the specified output stream.
    virtual std::ostream& print (std::ostream&) const;

  protected:
    /// \brief Hash function used in the cache.
    /// \param key key to hash.
    /// \return hashed key.
    hash_t hash_function (const_key_ref key) const;

  private:
    /// \brief Allocate memory based on the cache's size.
    void allocate ();

    /// \brief Update an existing cached value.
    void update (iterator iter, const_value_ref value);

    /// \brief Update an existing cached value.
    V& update (iterator iter);

    /// \brief Insert a value into the cache.
    V& insert (const_key_ref key);

    /// \brief Notice the tracker that the key was used.
    void bump (const_mapKey_ref key);

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

    /// \brief Hasher for the cache.
    hasher_t hasher_;
  };

  template <typename K, typename V, typename H>
  std::ostream&
  operator<< (std::ostream& o, const LRUCache<K,V,H>& cache);

} // end of namespace roboptim

# include <roboptim/core/cache.hxx>

#endif //! ROBOPTIM_CORE_CACHE_HH
