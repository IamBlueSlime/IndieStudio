/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common ByteBuffer.hpp
*/

#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

namespace IndieStudio {

    class ByteBuffer {
    public:
        ByteBuffer(std::size_t size = 128)
        {
            this->buffer = new char[size];
            this->size = size;
        }

        ByteBuffer(char *buf, std::size_t size) : ByteBuffer(size)
        {
            std::memcpy(this->buffer, buf, size);
            this->writeCursor = size;
        }

        ~ByteBuffer()
        {
            if (this->buffer != nullptr)
                delete [] this->buffer;
        }

        template<typename T>
        void write(const T &value, std::size_t size = sizeof(T))
        {
            this->assertWriteMemory(size);
            std::memcpy(this->buffer + this->writeCursor, &value, size);
            this->writeCursor += size;
        }

        template<typename T>
        void read(T *dest, std::size_t size = sizeof(T))
        {
            if (!this->assertReadMemory(size))
                return;

            std::memcpy(dest, this->buffer + this->readCursor, size);
            this->readCursor += size;
        }

        std::size_t getSize()
        {
            return this->size;
        }

        std::size_t getWriteCursor()
        {
            return this->writeCursor;
        }

        std::size_t getReadCursor()
        {
            return this->readCursor;
        }

        template<typename T>
        ByteBuffer &operator <<(const T &value)
        {
            this->write<T>(value);
            return *this;
        }

        template<typename T>
        ByteBuffer &operator >>(T &value)
        {
            this->read<T>(&value);
            return *this;
        }

        char *operator *()
        {
            return this->buffer;
        }

    protected:
    private:
        char *buffer;
        std::size_t size = 128;
        std::size_t step = 128;
        std::size_t writeCursor = 0;
        std::size_t readCursor = 0;

        void assertWriteMemory(std::size_t of)
        {
            while (this->writeCursor + of >= this->size) {
                this->buffer = static_cast<char *>(
                    std::realloc(this->buffer, this->size + this->step));
                this->size += this->step;
            }
        }

        bool assertReadMemory(std::size_t of)
        {
            return this->readCursor + of <= this->size;
        }
    };

}
