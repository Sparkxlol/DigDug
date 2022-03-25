*   --- Dig Dug ---
* 
* Team Members:
*   - Owen Keeler
*   - Will McAllister
*   - Eric Garcia
*   - Julia Albay
* 
* Classes:
* 
*   Utility:
*   - Spritesheet: This class inherits off sprite and is used a normal sprite
*   as the main object changed in each class. The one feature that spritesheet
*   adds off the normal sf::Sprite is the ability to change frames or change
*   where the viewable texture is on an image, allowing for easy animations
*   and image switches.
*   - Animator: This class utilises the spritesheet class to make abstracted
*   animations that can be auto-run or changed frame by frame. This makes
*   creating animations much easier, and animations much easier to work with.
*
*   Game Objects:
*   - GameObject: This class is the main class on which most interactive
*   objects or entites inherit off. It provides general functionality for
*   entities such as allowing movement, direction and activity.
*   - DigDug: This class is the "player" class and is the main class for
*   which the player inputs to. The player can move using the arrow keys,
*   and can shoot using z. DigDug can dig through the sand, editing
*   its appearance as well as kill enemy using a shot game object. Movement
*   is based on the 16x16 grid starting from the top left and digDug is forced
*   to move on either the x or y axis if not at both axes. When digDug dies
*   the level restarts and has a set amount of lives to do this until a full
*   "game over".
*   - Shot: This class is the pump or shot which Dig Dug uses to kill enemies.
*   This pump can attach to enemies which forces them to stay still, and with
*   three shots kills the opposing enemy. It is triggered with the z key and
*   has a delay between each shot preventing spam.
*   - Enemy: This class is where both the Pooka and the Fygar inherit from.
*   It has the main movement for each type and has random and fun choices
*   for where and when to move. This includes a pathing mechanism, a random
*   direction choice, the ability to float, the chance to run away from a 
*   falling rock and the eventual attempt to escape the level. These movements
*   are all decided by the collision with the sand the player has dug and 
*   makes for interesting gameplay. The enemy also eventually speeds up
*   as rounds progress making the game harder and harder as you get farther.
*   Enemies can be killed by pumping them three times continuously allowing
*   the player to gain points and move onto the next level if all enemies are
*   defeated. When pumped an enemy can be walked through allowing for creative
*   escapes from the player.
*   - Pooka: This class is basically the default enemy which virtually adds no
*   features onto the enemy class other than animations.
*   - Fygar: This class is the more unique enemy which has the ability to shoot
*   fire to attack the player. This fire shoots out at random times but is 
*   preceded by a white flashing animation to alert the player.
*   - Fire: This class is the fire which Fygar shoots out to attack the player.
*   It grows larger as time passes and eventually is set unactive. If the player
*   collides with it they die instantly as they would an enemy.
*   - Rock: This class is the rock which can fall and kill both the enemies
*   and the player. It is triggered if the sand under it is gone and falls
*   at a constant speed until hitting the ground. When an enemy or the player
*   is hit they are instantly killed bringing them to the bottom of where the
*   rock hits, until crumbling, setting the object non-active.
*   - Sand: This class does not actually inherit from object due to the lack
*   of needed methods, but it is close enough to a game object to be considered
*   one. Sand is one of the most integral parts of Dig Dug and every other
*   object depends on the existence of it. Dig Dug can dig through the sand
*   which allows enemies and the rock to go through it. This can create
*   unique paths and allows the enemy to attack the player. Sand has two
*   spritesheets rather than the normal one because one acts as the path
*   and one is the sand being dug. This top sand is masked as the player
*   walks through it creating a path to walk through. This sand is then
*   checked by the enemy to see if they can walk through it.
* 
*   Other:
*   - Score: This class is the score which pops up when either a enemy
*   is killed or a fruit is collected. The score which is then added to
*   the UI depends on where the enemy was killed and the method in which
*   it was killed (rock/player). This score is shown for a short time
*   before disappearing. When fruits are collided with by the player
*   a score is also shown, this score increasing as the levels progress.
*   - UI: This class is the user interface which shows the player useful
*   information such as the score, lives, round and the current high score.
*   The background of sand is shown using this class depending on the level.
*   It also shows the main menu before starting the game or after a game over.
*       Score: The score shown is gained from killing enemies or grabbing
*       fruit which spawns when enough sand is dug in a level. Killing 
*       an enemy with a rock provides more score and is an incentive to use
*       more challenging methods to kill enemies, rather than just pumping.
*       Lives: The lives in the game start at 3, showing only 2 lives in the
*       UI, like how the original game did. These lives decrease every time
*       dig dug is killed and once 0 lives are reached the game restarts.
*       A life can be gained at 10k points and after that at every 40k, which
*       can help to prolong a run.
*       Round: The game starts at round 1, resetting back to round 1 at round
*       99. These rounds are loaded from a file in the game class and have 12
*       unique layouts that repeat after completed. These rounds are also
*       shown through the flowers at the top, a small one representing a single
*       level while a large one represents 10.
*       Highscore: The highscore is saved into a text file at the end of a run
*       if it is higher than the current highscore. This can then be shown
*       when the game is opened again to always have a goal to beat.
*   - Game: This is the main class which changes and affects every other class
*   and object in the game. It does many things including loading every level,
*   creating every object, and updating and drawing each object every frame.
*   It loads levels from files and creates object, never deleting them until
*   destroyed. Rather, these objects are set as non active which is checked
*   when drawing/updating an object, preventing object creation during gameplay.
*   Game is passed into every single object and must be forward declared to
*   prevent errors. Passing game helps to allows interaction between objects
*   such as calling methods of other objects from another object or seeing
*   if other objects are active. Game also checks when levels are completed
*   either through dig dug dying or all enemies dying, loading the next level
*   either with a whole new layout or with non-killed enemies reset. One of the
*   more unique aspects of the game class is the existence of the fruit
*   spritesheet which spawns when enough tiles are dug through. This fruit 
*   then provides extra score if collided by the player.
* 
* Workflow:
*   - Start Up: When the game first is run, all of the objects are created
*   and the UI loads the main menu. This main menu can be passed loading
*   the initial level.
*   - Gameplay: Once into actual gameplay, the game object is updated every 100 frames.
*   The game updates every single active object with most objects running
*   collision first to check around them. They they either have move functions
*   or have sf::Clocks to change their behaviors. Sounds are played and stopped
*   based on actions done by the player or objects. Each of these objects
*   then run their animations, changing depending on what happened that frame
*   or if enough time has passed. After every object has completed its
*   update function, the game then draws all active functions to the screen
*   beginning with the ui and sand to allow dig dug and enemies to pass
*   over these images. This loop then completes until either a level is completed
*   or lost, loading the new one or reseting to the main menu, or if the 
*   exit button is clicked which causes all objects to be deleted including
*   the game.
* 
* The Future:
*   - 
*   - Two Player Mode: This would involve keeping the killed enemies and updated
*   sand for each player in a vector or class which could be done either in the
*   game class or in additional class. You would have to also have multiples
*   scores which could be done easily by implementing multiple UIs that load for
*   each player.
*

