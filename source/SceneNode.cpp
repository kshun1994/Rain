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

void SceneNode::rotateChildToEnd(SceneNode* child)
{
	for (int i = 0; i < children_.size(); i++)
	{
		if (children_[i].get() == child)
		{
			std::rotate(children_.begin() + i, children_.begin() + i + 1, children_.end());
		}
	}
}

SceneNode* SceneNode::getParent()
{
	return parent_;
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

sf::Vector2f SceneNode::getCollideDims() const
{
	// SceneNodes have no dimensions by default
	return sf::Vector2f(width_, height_);
}

sf::Vector2f SceneNode::getCollideOffset() const
{
	// SceneNodes have no offset by default
	return sf::Vector2f(xOffsetCorrected_, yOffset_);
}

unsigned int SceneNode::getType() const
{
	return 0;
}

bool SceneNode::checkIntersect(SceneNode& lhs, SceneNode& rhs)
{
	// Get rects of each box
	sf::FloatRect lhsRect = sf::FloatRect(lhs.getWorldPosition().x + lhs.getCollideOffset().x - (lhs.getCollideDims().x / 2),    // Need to account for the fact that Box origin isn't top-left
										  lhs.getWorldPosition().y + lhs.getCollideOffset().y - lhs.getCollideDims().y, 
										  lhs.getCollideDims().x, lhs.getCollideDims().y);

	sf::FloatRect rhsRect = sf::FloatRect(rhs.getWorldPosition().x + rhs.getCollideOffset().x - (rhs.getCollideDims().x / 2),
										  rhs.getWorldPosition().y + rhs.getCollideOffset().y - rhs.getCollideDims().y, 
										  rhs.getCollideDims().x, rhs.getCollideDims().y);

	return lhsRect.intersects(rhsRect);
}

void SceneNode::checkNodeCollide(SceneNode& node, std::set<std::pair<SceneNode*, SceneNode*>>& collidePairs)
{
	// Check intersection with given node and to make sure collisions aren't happening between boxes owned by the same entity
	if (this->parent_ != node.parent_ && checkIntersect(*this, node))
	{
		// std::set objects can only have one instance of any given unique object
		// std::minmax makes it so order doesn't matter: minmax(a, b) == minmax(b, a) and thus 
		//     a given intersect pair will only be added once even if computed twice
		collidePairs.insert(std::minmax(this, &node));
	}

	for (Ptr& child : children_)
	{
		child->checkNodeCollide(node, collidePairs);
	}

}

void SceneNode::checkSceneCollide(SceneNode& sceneGraph, std::set<std::pair<SceneNode*, SceneNode*>>& collidePairs)
{
	checkNodeCollide(sceneGraph, collidePairs);

	for (Ptr& child : sceneGraph.children_)
	{
		checkSceneCollide(*child, collidePairs);
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