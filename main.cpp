#include "db_engine.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== Custom Database Engine ===" << std::endl;
    std::cout << std::endl;

    db::DBEngine db("./data");

    std::cout << "Adding data..." << std::endl;
    db.put("name", "John Doe");
    db.put("age", "30");
    db.put("city", "New York");
    db.put("occupation", "Software Developer");

    std::cout << std::endl;
    std::cout << "Reading data:" << std::endl;
    std::string value;

    if (db.get("name", value)) {
        std::cout << "  name: " << value << std::endl;
    }

    if (db.get("age", value)) {
        std::cout << "  age: " << value << std::endl;
    }

    if (db.get("city", value)) {
        std::cout << "  city: " << value << std::endl;
    }

    if (db.get("occupation", value)) {
        std::cout << "  occupation: " << value << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Deleting 'age'..." << std::endl;
    db.del("age");

    if (!db.get("age", value)) {
        std::cout << "  age: not found (deleted)" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Flushing to disk..." << std::endl;
    db.flush();

    std::cout << std::endl;
    std::cout << "Program completed successfully!" << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}