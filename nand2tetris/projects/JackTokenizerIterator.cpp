#include <iostream>
#include <iterator> // 包含标准迭代器相关的定义
#include <JackTokenizer.h>
#include "JackTokenizerIterator.h"

// 构造函数
JackTokenizerIterator::JackTokenizerIterator(Token* p) : ptr(p) {}

// 解引用操作符
Token& JackTokenizerIterator::operator*() const { return *ptr; }

// 箭头操作符
Token* JackTokenizerIterator::operator->() const { return ptr; }

// 前置递增
JackTokenizerIterator& JackTokenizerIterator::operator++()
{
    ++ptr;
    return *this;
}

// 后置递增
JackTokenizerIterator JackTokenizerIterator::operator++(int)
{
    JackTokenizerIterator temp = *this;
    ++(*this);
    return temp;
}

// 前置递减
JackTokenizerIterator& JackTokenizerIterator::operator--()
{
    --ptr;
    return *this;
}

// 后置递减
JackTokenizerIterator JackTokenizerIterator::operator--(int)
{
    JackTokenizerIterator temp = *this;
    --(*this);
    return temp;
}

// 比较操作符
bool JackTokenizerIterator::operator==(const JackTokenizerIterator& other) const { return ptr == other.ptr; }
bool JackTokenizerIterator::operator!=(const ::JackTokenizerIterator& other) const { return ptr != other.ptr; }

// 随机访问迭代器的其他操作
JackTokenizerIterator& JackTokenizerIterator::operator+=(std::ptrdiff_t n)
{
    ptr += n;
    return *this;
}

JackTokenizerIterator& JackTokenizerIterator::operator-=(std::ptrdiff_t n)
{
    ptr -= n;
    return *this;
}

JackTokenizerIterator JackTokenizerIterator::operator+(std::ptrdiff_t n) const
{
    return JackTokenizerIterator(ptr + n);
}

JackTokenizerIterator JackTokenizerIterator::operator-(std::ptrdiff_t n) const
{
    return JackTokenizerIterator(ptr - n);
}

std::ptrdiff_t JackTokenizerIterator::operator-(const JackTokenizerIterator& other) const
{
    return ptr - other.ptr;
}

Token& JackTokenizerIterator::operator[](std::ptrdiff_t n) const
{
    return ptr[n];
}
