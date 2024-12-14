#include "array.h"
#include <gtest/gtest.h>

// Проверяем, что массив создался пустой
TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

// Проверяем вставку значений в массив и доступ ним
TEST(ArrayTest, InsertAndAccessElements) {
    Array<int> arr;
    arr.insert(5);
    arr.insert(10);
    arr.insert(15);

    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 10);
    EXPECT_EQ(arr[2], 15);
    EXPECT_EQ(arr.size(), 3);
}

// Проверяем вставку элемента в указанною позицию
TEST(ArrayTest, InsertAtSpecificIndex) {
    Array<int> arr;
    arr.insert(3);
    arr.insert(7);
    arr.insert(9);
    arr.insert(1, 20);

    EXPECT_EQ(arr[0], 3);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 7);
    EXPECT_EQ(arr[3], 9);
    EXPECT_EQ(arr.size(), 4);
}

// Проверяем вставку с перераспределением памяти
TEST(ArrayTest, InsertWithReallocation) {
    Array<int> arr;
    for (int i = 0; i < 30; ++i) {
        arr.insert(i * 2);
    }

    EXPECT_EQ(arr.size(), 30);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[29], 58);
}

// Проверяем удаление элемента из массива
TEST(ArrayTest, RemoveElement) {
    Array<int> arr;
    arr.insert(5);
    arr.insert(10);
    arr.insert(15);
    arr.remove(1);

    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 15);
    EXPECT_EQ(arr.size(), 2);
}

// Проверяем удаление элемента по неверному индексу
TEST(ArrayTest, RemoveElementOutOfBounds) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    EXPECT_THROW(arr.remove(5), std::out_of_range);
    EXPECT_THROW(arr.remove(-1), std::out_of_range);
}

// Проверяем доступ элемента по неверному индексу
TEST(ArrayTest, AccessOutOfBounds) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    EXPECT_THROW(arr[3], std::out_of_range);
    EXPECT_THROW(arr[-1], std::out_of_range);
}

// Проверяем конструктор опирования
TEST(ArrayTest, CopyConstructor) {
    Array<int> arr;
    arr.insert(100);
    arr.insert(200);

    Array<int> copiedArr = arr;

    EXPECT_EQ(copiedArr[0], 100);
    EXPECT_EQ(copiedArr[1], 200);
    EXPECT_EQ(copiedArr.size(), 2);
    EXPECT_EQ(arr[1], 200);
    EXPECT_EQ(arr[0], 100);
    EXPECT_EQ(arr.size(), 2);
}

// Проверяем конструктор перемещения
TEST(ArrayTest, MoveConstructor) {
    Array<int> arr;
    arr.insert(50);
    arr.insert(75);

    Array<int> movedArr = std::move(arr);

    EXPECT_EQ(movedArr[0], 50);
    EXPECT_EQ(movedArr[1], 75);
    EXPECT_EQ(movedArr.size(), 2);
    EXPECT_EQ(arr.size(), 0);
}

// Проверяем оператор присваивания копированием
TEST(ArrayTest, CopyAssignment) {
    Array<int> arr;
    arr.insert(30);
    arr.insert(40);

    Array<int> copiedArr;
    copiedArr = arr;

    EXPECT_EQ(copiedArr[0], 30);
    EXPECT_EQ(copiedArr[1], 40);
    EXPECT_EQ(copiedArr.size(), 2);
    EXPECT_EQ(arr[0], 30);
    EXPECT_EQ(arr[1], 40);
    EXPECT_EQ(arr.size(), 2);
}

// Проверяем оператор присваивания перемещением
TEST(ArrayTest, MoveAssignment) {
    Array<int> arr;
    arr.insert(100);
    arr.insert(200);

    Array<int> movedArr;
    movedArr = std::move(arr);

    EXPECT_EQ(movedArr[0], 100);
    EXPECT_EQ(movedArr[1], 200);
    EXPECT_EQ(movedArr.size(), 2);
    EXPECT_EQ(arr.size(), 0);
}

// Проверка итератора в прямом порядке
TEST(ArrayTest, IteratorForward) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    auto it = arr.iterator();
    EXPECT_EQ(it.get(), 10);
    it.next();
    EXPECT_EQ(it.get(), 20);
    it.next();
    EXPECT_EQ(it.get(), 30);
    it.next();
    EXPECT_FALSE(it.hasNext());
}

// Проверка итератора в обратном порядке
TEST(ArrayTest, IteratorReverse) {
    Array<int> arr;
    arr.insert(5);
    arr.insert(10);
    arr.insert(15);

    auto rit = arr.reverseIterator();
    EXPECT_EQ(rit.get(), 15);
    rit.next();
    EXPECT_EQ(rit.get(), 10);
    rit.next();
    EXPECT_EQ(rit.get(), 5);
    rit.next();
    EXPECT_FALSE(rit.hasNext());
}

// Проверка обращения к пустому массиву
TEST(ArrayTest, EmptyArray) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_THROW(arr[0], std::out_of_range);
}

// Проверка очистки массива
TEST(ArrayTest, ClearArray) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    arr.remove(0);
    arr.remove(0);
    arr.remove(0);

    EXPECT_EQ(arr.size(), 0);
}

// Проверка константного итератора
TEST(ArrayTest, ConstIterator) {
    Array<int> arr;
    arr.insert(100);
    arr.insert(200);
    arr.insert(300);

    auto cit = arr.const_iterator();
    EXPECT_EQ(cit.get(), 100);
    cit.next();
    EXPECT_EQ(cit.get(), 200);
    cit.next();
    EXPECT_EQ(cit.get(), 300);
    cit.next();
    EXPECT_FALSE(cit.hasNext());
}

// Проверка константного итератора в обратном порядке
TEST(ArrayTest, ConstReverseIterator) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    auto cit = arr.const_reverseIterator();
    EXPECT_EQ(cit.get(), 3);
    cit.next();
    EXPECT_EQ(cit.get(), 2);
    cit.next();
    EXPECT_EQ(cit.get(), 1);
    cit.next();
    EXPECT_FALSE(cit.hasNext());
}

// Проверка установки значения через итератор
TEST(ArrayTest, IteratorSetValue) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    auto it = arr.iterator();
    it.set(10);
    EXPECT_EQ(arr[0], 10);

    auto rit = arr.reverseIterator();
    rit.set(20);
    EXPECT_EQ(arr[2], 20);
}

// Проверка вставки отрицательных значений в массив
TEST(ArrayTest, InsertNegativeValues) {
    Array<int> arr;
    arr.insert(-5);
    arr.insert(-10);

    EXPECT_EQ(arr[0], -5);
    EXPECT_EQ(arr[1], -10);
    EXPECT_EQ(arr.size(), 2);
}

// Попытку удаления элемента из пустого массива
TEST(ArrayTest, RemoveElementFromEmptyArray) {
    Array<int> arr;
    EXPECT_THROW(arr.remove(0), std::out_of_range);
}


int main(int argc, char** argv) {
//    Array<int> arr;
//
//    arr.insert(10);
//    arr.insert(20);
//    arr.insert(30);
//
//    for (auto it = arr.iterator(); it.hasNext(); it.next()) {
//        std::cout << "Before change: " << it.get() << std::endl;
//        it.get() += 5;
//    }
//
//    for (auto cit = arr.const_iterator(); cit.hasNext(); cit.next()) {
//        std::cout << "ConstIterator: " << cit.get() << std::endl;
//        // cit.get() += 5; // Cannot assign to return value because function 'get' returns a const value
//    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}