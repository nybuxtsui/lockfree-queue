#ifndef _SRVLIB_QUEUE_H_
#define _SRVLIB_QUEUE_H_

#include <stddef.h>

namespace srv {

template<typename T, typename LOCK>
class queue {
private:
    struct Node {
        Node *next;
        T data;
    };
public:
    queue() {
        head_ = tail_ = new Node();
        head_->next = NULL;
    }

    ~queue() {
        while (head_) {
            Node *t = head_;
            head_ = head_->next;
            delete t;
        }
    }

    void enqueue(const T &data) {
        Node *node = new Node();
        node->data = data;
        lock_.Lock();
        tail_->next = node;
        tail_ = node;
        lock_.Unlock();
    }

    int dequeue(T *data) {
        lock_.Lock();
        Node *node = head_;
        Node *new_head = node->next;
        if (new_head == NULL) {
            lock_.Unlock();
            return -1;
        }
        *data = new_head->data;
        head_ = new_head;
        lock_.Unlock();
        delete node;
        return 0;
    }

    bool empty() {
        return head_ == tail_;
    }
private:
    Node *head_;
    Node *tail_;
    LOCK  lock_;
};

}

#endif // _QUEUE_H_
