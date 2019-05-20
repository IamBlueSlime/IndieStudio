/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Signature.hpp
*/

#pragma once

#include <functional>
#include "indiestudio/common/ByteBuffer.hpp"

namespace IndieStudio {

    class Signature {
	public:
        template<typename Component>
        struct of {
            template<typename ...Members>
            struct with {
                static void pack(const Component &object, ByteBuffer &buffer)
                {
                    std::size_t delta = 0;
                    Signature::internalPack<Members...>(
                        static_cast<const void *>(&object), buffer, delta);
                }

                static void unpack(Component &object, ByteBuffer &buffer)
                {
                    std::size_t delta = 0;
                    Signature::internalUnpack<Members...>(
                        static_cast<void *>(&object), buffer, delta);
                }
            };
        };

	protected:
	private:
        template<typename T>
        static void packType(const T &type, ByteBuffer &buffer)
        {
            buffer << type;
        }

        template<typename T>
        static void unpackType(T &type, ByteBuffer &buffer)
        {
            buffer >> type;
        }

        template<typename ...Empty>
        static typename std::enable_if<sizeof ...(Empty) == 0>::type internalPack(
            const void *object, ByteBuffer &buffer, std::size_t &delta)
        {
            (void) object;
            (void) buffer;
            (void) delta;
        }

        template<typename ...Empty>
        static typename std::enable_if<sizeof ...(Empty) == 0>::type internalUnpack(
            void *object, ByteBuffer &buffer, std::size_t &delta)
        {
            (void) object;
            (void) buffer;
            (void) delta;
        }

        template<typename T, typename ...Other>
        static void internalPack(const void *object, ByteBuffer &buffer, std::size_t &delta)
        {
            const T *targetPos = reinterpret_cast<const T *>(static_cast<const char *>(object) + delta);
            packType(*targetPos, buffer);
            delta += sizeof(T);
            internalPack<Other...>(object, buffer, delta);
        }

        template<typename T, typename ...Other>
        static void internalUnpack(void *object, ByteBuffer &buffer, std::size_t &delta)
        {
            T *targetPos = reinterpret_cast<T *>(static_cast<char *>(object) + delta);
            unpackType(*targetPos, buffer);
            delta += sizeof(T);
            internalUnpack<Other...>(object, buffer, delta);
        }
    };

}
