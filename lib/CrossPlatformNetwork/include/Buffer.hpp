/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformServer
*/

#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <deque>

#include <utility>
#include <memory>
#include <iomanip>

#define DEQUE_DATA(dequeObj) (&(*(dequeObj.begin())))

namespace cpn {
    /**
     ** @brief Class made to store raw data.
     **
     */
    class Buffer {
    public:
        /**
         * @brief Construct a new Buffer object.
         */
        Buffer();
        Buffer(const void *data, size_t size);
        Buffer(const std::deque<uint8_t> &data, size_t size);
        Buffer(const std::deque<uint8_t> &data);
        Buffer(const cpn::Buffer &other);
        Buffer(cpn::Buffer &&other);

        /**
         * @brief Destroy the Buffer object.
         */
        ~Buffer();

        Buffer operator=(const cpn::Buffer &other)
        {
            if (this != &other)
                this->copy(other);
            return (*this);
        }

        Buffer& operator=(Buffer&& other) {
            if (this != &other) {
                this->_buffer = std::move(other._buffer);
            }
            return (*this);
        }

        /**
         ** @brief Copy the buffer in parameter into this buffer.
         **
         ** @param other The buffer you want to copy.
         */
        void copy(const cpn::Buffer &other) noexcept;

        /**
         ** @brief Write at the end of the buffer {size} bytes of {data}.
         **
         ** @param data The data you want to be written.
         ** @param size The size bytes of data.
         ** @return size_t
         */
        size_t writeIn(const void *data, size_t size) noexcept;

        /**
         ** @brief Write at the end of the buffer {size} byte of {data}.
         **
         ** @param data The data you want to be written.
         ** @param size The size byte of data.
         ** @return size_t
         */
        size_t writeIn(const std::deque<uint8_t> &data, size_t size) noexcept;

        /**
         ** @brief Write at the whole vector at the end of the buffer.
         **
         ** @param data The data you want to be written.
         ** @return size_t
         */
        size_t writeIn(const std::deque<uint8_t> &data) noexcept;

        /**
         ** @brief Return an std::vector filled with up to {size} bytes in.
         **
         ** @param size The number of byte you want to read.
         ** @return std::vector<uint8_t>
         */
        std::deque<uint8_t> readIn(size_t size) noexcept;

        /**
         ** @brief Return everything that was in the buffer.
         **
         */
        std::deque<uint8_t> readIn(void) noexcept;

        /**
         ** @brief Empty the buffer.
         **
         */
        void clear(void) noexcept;

        /**
         ** @brief Return the size of the buffer.
         **
         ** @return size_t
         */
        size_t size(void) const noexcept;

        /**
         ** @brief Return the buffer.
         **
         ** @return std::deque<uint8_t>&
         */
        std::deque<uint8_t> &get(void) noexcept;

        /**
         ** @brief Return the buffer.
         **
         ** @return const std::deque<uint8_t>&
         */
        const std::deque<uint8_t> &get(void) const noexcept;

    private:
        std::deque<uint8_t> _buffer;
    };

}
