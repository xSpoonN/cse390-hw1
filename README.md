# CSE 390 - HW1 - Undergraduate
> Contributors:\
> James Leonardi - <james.leonardi@stonybrook.edu>\
> Kevin Tao - <kevin.tao@stonybrook.edu>

## Input Format
The first line of the input file should be `MAX_CHARGE: X`, where X is the max charge.

The second line should be `MAX_STEPS: Y`, where Y is the max steps before termination.

Starting on the third line, a representation of the house is included with the following rules:
- `.` represents a wall. (TODO: test with ' ')
- `+` represents the charger (note that the robot will start on the charger).
- `0-9` represent dirt (0 = clean, 9 = maximum dirt).

Example input file:

<pre>
MAX_CHARGE: 300
MAX_STEPS: 5000
..........
.+0412432.
.101132.7.
.....4..5.
.102449.1.
.023005.0.
..........
</pre>

If the given house representation is not a perfect rectangle, the blank spaces will be filled with walls.

## Output
Upon completion, the program will generate a file `output.txt` in the directory it was run from.
Each line in this file represents a choice the robot has made (move direction or stay).
Additionally, the line will include whether the robot cleaned dirt or gained charge during that step.

At the end of the output file, a summary is displayed, which includes:
- The total steps taken
- The amount of dirt left
- Whether the robot ran out of battery
- Whether the robot was successful in cleaning the house

## Solution Approach
In order to effectively separate the algorithm from the robot, we took an object-oriented approach.

### Class Information
The `Robot` class is responsible for keeping track of the robot's current position, battery level, and steps taken.
It also exposes methods related to the robot's "sensors" (adjacent wall sensor, battery sensor, and dirt sensor) for use by the algorithm.
See `robot.h` for more information.

The `Controller` class houses the movement algorithm and is exclusively concerned with deciding the robot's next step.
It has one public method `get_next_step()`, and uses no information outside of the robot's specified sensors to make its decision.
See `controller.h` for more information.

### Normal Operation
Upon being invoked, the program parses the input file, extracting the max charge and max steps fields.
It then stores the characters representing the house map inside a `vector<vector<char>>`.
A `Robot` object is constructed with this information, and when `robot.clean_house()` is invoked, the robot enters a loop until it runs out of steps or finds itself out of battery.

The Robot "has a" Controller, which it creates in its constructor.
While the Robot *does* have access to the house map, it is used exclusively as metadata (such as terminating the program early if there is no dirt to clean and the robot is on its charger), and it does not expose any of this to the controller.
At each iteration of the loop, the robot will ask its controller for the next step.
The details are outlined below:

#### Controller Decisions
The controller lays "breadcrumbs" to path back to the charger.
When invoked, it will first check whether this breadcrumb trail is longer than the robot's remaining battery.
If so, it will begin pathing back to the charger, and upon reaching the charger, it will remain there until the robot's battery matches its recorded initial battery.

Otherwise, it will call a naive algorithm which chooses a random direction (making sure not to choose a direction blocked by a wall).
