#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>

template<typename T>
class ThreadSafeVector {
public:
    // Додає елемент в кінець вектора
    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.push_back(value);
    }

    // Отримує елемент за індексом
    T at(size_t index) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= vector_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return vector_[index];
    }

    // Видаляє елемент за індексом
    void erase(size_t index) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= vector_.size()) {
            throw std::out_of_range("Index out of range");
        }
        vector_.erase(vector_.begin() + index);
    }

    // Повертає кількість елементів у векторі
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.size();
    }

    // Очистити вектор
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.clear();
    }

    // Перевіряє чи вектор порожній
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.empty();
    }

private:
    mutable std::mutex mutex_; // Мютекс для синхронізації доступу
    std::vector<T> vector_;    // Стандартний вектор, який ми обгортаємо
};

int main() {
    ThreadSafeVector<int> safeVec;

    // Запускаємо кілька потоків, які працюють з одним і тим же вектором
    std::thread t1([&safeVec]() {
        for (int i = 0; i < 100; ++i) {
            safeVec.push_back(i);
        }
        });

    std::thread t2([&safeVec]() {
        for (int i = 0; i < 100; ++i) {
            safeVec.push_back(i + 100);
        }
        });

    t1.join();
    t2.join();

    // Виводимо кількість елементів в векторі
    std::cout << "Size of vector: " << safeVec.size() << std::endl;

    return 0;
}