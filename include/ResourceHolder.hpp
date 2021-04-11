#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:
		void													load(Identifier id, const std::string& filename);

		template <typename Parameter>
		void													load(Identifier id, const std::string& filename, const Parameter& secondParam);

		Resource&												get(Identifier id);
		const Resource&											get(Identifier id) const;

	private:
		void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)

	private:
		std::map<Identifier, std::unique_ptr<Resource>>		mResourceMap; // mapping of enums to a unique pointer to a Resource object
};

#include "ResourceHolder.inl"