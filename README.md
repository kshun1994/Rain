# Rain Engine
Rainy days

# Ideas
- Wrapper to output keyboard/joystick-agnostic input
- Input stream
	- Store previous inputs
	- Retrieve inputs over specific buffer period number of frames
	- Helper function to call that specifies input priorities and buffer period (-e.g.-, half circles get longer input windows than quarter circles)
- `addState()` helper function to call when adding a state to an entity that takes care of mutual exclusivities in state
- Rollback implementation [[Pseudocode example](https://gist.github.com/rcmagic/f8d76bca32b5609e85ab156db38387e9)]
	- Store game state
	- Restore game state
	- Recalculate game state over n frames
- Hit vectors?
	- Just use parabolic arc formula and inputs of initial angle, velocity, and object weight?
- Allow for coding basic movement (walk, dashes, jump) as special/unique moves so you can cancel into/out of them (like Kaori dash from EFZ)

# TODO
- [ ] Make it so camera view movement is always smooth
	- Currently if one character moves forward a lot in the space of one frame the camera will also move a lot in one frame, which is really jarring
	- Need to figure out the math to make it into a smooth glide to "catch up" to the actual midpoint between characters
	- Probably some function where camera view movement velocity is proportional to distance from destination
- [ ] Make it so Action inputs can be specified as "on press" versus "input for frame" 
	- i.e. whether or not holding an input will do something on the first frame or if it's triggered by the actual button press
- [ ] Add a more sophisticated Action priority system instead of the iteration order thing currently done in Action::handleInput()
- [ ] Have some way of specifying draw priority (i.e. be able to determine what entities are on top of the others on any given display frame)
- [ ] Add camera zoom-in/-out based on between-Character distance and shift camera view to stay within stage bounds
- [ ] Fix the horrible mess that is the whole unique_ptr/shared_ptr conversions happening with SceneNode objects (like in Box detachment in Action)
- [ ] Create an in-game logger to display things in World
	- Create an Observer that monitors World entities for things to print to log
- [ ] Fix charge input trigger so the charge has to be continuous
- [ ] Expand settings state to allow joystick button binding
	- Analog stick threshold/axis binding
	- Make sure dpad inputs override analog stick inputs
- [ ] Device select state: 
	- List available input devices
	- "Move" device left or right to claim 1P/2P
- [ ] Pause screen stuff
	- [ ] Frame stepping
---------------------------------------------------------------------------------------------------------------------------------------------
- [X] Add cancellable recovery to jumps
	- Enkidu uses his crouch-to-stand animation after landing
- [X] Extending Action class
	- [X] Make it so boxes can change throughout Action duration
	- [X] Add recoveries that are cancellable into specific other Actions
	- [X] Fix the animation frame looping (or lack thereof) on JumpAction
- [X] Make it so inputting Actions during other options doesn't just queue it up indefinitely
	- E.g. currently hitting 5B during a jump will have Enk immediately do 5B upon landing no matter how long the jump is
- [X] Add jumping
- [X] Both forward and back walks have transition-into animations; include those somehow into Action::enter()
- [X] Create a class to handle attacks
	- Generate hitbox/hurtbox/collision box child nodes to attach to a character 
	- Ideally the class would just read in whatever data generated by the below move creation GUI
	- Would make use of animation class; read in animation frame details from same data file
- [X] Fix hardcoded pixel values
	- Window resolution should be changeable and everything should scale to match
- [X] Consolidate Facing functions to Entity and remove from Character
- [X] Need some way to easily identify Boxes and their subtypes within World
- [X] Finish collidebox intersect resolution math
- [X] Add state-based system to Characters
- [X] Added teabagging functionality
- [X] Make it so changing Entity::Facing actually changes sprite direction
- [X] Create a trigger class for charge inputs or expand InputTrigger to handle those too
- [X] Figure out where Enkidu's disappeared to
- [X] **BUG** When keybindings are changed, old keybindings appear to persist and simultaneously add inputs while in GameState
	- If ESDF is set to directions instead of WASD, numpad input when pressing F ("right") is simultaneously 6 and 5. Pressing D ("down") is simultaneously 2 and 6
- [X] Gamepad buttons
	- Try plugging a bunch of controllers in and have the engine output whatever button's being pressed to debug log
- [X] Include update rate in Applications::mStatisticsText
- [X] Try creating some test trigger functions for motion inputs
	- Observe the input stream with variable buffer, output debug log text when relevant motion is detected
- [X] Get rid of command-style entity modification
	- Just handle everything through input stream
	- Entities have access to inputs over the last X frames
	- Inputs are represented as controller state per frame, represented as bit flag sum int
- [X] SOCD
	- Currently possible to input opposite directions at the same time---needs to be fixed
- [X] Input wrapper---incorporate into Player class
	- Player class takes InputWrapper as member
	- Device ID
	- Keybindings
	- Passes stream of device-agnostic input (i.e. numpad notation or something)
- [X] Create a full animation class
	- Takes a spritesheet and a range of sprite IDs; get texture rect pos/dim from IDs
	- Need to be able to specify number of frames each animation frame is displayed for
	- Current rudimentary class needs to be modified to take an array of durations
- [X] Tick-based game logic updates
	- Rendering rate is not tied to game logic
	- Game logic updates at 60 fps
	- Rendering takes snapshots of game logic; if a user drops frames then rendering will skip game logic that has already been done

# Notes from Skullgirls
## Menus and options
- Button config
	- Tap to set is most convenient
	- Should be available on character select
- Character select
	- Players should be able to unpick characters at any point
	- Players should always be able to return to character select (start menu, after match, etc.)
	- Wincount should be visible; not winstreak
	- If a player goes back to char select after a match, the other player's character/palette/etc should already be pre-picked; they shouldn't have to redo their choices but they should be able to unpick if they want
- Pausing
	- Pausing should be easy in singleplayer modes and difficult to do accidentally in two-player modes (SG requires start button hold for 30f)
	- Pause menu should clearly label which player paused
	- If a controller disconnects only briefly (<5f or so) the game should not bring up any menus; non-participating controllers should not be able to do anything at all once controllers are assigned to players
- Random stage/music should rotate every match instead of picking just once
- Replay saving
	- Both online and offline
	- Be able to set default settings for whether replays should be saved or not
	- Have an option in-game to export a replay to video file
- Netplay
	- Have fight request from training mode (apparently this is difficult to implement)
	- Have all-play lobbies (multiple games can be done at the same time in a lobby)
- Training mode
	- Have the option for dummy recordings to correct side for inputs or not; correct by default
	- Any random elements should be specifiable in option
	- Character-specific options (unique meters etc) should be modifiable
	- Training options should also be offered before character select etc
	- Hitbox display should be available to players
	- Frame data display (startup and advantage), should be dynamic
		- SG-style visual detailed frame data display
		- Color-coded property display
	- 1f increment move-fowards (pause -> "see game" -> left/right to move back/forward?)
	- Save states (save/load/clear) with bindable button
	- Dummy modes
		- Repeat
		- As reversal (play back everything until first button press, button is "pressed" on reversal)
## Gameplay
- Trades
	- Traded hits should be handled differently
	- Hitstop should keep you in attacking animation frames *specifically* for trades
	- Helps observers parse exactly what moves are happening
- Hurtboxes during hitstun/blockstun should look like your character's idle
	- Hurtboxes should never be random or something you have to guess about after hit
	- Moves should not whiff because your opponent was hit by a previous attack
- Crossups
	- Opponent should move *toward* you after the crossup; you do not lose positional advantage after a crossup
	- Face the opponent in the opposite direction from the way the crossing-up character is facing; opponent facing should not update until after the crossup
	- Opponents should always be pushed backward relative to their own facing
- Prevent unblockables
	- Straight-up unblockable attacks should be blockable/avoidable if the opponent is already in blockstun
	- Left-right unblockables
		- Objects (aegis/projectiles) should always be able to be blocked relative to the opponent character
	- High-low unblockables
		- SG: you are treated as blocking both ways specifically during hitstop
	- Throw-hit unblockables
		- Solved by below point
- Only allow characters to be hit by one thing per frame
- Point: where a character actually is in space
	- When jumping you land once your point reaches the ground
	- Point should be able to go through the ground to some extent during jumping attacks to allow attacks to still hit crouching grounded characters (otherwise landing will occur before the attack makes contact if the hitbox is high relative to the point)
- Visual effects should be palettized; or at least P1/P2 visual effects should be different if not using palettized effects
- Combo meter
	- Combo number
	- Damage
	- Valid/invalid states, which hits were invalid
	- Appear immediately from the second-hit of a combo onward (not after a combo ends like in SF4)
- Audio
	- Hit voice
- Frame skip: game can easily be made faster if you want
	- Inputs need to sampled on frames not shown
	- Otherwise you get stuff where blocking inputs aren't read on skipped frames and attacks are randomly unblockable
- Blocking
	- Do not have a preblock state; if you are in a state where you can block, you should be able to move straight into blocking
- Stop timer during character cinematics
	- Also don't let cinematic attacks like supers kill midway through the animation
- Throw
	- Make sure that if a character is being thrown and they are somehow able to get out of being thrown, do not allow opponent character to still have control over them
		- Example: weird MvC2 throw bugs
	- If one-button throws: do not allow other inputs on the same frame as throw to give that input and also a throw attempt (GG throw input OS)
		- Essentially if someone goes for a throw they shouldn't be allowed to attempt multiple things at once
	- Crouch teching?
		- SG: can't tech throws if holding down
		- SFV: Crouch teching just whiffs throw unlike cr.LK like in SF4
- Hits
	- If two entities are hit at the same time, the hit should get distributed to both entities
	- No "pet absorbs the hit for the owner" trash like in ACR
- Inputs
	- "Sloppy" DPs (626) should be a thing
	- Allow qcf, dp, and then hcf is always qcf
	- KOF has a unique priority system where inputs are allowed to be done one notch "past" its movement
		- 236 is allowed to be 2369
		- 623 is allowed to be 6236
		- Thus if an input has a 236 in it, ending at 9 will force it to be a fireball as DPs cannot end in 9, even if a 6 was done immediately prior to the 236
- Mike Z hot takes
	- No reason to force a player to hit three buttons at the same time for any reason ever
		- Pad players can't do this easily
		- Players should never be forced to use macros in order to perform basic gameplay mechanics
	- No reason to ever require players to do a half circle
	- Guard meter/guard break
		- RISC is the gold standard: you can block as long as you like, but the longer you block the riskier the eventual hit
		- Guard break leads to being punished for blocking correctly and can lead to setups that guarantee guard break
	- Stun/dizzy
		- Getting hit a lot shouldn't lead to being penalized by getting hit even more

# GUI for creating moves
- Mouse-drawable rectangles for hitbox/hurtbox/collision
- Display position/dims in text boxes for precise editing
- Editable frame data and animation frame IDs/durations
- Customizable hit vector parameters and hitstun/blockstun durations
- Onion skinning to compare changes in boxes between adjacent animation frames
- "Play" button with speed settings that allows you to play back the move animation
- Components:
	- Viewing window to render animation frames and drawn boxes
	- Text boxes and radio buttons for parameters and properties
		- Box positions and dimensions (drawable and selectable as hit/hurt/collision)
		- Extra entity placement (-e.g.-, sword trails and particles)
		- Invul (different types)
		- Character movement (to be propagated up to parent character node)
		- Inflicted damage/hitstun/blockstun/proration
		- Blocking flags (high/low/mid/air/etc.)
	- Animation frame selector (text box for spritesheet ID) and duration
	- "Propagate to identical animation frame" option to mirror data to frames with same animation frame
	- Data saves to external file to be read by game when generating character movetable
		- Automatically tag frames with hitboxes with an Active tag so other functions can differentiate between startup/active/recovery
