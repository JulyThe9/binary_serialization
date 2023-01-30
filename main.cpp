#include <iostream>
#include <vector>
#include <bitset>
#include <assert.h>
#include <fstream>
#include <cfloat>
#include <string>

#include <cstdint>

#include "Types.h"
#include "Core.h"
#include "Root.h"
#include "Primitive.h"
#include "Array.h"
#include "Object.h"


namespace EventSystem
{
    class Event; // needed?

    class System
    {
    public:
        friend class Event;
    private:
        std::string name;
        int32_t descriptor;
        int16_t index;
        bool active;
        std::vector<Event*> events;
    public:
        System(std::string);
        ~System();
    public:
        void addEvent(Event*);
        Event *getEvent();
        const bool isActive() const;

        void serialize();
    };

    class Event
    {
    public:
        enum class DeviceType : int8_t
        {
            KEYBOARD = 1,
            MOUSE,
            TOUCHPAD,
            JOYSTICK
        };
        DeviceType dType;
        System *system = nullptr;
    public:
        Event(DeviceType);
        const DeviceType getDType() const;
        
        // TODO: why does it say "too many arguments" unless specified as friend?
        friend std::ostream &operator<<(std::ostream &stream, const DeviceType dType)
        {
            std::string result;
#define PRINT(a) result = #a; // C++ preprocessor macro: stringify
            switch(dType)
            {
                //TODO: why does prepending DeviceType:: remove the error?
                case DeviceType::KEYBOARD: PRINT(KEYBOARD); break;
                case DeviceType::MOUSE: PRINT(MOUSE); break;
                case DeviceType::TOUCHPAD: PRINT(TOUCHPAD); break;
                case DeviceType::JOYSTICK: PRINT(JOYSTICK); break;
            }
            return stream << result;
        }

        void bind(System*, Event*);
    };

    class KeyboardEvent : public Event
    {
    private:
        int8_t keyCode;
        bool pressed;
        bool released;
    public:
        KeyboardEvent(int8_t, bool, bool);
    };

    // definition
    System::System(std::string name) :
        name(name),
        descriptor(123),
        index(1),
        active(true) {}

    System::~System()
    {
        // TODO:
    }

    void System::addEvent(Event *e)
    {
        e->bind(this, e);
    }

    Event *System::getEvent()
    {
        return events.front();
    }

    const bool System::isActive() const
    {
        // MASSIVE TODO: SYSTEM IS NOT DEFINED IN SYSTEM
        /* if (!system)
            return false; */
        return active;
    }

    void System::serialize()
    {

    }

    Event::Event(DeviceType dType)
    {
        this->dType = dType;
    }

    void Event::bind(System* system, Event* e)
    {
        this->system = system;
        this->system->events.push_back(e);
    }

    const Event::DeviceType Event::getDType() const
    {
        return this->dType;
    }

    KeyboardEvent::KeyboardEvent(int8_t keyCode, bool pressed, bool released)
        :
        Event(Event::DeviceType::KEYBOARD),
        keyCode(keyCode),
        pressed(pressed),
        released(released)
        {}
}

using namespace EventSystem;

int main(int argc, char* argv[]) // equivalent to char** argv
{
    assert(true);

    (void)argc;
    (void)argv;

    ObjectModel::Primitive *prim = 
        ObjectModel::Primitive::create<int32_t>("int32", ObjectModel::PrimitiveTypes::I32, 10); //76745);    
    Core::Util::retrieveAndSave(prim);

    std::vector<int32_t> vect = {1,3,5,7};
    ObjectModel::Array *arr = 
        ObjectModel::Array::createArray<int32_t>("array", ObjectModel::PrimitiveTypes::I32, vect);
    Core::Util::retrieveAndSave(arr);

    std::string str = "Hello world!";
    ObjectModel::Array *strArr =
        ObjectModel::Array::createString("strArr", str);
    Core::Util::retrieveAndSave(strArr);

    ObjectModel::Object *objExt = new ObjectModel::Object("ObjExternal");
    objExt->addEntity(prim);
    objExt->addEntity(arr);
    objExt->addEntity(strArr);

    ObjectModel::Object *objInt = new ObjectModel::Object("ObjInternal");
    objInt->addEntity(prim);
    Core::Util::retrieveAndSave(objInt);

    objExt->addEntity(objInt);
    Core::Util::retrieveAndSave(objExt);

#if 0
    System Foo("Foo");
    Event *e = new KeyboardEvent('a', true, false);

    Foo.addEvent(e);

    KeyboardEvent *kb = static_cast<KeyboardEvent*>(Foo.getEvent());

    Foo.serialize();
#endif
    return 0;
}