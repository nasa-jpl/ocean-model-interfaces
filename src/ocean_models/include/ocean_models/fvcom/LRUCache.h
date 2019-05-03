#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>


namespace ocean_models
{

/**
 * Generic implementation of an LRUCache
 */
template <class K, class V>
class LRUCache
{

private: 

public:
    typedef typename std::pair<K, V> key_value_pair_t;
    typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;


    LRUCache() :
        max_size(10)
    {}

    LRUCache(size_t max_size) :
        max_size(max_size)
    {}

    /**
     * Put a value into the cache with a key
     * @param key key corresponding to the value
     * @param value value to add to the list
     */
    void put(const K &key, const V &value)
    {
        //find the key in the map
        auto it = item_map.find(key);

        //push the key and value to the front of the list
        item_list.push_front(key_value_pair_t(key, value));

        //if the key is already in the map erase it
        if (it != item_map.end()) 
        {
            item_list.erase(it->second);
            item_map.erase(it);
        }

        //add the iterator for key,value pair to the map
        item_map[key] = item_list.begin();
        
        //remove key,values if over the max size
        if (item_map.size() > max_size) 
        {
            auto last = item_list.end();
            last--;
            item_map.erase(last->first);
            item_list.pop_back();
        }
    }

    /**
     * Get a value from the cache
     * @param key key corresponding to the value to retrieve
     */
    V& get(const K &key)
    {
        //find the key
        auto it = item_map.find(key);
        if (it == item_map.end()) 
        {
            throw std::range_error("There is no such key in cache");
        } 
        else 
        {
            //move the item to the front of the list
            item_list.splice(item_list.begin(), item_list, it->second);
            return it->second->second;
        }

    }

    /**
     * Check if a key is in the cache
     * @param key key to check for
     */
    bool exists(const K &key) const
    {
        return item_map.find(key) != item_map.end();
    }

    size_t size() const
    {
        return item_map.size();
    }
private:
    std::unordered_map<K, list_iterator_t> item_map;
    std::list<key_value_pair_t> item_list;
    size_t max_size;

};

}
#endif