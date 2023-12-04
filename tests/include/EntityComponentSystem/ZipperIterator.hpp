/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** ZipperIterator
*/

#pragma once

#include <utility>
#include <vector>

/**
 * @brief ZipperIterator class is an iterator that iterates over multiple containers at once.
 * 
 * @tparam Containers The containers to iterate over.
 */
template <typename ... Containers>
class Zipper;

/**
 * @brief ZipperIterator class is an iterator that iterates over multiple containers at once.
 * 
 * @tparam Containers The containers to iterate over.
 */
template <typename ... Containers>
class ZipperIterator {
    template <class Container>
    using iterator_t = decltype(std::declval<Container>().begin());

    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;
    public:
        using value_type = std::tuple<decltype(*(std::declval<it_reference_t<Containers>>()))&...>;
        using reference = value_type;
        using pointer = void;
        using difference_type = size_t;
        using iterator_category = std::forward_iterator_tag;
        using iterator_tuple = std::tuple<iterator_t<Containers>...>;

        /**
         * @brief Constructs a new ZipperIterator object.
         * 
         * @param it_tuple The tuple of iterators.
         * @param max The maximum number of elements to iterate over.
         */
        friend Zipper<Containers...>;
        ZipperIterator(iterator_tuple const &it_tuple, size_t max) : _current(it_tuple), _max(max), _idx(0) 
        {
            if (_idx < _max)
                if (!allSet(seq))
                    ++*this;
        };

    public:
        /**
         * @brief Copy constructor for ZipperIterator.
         * 
         * @param z The ZipperIterator to copy.
         */
        ZipperIterator(ZipperIterator const &z) : _current(z._current), _max(z._max), _idx(z._idx) {};

        /**
         * @brief Prefix increment operator.
         * 
         * @return ZipperIterator& The incremented iterator.
         */
        ZipperIterator &operator++()
        {
            incrAll(seq);
            return (*this);
        };

        /**
         * @brief Postfix increment operator.
         * 
         * @param int Dummy parameter to differentiate from prefix increment.
         * @return ZipperIterator The iterator before increment.
         */
        ZipperIterator operator++(int)
        {
            ZipperIterator temp = *this;
            ++*this;
            return (temp);
        }

        /**
         * @brief Dereference operator.
         * 
         * @return value_type The tuple of references to the current elements.
         */
        value_type operator*()
        {
            return (toValue(seq));
        };

        /**
         * @brief Arrow operator.
         * 
         * @return value_type The tuple of references to the current elements.
         */
        value_type operator->()
        {
            return (toValue(seq));
        }

        /**
         * @brief Inequality operator.
         * 
         * @param lhs The left-hand side iterator.
         * @param rhs The right-hand side iterator.
         * @return true If the iterators are not equal.
         * @return false If the iterators are equal.
         */
        friend bool operator!=(ZipperIterator const &lhs, ZipperIterator const &rhs)
        {
            return (lhs._current != rhs._current);
        }

        /**
         * @brief Equality operator.
         * 
         * @param lhs The left-hand side iterator.
         * @param rhs The right-hand side iterator.
         * @return true If the iterators are equal.
         * @return false If the iterators are not equal.
         */
        friend bool operator==(ZipperIterator const &lhs, ZipperIterator const &rhs)
        {
            return (lhs._current == rhs._current);
        }
    
    private:
        /**
         * @brief Helper function to increment all iterators.
         * 
         * @tparam Is The indices of the iterators.
         * @param seq The sequence of indices.
         */
        template <size_t ...Is>
        void incrAll(std::index_sequence<Is...>)
        {
            if (_idx >= _max)
                return;
            (..., (std::get<Is>(_current)++));
            _idx++;
            if (_idx < _max && !allSet(seq))
                incrAll(seq);
        }

        /**
         * @brief Helper function to check if all iterators are set.
         * 
         * @tparam Is The indices of the iterators.
         * @param seq The sequence of indices.
         * @return true If all iterators are set.
         * @return false If any iterator is not set.
         */
        template <size_t ...Is>
        bool allSet(std::index_sequence<Is...>)
        {
            return (... && ((*std::get<Is>(_current)).has_value()));
        }

        /**
         * @brief Helper function to convert the current elements to a tuple of references.
         * 
         * @tparam Is The indices of the iterators.
         * @param seq The sequence of indices.
         * @return value_type The tuple of references to the current elements.
         */
        template <size_t ...Is>
        value_type toValue(std::index_sequence<Is...>)
        {
            return (std::tie((*std::get<Is>(_current)).value()...));
        }
    private:
        iterator_tuple _current; /**< The tuple of iterators. */
        size_t _max; /**< The maximum number of elements to iterate over. */
        size_t _idx; /**< The current index. */

        static constexpr std::index_sequence_for<Containers...> seq{}; /**< The sequence of indices for the containers. */
};
