#pragma once

#ifndef SIMPOOL_HPP
#define SIMPOOL_HPP

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#if defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

namespace {
/**
 *
 * @class DLList
 * @brief A "memory neutral" (intrusive) doubly-linked list.
 * @author Emery Berger
 */

class DLList {
public:
  inline DLList() { clear(); }

  class Entry;

  /// Clear the list.
  inline void clear() {
    head.setPrev(&head);
    head.setNext(&head);
  }

  /// Is the list empty?
  inline bool isEmpty() const { return (head.getNext() == &head); }

  /// Get the head of the list.
  inline Entry *get() {
    const Entry *e = head.next;
    if (e == &head) {
      return nullptr;
    }
    head.next = e->next;
    head.next->prev = &head;
    return (Entry *)e;
  }

  /// Remove one item from the list.
  inline void remove(Entry *e) { e->remove(); }

  /// Insert an entry into the head of the list.
  inline void insert(Entry *e) { e->insert(&head, head.next); }

  /// An entry in the list.
  class Entry {
  public:
    //  private:
    inline void setPrev(Entry *p) {
      assert(p != nullptr);
      prev = p;
    }
    inline void setNext(Entry *p) {
      assert(p != nullptr);
      next = p;
    }
    inline Entry *getPrev() const { return prev; }
    inline Entry *getNext() const { return next; }
    inline void remove() const {
      prev->setNext(next);
      next->setPrev(prev);
    }
    inline void insert(Entry *p, Entry *n) {
      prev = p;
      next = n;
      p->setNext(this);
      n->setPrev(this);
    }
    Entry *prev;
    Entry *next;
  };

private:
  /// The head of the list.
  Entry head;
};
} // namespace

class SimPool {
public:
  static inline constexpr size_t align(size_t sz) {
    return (sz + alignof(std::max_align_t) - 1) &
           ~(alignof(std::max_align_t) - 1);
  }

  SimPool(uint64_t maxSize = 0) : _maxSize(maxSize), _size(0) {
    //    static bool v = printInfo();
  }

  void resetSize(uint64_t maxSize) {
    _maxSize = maxSize;
    _size = 0;
  }

  bool printInfo() {
    std::cout << "SimPool initialized." << std::endl;
    return true;
  }

  ~SimPool() { release(); }

  inline void *malloc(size_t sz) {
    if ((_maxSize) && (sz + _size > _maxSize)) {
      return nullptr;
    }
    // sz = align(sz);
    // Get an object of the requested size plus the header.
    auto *b =
        reinterpret_cast<DLList::Entry *>(::malloc(sz + sizeof(DLList::Entry)));
    if (b) {
      // Update current size of the pool.
      if (_maxSize) {
        _size += getSize(b);
      }
      // Add to the front of the linked list.
      _list.insert(b);
      // Return just past the header.
      return reinterpret_cast<void *>(b + 1);
    } else {
      return nullptr;
    }
  }

  inline void *allocate(size_t sz) {
    std::cout << "malloc(" << sz << ")" << std::endl;
    return malloc(sz);
  }

  inline void free(void *ptr) {
    // Find the actual block header.
    auto *b = reinterpret_cast<DLList::Entry *>(ptr) - 1;
    // Remove from the linked list.
    _list.remove(b);
    if (_maxSize) {
      _size -= getSize(b);
    }
    // Free the object.
    ::free(b);
  }

  inline void deallocate(void *ptr) { free(ptr); }

  bool isEmpty() { return _list.isEmpty(); }

  void release() {
    // Iterate through the list, freeing each item.
    auto *e = _list.get();
    while (e) {
      ::free(e);
      e = _list.get();
    }
    _size = 0;
  }

  void rewind() { release(); }

private:
  uint64_t getSize(void *ptr) {
#if defined(__APPLE__)
    auto sz = malloc_size(ptr);
#elif defined(_WIN32)
    auto sz = _msize(ptr);
#else
    auto sz = malloc_usable_size(ptr);
#endif
    return sz;
  }

  uint64_t _maxSize;
  uint64_t _size;
  DLList _list;
};

#endif
