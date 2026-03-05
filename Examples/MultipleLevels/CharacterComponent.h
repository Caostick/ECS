#pragma once

#include <string>

struct CharacterComponent {
	CharacterComponent(const char* name) : Name(name) {}
	CharacterComponent(CharacterComponent&&) = default;

	CharacterComponent(const CharacterComponent&) = delete;
	CharacterComponent& operator = (const CharacterComponent&) = delete;
	CharacterComponent& operator = (CharacterComponent&&) = delete;

	std::string Name;
};