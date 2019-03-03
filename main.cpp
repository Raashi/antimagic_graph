#include <iostream>
#include <vector>


void print_vector(const std::vector<int> vec) {
    for (int a: vec)
        std::cout << a << ' ';
    std::cout << std::endl;
}

void _all_permutations(int n, int* free, std::vector<int> container) {
    for (int i = 0; i < n; ++i) {
        if (free[i]) {
            free[i] = false;
            container.push_back(i);
            if (container.size() == n)
                print_vector(container);
            else
                _all_permutations(n, free, container);
            container.pop_back();
            free[i] = true;
        }
    }
}


void all_permutations(int n) {
    int* free = new int[n];
    for (int i = 0; i < n; ++i)
        free[i] = true;
    _all_permutations(n, free, std::vector<int>());
}


int main() {
    std::cout << "Enter number of permutations: ";
    int n;
    std::cin >> n;
    all_permutations(n);
    return 0;
}