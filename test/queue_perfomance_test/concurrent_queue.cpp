#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include <functional>
#include <tbb/concurrent_queue.h>

using namespace std;
using std::chrono::system_clock;
using std::chrono::duration;

class Entry
{
public:
    Entry(uint64_t id) : id_(id)
    {
    }

private:
    uint64_t id_;
};

class Queue
{
public:
    Queue() = default;
    virtual void Push(shared_ptr<Entry> item) = 0;
    virtual shared_ptr<Entry> Pop() = 0;
};

class StlQueue : public Queue
{
public:
    StlQueue() = default;
    void Push(shared_ptr<Entry> item) override
    {
        std::lock_guard<mutex> lock(lock_);
        q_.push(item);
    }

    shared_ptr<Entry> Pop() override
    {
        if (!q_.size())
            return nullptr;

        std::lock_guard<mutex> lock(lock_);
        shared_ptr<Entry> t = q_.front();
        q_.pop();

        return t;
    }

private:
    queue<shared_ptr<Entry>> q_;
    mutex lock_;
};

class TbbQueue : public Queue
{
public:
    TbbQueue() = default;
    void Push(shared_ptr<Entry> item) override
    {
        q_.push(item);
    }

    shared_ptr<Entry> Pop() override
    {
        shared_ptr<Entry> t;
        if (!q_.try_pop(t))
            return nullptr;
        return t;
    }

private:
    tbb::concurrent_queue<shared_ptr<Entry>> q_;
};

static const uint64_t MAX = 1000000000;

void p_thread(Queue& queue, uint64_t count)
{
    while (count != 0)
    {
        queue.Push(make_shared<Entry>(count));
        count--;
    }
}

void c_thread(Queue& queue, uint64_t count)
{
    while (count != 0)
    {
        auto i = queue.Pop();
        if (i == nullptr)
            continue;
        count--;
    }
}

void Run(Queue& q, int t_count)
{
    cout << typeid(q).name() << " thread count: " << t_count << endl;
    auto start = system_clock::now();

    vector<thread> v;
    thread c1(c_thread, std::ref(q), MAX);

    for (int i = 0; i < t_count; ++i)
        v.emplace_back(thread(p_thread, std::ref(q), MAX / t_count));

    for (auto& t : v)
        t.join();
    c1.join();

    auto end = system_clock::now();
    cout << "elpased " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
}

int main()
{
    TbbQueue q1;
    Run(q1, 1);


    StlQueue q2;
    Run(q2, 1);


    return 0;
}