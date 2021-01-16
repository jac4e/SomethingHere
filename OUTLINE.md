# Main Outline

    PROGRAM START

    User defines properties of world and number of agents through GUI

    Agents are randomly generated

    GENERATION LOOP

        SIMULATION

            World is generated based on user defined properties

            Agents are then randomly placed in the world

            Simulation starts

            Simulation ends when all agents are dead (death occurs when agent runs out of energy or consumes too much energy)

        REPRODUCTION

            Agent fitnesses are determined by timeAlive*energyUsed
            
            50% of agents are then removed from the population, the better the agents' fitness the less likely they are to be removed

            Each remaining agent is randomly paired off with another agent of similar or higher fitness. These couple's each produce 2 children
            
            Children's properties are determined using a variation of 1 point cross-over, these properties are then slightly mutated using some sort of gaussian distribution with the mean being the pre-mutated value

## Agent Outline

Agents have two sets of properties, physical and personality.

### Personality

Personality properties range from 0-1 and effect the agents decision-making.

- Aggression
  - How likely they are to attack other agents instead of collecting energy packets
- Pickiness
  - How likely they will go for the most fulfilling energy source instead of the nearest source

### Physical

Physical properties are skill point based, agents have a limit of X amount of points to be distributed to the physical properties.

- Storage
  - Determines how much energy an agent can store
- Strength
  - How much energy an agent steals when it attacks another
  - Larger values should require more energy per attack
- Agility
  - How many ticks it takes to move
  - Larger values should require more energy per movement