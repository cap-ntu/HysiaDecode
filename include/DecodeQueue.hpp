/*
 * Author:		Wang Yongjie
 * Email :		yongjie.wang@ntu.edu.sg
 * Discription: queue for decoded frame
 *
 */

#ifndef _DecodeQueue_HPP_
#define _DecodeQueue_HPP_

#include<semaphore.h>
#include<vector>
using std::vector;

template<class T>
class DecodeQueue{
    private:
    vector<T> _data;
    int _head; // queue head
    int _end;  // queue tail
    int _size;   // queue size
    sem_t sem; // linux semaphore
    public:
    DecodeQueue(int s){
        _data.resize(s);
        _head = 0;
        _end = 0;
        _size = s;
        sem_init(&sem, 0, 0);  
    }
    ~DecodeQueue(){
        _data.clear();
        sem_destroy(&sem);
    }
    // if queue is full, discard the incoming data
	// if push data fail, return -1
    int push(T t){
        int size;
        sem_getvalue(&sem, &size);
        if(size==_size) return -1;
        _data[_head] = t;
        _head = (_head+1)%_size;
        sem_post(&sem);
    }
    // if queue is empty, wait
    T pop(){
        sem_wait(&sem);
        T t = _data[_end];
        _end = (_end+1)%_size;
        return t;
    }
};

#endif