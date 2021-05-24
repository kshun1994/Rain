
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename))
	{
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

	// If loading successful, insert resource to map
	insertResource(id, std::move(resource));
}

//template <typename Resource, typename Identifier>
//template <typename Parameter>
//void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
//{
//	std::unique_ptr<Resource> resource(new Resource());
//	if (!resource->loadFromFile(filename, secondParam))
//	{
//		throw std::runtime_error("ResourceHolder::Load - Failed to load " + filename);
//	}
//
//	// If loading successful, insert resource to map
//	insertResource(id, std::move(resource));
//}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = resourceMap_.find(id);
	assert(found != resourceMap_.end()); // trigger breakpoint if requested texture not found in map; e.g. if texture was not loaded before accessed

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = resourceMap_.find(id);
	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto inserted = resourceMap_.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second); // will trigger a breakpoint during runtime if false; i.e. if texture is not successfully inserted
}