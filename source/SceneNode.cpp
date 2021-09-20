#include "rnpch.h"
#include "SceneNode.h"

SceneNode::SceneNode()
	: children_()
	, parent_(nullptr)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->parent_ = this;
	children_.push_back(std::move(child));
}


SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(children_.begin(), children_.end(), [&](Ptr& p) -> bool { return p.get() == &node; }); // return iterator to wanted node

	assert(found != children_.end()); // trigger breakpoint if wanted node is not found in vector (i.e. iteration in previous line reached .end() and thus outside vector)

	Ptr result = std::move(*found); // move found node out of container into result (empties vector element)
	result->parent_ = nullptr; // set parent pointer to nullptr
	children_.erase(found); // remove empty element from vector
	return result;
}

void SceneNode::detachChildren()
{
	for (Ptr child : children_)
	{
		this->detachChild(*child);
	}
}

std::vector<SceneNode*> SceneNode::getChildren()
{
	std::vector<SceneNode*> ptrs(children_.size());

	for (std::shared_ptr<SceneNode>& child : children_)
	{
		ptrs.push_back(child.get());
	}

	return ptrs;
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity; // identity transform does nothing; base to multiply all parent transforms onto

	for (const SceneNode* node = this; node != nullptr; node = node->parent_) // iterate up recursively through parent nodes
	{
		transform = node->getTransform() * transform;
	}

	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

unsigned int SceneNode::getCategory() const // return unsigned int instead of Category::Type allows for multiple categories via | operator
{
	return Category::Scene;
}

void SceneNode::onCommand(const Command& command)
{
	if (command.category & getCategory()) // if category of command matches node category
	{
		command.action(*this); // run command on node
	}

	for (const Ptr& child : children_)
	{
		child->onCommand(command); // propagate command down node children chain
	}
}

void SceneNode::update()
{
	updateCurrent();
	updateChildren();
}

void SceneNode::updateCurrent()
{
	// do nothing by default
}

void SceneNode::updateChildren()
{
	for (const Ptr& child : children_)
	{
		child->update();
	}
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);

	for (const Ptr& child : children_)
	{
		child->draw(target, states);
	}
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Ptr& child : children_)
	{
		child->draw(target, states);
	}
}