#pragma once

// A component class for adding Properties to Actions
// Examples of Properties include:
//   - Hitboxes/hurtboxes/collision boxes
//   - Invincibility (true, projectile, etc.)
//   - Movement
//   - Cancel window
//   - Parry box
//   - Meter expenditure

class Action; // Forward declaration
class Character; // Forward declaration

class Property
{
public:
					Property() = default;
	explicit		Property(int duration);
	virtual			~Property() {};

	virtual void	update(Character& character) {};

protected:
	int				duration_;
};
