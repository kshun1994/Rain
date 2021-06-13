#pragma once
#include <SFML/Graphics.hpp>

class Action
{
};

class InputDelegate
{
public:
	virtual void SetInput() = 0;
};

	class HasInput : public InputDelegate
	{
	public:
		virtual void SetInput() override
		{
		};
	};

	class HasNoInput : public InputDelegate
	{
	public:
		virtual void SetInput() override { /* Nothing */ };
	};

class HitboxDelegate
{
public:
	virtual void setHitbox(sf::IntRect rect) = 0;
};

	class HasHitbox : public HitboxDelegate
	{
	public:
		virtual void setHitbox(sf::IntRect rect) override;
	};

	class HasNoHitbox : public HitboxDelegate
	{
	public:
		virtual void setHitbox(sf::IntRect rect) override { /* Nothing */ };
	};

class HurtboxDelegate
{
public:
	virtual void setHurtbox(sf::IntRect rect) = 0;
};

	class HasHurtbox : public HurtboxDelegate
	{
	public:
		virtual void setHurtbox(sf::IntRect rect) override;
	};

	class HasNoHurtbox : public HurtboxDelegate
	{
	public:
		virtual void setHurtbox(sf::IntRect rect) override { /* Nothing */ };
	};

class CollideboxDelegate
{
public:
	virtual void setCollidebox(sf::IntRect rect) = 0;
};

	class HasCollidebox : public CollideboxDelegate
	{
	public:
		virtual void setCollidebox(sf::IntRect rect) override;
	};

	class HasNoCollideBox : public CollideboxDelegate
	{
	public:
		virtual void setCollidebox(sf::IntRect rect) override { /* Nothing */ };
	};
