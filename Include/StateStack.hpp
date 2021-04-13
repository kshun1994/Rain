#pragma once
#include <State.hpp>
#include <StateIdentifiers.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

class StateStack : private sf::NonCopyable
{
public: 
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

public:
	explicit StateStack(State::Context context);

	template <typename T>
	void														registerState(States::ID stateID); // inserts state mappings
	void														update(sf::Time dt);
	void														draw();
	void														handleEvent(const sf::Event& event);

	void														pushState(States::ID stateID); // add state to active stack
	void														popState(); // remove state from active stack
	void														clearStates();

	bool														isEmpty() const;

private:
	State::Ptr													createState(States::ID stateID);
	void														applyPendingChanges();

private:
	struct PendingChange // an object that stores a change type and the state to perform the change on
	{
		explicit												PendingChange(Action action, States::ID stateID = States::None);

		Action													action;
		States::ID												stateID;
	};

private:
	std::vector<State::Ptr>										mStack;
	std::vector<PendingChange>									mPendingList;
	State::Context												mContext;
	std::map<States::ID, std::function<State::Ptr()>>			mFactories; // factory functions that create states on demand, mapped to IDs
};