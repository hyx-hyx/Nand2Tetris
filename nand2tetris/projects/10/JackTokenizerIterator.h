#pragma once
#include <iostream>
#include <iterator> // 包含标准迭代器相关的定义
#include <JackTokenizer.h>

// 定义一个迭代器类
class JackTokenizerIterator : public std::iterator<std::random_access_iterator_tag, Token>
{
private:
    Token* ptr; // 指向数组元素的指针

public:
    // 构造函数
    JackTokenizerIterator(Token* p) : ptr(p){};

    // 解引用操作符
    Token& operator*() const;

    // 箭头操作符
    Token* operator->() const;

    // 前置递增
    JackTokenizerIterator& operator++();
    // 后置递增
    JackTokenizerIterator operator++(int);

    // 前置递减
    JackTokenizerIterator& operator--();

    // 后置递减
    JackTokenizerIterator operator--(int);

    // 比较操作符
    bool operator==(const JackTokenizerIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const JackTokenizerIterator& other) const { return ptr != other.ptr; }

    // 随机访问迭代器的其他操作
    JackTokenizerIterator& operator+=(std::ptrdiff_t n);

    JackTokenizerIterator& operator-=(std::ptrdiff_t n);

    JackTokenizerIterator operator+(std::ptrdiff_t n) const;

    JackTokenizerIterator operator-(std::ptrdiff_t n) const;

    std::ptrdiff_t operator-(const JackTokenizerIterator& other) const;

    Token& operator[](std::ptrdiff_t n) const;
};