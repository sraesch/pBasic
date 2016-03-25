// This file is part of pBasic, a basic collection of concurrent
// algorithms and container.
//
// Copyright (C) 2016 Sascha Raesch <sascha.raesch@gmail.com>
//
// It is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <cstdint>
#include <atomic>
#include <memory>
#include <functional>

namespace pBasic
{
    /**
     * An implementation related to std::vector for conccurent access.
     */
    template<class T,
             class Allocator = std::allocator<T>>
    class concurrent_vector
    {
    public:
        using value_type = T;
        using reference = T&;
        using allocator_type = Allocator;

    public:
        concurrent_vector()
            : m_alloc()
            , m_capacity(0)
            , m_preSize(0)
            , m_size(0)
            , m_values(nullptr)
            , m_allocMutex()
        {
        }

        ~concurrent_vector()
        {
            clear();
            shrink_to_fit();
        }

    public:
        /**
         * @return Returns true if the vector has zero elements and false otherwise.
         */
        inline bool empty() const
        {
            return m_size <= 0;
        }

        /**
         * @return Returns the size of the vector, i.e., the number of elements.
         */
        inline std::size_t size() const
        {
            return m_size;
        }

        /**
         * @return Returns the capacity, i.e., how many elements the vector can have
         *         without perform a reallocation.
         */
        inline std::size_t capacity() const
        {
            return m_capacity;
        }

        /**
         * Adds new element.
         *
         * @param value Constant reference onto the given value
         */
        void push_back(const value_type &value)
        {
            const std::size_t idx = m_preSize++;

            if (idx >= m_capacity)
                internalReserve(idx + 1, m_size);

            {
                boost::shared_lock<boost::shared_mutex> lock(m_allocMutex);

                m_alloc.construct(m_values + idx, value);
                m_size++;
            }
        }

        /**
         * Adds new element.
         *
         * @param value Constant reference onto the given value
         */
        void push_back(value_type &&value)
        {
            const std::size_t idx = m_preSize++;

            if (idx >= m_capacity)
                internalReserve(idx + 1, m_size);

            {
                boost::shared_lock<boost::shared_mutex> lock(m_allocMutex);

                m_alloc.construct(m_values + idx, std::move(value));
                m_size++;
            }
        }

        /**
         * Directly constructs element inside the vector.
         *
         * @param value Constant reference onto the given value
         */
        template<class... Params>
        void emplace_back(Params... params)
        {
            const std::size_t idx = m_preSize++;

            if (idx >= m_capacity)
                internalReserve(idx + 1, m_size);

            {
                boost::shared_lock<boost::shared_mutex> lock(m_allocMutex);

                m_alloc.construct(m_values + idx, params...);
                m_size++;
            }
        }

        /**
         * Reserves space for at least 'num' elements.
         * The resulting capacity can be larger.
         *
         * @param num The minimal capacity
         */
        void reserve(std::size_t num)
        {
            internalReserve(num, m_size);
        }

        /**
         * Clears the vector.
         */
        void clear()
        {
            boost::upgrade_lock<boost::shared_mutex> lock(m_allocMutex);
            boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

            // call destructor for each element
            std::for_each(m_values, m_values + m_size, [this](value_type &v) { m_alloc.destroy(&v); });

            // reset size
            m_size = 0;
            m_preSize = 0;
        }

        /**
         * Reallocates memory s.t. capacity == size.
         */
        void shrink_to_fit()
        {
            boost::upgrade_lock<boost::shared_mutex> lock(m_allocMutex);
            boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

            internalChangeCapacityNoLock(m_size, m_size);
        }

        /**
         * Resizes the array to n elements. If n < size(), then elements are released and
         * created otherwise with the default constructor.
         *
         * @param n The new size of the vector
         */
        void resize(std::size_t n)
        {
            boost::upgrade_lock<boost::shared_mutex> lock(m_allocMutex);
            boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

            // release elements if array is shrinked
            if (n < m_size)
                std::for_each(m_values + n, m_values + m_size, [this](value_type &v) { m_alloc.destroy(&v); });
            else if(n > m_size)
                // create new elements of the new size is larger
            {
                internalChangeCapacityNoLock(n, m_size);

                // create new elements
                std::for_each(m_values + m_size, m_values + n, [this](value_type &v) { m_alloc.construct(&v); });
            }

            m_size = n;
        }

    private:
        /**
        * Reserves space for at least 'num' elements.
        * The resulting capacity can be larger.
        *
        * @param num The minimal capacity
        * @param numCopy The number of elements to copy from the old array.
        */
        void internalReserve(std::size_t num, std::size_t numCopy)
        {
            if (num > m_capacity)
            {
                const std::size_t newCapacity = nextCapacity(m_capacity, num);
                internalChangeCapacityWithLock(newCapacity, numCopy);
            }
        }

        /**
         * Changes the capacity
         *
         * @param newCapacity The new capacity
         * @param numCopy The number of elements to copy from the old array
         */
        void internalChangeCapacityWithLock(std::size_t newCapacity, std::size_t numCopy)
        {
            boost::upgrade_lock<boost::shared_mutex> lock(m_allocMutex);
            boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

            internalChangeCapacityNoLock(newCapacity, numCopy);
        }

        /**
        * Changes the capacity
        *
        * @param newCapacity The new capacity
        * @param numCopy The number of elements to copy from the old array
        */
        void internalChangeCapacityNoLock(std::size_t newCapacity, std::size_t numCopy)
        {
            // allocate
            assert(newCapacity >= m_size);
            value_type *newValues = m_alloc.allocate(newCapacity);

            // move elements from old array to the new array
            for (std::size_t i = 0; i < numCopy; ++i)
                m_alloc.construct(newValues + i, std::move(m_values[i]));

            // delete old array and set new
            m_alloc.deallocate(m_values, m_capacity);
            m_values = newValues;

            // change capacity
            m_capacity = newCapacity;
        }

    private:
        /**
         * Computes the next capacity based on the current capacity and a given
         * minimal capacity.
         *
         * @param curCapacity The current capacity
         * @param minCapacity A lower bound for the resulting capacity
         *
         * @return Returns a new capacity value >= curCapacity, minCapacity
         */
        static std::size_t nextCapacity(std::size_t curCapacity, std::size_t minCapacity)
        {
            while (curCapacity < minCapacity)
                curCapacity += std::max<std::size_t>(1, curCapacity / 4);

            return curCapacity;
        }

    private:
        allocator_type                  m_alloc;
        std::atomic<std::size_t>        m_capacity;
        std::atomic<std::size_t>        m_preSize;
        std::atomic<std::size_t>        m_size;
        value_type                      *m_values;

        boost::shared_mutex             m_allocMutex;
    };

} // end of namespace pBasic
