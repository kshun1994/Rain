#include "rnpch.h"
#include "Character.h"
#include "ResourceHolder.h"
#include "Utility.h"


Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
	case Character::Yuzuriha:
		return Textures::ID::Yuzuriha;
	case Character::Enkidu:
		return Textures::ID::Enkidu;
	case Character::Shun:
		return Textures::ID::Shun;
	}
}

Character::Character(Type type, const TextureHolder& textures)
: type_(type)
, health_(0.f)
, meter_(0.f)
, facing_(Facing::Right)
, posture_(Posture::Standing)
, gravity_(1.f)
, spriteStruct_()
, actions_(20)
, actionID_(0)
{
	sprite_.setTexture(textures.get(toTextureID(type)));

	SpriteStruct EnkSprite;
	EnkSprite.spriteDims = sf::Vector2(1600, 768);

	EnkSprite.standIDs.resize(16);
	std::iota(EnkSprite.standIDs.begin(), EnkSprite.standIDs.end(), 0);
	EnkSprite.standDurs.resize(16, 5);
		
	EnkSprite.crouchIDs.resize(29); // First 3 frames are stand-to-crouch, last 10 frames are crouch-to-stand
	std::iota(EnkSprite.crouchIDs.begin(), EnkSprite.crouchIDs.end(), 16);
	EnkSprite.crouchDurs.resize(EnkSprite.crouchIDs.size(), 5);
	EnkSprite.crouchDurs[0] = 2;
	EnkSprite.crouchDurs[1] = 2;
	EnkSprite.crouchDurs[2] = 2;
	for (auto i = 19; i < EnkSprite.crouchDurs.size(); ++i)
	{
		EnkSprite.crouchDurs[i] = 4;
	}

	// For appending to stuff like jump animations
	std::vector<int> crouchToStandIDs;
	crouchToStandIDs.resize(10);
	std::iota(crouchToStandIDs.begin(), crouchToStandIDs.end(), 44);
	std::vector<int> crouchToStandDurs(10, 4);

	EnkSprite.fWalkIDs.resize(9);
	std::iota(EnkSprite.fWalkIDs.begin(), EnkSprite.fWalkIDs.end(), 54);
	EnkSprite.fWalkDurs.resize(EnkSprite.fWalkIDs.size(), 6);

	EnkSprite.bWalkIDs.resize(11);
	std::iota(EnkSprite.bWalkIDs.begin(), EnkSprite.bWalkIDs.end(), 63);
	EnkSprite.bWalkDurs.resize(EnkSprite.bWalkIDs.size(), 7);

	std::vector<int> crouchToStandInds = {35, 36, 37, 38, 39, 40, 41, 42, 43, 44};
	EnkSprite.jumpIDs.resize(9); 
	std::iota(EnkSprite.jumpIDs.begin(), EnkSprite.jumpIDs.end(), 45);
	// Jump animation has to include crouch-to-stand as recovery animation
	//EnkSprite.jumpIDs.insert(EnkSprite.jumpIDs.end(), std::make_move_iterator(crouchToStandInds.begin()), std::make_move_iterator(crouchToStandInds.end()));
	EnkSprite.jumpDurs.resize(EnkSprite.jumpIDs.size(), 5);
	EnkSprite.jumpDurs[0] = 4; // Jump startup is 4f why not
	// Append crouch-to-stand as recovery
	//EnkSprite.jumpIDs.insert(EnkSprite.jumpIDs.end(), crouchToStandIDs.begin(), crouchToStandIDs.end());
	//EnkSprite.jumpDurs.insert(EnkSprite.jumpDurs.end(), crouchToStandDurs.begin(), crouchToStandDurs.end());

	EnkSprite.jumpRecoveryIDs.resize(10);
	std::iota(EnkSprite.jumpRecoveryIDs.begin(), EnkSprite.jumpRecoveryIDs.end(), 35);
	EnkSprite.jumpRecoveryDurs.resize(EnkSprite.jumpRecoveryIDs.size(), 4);

	EnkSprite.standBIDs.resize(8);
	std::iota(EnkSprite.standBIDs.begin(), EnkSprite.standBIDs.end(), 74);
	// Enk 5B SAR: 9/3/20
	// 3 animation frames for startup
	// 1 animation frame for active
	// 4 animation frames for recovery
	EnkSprite.standBDurs = {3, 3, 2, 5, 4, 4, 4, 5};


	EnkSprite.originX = 635.f; // Horizontal center of sprite (regardless of original image dimensions)
	EnkSprite.originY = 83.f;  // Distance from bottom of image where sprite "touches the ground"


	SpriteStruct YuzuSprite;
	YuzuSprite.spriteDims = sf::Vector2(864, 640);
	YuzuSprite.originX = 432;
	YuzuSprite.originY = 44;
	for (int i = 0; i != 36; i++)
	{
		YuzuSprite.standIDs.push_back(i);
		YuzuSprite.standDurs.push_back(5);
	}

	if (type_ == Type::Enkidu)
	{
		// Initialize input triggers
		std::vector<std::vector<unsigned int>> inputs =
		{
			{ 2, 3, 6 },					// qcf
			{ 6, 2, 3 },					// dp
			{ 2, 1, 4 },					// qcb
			{ 4, 2, 1 },					// reverse dp
			{ 4, 1, 2, 3, 6},				// hcf
			{ 6, 3, 2, 1, 4},				// hcb
			{ 2, 5, 2 },					// down down
			{ 2, 3, 6, 2, 3, 6 },			// double qcf
			{ 2, 8 },						// flash kick (charge)
			{ 4, 6 },						// sonic boom (charge)
			{ 4, 6, 4, 6 },					// back-charge super
			{ 1, 3, 1, 9 },					// delta super (charge)
			{ 4, 1, 2},						// quarter-circle back to down
			{ 2, 8},						// super jump
		};

		std::vector<unsigned int> buffers =
		{
			Constants::INPUT_BUFFER_236,
			Constants::INPUT_BUFFER_623,
			Constants::INPUT_BUFFER_214,
			Constants::INPUT_BUFFER_421,
			Constants::INPUT_BUFFER_HCF,
			Constants::INPUT_BUFFER_HCB,
			Constants::INPUT_BUFFER_22,
			40,
			5,
			10,
			20,
			20,
			Constants::INPUT_BUFFER_236,
			10,
		};

		for (int i = 0; i < inputs.size(); i++)
		{
			inputTriggers_.push_back(std::make_unique<InputTrigger>());
			inputTriggers_[i]->setMotion(inputs[i]);
			inputTriggers_[i]->setBuffer(buffers[i]);
		}

		inputTriggers_[8]->setCharge(40, std::vector<bool>{true, true});					// flash kick
		inputTriggers_[9]->setCharge(40, std::vector<bool>{true, false});					// sonic boom
		inputTriggers_[10]->setCharge(40, std::vector<bool>{true, true, true, true});		// double-charge super
		inputTriggers_[11]->setCharge(40, std::vector<bool>{false, false, false, true});	// delta super

		spriteStruct_ = EnkSprite;

		// Create Enkidu standing state
		std::unique_ptr<Action> standAction = std::make_unique<Action>();
		standAction->setAnimationFrames(spriteStruct_.standIDs, spriteStruct_.standDurs, spriteStruct_.spriteDims);
		standAction->setLoopBounds(0, sum_vector(spriteStruct_.standDurs));
		Action::Boxes standBoxes;
		standBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		standBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		standAction->setBoxes(0, standBoxes);
		standAction->setCancelType(Action::CancelType::Idle);
		standAction->setCancel(Action::CancelType::All & ~Action::CancelType::Self, 0, sum_vector(spriteStruct_.standDurs));

		// Crouching state
		std::unique_ptr<HeldAction> crouchAction = std::make_unique<HeldAction>();
		crouchAction->setAnimationFrames(spriteStruct_.crouchIDs, spriteStruct_.crouchDurs, spriteStruct_.spriteDims);
		crouchAction->setLoopBounds(sum_vector(spriteStruct_.crouchDurs, 3), sum_vector(spriteStruct_.crouchDurs, 19));
		Action::Boxes crouchBoxes;
		crouchBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 180.f, 220.f)));
		crouchBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -150.f, 110.f, 100.f)));
		crouchBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 200.f)));
		crouchAction->setBoxes(0, crouchBoxes);
		Action::Boxes crouchToStandBoxes;
		crouchToStandBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		crouchToStandBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		crouchAction->setBoxes(sum_vector(spriteStruct_.crouchDurs, 20), crouchToStandBoxes);
		crouchAction->addProperty(Action::Property::Recovery, boost::irange(sum_vector(spriteStruct_.crouchDurs, 19), sum_vector(spriteStruct_.crouchDurs)));
		crouchAction->setCancelType(Action::CancelType::Basic);
		crouchAction->setCancel(Action::CancelType::Basic | Action::CancelType::Self, sum_vector(spriteStruct_.crouchDurs, 19), sum_vector(spriteStruct_.crouchDurs));

		// Forward walk state
		std::unique_ptr<HeldAction> fWalkAction = std::make_unique<HeldAction>();
		fWalkAction->setAnimationFrames(spriteStruct_.fWalkIDs, spriteStruct_.fWalkDurs, spriteStruct_.spriteDims);
		fWalkAction->setLoopBounds(0, sum_vector(spriteStruct_.fWalkDurs));
		fWalkAction->setMovePerFrame(std::vector<sf::Vector2f>(sum_vector(spriteStruct_.fWalkDurs), sf::Vector2f(7.f, 0)));
		//std::vector<sf::Vector2f> movePerFrame(std::accumulate(spriteStruct_.fWalkDurs.begin(), spriteStruct_.fWalkDurs.end(), 0));
		//float maxSpeed = 20.f;
		//for (int i = 0; i < movePerFrame.size(); ++i)
		//{
		//	movePerFrame[i].x = (maxSpeed / 2.f) * sinf(((2.f * M_PI) / movePerFrame.size()) * (i - (static_cast<float>(movePerFrame.size()) / 4.f))) + (maxSpeed / 2.f);
		//}
		//fWalkAction->setMovePerFrame(movePerFrame);
		Action::Boxes fWalkBoxes;
		fWalkBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		fWalkBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		fWalkAction->setBoxes(0, fWalkBoxes);
		fWalkAction->setCancelType(Action::CancelType::Basic);
		fWalkAction->setCancel(Action::CancelType::All & ~Action::CancelType::Self, 0, sum_vector(spriteStruct_.fWalkDurs));

		// Backward walk state
		std::unique_ptr<HeldAction> bWalkAction = std::make_unique<HeldAction>();
		bWalkAction->setAnimationFrames(spriteStruct_.bWalkIDs, spriteStruct_.bWalkDurs, spriteStruct_.spriteDims);
		bWalkAction->setLoopBounds(sum_vector(spriteStruct_.bWalkDurs, 2), sum_vector(spriteStruct_.bWalkDurs));
		bWalkAction->setMovePerFrame(std::vector<sf::Vector2f>(sum_vector(spriteStruct_.bWalkDurs), sf::Vector2f(-5.f, 0)));
		Action::Boxes bWalkBoxes;
		bWalkBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		bWalkBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		bWalkAction->setBoxes(0, bWalkBoxes);
		bWalkAction->setCancelType(Action::CancelType::Basic);
		bWalkAction->setCancel(Action::CancelType::All & ~Action::CancelType::Self, 0, sum_vector(spriteStruct_.bWalkDurs));

		// Jump recovery
		std::unique_ptr<Action> jumpRecoveryAction = std::make_unique<Action>();
		jumpRecoveryAction->setAnimationFrames(spriteStruct_.jumpRecoveryIDs, spriteStruct_.jumpRecoveryDurs, spriteStruct_.spriteDims);
		Action::Boxes jumpRecoveryBoxes;
		jumpRecoveryBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		jumpRecoveryBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		jumpRecoveryAction->setBoxes(0, jumpRecoveryBoxes);
		jumpRecoveryAction->setCancel(Action::CancelType::All & ~Action::CancelType::Idle, 4, sum_vector(spriteStruct_.jumpRecoveryDurs)); // 4f landing recovery

		// Forward jump state
		std::unique_ptr<JumpAction> fJumpAction = std::make_unique<JumpAction>();
		fJumpAction->setAnimationFrames(spriteStruct_.jumpIDs, spriteStruct_.jumpDurs, spriteStruct_.spriteDims);
		// 7 animation frames until falling loop animation
		// Falling loop is 2 animation frames
		fJumpAction->setLoopBounds(sum_vector(spriteStruct_.jumpDurs, 7), sum_vector(spriteStruct_.jumpDurs, 9));
		fJumpAction->setJumpStartup(4);
		float jumpInitialVelocity = 35.f;
		float jumpLaunchAngle     = 65.f;
		fJumpAction->setJumpBallistics(jumpInitialVelocity, jumpLaunchAngle);
		Action::Boxes fJumpStartupBoxes;
		fJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		fJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		fJumpAction->setBoxes(0, fJumpStartupBoxes);
		Action::Boxes fJumpBoxes;
		fJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -100.f, 140.f, 280.f)));
		fJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, -120.f, 100.f, 220.f)));
		fJumpAction->setBoxes(4, fJumpBoxes);
		fJumpAction->setCancelType(Action::CancelType::Basic);
		fJumpAction->setDestinationActionID(COMMON_ACTION_JUMP_RECOVERY);

		// Back jump state
		std::unique_ptr<JumpAction> bJumpAction = std::make_unique<JumpAction>();
		bJumpAction->setAnimationFrames(spriteStruct_.jumpIDs, spriteStruct_.jumpDurs, spriteStruct_.spriteDims);
		bJumpAction->setLoopBounds(sum_vector(spriteStruct_.jumpDurs, 7), sum_vector(spriteStruct_.jumpDurs, 9));
		bJumpAction->setJumpStartup(4);
		jumpLaunchAngle     = 115.f;
		bJumpAction->setJumpBallistics(jumpInitialVelocity, jumpLaunchAngle);
		Action::Boxes bJumpStartupBoxes;
		bJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		bJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		bJumpAction->setBoxes(0, bJumpStartupBoxes);
		Action::Boxes bJumpBoxes;
		bJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -100.f, 140.f, 280.f)));
		bJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, -120.f, 100.f, 220.f)));
		bJumpAction->setBoxes(4, bJumpBoxes);
		bJumpAction->setCancelType(Action::CancelType::Basic);
		bJumpAction->setCancel(Action::CancelType::All, 0, sum_vector(spriteStruct_.jumpDurs));
		bJumpAction->setDestinationActionID(COMMON_ACTION_JUMP_RECOVERY);

		// Neutral jump state
		std::unique_ptr<JumpAction> nJumpAction = std::make_unique<JumpAction>();
		nJumpAction->setAnimationFrames(spriteStruct_.jumpIDs, spriteStruct_.jumpDurs, spriteStruct_.spriteDims);
		nJumpAction->setLoopBounds(sum_vector(spriteStruct_.jumpDurs, 7), sum_vector(spriteStruct_.jumpDurs, 9));
		nJumpAction->setJumpStartup(4);
		jumpLaunchAngle     = 90.f;
		nJumpAction->setJumpBallistics(jumpInitialVelocity, jumpLaunchAngle);
		Action::Boxes nJumpStartupBoxes;
		nJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		nJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		nJumpAction->setBoxes(0, nJumpStartupBoxes);
		Action::Boxes nJumpBoxes;
		nJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -100.f, 140.f, 280.f)));
		nJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, -120.f, 100.f, 220.f)));
		nJumpAction->setBoxes(4, nJumpBoxes);
		nJumpAction->setCancelType(Action::CancelType::Basic);
		nJumpAction->setDestinationActionID(COMMON_ACTION_JUMP_RECOVERY);

		// Super jump state for testing
		std::unique_ptr<JumpAction> sJumpAction = std::make_unique<JumpAction>();
		sJumpAction->setAnimationFrames(spriteStruct_.jumpIDs, spriteStruct_.jumpDurs, spriteStruct_.spriteDims);
		sJumpAction->setLoopBounds(sum_vector(spriteStruct_.jumpDurs, 7), sum_vector(spriteStruct_.jumpDurs, 9));
		sJumpAction->setJumpStartup(4);
		jumpLaunchAngle = 60.f;
		sJumpAction->setJumpBallistics(jumpInitialVelocity * 1.25, 45);
		Action::Boxes sJumpStartupBoxes;
		sJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		sJumpStartupBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		sJumpAction->setBoxes(0, sJumpStartupBoxes);
		Action::Boxes sJumpBoxes;
		sJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -100.f, 140.f, 280.f)));
		sJumpBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, -120.f, 100.f, 220.f)));
		sJumpAction->setBoxes(4, sJumpBoxes);
		sJumpAction->setCancelType(Action::CancelType::Basic);
		sJumpAction->setCancel(Action::CancelType::All, 0, sum_vector(spriteStruct_.jumpDurs));
		sJumpAction->setDestinationActionID(COMMON_ACTION_JUMP_RECOVERY);

		setGravity(2.00f);

		// Stand B
		std::unique_ptr<Action> standBAction = std::make_unique<Action>();
		standBAction->setAnimationFrames(spriteStruct_.standBIDs, spriteStruct_.standBDurs, spriteStruct_.spriteDims);
		standBAction->setCancelType(Action::CancelType::Normal);
		//standBAction->setCancel(Action::CancelType::All, 0, sum_vector(spriteStruct_.jumpDurs));
		Action::Boxes startup1;
		startup1.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,     0.f,    0.f,  140.f,  330.f)));
		startup1.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	    0.f,    0.f,  100.f,  310.f)));
		standBAction->setBoxes(0, startup1);
		Action::Boxes startup2;
		startup2.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,    -17.f,    0.f,  155.f,  330.f)));
		startup2.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,     29.f,  -67.f,   99.f,  212.f)));
		startup2.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,    -79.f, -161.f,   82.f,  189.f)));
		startup2.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	     0.f,    0.f,  100.f,  310.f)));
		standBAction->setBoxes(3, startup2);
		Action::Boxes startup3;
		startup3.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,   -30.f,    0.f,  180.f,  330.f)));
		startup3.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,    37.f,  -51.f,  134.f,  167.f)));
		startup3.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,    54.f, -183.f,  162.f,  123.f)));
		startup3.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,   -96.f, -212.f,  103.f,  153.f)));
		startup3.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	    0.f,    0.f,  100.f,  310.f)));
		standBAction->setBoxes(5, startup3);
		Action::Boxes active;
		active.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	  -30.f,    0.f,  180.f,  330.f)));
		active.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	   45.f,  -46.f,  119.f,  259.f)));
		active.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	  111.f,  -98.f,   89.f,  143.f)));
		active.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	  -84.f, -187.f,  127.f,  202.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit1", Box::Type::Hit,	  132.f, -121.f,  173.f,  115.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit2", Box::Type::Hit,	   98.f, -216.f,  108.f,   75.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit3", Box::Type::Hit,	  127.f, -261.f,   88.f,   51.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit4", Box::Type::Hit,	  187.f, -167.f,  210.f,   92.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit5", Box::Type::Hit,	  202.f, -240.f,  119.f,   51.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit6", Box::Type::Hit,	  221.f, -251.f,  141.f,   70.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit7", Box::Type::Hit,	  228.f, -121.f,   78.f,   62.f)));
		active.push_back(std::move(std::make_shared<Box>("Enk5BActive1Hit8", Box::Type::Hit,	  280.f, -149.f,   63.f,  152.f)));
		active.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	    0.f,	0.f,  100.f,  310.f)));
		//active.push_back(std::move(std::make_shared<Box>(Box::Type::Hit,	    0.f,	0.f,  100.f,  310.f)));
		//active.push_back(std::move(std::make_shared<Box>("Enk5BActive1TestHit", Box::Type::Hit, 100.f, -100.f, 400.f, 300.f)));
		standBAction->setBoxes(8, active);
		Action::Boxes recover1;
		recover1.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	   -30.f,    0.f,  180.f,  330.f)));
		recover1.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	    45.f,  -46.f,  119.f,  259.f)));
		recover1.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	   111.f,  -98.f,   89.f,  143.f)));
		recover1.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,	   -84.f, -187.f,  127.f,  202.f)));
		recover1.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	     0.f,	 0.f,  100.f,  310.f)));
		standBAction->setBoxes(11, recover1);
		Action::Boxes recover2;
		recover2.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,    -25.f,    0.f,  140.f,  330.f)));
		recover2.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,     18.f,  -64.f,  109.f,  199.f)));
		recover2.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,    -56.f, -199.f,  118.f,  176.f)));
		recover2.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	     0.f,    0.f,  100.f,  310.f)));
		standBAction->setBoxes(21, recover2);
		Action::Boxes recover3;
		recover3.push_back(std::move(std::make_shared<Box>(Box::Type::Hurt,      0.f,    0.f,  140.f,  330.f)));
		recover3.push_back(std::move(std::make_shared<Box>(Box::Type::Push,	     0.f,    0.f,  100.f,  310.f)));
		standBAction->setBoxes(25, recover3);
		std::vector<sf::Vector2f> moveVec(sum_vector(spriteStruct_.standBDurs));
		// Stuff like this looks unnatural if you don't limit movement to the moments animation frames change
		moveVec[sum_vector(spriteStruct_.standBDurs, 1)].x =  15.f;
		moveVec[sum_vector(spriteStruct_.standBDurs, 2)].x =  15.f;
		moveVec[sum_vector(spriteStruct_.standBDurs, 3)].x =  15.f;
		moveVec[sum_vector(spriteStruct_.standBDurs, 6)].x = -10.f;
		moveVec[sum_vector(spriteStruct_.standBDurs, 7)].x =  -5.f;
		standBAction->setMovePerFrame(moveVec);

		actions_[COMMON_ACTION_STAND].first				= std::move(standAction);
		actions_[COMMON_ACTION_CROUCH].first			= std::move(crouchAction);
		actions_[COMMON_ACTION_F_WALK].first			= std::move(fWalkAction);
		actions_[COMMON_ACTION_B_WALK].first			= std::move(bWalkAction);
		actions_[COMMON_ACTION_F_JUMP].first			= std::move(fJumpAction);
		actions_[COMMON_ACTION_B_JUMP].first			= std::move(bJumpAction);
		actions_[COMMON_ACTION_N_JUMP].first			= std::move(nJumpAction);
		actions_[COMMON_ACTION_JUMP_RECOVERY].first		= std::move(jumpRecoveryAction);
		actions_[COMMON_ACTION_TEST].first				= std::move(sJumpAction);
		actions_[COMMON_ACTION_5B].first				= std::move(standBAction);

		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_STAND,  false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_CROUCH, false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_F_WALK, false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_B_WALK, false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_F_JUMP, false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_B_JUMP, false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_N_JUMP, false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_TEST,   false));
		//stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_5B,     false));
	}
	else if (type_ == Type::Yuzuriha)
	{
		spriteStruct_ = YuzuSprite;

		// Create Yuzu standing state
		std::unique_ptr<Action> standAction = std::make_unique<Action>();
		standAction->setAnimationFrames(spriteStruct_.standIDs, spriteStruct_.standDurs, spriteStruct_.spriteDims);
		standAction->setLoopBounds(0, sum_vector(spriteStruct_.standDurs));
		Action::Boxes standBoxes;
		standBoxes.push_back(std::move(std::make_shared<Box>("YuzuIdleHurt1", Box::Type::Hurt, 0.f, 0.f, 110.f, 315.f)));
		standBoxes.push_back(std::move(std::make_shared<Box>(Box::Type::Push, 0.f, 0.f, 100.f, 310.f)));
		standAction->setBoxes(0, standBoxes);

		//standAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 110.f, 315.f)));
		//standAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Push,	 0.f, 0.f, 100.f, 310.f)));

		actions_[COMMON_ACTION_STAND].first = std::move(standAction);
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_STAND,  false));
	}

	action_ = actions_[COMMON_ACTION_STAND].first.get(); // Start standing
	action_->enter(*this);
	setCurrentActionID(COMMON_ACTION_STAND);

	sprite_.setFrames(spriteStruct_.standIDs, spriteStruct_.standDurs, spriteStruct_.spriteDims);
	sprite_.setOrigin(spriteStruct_.originX, spriteStruct_.spriteDims.y - spriteStruct_.originY);

	health_			= 1000.f;
	meter_			= 0.f;
}

Character::~Character()
{
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);

	#ifdef RN_DEBUG
		// Draw a cross at character's origin position
		float segmentLength = 20.f;
		sf::Vector2f coord = this->getPosition();
		//RN_DEBUG("Current position - ({}, {})", coord.x, coord.y);

		sf::Vertex vertLine[] =
		{
			sf::Vertex(sf::Vector2f(coord.x, coord.y - segmentLength), sf::Color::Cyan),
			sf::Vertex(sf::Vector2f(coord.x, coord.y + segmentLength), sf::Color::Cyan)
		};
		sf::Vertex horiLine[] =
		{
			sf::Vertex(sf::Vector2f(coord.x - segmentLength, coord.y), sf::Color::Cyan),
			sf::Vertex(sf::Vector2f(coord.x + segmentLength, coord.y), sf::Color::Cyan)
		};

		target.draw(vertLine, 2, sf::Lines);
		target.draw(horiLine, 2, sf::Lines);
	#endif // RN_DEBUG
}

void Character::updateCurrent()
{
	//if (this->getType() == Character::Type::Enkidu)
	//{
	//	std::string facingStr = this->getFacing() == Entity::Facing::Right ? "Right" : "Left";
	//	RN_DEBUG("Character facing - {}", facingStr);
	//}

	action_->update(*this);
	sprite_.update();
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}

Character::Type Character::getCharacterType() const
{
	return type_;
}

void Character::handleInput(Player::TaggedInput input)
{
	for (int i = 0; i < inputTriggers_.size(); i++)
	{
		inputTriggers_[i]->update(input.second);
	}

	parseInput(input.second);

	Action* action = action_->handleInput(*this, actions_);

	clearActions();

	if (action != nullptr)
	{
		action_ = action;

		action_->enter(*this);
	}
}

void Character::parseInput(unsigned int input)
{
	// Flip bools in stateMap_ based on current input
	
	// Check for forward walk
	if ((input & 15) == 6)
	{
		actions_[COMMON_ACTION_F_WALK].second = true;
	}
	
	// Check for forward jump
	if ((input & 15) == 9)
	{
		actions_[COMMON_ACTION_F_JUMP].second = true;
	}
	
	// Check for back jump
	if ((input & 15) == 7)
	{
		actions_[COMMON_ACTION_B_JUMP].second = true;
	}
	
	// Check for neutral jump
	if ((input & 15) == 8)
	{
		actions_[COMMON_ACTION_N_JUMP].second = true;
	}
	
	// Check for back walk
	if ((input & 15) == 4)
	{
		actions_[COMMON_ACTION_B_WALK].second = true;
	}
	
	// Check for crouch
	if ((input & 15) <= 3)
	{
		actions_[COMMON_ACTION_CROUCH].second = true;
	}

	// Check for 5B
	if (input & Input::B)
	{
		actions_[COMMON_ACTION_5B].second = true;
	}

	// Check for stand .stand)
	if ((input & 15) == 5)
	{
		actions_[COMMON_ACTION_STAND].second = true;
	}

	std::vector<std::string> inputString =
	{
		"quarter circle forward",
		"dragon punch",
		"quarter circle backward",
		"reverse dragon punch",
		"half circle forward",
		"half circle backward",
		"down down",
		"double quarter circle forward",
		"down-charge to up",
		"back-charge to forward",
		"back-charge super",
		"delta super",
		"quarter circle back to down",
		"super jump",
	};

	for (int i = 0; i < inputTriggers_.size(); i++)
	{
		if (inputTriggers_[i]->isTriggered())
		{
			RN_DEBUG("Motion input detected: {}.", inputString[i]);
			//actions_[COMMON_ACTION_BACK_CHARGE].second = true;
			if (i == COMMON_ACTION_TEST)
			{
				actions_[COMMON_ACTION_TEST].second = true;
			}
		}
	}
}

void Character::clearActions()
{
	// Sets all bools in stateMap to false; to be used after inputs have been parsed for the current frame
	for (ActionPair& actionPair : actions_)
	{
		actionPair.second = false;
	}
}

int Character::getCurrentActionID()
{
	return actionID_;
}

void Character::setCurrentActionID(int id)
{
	actionID_ = id;
}

Action* Character::getCurrentAction() const
{
	return action_;
}

float Character::getHealth() const
{
	return health_;
}

void Character::setHealth(float value)
{
	health_ = value;
}

void Character::subtractHealth(float value)
{
	health_ -= value;
	if (health_ < 0.f)
	{
		health_ = 0.f;
	}
}

float Character::getMeter() const
{
	return meter_;
}

void Character::setMeter(float value)
{
	meter_ = value;
}

void Character::addMeter(float value)
{
	meter_ += value;
	if (meter_ > Constants::METER_MAX)
	{
		meter_ = Constants::METER_MAX;
	}
}

void Character::subtractMeter(float value)
{
	meter_ -= value;
	if (meter_ < 0.f)
	{
		meter_ = 0.f;
	}
}

Character::Posture Character::getPosture() const
{
	return posture_;
}

void Character::setPosture(Posture posture)
{
	posture_ = posture;
}

float Character::getGravity() const
{
	return gravity_;
}

void Character::setGravity(const float& gravity)
{
	gravity_ = gravity;
}

void Character::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& rect)
{
	sprite_.setFrames(frameIDs, durations, rect);
}

void Character::setAnimationRepeat(bool flag)
{
	sprite_.setRepeating(flag);
}

void Character::setCurrentAnimationTick(const int& tick)
{
	sprite_.setCurrentTick(tick);
}

void Character::setCurrentAnimationFrame(const int& frame)
{
	sprite_.setCurrentAnimationFrame(frame);
}
