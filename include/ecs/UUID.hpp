/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** UUID
*/

#ifndef UUID_HPP_
#define UUID_HPP_

#include <string>
#include <sstream>
#include <time.h>
#include <iostream>

class UUID {
	public:
		UUID() = default;
		inline UUID(const std::size_t id) { id_ = id; };
		inline UUID(const UUID& other) { id_ = std::size_t(other.id_); };

		~UUID() = default;

        inline void generate() { id_ = clock(); };
        inline const std::size_t &getUUID() const { return id_; };

        inline bool operator==(const UUID& other) { return id_ == other.getUUID(); };
        inline bool operator!=(const UUID& other) { return id_ != other.getUUID(); };
        inline UUID& operator=(const UUID& other) { id_ = other.getUUID(); return *this; };

	protected:
	private:
        std::size_t id_;
};

std::ostream &operator<<(std::ostream &stream, const UUID &id);

#endif /* !UUID_HPP_ */
