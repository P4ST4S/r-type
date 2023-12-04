/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** Zipper
*/

#pragma once

#include "ZipperIterator.hpp"

/**
 * @brief A class template that allows to iterate over multiple containers simultaneously.
 * 
 * @tparam Containers The containers to iterate over.
 */
template <typename ... Containers>
class Zipper {
    public:
        using iterator = ZipperIterator<Containers...>;
        using iterator_tuple = typename iterator::iterator_tuple;

        /**
         * @brief Construct a new Zipper object.
         * 
         * @param containers The containers to iterate over.
         */
        Zipper(Containers &... containers)
        {
            _begin = std::make_tuple(containers.begin()...);
            _size = computeSize(containers...);
            _end = computeEnd(containers...);
        };

        /**
         * @brief Returns an iterator pointing to the first element in the Zipper container.
         * 
         * @return iterator An iterator to the beginning of the Zipper container.
         */
        iterator begin()
        {
            return (iterator(_begin, _size));
        }

        /**
         * @brief Returns an iterator pointing to the end of the Zipper.
         * 
         * @return iterator An iterator pointing to the end of the Zipper.
         */
        iterator end()
        {
            return (iterator(_end, 0));
        }
    private:
        /**
         * @brief Compute the size of the smallest container.
         * 
         * @param containers The containers to iterate over.
         * @return size_t The size of the smallest container.
         */
        static size_t computeSize(Containers &...containers)
        {
            return (std::min({containers.size()...}));
        }

        /**
         * @brief Compute the end iterator tuple.
         * 
         * @param containers The containers to iterate over.
         * @return iterator_tuple The end iterator tuple.
         */
        static iterator_tuple computeEnd(Containers &... containers){
            size_t size = computeSize(containers...);
            return (std::make_tuple(std::next(containers.begin(), size)...));
        }
    private:
        iterator_tuple _begin; /**< The begin iterator tuple. */
        iterator_tuple _end; /**< The end iterator tuple. */
        size_t _size; /**< The size of the smallest container. */
};
