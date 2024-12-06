#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

class Event {
public:
    virtual void trigger(const string& petName) = 0;
    virtual ~Event() {}
};

class Illness : public Event {
public:
    void trigger(const string& petName) override {
        cout << petName << " feels sick and loses happiness!\n";
    }
};

class Surprise : public Event {
public:
    void trigger(const string& petName) override {
        cout << petName << " found a toy and gains happiness!\n";
    }
};

class Pet {
protected:
    string name;
    int hunger;
    int happiness;
    int energy;

public:
    Pet(const string& petName) : name(petName), hunger(50), happiness(50), energy(50) {}

    string getName() const { return name; }

    virtual void feed(int food) {
        hunger = max(0, hunger - food);
        cout << name << " has been fed. Hunger level: " << hunger << "\n";
    }

    virtual void play() {
        if (energy > 10) {
            happiness += 10;
            energy -= 10;
            cout << name << " played and is happier! Happiness: " << happiness << ", Energy: " << energy << "\n";
        } else {
            cout << name << " is too tired to play.\n";
        }
    }

    virtual void rest() {
        energy += 20;
        cout << name << " rested and gained energy! Energy: " << energy << "\n";
    }

    virtual void performTrick(const string& trick) {
        cout << name << " performs a " << trick << " trick!\n";
    }

    void saveState(const string& filename) {
        ofstream outFile(filename);
        if (outFile) {
            outFile << name << "\n" << hunger << "\n" << happiness << "\n" << energy << "\n";
            outFile.close();
            cout << "Pet state saved to " << filename << ".\n";
        } else {
            cerr << "Error saving pet state.\n";
        }
    }

    void loadState(const string& filename) {
        ifstream inFile(filename);
        if (inFile) {
            getline(inFile, name);
            inFile >> hunger >> happiness >> energy;
            inFile.close();
            cout << "Pet state loaded from " << filename << ".\n";
        } else {
            cerr << "Error loading pet state.\n";
        }
    }

    virtual ~Pet() {}
};

class Dog : public Pet {
public:
    Dog(const string& petName) : Pet(petName) {}

    void bark() {
        cout << name << " barks! Woof woof!\n";
    }

    void play() override {
        if (energy > 15) {
            happiness += 15;
            energy -= 15;
            cout << name << " plays fetch and feels ecstatic! Happiness: " << happiness << ", Energy: " << energy << "\n";
        } else {
            cout << name << " is too tired to play fetch.\n";
        }
    }
};

class Cat : public Pet {
public:
    Cat(const string& petName) : Pet(petName) {}

    void meow() {
        cout << name << " meows! Meow!\n";
    }

    void play() override {
        if (energy > 10) {
            happiness += 10;
            energy -= 10;
            cout << name << " plays with a ball of yarn! Happiness: " << happiness << ", Energy: " << energy << "\n";
        } else {
            cout << name << " is too tired to play with yarn.\n";
        }
    }
};

enum Action {
    FEED = 1,
    PLAY,
    REST,
    PERFORM_TRICK,
    TRIGGER_EVENT,
    SAVE_STATE,
    LOAD_STATE,
    EXIT
};

void triggerRandomEvent(const string& petName) {
    int eventType = rand() % 2;
    unique_ptr<Event> event;
    if (eventType == 0) {
        event = make_unique<Illness>();
    } else {
        event = make_unique<Surprise>();
    }
    event->trigger(petName);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    cout << "Welcome to the Virtual Pet Simulator!\n";
    cout << "Choose your pet type:\n";
    cout << "1. Dog\n2. Cat\n";
    int petChoice;
    cin >> petChoice;

    unique_ptr<Pet> myPet;

    if (petChoice == 1) {
        string petName;
        cout << " Enter the name of your dog: ";
        cin >> petName;
        myPet = make_unique<Dog>(petName);
    } else if (petChoice == 2) {
        string petName;
        cout << "Enter the name of your cat: ";
        cin >> petName;
        myPet = make_unique<Cat>(petName);
    } else {
        cout << "Invalid choice! Exiting.\n";
        return 1;
    }

    cout << "Your pet's name is " << myPet->getName() << ".\n";

    int choice;
    while (true) {
        cout << "\nChoose an action:\n";
        cout << "1. Feed\n2. Play\n3. Rest\n4. Perform Trick\n5. Trigger Random Event\n6. Save Pet State\n7. Load Pet State\n8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case FEED:
                myPet->feed(10);
                break;
            case PLAY:
                myPet->play();
                break;
            case REST:
                myPet->rest();
                break;
            case PERFORM_TRICK:
                myPet->performTrick("spin");
                break;
            case TRIGGER_EVENT:
                triggerRandomEvent(myPet->getName());
                break;
            case SAVE_STATE:
                myPet->saveState("pet_state.txt");
                break;
            case LOAD_STATE:
                myPet->loadState("pet_state.txt");
                break;
            case EXIT:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
                break;
        }
    }

    return 0;
}