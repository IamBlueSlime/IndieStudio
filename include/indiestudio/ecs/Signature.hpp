/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Signature.hpp
*/

#pragma once

#include <array>
#include <functional>
#include "indiestudio/common/ByteBuffer.hpp"

namespace IndieStudio {

    class Signature {
	public:
        template<typename Component>
        struct of {
            template<typename ...Members>
            struct with {
                struct at {
                    std::array<std::size_t, sizeof ...(Members)> offsets;

                    at(std::array<std::size_t, sizeof ...(Members)> offsets)
                        : offsets(offsets)
                    {}

                    void pack(const Component &object, ByteBuffer &buffer)
                    {
                        Signature::internalPack<sizeof ...(Members), Members...>(
                            static_cast<const void *>(&object), buffer,
                            this->offsets, 0);
                    }

                    void unpack(Component &object, ByteBuffer &buffer)
                    {
                        Signature::internalUnpack<sizeof ...(Members), Members...>(
                            static_cast<void *>(&object), buffer,
                            this->offsets, 0);
                    }
                };
            };
        };

	protected:
	private:
        template<typename T>
        static void packType(const T &type, ByteBuffer &buffer)
        {
            static_assert(std::is_fundamental<T>::value);
            buffer << type;
        }

        template<typename T>
        static void unpackType(T &type, ByteBuffer &buffer)
        {
            static_assert(std::is_fundamental<T>::value);
            buffer >> type;
        }

        template<std::size_t N, typename ...Empty>
        static typename std::enable_if<sizeof ...(Empty) == 0>::type internalPack(
            const void *object, ByteBuffer &buffer,
            const std::array<std::size_t, N> &offsets, std::size_t idx)
        {
            (void) object;
            (void) buffer;
            (void) offsets;
            (void) idx;
        }

        template<std::size_t N, typename ...Empty>
        static typename std::enable_if<sizeof ...(Empty) == 0>::type internalUnpack(
            void *object, ByteBuffer &buffer,
            const std::array<std::size_t, N> &offsets, std::size_t idx)
        {
            (void) object;
            (void) buffer;
            (void) offsets;
            (void) idx;
        }

        template<std::size_t N, typename T, typename ...Other>
        static void internalPack(const void *object, ByteBuffer &buffer,
            const std::array<std::size_t, N> &offsets, std::size_t idx)
        {
            const T *targetPos = reinterpret_cast<const T *>(static_cast<const char *>(object) + offsets[idx]);
            packType(*targetPos, buffer);
            internalPack<N, Other...>(object, buffer, offsets, idx + 1);
        }

        template<std::size_t N, typename T, typename ...Other>
        static void internalUnpack(void *object, ByteBuffer &buffer,
            const std::array<std::size_t, N> &offsets, std::size_t idx)
        {
            T *targetPos = reinterpret_cast<T *>(static_cast<char *>(object) + offsets[idx]);
            unpackType(*targetPos, buffer);
            internalUnpack<N, Other...>(object, buffer, offsets, idx + 1);
        }
    };

}
