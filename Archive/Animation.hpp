#include <SFML/Graphics.hpp>

struct Frame
{
	sf::IntRect rect;
	int duration; // in frames (60 fps)
};

class Animation
{
public:
												Animation(sf::Sprite& target);
	//virtual										~Animation();
	void										addFrame(Frame&& frame);
	void										update(double elapsed);
	const double								getLength() const { return mTotalLength; }

private:
	std::vector<Frame>							mFrames;
	double										mTotalLength;
	double										mProgress;
	sf::Sprite&									mTarget;
};