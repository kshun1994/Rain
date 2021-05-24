#pragma once

#include "Category.h"
#include "Command.h"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>


class SceneNode :	public		sf::Transformable, 
					public		sf::Drawable, 
					private		sf::NonCopyable
{
public: 
	typedef std::shared_ptr<SceneNode> Ptr;
	//typedef SceneNode* Ptr;

public:
									SceneNode();

	void							attachChild(Ptr child);
	Ptr								detachChild(const SceneNode& node);

	void							update();

	sf::Vector2f					getWorldPosition() const;
	sf::Transform					getWorldTransform() const;

	virtual unsigned int			getCategory() const;
	void							onCommand(const Command& command);

private:
	virtual void					updateCurrent();
	void							updateChildren();

	virtual void					draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void							drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr>				mChildren;
	SceneNode*						mParent;
};