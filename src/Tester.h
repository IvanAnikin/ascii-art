#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include "MainManager.h"

/** @brief a class to test my code functionality*/
class Tester {
public:
    /** @brief a constructor without parameters*/
    Tester(){}

    /** @brief a desctructor */
    ~ Tester() = default;
    
    /** @brief function testing the MainManager function loadFilters and the filters themselves*/
    bool testFilters();

    /** @brief function testing the MainManager functions removing leading and ending whitespaces */
    bool testSpaces();
};