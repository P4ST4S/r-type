# How to use the ECS library

## What is an ECS

An ECS or Entity Component System is a software architectural pattern mostly used in video game development for the representation of game world objects.
An ECS comprises entities composed from components of data, with systems which operate on entities components.

Entity: An entity represents a general-purpose object. In a game engine context, for example, every coarse game object is represented as an entity. Usually, it only consists of a unique id. Implementations typically use a plain integer for this.

Component: A component labels an entity as possessing a particular aspect, and holds the data needed to model that aspect. For example, every game object that can move might have a Position and Velocity component associated with its entity. Implementations typically use structs, classes, or associative arrays.

System: A system is a process which acts on all entities with the desired components. For example, a physics system may query for entities having velocity and position components, and iterate over the results doing physics calculations on the sets of components for each entity.


## Introduction

Through this document you will learn to use the ECS library.
To better understand the library, we go through the different explanations to create a basic program that allows you to move spacesships in space.


## Components

To create a program which move spaceships we need them to have a position and a velocity, so we are going to create the two component class **Position** and **Velocity**. 

```c++

class Position{
    public:
        double _x;
        double _y;

        Position(double x, double y) : _x(x), _y(y) {};
}

class Velocity{
    public:
        double _x;
        double _y;

        Velocity(double x, double y) : _x(x), _y(y) {};
}

```

The two class are components which can respectively contains the position and velocity data of an entity. 
Please note that a component can be of any type, it does not have to be just a class.


Now we are going to see the way used by the library to store the components.

## SparseArray

The class **SparseArray** is an array very similar to a std::vector but unlike a vector, we can verify if an occurence contains a value initialized or not before using it that permit us to avoid memory error.
that thanks to the class std::optional. **(Yes you begin to understand that the SparseArray is simply a std::vector of std::optional)**

In a ECS for every Component type we have a sparse array for it.
So for our program we need to create two sparse array, one for the Position and a second for the Velocity, for now we will only use the Position class for the next exemples
but the syntax is the same for the Velocity.

```c++

SparseArray<Position> positionArray(); // Initialization of a SparseArray of Position components
std::vector<std::optional<Position>> positionArray(); // Same

```

We can add a new element to a sparse array using the methods **insertAt** or **emplaceAt**.
Those two methods have differencies so we are going to see them separately starting with insertAt.

insertAt contains two parameters, first **the position of the new element in the array** and **the new element to add**.

```c++

Position myPosition(5, 10);
positionArray.insertAt(3, myPosition);

```

insertAt **copy** the element myPosition and add it in the array.
But if for a large quantity of data creating a copy will use a lot of ressource so if you don't need it you should probably use the method emplaceAt.
emplaceAt contains as parameter **the position of the new element** in the array and **as many other parameters that the constructor of the new element have**.

```c++

positionArray.emplaceAt(3, 5, 10)

```

emplaceAt **construct** the new element instead of copying it as insertAt.
Those two methods do the same thing but not in the same way, so if you have to add an element, think carefully about which of the two would be the most judicious to choose
You don't have to think of resizing the array insertAt and emplaceAt do it for you.

You can also use **the overload operator[]** to access to a specific component of the array, you don't receve directly the component but a std::optional of the component
so the emplacement can probably be empty, you can verify before using it.

```c++

std::optional<Position> myOptionalPosition = positionArray[3];

if (myOptionalPosition.has_value()){ // has_value is a method of std::optional to verify if the object contains a value
    Position myPosition = myOptionalPosition.value() // value is a method of std::optional to access to the value stored in the object.
}

```

You are free to look at the other methods of the class.

## Registry

We are now going to see the core part of library which is used to manage every part of the ecs, the class **Registry**.

```c++

// As you can see a registry is basicaly initilized like this.
Registry myRegistry();

```

The class Registry is responsible for several things:

-Manage entities **(Spawn them, Kill them)**

-Manage components **(Register them, Add them to an entity, Get them)**

-Manage systems **(Add them, Run them)**

### Entities management

In an ECS an entity is an **id**, this id is used as an unique index to link or access a component to an entity.

You can spawn an entity with the method spawnEntity which return a new Entity class.

```c++

    Entity entityClass = myRegistry.spawnEntity(); // Returned to an Entity class
    std::size_t entityId = myRegistry.spawnEntity(); // Converted to a size_t

```

As you can see in the last line we can convert an entity to a size_t but the reverse is only possible with the method **entityFromIndex** of the registry class.

```c++
    Entity entityClass = myRegistry.entityFromIndex(entityId);
```

When you finish using an entity you can kill them with the method **killEntity**.

```c++
    myRegistry.killEntity(entityClass);
```

Now we are going to see how the registry manage the component and how to use them

### Component management

Previously whe saw what a sparse array is and how to use it. Now we will see how it is used in the registry to manage components.

In an ECS whe need to have an array for each type of component so to do this we have to register them in the registry through the method
**registerComponent** the method will create a new sparse array for the component registered.

The method registerComponent need to have **the type of the component to register as a template parameter** of the function and **a function as parameter**.
The function in parameter is a function which is used to destroy a component of the type specified as a template parameter because given that the type of the components registered
in the registry are not explicitly precised the registry can't access to them so at the destruction of an entity he will call every destruction function for each component type.

The signature of the function should be:

```c++
void (Registry &, entity_t const &);
```

Here is an exemple of a destruction function for the component position:

```c++

void destroyPosition(Registry &registry, entity_t const &entity)
{
    SparseArray<Position> positionArray = registry.get_component<Position>(); // we will see this method later
    
    positionArray[entity].reset(); // reset is a method of the class std::optional to destroy the value in the object
}

```

To register the component Position in the registry you should do like this:

```c++

myRegistry.registerComponent<Position>(destroyPosition);

```

Now that we have registered our component type we need to store it in the registry for our specific entities, to do that we will use the method **addComponent** or **emplaceComponent**.
The difference between the two is the same insertAt and emplaceAt methods of the class SparesArray, besides they respectively call them.

The method addComponent need to have **the type of the component to store as a template parameter**, **the entity to which you want to give the component** and **the component to add**.

```c++

Entity myEntity = myRegistry.spawnEntity()

Position myEntityPosition(10, 10);

myRegistry.addComponent<Position>(myEntity, myEntityPosition);

```

The method emplaceAt need to have **the type of the component to store as a template parameter**, **the entity to which you want to give the component** and **as many other parameters that the constructor of the new element have**

```c++

Entity myEntity = myRegistry.spawnEntity()

myRegistry.addComponent<Position>(myEntity, 10, 10);

```

Yes we can addComponents but how to remove them?
To remove a component you have to use the method removeComponent which have **the type of the component to destroy as template parameter** and **the entity which will see its component destroyed**

```c++

myRegistry.remove_componenet<Position>(myEntity);

```

Now we will see how to access to the SparseArray of a specific component to manipulte it. Its simple, you just need to call **get_component** !
The method registerComponent only need **the type of the component stored in the array you want**.

```c++

SparseArray<Position> positionArray = myRegistry.get_component<Position>();

```

You know now how to manage the components of the registry, in the next part you will see how to use third part of the ecs, **Systems**.

### Systems management

The **system** management can store functions as system and run them, for the case of our spaceships we want a system that will update the position of every entities of our spaceship according to the position and the velocity, to do this we first need to create and store the system.

But how should the signature of the system look like ?
Like this:

```c++

template<typename ... Components>
void (Registry &, SparseArray<Components>...)

```

It may seem intimidating but it's okay.
The first parameters of the system should be a registry and after that it only mean that the prototype can be as many SparseArray of component as you want, the components just have to exist.
Here is an exemple of the position updating system:

```c++

void position_updating_system(Registry &registry, SparseArray<Position> &positionArray, SparseArray<Velocity> &velocityArray)
{
    for (size_t i = 0; i < positionArray.size() && i < velocityArray.size(); i++){
        if (positionArray[i].has_value() && velocityArray[i].has_value()){
            positionArray[i]->x += velocityArray[i]->x; // operator->() do the same thing as value()
            positionArray[i]->y += velocityArray[i]->y;
        }
    }
}

```

To add this system to the registry you have to use **addSystem** with for type of the components used in the system as a template parameter and the system as parameter.

```c++

myRegistry.addSystem<Position, Velocity>(position_updating_system);

```

And to run every system of the registry you have to use **runSystems**.

```c++

myRegistry.runSystems();

```

### Exemple

Now that we saw how to use the ecs, i will show you a complete exemple of is usage through a spaceshing racing program.
Two spaceships will face each other in a racing, the first to reach 100km of distance wins the race but their velocity is recalculted
at each start of loop so good luck to them.

```c++

class Position{
    public:
        double _x;
        double _y;

        Position(double x, double y) : _x(x), _y(y) {};
}

class Velocity{
    public:
        double _x;
        double _y;

        Velocity(double x, double y) : _x(x), _y(y) {};
}

void destroyPosition(Registry &registry, entity_t const &entity)
{
    SparseArray<Position> positionArray = registry.get_component<Position>();
    
    positionArray[entity].reset();
}

void destroyVelocity(Registry &registry, entity_t const &entity)
{
    SparseArray<Velocity> positionArray = registry.get_component<Position>();
    
    positionArray[entity].reset();
}

void position_updating_system(Registry &registry, SparseArray<Position> &positionArray, SparseArray<Velocity> &velocityArray)
{
    for (size_t i = 0; i < positionArray.size() && i < velocityArray.size(); i++){
        if (positionArray[i].has_value() && velocityArray[i].has_value()){
            positionArray[i]->x += velocityArray[i]->x;
            positionArray[i]->y += velocityArray[i]->y;
        }
    }
}

void change_velocity_system(Registry &registry, SparseArray<Velocity> &velocityArray)
{
    for (size_t i = 0; i < velocityArray.size(); i++){
        if (velocityArray[i].has_value()){
            velocityArray[i]->x = (rand() % 10);
        }
    }
}

int main(void)
{
    Registry myRegistry();
    Entity Spaceship1 = myRegistry.spawnEntity();
    Entity Spaceship2 = myRegistry.spawnEntity();
    srand(time(NULL));
    
    myRegistry.registerComponent<Position>(destroyPosition);
    myRegistry.registerComponent<Velocity>(destroyVelocity);

    SparseArray<Position> &posArray = myRegistry.getComponents<Position>();

    myRegistry.emplaceComponent<Position>(Spaceship1, 0, 0);
    myRegistry.emplaceComponent<Position>(Spaceship1, 0, 1);
    
    myRegistry.emplaceComponent<Velocity>(Spaceship1, (rand() % 10), 0);
    myRegistry.emplaceComponent<Velocity>(Spaceship1, (rand() % 10), 0);

    myRegistry.addSystem<Velocity>(change_velocity_system);
    myRegistry.addSystem<Position, Velocity>(position_updating_system);
    while (posArray[Spaceship1].x < 100 && posArray[Spaceship2].x < 100){
        myRegistry.runSystems();
        sleep(1);
    }
    if (posArray[Spaceship1].x > posArray[Spaceship2].x)
        return (1);
    if (posArray[Spaceship1].x < posArray[Spaceship2].x)
        return (2);
    return (0);
}

```

Before ending i want to show you a last class, the class **Zipper**.

## Zipper

The class **Zipper** is a class which permit us to traverse several SparseArray at the same time avoiding the occurences which don't have a component initialized in all the arrays.

To do that we need to initialize the class like this:

```c++

SpaceArray<Position> positionArray;
SpaceArray<Velocity> velocityArray;

Zipper<SparseArray<Position>, SparseArray<Velocity>> zipper(positionArray, velocityArray);

```

The zipper class contains only two methods **begin** and **end**.
They don't have parameters but they have the same return, an iterator tuple of SparseArray.
Which represent the begining and the end of the tuple of SparseArray.

If its not clear don't worry, to better understand this class i will show you the same exemple code of the racing of spaceships but now using Zipper.

```c++

class Position{
    public:
        double _x;
        double _y;

        Position(double x, double y) : _x(x), _y(y) {};
}

class Velocity{
    public:
        double _x;
        double _y;

        Velocity(double x, double y) : _x(x), _y(y) {};
}

void destroyPosition(Registry &registry, entity_t const &entity)
{
    SparseArray<Position> positionArray = registry.get_component<Position>();
    
    positionArray[entity].reset();
}

void destroyVelocity(Registry &registry, entity_t const &entity)
{
    SparseArray<Velocity> positionArray = registry.get_component<Position>();
    
    positionArray[entity].reset();
}

void position_updating_system(Registry &registry, SparseArray<Position> &positionArray, SparseArray<Velocity> &velocityArray)
{
    Zipper zipper<SparseArray<Position>, SparseArray<Velocity>>(positionArray, velocityArray)

    for (auto begin = zipper.begin(), auto end = zipper.end(); begin != end; begin++){            
            std::get<Position>(*begin)->x += std::get<Velocity>(*begin)->x;
            std::get<Position>(*begin)->y += std::get<Velocity>(*begin)->y;
    }
}

void change_velocity_system(Registry &registry, SparseArray<Velocity> &velocityArray)
{
    Zipper zipper<SparseArray<Velocity>>(velocityArray)

    for (auto begin = zipper.begin(), auto end = zipper.end(); begin != end; begin++){
            std::get<Velocity>(*begin)->x = (rand() % 10);
    }
}

int main(void)
{
    Registry myRegistry();
    Entity Spaceship1 = myRegistry.spawnEntity();
    Entity Spaceship2 = myRegistry.spawnEntity();
    srand(time(NULL));
    
    myRegistry.registerComponent<Position>(destroyPosition);
    myRegistry.registerComponent<Velocity>(destroyVelocity);

    SparseArray<Position> &posArray = myRegistry.getComponents<Position>();

    myRegistry.emplaceComponent<Position>(Spaceship1, 0, 0);
    myRegistry.emplaceComponent<Position>(Spaceship1, 0, 1);
    
    myRegistry.emplaceComponent<Velocity>(Spaceship1, (rand() % 10), 0);
    myRegistry.emplaceComponent<Velocity>(Spaceship1, (rand() % 10), 0);

    myRegistry.addSystem<Velocity>(change_velocity_system);
    myRegistry.addSystem<Position, Velocity>(position_updating_system);
    while (posArray[Spaceship1].x < 100 && posArray[Spaceship2].x < 100){
        myRegistry.runSystems();
        sleep(1);
    }
    if (posArray[Spaceship1].x > posArray[Spaceship2].x)
        return (1);
    if (posArray[Spaceship1].x < posArray[Spaceship2].x)
        return (2);
    return (0);
}

```

## Conclusion

Congratulation, the document is now finish, you should now be able to make your own project with the ecs library.
For more information about the Entity Component System software architecture internet is your friend.