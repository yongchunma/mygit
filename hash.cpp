#include <iostream>
using namespace std;

class Student
{
public:
    Student(const string& name = "", int age = 0)
    : name_(name),
      age_(age)
    {

    }

private:
    string name_;
    int age_;

};
//桶的状态
enum State
{
    STATE_UNUSE, //从未使用过的
    STATE_USING, //正在使用的
    STATE_DEL  , //元素被删除
};

//桶的类型
struct Bucket
{
    Bucket(int key = 0, State state = STATE_UNUSE) 
    : key_(key)
    , state_(state)
    {}
    int key_;
    State state_;

};

class HashTable
{
public:
    HashTable(int size = primes_[0], double loadFactor_ = 0.75)
        : useBucketNum_(0)
        ,loadFactor_(loadFactor_)
        ,primeIdx_(0)
    {
        if(size != primes_[0])
        {
            for(;primeIdx_<PRIME_SIZE;primeIdx_++)
            {
                if(primes_[primeIdx_] > size)
                {
                    break;
                }
            }
            if(primeIdx_ == PRIME_SIZE)
            {
                primeIdx_--;
            }
        }

        tableSize_ = primes_[primeIdx_] ;
        table_ = new Bucket[tableSize_];
    }
    ~HashTable()
    {
        delete[]table_;
        table_ = nullptr;
    }
public:
    //插入元素
    bool insert(int key)
    {
        //考虑扩容
        double factor = useBucketNum_*1.0 / tableSize_;
        cout << "factor" << factor << endl;
        if(factor > loadFactor_)
        {
            expand();
        }

        int idx = key % tableSize_;

        int i = idx;
        do
        {
           if (table_[i].state_ != STATE_USING)
            {
                table_[i].state_ = STATE_USING;
                table_[i].key_ = key;
                useBucketNum_++;
                return true;
            }
            i = (i+1) % tableSize_;

        } while ( i != idx);

        return false;
    }

//删除元素
    bool erase(int key)
    {
        int idx = key % tableSize_;
        int i =idx;
        do
        {
            if(table_[i].state_ == STATE_USING && table_[i].key_ == key)
            {
                table_[i].state_ = STATE_DEL;
                useBucketNum_--;
            }
             i = (i+1) % tableSize_;
        } 
        while (table_[i].state_ == STATE_UNUSE && i != idx);
        return true;
    }

    //查询
    bool find(int key)
    {
         int idx = key % tableSize_;
        int i =idx;
        do
        {
            if(table_[i].state_ == STATE_USING && table_[i].key_ == key)
            {
                return true;
            }
             i = (i+1) % tableSize_;
        } 
        while (table_[i].state_ == STATE_UNUSE && i != idx);
        return false;
    }

private:
    void expand()
    {
        ++primeIdx_;
        if(primeIdx_ == PRIME_SIZE)
        {
            throw "HashTable is too large! can not expand anymore!";
        }

        Bucket *newTable = new Bucket[primes_[primeIdx_]];
        for(int i = 0; i < tableSize_; i++ )
        {
            if(table_[i].state_ == STATE_USING)
            {
                int idx = table_[i].key_ % primes_[primeIdx_];
                int k = idx;
                do 
                {
                    if (newTable[k].state_ != STATE_USING)
                    {
                        newTable[k].state_ = STATE_USING;
                        newTable[k].key_   = table_[i].key_;
                        break;
                    }

                    k = (k + 1) % primes_[primeIdx_];

                }while(k != idx);
            }
        }

        delete[]table_;
        table_ = newTable;
        tableSize_ = primes_[primeIdx_];
    }


private:
    Bucket* table_;//指向动态开辟的哈希表
    int tableSize_;//哈希当前的长度
    int useBucketNum_; //使用的个数
    double loadFactor_; // 哈希装在因子
    static const int PRIME_SIZE = 10; // 素数表的大小
    static int primes_[PRIME_SIZE];   // 素数表
    int primeIdx_;                    // 当前素数表使用的下标
};
int HashTable::primes_[PRIME_SIZE] = {3,7,23,47,97,251,443,911,1471,42773};

int main()
{
    // //找素数
    // int data = 3;
    // for(int i = data; i < 10000; i++)
    // {
    //     int j = 2;
    //     for(; j < i; j++)
    //     {
    //         if(i % j == 0)
    //         {
    //             break;
    //         }
    //     }
    //     if(j == i)
    //     {
    //         cout << i << " ";
    //     }
    // }
    HashTable htable;
    htable.insert(21);
    htable.insert(32);
    htable.insert(14);
    htable.insert(15);

    htable.insert(22);
    return 0;
}