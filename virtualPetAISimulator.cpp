#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

// Abstract class for events
class Event {
public:
    virtual void trigger(const std::string& petName) = 0; // Pure virtual function
    virtual ~Event() {}
};

// Derived class for illness event
class Illness : public Event {
public:
    void trigger(const std::string& petName) override {
        std::cout << petName << " feels sick and loses happiness!\n";
    }
};

// Derived class for surprise event
class Surprise : public Event {
public:
    void trigger(const std::string& petName) override {
        std::cout << petName << " found a toy and gains happiness!\n";
    }
};

// Base Pet class
class Pet {
protected:
    std::string name;
    int hunger;
    int happiness;
    int energy;

public:
    Pet(const std::string& petName) : name(petName), hunger(50), happiness(50), energy(50) {}

    std::string getName() const { return name; }

    virtual void feed(int food) {
        hunger = std::max(0, hunger - food);
        std::cout << name << " has been fed. Hunger level: " << hunger << "\n";
    }

    virtual void play() {
        if (energy > 10) {
            happiness += 10;
            energy -= 10;
            std::cout << name << " played and is happier! Happiness: " << happiness << ", Energy: " << energy << "\n";
        } else {
            std::cout << name << " is too tired to play.\n";
        }
    }

    virtual void rest() {
        energy += 20;
        std::cout << name << " rested and gained energy! Energy: " << energy << "\n";
    }

    virtual void performTrick(const std::string& trick) {
        std::cout << name << " performs a " << trick << " trick!\n";
    }

    // Save pet state to a file
    void saveState(const std::string& filename) {
        std::ofstream outFile(filename);
        if (outFile) {
            outFile << name << "\n" << hunger << "\n" << happiness << "\n" << energy << "\n";
            outFile.close();
            std::cout << "Pet state saved to " << filename << ".\n";
        } else {
            std::cerr << "Error saving pet state.\n";
        }
    }

    // Load pet state from a file
    void loadState(const std::string& filename) {
        std::ifstream inFile(filename);
        if (inFile) {
            std::getline(inFile, name);
            inFile >> hunger >> happiness >> energy;
            inFile.close();
            std::cout << "Pet state loaded from " << filename << ".\n";
        } else {
            std::cerr << "Error loading pet state.\n";
        }
    }

    virtual ~Pet() {}
};

// Derived Dog class
class Dog : public Pet {
public:
    Dog(const std::string& petName) : Pet(petName) {}

    void bark() {
        std::cout << name << " barks! Woof woof!\n";
    }

    void play() override {
        if (energy > 15) {
            happiness += 15;
            energy -= 15;
            std::cout << name << " plays fetch and feels ecstatic! Happiness: " << happiness << ", Energy: " << energy << "\n";
        } else {
            std::cout << name << " is too tired to play fetch.\n";
        }
    }
};

// Derived Cat class
class Cat : public Pet {
public:
    Cat(const std::string& petName) : Pet(petName) {}

    void meow() {
        std::cout << name << " meows! Meow!\n";
    }

    void play() override {
        if (energy > 10) {
            happiness += 10;
            energy -= 10;
            std::cout << name << " plays with a ball of yarn! Happiness: " << happiness << ", Energy: " << energy << "\n";
        } else {
            std::cout << name << " is too tired to play with yarn.\n";
        }
    }
};

// Enum for actions
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

// Function to handle random events
void triggerRandomEvent(const std::string& petName) {
    int eventType = rand() % 2;
    std::unique_ptr<Event> event;
    if (eventType == 0) {
        event = std::make_unique<Illness>();
    } else {
        event = std::make_unique<Surprise>();
    }
    event->trigger(petName);
}

// Main function
int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random events

    // Create a dog pet
    std::unique_ptr<Pet> myPet = std::make_unique<Dog>("Buddy");

    std::cout << "Welcome to the Virtual Pet Simulator!\n";
    std::cout << "Your pet's name is " << myPet->getName() << ".\n";

    int choice;
    while (true) {
        std::cout << "\nChoose an action:\n";
        std::cout << "1. Feed\n2. Play\n3. Rest\n4. Perform Trick\n5. Trigger Random Event\n6. Save Pet State\n7. Load Pet State\n8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

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
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice! Try again.\n";
                break;
        }
    }

    return 0;
}