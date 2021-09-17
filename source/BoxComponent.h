#pragma once

class Entity;

class BoxComponent
{
public:
	enum Type
	{
		Push		= 0,
		Hit			= 1,
		Hurt		= 2
	};

public:
							BoxComponent() = default;
							BoxComponent(Entity& entity, Type type, float offsetX, float offsetY, float width, float height); // Defined by pixel offset from relevant sprite
	virtual					~BoxComponent();

	void					update();
	void					draw(sf::RenderTarget& target);

	Type					getType();

	sf::RectangleShape		getBox();
	bool					checkIntersect(const sf::FloatRect& rect);

private:
	Entity&					entity_;
	sf::RectangleShape		box_;

	Type					type_;

	float					offsetX_;
	float					offsetY_;
};

