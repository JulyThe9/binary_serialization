#include <iostream>
#include <string>
#include <assert.h>

#include "Types.h"
#include "Core.h"
#include "Root.h"
#include "Primitive.h"
#include "Array.h"
#include "Object.h"

static uint64_t generateID()
{
    static uint64_t id = 1;
    return id++;
}

namespace EventSystem
{
    class Event; 

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
        System(const std::string &);
        ~System();
    public:
        void addEvent(Event *);
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
    private:
        uint64_t ID = 0;
        DeviceType dType;
        System *system = nullptr;
    public:
        Event(const DeviceType);
        const DeviceType getDType() const;
        const uint64_t getID() const;
        
        /*virtual*/ void serialize(ObjectModel::Object *);

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
    };

    class KeyboardEvent : public Event
    {
    private:
        int8_t keyCode;
        bool pressed;
        bool released;
    public:
        KeyboardEvent(const int8_t, const bool, const bool);
        void serialize(ObjectModel::Object *);
    };

    // definition
    System::System(const std::string &name) :
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
        events.push_back(e);
    }

    Event *System::getEvent()
    {
        return events.front();
    }

    const bool System::isActive() const
    {
        return active;
    }

    void System::serialize()
    {
        ObjectModel::Object *system = new ObjectModel::Object("SysInfo");

        ObjectModel::Array *nameArr = ObjectModel::Array::createString("sysName", this->name);
        ObjectModel::Primitive *descPrim = ObjectModel::Primitive::create<int32_t>("desc", ObjectModel::PrimitiveTypes::I32, this->descriptor); 
        ObjectModel::Primitive *indexPrim = ObjectModel::Primitive::create<int16_t>("index", ObjectModel::PrimitiveTypes::I16, this->index);
        // NOTE: sizeof(bool) not defined by the standard, so let type deduction do its job
        ObjectModel::Primitive *activePrim = ObjectModel::Primitive::create("active", ObjectModel::PrimitiveTypes::BOOL, this->active);

        system->addEntity(nameArr);
        system->addEntity(descPrim);
        system->addEntity(indexPrim);
        system->addEntity(activePrim);

        for (Event *event : events)
        {
            // NOTE: not safe, but so far I am working with keyboard events only, for now
            KeyboardEvent *kbEvent = static_cast<KeyboardEvent*>(event);
            ObjectModel::Object *eventObject = new ObjectModel::Object("Event: " + std::to_string(event->getID()));
            kbEvent->serialize(eventObject);
            system->addEntity(eventObject);
        }
        Core::Util::retrieveAndSave(system);

        delete system;
    }

    Event::Event(const DeviceType dType)
    {
        this->dType = dType;
        this->ID = generateID();
    }

    const Event::DeviceType Event::getDType() const
    {
        return this->dType;
    }

    const uint64_t Event::getID() const
    {
        return ID;
    }

    void Event::serialize(ObjectModel::Object *obj)
    {
        assert(obj);

        ObjectModel::Primitive *dTypePrim = ObjectModel::Primitive::create<int8_t>("dType", ObjectModel::PrimitiveTypes::I64, static_cast<int8_t>(this->dType));
        ObjectModel::Primitive *idPrim = ObjectModel::Primitive::create<uint64_t>("ID", ObjectModel::PrimitiveTypes::U64, this->getID());
        
        obj->addEntity(dTypePrim);
        obj->addEntity(idPrim);
    }

    KeyboardEvent::KeyboardEvent(const int8_t keyCode, const bool pressed, const bool released)
        :
        Event(Event::DeviceType::KEYBOARD),
        keyCode(keyCode),
        pressed(pressed),
        released(released)
        {}

    void KeyboardEvent::serialize(ObjectModel::Object *obj)
    {
        assert(obj);    
        Event::serialize(obj);

        ObjectModel::Primitive *keyCodePrim = ObjectModel::Primitive::create<int16_t>("keyCode", ObjectModel::PrimitiveTypes::I16, this->keyCode);
        ObjectModel::Primitive *pressedPrim = ObjectModel::Primitive::create("pressed", ObjectModel::PrimitiveTypes::BOOL, this->pressed);
        ObjectModel::Primitive *releasedPrim = ObjectModel::Primitive::create("released", ObjectModel::PrimitiveTypes::BOOL, this->released);

        obj->addEntity(keyCodePrim);
        obj->addEntity(pressedPrim);        
        obj->addEntity(releasedPrim);        
    }
}

using namespace EventSystem;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

#if 0
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
#endif

#if 1
    System Foo("Foo");
    Event *e = new KeyboardEvent('a', true, false);
    Foo.addEvent(e);

    KeyboardEvent *kb = static_cast<KeyboardEvent*>(Foo.getEvent());

    Foo.serialize();
#endif

    return 0;
}