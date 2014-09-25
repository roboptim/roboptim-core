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

#ifndef ROBOPTIM_CORE_CACHE_HXX
# define ROBOPTIM_CORE_CACHE_HXX

# include <algorithm>

namespace roboptim
{
  template <typename K, typename V>
  LRUCache<K,V>::LRUCache (size_t size)
    : size_ (size),
      tracker_ (),
      map_ (),
      pool_ (size)
  {
  }

  template <typename K, typename V>
  LRUCache<K,V>::~LRUCache ()
  {}

  template <typename K, typename V>
  size_t LRUCache<K,V>::size () const
  {
    return size_;
  }

  template <typename K, typename V>
  void LRUCache<K,V>::resize (size_t size)
  {
    if (size != size_)
      {
	size_ = size;
	allocate ();
      }
  }

  template <typename K, typename V>
  void LRUCache<K,V>::allocate ()
  {
    // FIXME: breaks iterators in map_?
    map_.rehash (size_);
    tracker_.resize (size_);
  }

  template <typename K, typename V>
  void LRUCache<K,V>::clear ()
  {
    map_.clear ();
    tracker_.clear ();
  }

  template <typename K, typename V>
  void LRUCache<K,V>::insert (const K& key, const V& value)
  {
    V& v = insert (key);
    v = value;
  }

  template <typename K, typename V>
  V& LRUCache<K,V>::insert (const K& key)
  {
    typename valuePool_t::iterator v_it;

    // If the key is already in the map
    iterator iter = map_.find (key);
    if (iter != map_.end ())
      {
	bump (iter->first);
	return *(iter->second);
      }

    // If the cache is full, remove the LRU element
    if (map_.size () >= size_)
      {
	const_iterator it = findLRU ();

	// Remove the key from the tracker
	tracker_.pop_front ();

	// Update the iterator for the pool
	v_it = it->second;

	// Remove the LRU element from the map
	map_.erase (it);
      }
    else // cache not full
      {
	// Get the first free pool element
	v_it = pool_.begin ()
	  + static_cast<typename valuePool_t::iterator::difference_type>
	  (tracker_.size ());
      }

    // Add the new key to the tracker
    tracker_.push_back (key);

    // Add the new key to the map
    typename map_t::value_type p (key, v_it);
    map_.insert (p);

    return *v_it;
  }

  template <typename K, typename V>
  void LRUCache<K,V>::update (iterator iter, const V& value)
  {
    bump (iter->first);
    *(iter->second) = value;
  }

  template <typename K, typename V>
  V& LRUCache<K,V>::update (iterator iter)
  {
    bump (iter->first);
    return *(iter->second);
  }

  template <typename K, typename V>
  typename LRUCache<K,V>::iterator
  LRUCache<K,V>::begin ()
  {
    return map_.begin ();
  }

  template <typename K, typename V>
  typename LRUCache<K,V>::iterator
  LRUCache<K,V>::end ()
  {
    return map_.end ();
  }

  template <typename K, typename V>
  typename LRUCache<K,V>::const_iterator
  LRUCache<K,V>::cbegin () const
  {
    return map_.cbegin ();
  }

  template <typename K, typename V>
  typename LRUCache<K,V>::const_iterator
  LRUCache<K,V>::cend () const
  {
    return map_.cend ();
  }

  template <typename K, typename V>
  typename LRUCache<K,V>::const_iterator
  LRUCache<K,V>::find (const K& key) const
  {
    return map_.find (key);
  }

  template <typename K, typename V>
  V& LRUCache<K,V>::operator [] (const K& key)
  {
    typename map_t::iterator it = map_.find (key);

    if (it != map_.end ())
      return update (it);
    else
      return insert (key);
  }

  template <typename K, typename V>
  typename LRUCache<K,V>::const_iterator
  LRUCache<K,V>::findLRU () const
  {
    // Find the LRU element
    return map_.find (tracker_.front ());
  }

  template <typename K, typename V>
  void LRUCache<K,V>::bump (const K& key)
  {
    // TODO: optimize this (remove the need for a search algorithm)
    typename keyTracker_t::iterator
      iter = std::find (tracker_.begin (), tracker_.end (), key);
    assert (iter != tracker_.end ());

    tracker_.splice (tracker_.end (),
                     tracker_,
                     iter);

    assert (tracker_.size () <= size_);
  }


  template <typename K, typename V>
  std::ostream& LRUCache<K,V>::print (std::ostream& o) const
  {
    o << "{";
    for (const_iterator iter = map_.begin ();
         iter != map_.end (); iter++)
      {
	if (iter != map_.begin ())
	  o << ", ";
	o << iter->first << ": " << *(iter->second);
      }
    o << "}";

    return o;
  }

  template <typename K, typename V>
  std::ostream&
  operator<< (std::ostream& o, const LRUCache<K,V>& cache)
  {
    return cache.print (o);
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_CACHE_HXX
