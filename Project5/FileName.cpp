#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>

template<typename T>
class ThreadSafeVector {
public:
    // ���� ������� � ����� �������
    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.push_back(value);
    }

    // ������ ������� �� ��������
    T at(size_t index) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= vector_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return vector_[index];
    }

    // ������� ������� �� ��������
    void erase(size_t index) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= vector_.size()) {
            throw std::out_of_range("Index out of range");
        }
        vector_.erase(vector_.begin() + index);
    }

    // ������� ������� �������� � ������
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.size();
    }

    // �������� ������
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.clear();
    }

    // �������� �� ������ �������
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.empty();
    }

private:
    mutable std::mutex mutex_; // ������ ��� ������������ �������
    std::vector<T> vector_;    // ����������� ������, ���� �� ���������
};

int main() {
    ThreadSafeVector<int> safeVec;

    // ��������� ����� ������, �� �������� � ����� � ��� �� ��������
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

    // �������� ������� �������� � ������
    std::cout << "Size of vector: " << safeVec.size() << std::endl;

    return 0;
}