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

namespace ObjectModel
{
    class Array : public Root
    {
    private:
        int8_t type = 0;
        int16_t count = 0;
        std::vector<int8_t>* data = nullptr;
    private:
        Array();
    public:
        template <typename T>
        // vector instead of arr, because we don't know the size at compile time
        static Array *createArray(std::string name, PrimitiveTypes type, std::vector<T> &value)
        {
            Array *arr = new Array();
            arr->setName(name);
            arr->wrapperType = static_cast<int8_t>(WrapperTypes::ARRAY);
            arr->type = static_cast<int8_t>(type);    
        
            arr->count = value.size();

            arr->data = new std::vector<int8_t>(value.size() * sizeof (T));
            arr->size += (value.size() * sizeof(T));

            int16_t iter = 0;
            Core::template encode(arr->data, &iter, value);

            return arr;
        }
        static Array *createString(std::string name, std::string &value)
        {
            Array *str = new Array();
            str->setName(name);
            str->wrapperType = static_cast<int8_t>(WrapperTypes::STRING);
            str->type = static_cast<int8_t>(PrimitiveTypes::I8);
        
            str->count = value.size();

            str->data = new std::vector<int8_t>(value.size());
            str->size += (value.size());

            int16_t iter = 0;
            Core::template encode(str->data, &iter, value);

            return str;
        }

        void pack(std::vector<int8_t>*, int16_t*);
    };

    class Object : public Root
    {

    };
}

namespace ObjectModel
{
    // definition



    Array::Array()
    {
        size += sizeof(type);
        size += sizeof(count);
    }

    void Array::pack(std::vector<int8_t>* buffer, int16_t* iterator)
    {
        Core::encode<std::string>(buffer, iterator, name);
        Core::encode<int16_t>(buffer, iterator, nameLength);
        Core::encode<int8_t>(buffer, iterator, wrapperType);
        Core::encode<int8_t>(buffer, iterator, type);
        Core::encode<int16_t>(buffer, iterator, count);
        Core::encode<int8_t>(buffer, iterator, *data);
        Core::encode<int32_t>(buffer, iterator, size);
    }
}

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

    ObjectModel::Primitive *p = 
        ObjectModel::Primitive::create<int32_t>("int32", ObjectModel::PrimitiveTypes::I32, 10); //76745);    
    Core::Util::retrieveAndSave(p);

    std::vector<int32_t> vect = {1,3,5,7};
    ObjectModel::Array *arr = 
        ObjectModel::Array::createArray<int32_t>("int1234567", ObjectModel::PrimitiveTypes::I32, vect);
    Core::Util::retrieveAndSave(arr);

    std::string str = "Hello world!";
    ObjectModel::Array *strArr =
        ObjectModel::Array::createString("strArr", str);
    Core::Util::retrieveAndSave(strArr);


#if 0
    System Foo("Foo");
    Event *e = new KeyboardEvent('a', true, false);

    Foo.addEvent(e);

    KeyboardEvent *kb = static_cast<KeyboardEvent*>(Foo.getEvent());
|
    Foo.serialize();
#endif
    return 0;
}