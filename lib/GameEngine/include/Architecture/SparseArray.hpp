/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** SparseArray
*/

#pragma once

#include <vector>
#include <optional>


/**
 * @brief A sparse array implementation that stores optional components.
 * 
 * @tparam Component The type of the component stored in the array.
 */
template <typename Component> 
class SparseArray {
    public:
        using value_type = std::optional<Component>; /**< The type of the stored value. */
        using allocator_type = std::allocator<value_type>; /**< The type of the allocator used to allocate memory for the stored values. */
        using reference_type = value_type &; /**< The type of the reference to the stored value. */
        using const_reference_type = value_type const &; /**< The type of the const reference to the stored value. */
        using container_t = std::vector<value_type>; /**< The type of the container used to store the values. */
        using size_type = typename container_t::size_type; /**< The type of the size of the container. */
        using iterator = typename container_t::iterator; /**< The type of the iterator for the container. */
        using const_iterator = typename container_t::const_iterator; /**< The type of the const iterator for the container. */

    public:
        /**
         * @brief Default constructor.
         */
        SparseArray() : _data() {};

        /**
         * @brief Copy constructor.
         * 
         * @param copy The SparseArray to copy.
         */
        SparseArray(SparseArray const &copy) : _data(copy._data) {};

        /**
         * @brief Move constructor.
         * 
         * @param move The SparseArray to move.
         */
        SparseArray(SparseArray &&move) noexcept : _data(move._data) 
        {
            move._data.clear();
        };

        /**
         * @brief Destructor.
         */
        ~SparseArray() 
        {
            _data.clear();
        };

        /**
         * @brief Copy assignment operator.
         * 
         * @param copy The SparseArray to copy.
         * @return SparseArray& The copied SparseArray.
         */
        SparseArray & operator=(SparseArray const &copy) 
        {
            _data = copy._data;
            return *this;
        };

        /**
         * @brief Move assignment operator.
         * 
         * @param move The SparseArray to move.
         * @return SparseArray& The moved SparseArray.
         */
        SparseArray & operator=(SparseArray &&move) noexcept 
        {
            _data = move._data;
            move._data.clear(); 
            return *this;
        };

        /**
         * @brief Access operator.
         * 
         * @param idx The index of the value to access.
         * @return reference_type The reference to the value at the given index.
         */
        reference_type operator[](size_t idx)
        {
            return _data[idx];
        };

        /**
         * @brief Const access operator.
         * 
         * @param idx The index of the value to access.
         * @return const_reference_type The const reference to the value at the given index.
         */
        const_reference_type operator[](size_t idx) const
        {
            return _data[idx];
        };

        /**
         * @brief Returns an iterator to the beginning of the container.
         * 
         * @return iterator The iterator to the beginning of the container.
         */
        iterator begin()
        {
            return _data.begin();
        };

        /**
         * @brief Returns a const iterator to the beginning of the container.
         * 
         * @return const_iterator The const iterator to the beginning of the container.
         */
        const_iterator begin() const
        {
            return _data.begin();
        };

        /**
         * @brief Returns a const iterator to the beginning of the container.
         * 
         * @return const_iterator The const iterator to the beginning of the container.
         */
        const_iterator cbegin() const
        {
            return _data.cbegin();
        };

        /**
         * @brief Returns an iterator to the end of the container.
         * 
         * @return iterator The iterator to the end of the container.
         */
        iterator end()
        {
            return _data.end();
        };

        /**
         * @brief Returns a const iterator to the end of the container.
         * 
         * @return const_iterator The const iterator to the end of the container.
         */
        const_iterator end() const
        {
            return _data.end();
        };

        /**
         * @brief Returns a const iterator to the end of the container.
         * 
         * @return const_iterator The const iterator to the end of the container.
         */
        const_iterator cend() const
        {
            return _data.cend();
        };

        /**
         * @brief Returns the size of the container.
         * 
         * @return size_type The size of the container.
         */
        size_type size() const
        {
            return _data.size();
        };

        /**
         * @brief Inserts a value at the given position.
         * 
         * @param pos The position to insert the value at.
         * @param comp The value to insert.
         * @return reference_type The reference to the inserted value.
         */
        reference_type insertAt(size_type pos , Component const &comp)
        {
            if (_data.size() <= pos)
                _data.resize(pos);
            _data[pos].reset();
            _data[pos].emplace(comp);
            return _data[pos];
        };

        /**
         * @brief Inserts a value at the given position.
         * 
         * @param pos The position to insert the value at.
         * @param comp The value to insert.
         * @return reference_type The reference to the inserted value.
         */
        reference_type insertAt(size_type pos, Component &&comp)
        {
            if (_data.size() <= pos)
                _data.resize(pos + 1);
            _data[pos].reset();
            _data[pos].emplace(std::forward<Component>(comp));
            return _data[pos];
        };

        /**
         * @brief Emplaces a value at the given position.
         * 
         * @tparam Params The types of the parameters to construct the value with.
         * @param pos The position to emplace the value at.
         * @param args The arguments to construct the value with.
         * @return reference_type The reference to the emplaced value.
         */
        template <class ... Params >
        reference_type emplaceAt ( size_type pos, Params &&... args)
        {
            std::allocator<allocator_type> myAllocator = _data.get_allocator();

            if (_data.size() <= pos)
                _data.resize(pos + 1);
            if (_data[pos].has_value())
                std::allocator_traits<decltype(myAllocator)>::destroy(myAllocator, &(_data[pos]));
            std::allocator_traits<decltype(myAllocator)>::construct(myAllocator, &(_data[pos]), std::forward<Params>(args)...);
            return _data[pos];
        }

        /**
         * @brief Erases the value at the given position.
         * 
         * @param pos The position to erase the value at.
         */
        void erase(size_type pos)
        {
            _data[pos].reset();
        };

        /**
         * @brief Returns the index of the given value.
         * 
         * @param value The value to find the index of.
         * @return size_type The index of the value, or the size of the container if the value is not found.
         */
        size_type getIndex(value_type const &value) const
        {
            for (size_type i = 0; i < _data.size(); i++) {
                if (_data[i] == value)
                    return i;
            }
            return _data.size();
        };

    private:
        container_t _data ; /**< The container used to store the values. */
};