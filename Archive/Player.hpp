#include <CommandQueue.hpp>
#include <SFML/Window/Event.hpp>

#include <map>

class Player
{
public:
	enum Action
	{
		InputLeft,
		InputRight,
		InputUp,
		InputDown,
		InputA,
		InputB,
		InputC,
		InputD,
		ActionCount
	};

public:
	void										handleEvent(const sf::Event& event, CommandQueue& commands);
	void										handleRealtimeInput(CommandQueue& commands);

	void										assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key							getAssignedKey(Action action) const;

private:
	static bool									isRealtimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Action>			mKeyBinding;
	std::map<Action, Command>					mActionBinding;
};