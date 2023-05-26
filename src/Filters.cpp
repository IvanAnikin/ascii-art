#include "Filters.h"

void Filter::switchActive() {
    isActive = !isActive;
}

std::ostream& operator<<(std::ostream& os, const Filter& filter) {
    os << (filter.isActive ? "\033[32m[ on  ]\t\033[0m" : "\033[31m[ off ]\t\033[0m") << "\t";
    filter.print(os);
    return os;
}
