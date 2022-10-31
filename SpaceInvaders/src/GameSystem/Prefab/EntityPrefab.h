#pragma once
#include <vector>
#include <memory>

template<typename... Ts>
struct EntityPrefab
{
	EntityPrefab(Ts... args)
	{
		components = std::tuple<Ts...>(args...);
	}

	std::shared_ptr<EntityPrefab<Ts...>> MakeShared()
	{
		return std::make_shared<EntityPrefab<Ts...>>(std::get<Ts>(components)...);
	}

	std::tuple<Ts...> components;
};