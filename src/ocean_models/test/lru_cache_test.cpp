#include "fvcom/LRUCache.h"
#include <gtest/gtest.h>

using namespace ocean_models;

const int NUM_OF_TEST2_RECORDS = 100;
const int TEST2_CACHE_CAPACITY = 50;

TEST(LRUCacheTest, SimplePut) {
    LRUCache<int, int> cache_lru(1);
    cache_lru.put(7, 777);
    EXPECT_TRUE(cache_lru.exists(7));
    EXPECT_EQ(777, cache_lru.get(7));
    EXPECT_EQ(1, cache_lru.size());
}

TEST(LRUCacheTest, MissingValue) {
    LRUCache<int, int> cache_lru(1);
    EXPECT_THROW(cache_lru.get(7), std::range_error);
}

TEST(LRUCacheTest, KeepsAllValuesWithinCapacity) {
    LRUCache<int, int> cache_lru(TEST2_CACHE_CAPACITY);

    for (int i = 0; i < NUM_OF_TEST2_RECORDS; ++i) {
        cache_lru.put(i, i);
    }

    for (int i = 0; i < NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
        EXPECT_FALSE(cache_lru.exists(i));
    }

    for (int i = NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST2_RECORDS; ++i) {
        EXPECT_TRUE(cache_lru.exists(i));
        EXPECT_EQ(i, cache_lru.get(i));
    }

    size_t size = cache_lru.size();
    EXPECT_EQ(TEST2_CACHE_CAPACITY, size);
}

TEST(LRUCacheTest, KeepLastUsed) {
    LRUCache<int, int> cache_lru(5);
    
    for (int i = 0; i < 5; ++i) {
        cache_lru.put(i, i);
    }

    cache_lru.get(4);
    cache_lru.get(2);
    cache_lru.get(3);
    cache_lru.get(0);
    cache_lru.get(1);

    cache_lru.put(5, 5);

    EXPECT_FALSE(cache_lru.exists(4));

    EXPECT_TRUE(cache_lru.exists(5));
    EXPECT_EQ(5, cache_lru.get(5));

    for (int i = 0; i < 4; ++i) {
        EXPECT_TRUE(cache_lru.exists(i));
        EXPECT_EQ(i, cache_lru.get(i));
    }
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
