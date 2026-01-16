#ifndef ABILITY_SYSTEM_H_
#define ABILITY_SYSTEM_H_

//events enum; all code scales automatically when a new event is added, as long as its inserted before EV_COUNT
// if an event is added here, any ability can use Subscribe(event, &self) to be notified when that even fires
// unique events like USE don't have subscriber lists, but can be fired on a specific ability instance on a specific entity
// all abilities are sent to the respective target's abilities (if they have one), even if they're not subscribed
//TODO decide on actual information sent in each of these events. Ensure proper documentation and handling whenever sent. current documentation is placeholder
typedef enum{
    TURN_START,     // contains a pointer to the entity whose turn is starting
    TURN_END,       // contains a pointer to the entity whose turn is ending
    SAVE,           // used whenever an entity attempts a saving throw; info contains the entity making the save, type of save, fail/success, advantage or disadvantage, and the outcomes if save is passed/failed
    ATTACK,         // used for different attack types; info contains the attacker entity, the target entity, the potential outcome of a successful attack, and whether the attack is melee/ranged/magic
    CHECK,          // used whenever an entity makes and ability check; info contains the entity making the check, The target of the ability check (if applicable), the check type (proficiency), and the check outcomes
    MOVED,          // used for whenever an entity moves; info contains the entity moving
    MAGIC_ACTION,
    CHARACTERISTIC_CHANGE, 
    EV_COUNT,       // keep at end of fired events; a counter of how many fired events exist
    USE,            // unique event, sent directly to the ability being used on the specific entity it's used on
    INITIAL,        // unique event, used when an ability is first added/created; instatiates uses/turns_remaining/etc. on an ability
    REMOVE,         // unique event, used when an ability instance is removed from an entity, cleans up any varaibles or statuses
    GET_DATA,       // unique event used to retrieve the name and formatting of an ability instance from the ability function (which is constant and generic)
    EV_TOTAL        // total number of events, including unqiue ones
}Event;

//used by info nodes 
typedef enum{
    UNKOWN,
    INT,
    STR,
    LIST,
    ENTITY
}Datatype;

//nodes for a linked list
typedef struct node{
    char* name;         //name of the piece of data being passed; used for id/indexing, so multiple same instances will cause bugs
    void* value;        //pointer to the value of the node; could point to an entity or a piece of data TODO: need to ensure some data is cleared here, like a unique int, but if it points to an entity or something else important that does NOT happen
    Datatype type;      //integer representing what type of data the node is
    struct node* next;    //next node in linked list
}node;

struct Ability;

// function pointer template that all abilities use
typedef void (abilfunction)(node* info, Event event, struct Ability* instance);

typedef struct Ability{
    //TODO figure out some architecture so the ability holder access isn't a weird void pointer
    void* ability_holder; //tracks the entity which this ability is present on. entity is not defined in this scope, so void pointer
    node* variables;   //all variables present on an ability 
    abilfunction* abilfunction;  //a function pointer to the generic function which the ability has. Also used as an of ID to show the kind of ability it is.
}Ability;

typedef struct{
    int leng;           //total length of array
    int count;          //count of current elements
    Ability** elems;    //array of elements
}Dynamic_array;

typedef struct{
    Dynamic_array eventsubs[EV_COUNT];
}Event_manager;

void DEBUG_PrintManager();

//initializes the event manager
void InitializeEventManager();

//adds a subscriber to a list; uses an array which doubles in length when it runs out of space
void Subscribe(Event event, void* ability);

//removes a given subscriber from a list
// only reduces the size of the array if we go below 1/8 of the current max length and only quarters the value to retain space (and wont go below 32 items)
void Unsubscribe(Event event, void* ability);

//fires an event and supplies the given information in linked list format
void FireEvent(Event event, node* info);

//searches an info linked list, provided as the first arguement, for a node with a string name passed as the second arguement, returns a pointer to the value
void* ListSearch(node* info, char* query);

//add a new node to a list. The list can be empty and doesn't need to be initialized to anything beyond a NULL node pointer
//name can be a constant string
//a little pointermancy is required;
// we need the address of the start of the variable list, since it may be empty
// we can use a constant string for the name. Spaces can be used.
// we need to allocate the space and assign our variable outside of this function, since it could be anything, and the function doesn't know, and we then need to cast it to void*
void AddNode(node** start, char* name, void* value, Datatype type);

//completely clear a linked-list of info
void FreeList(node* start);

void NotifyAbility(node* info, Event event, Ability* ability);

#endif // ABILITY_SYSTEM_h_