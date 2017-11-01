#ifndef __BITARRAY_H__
#define __BITARRAY_H__

// sample
//int main ()
//{
//  eBitArray odds(20);
//
//  for (int i = 0; i < 20; ++i)
//    odds [i] = (i % 2 == 1);    // Setting
//
//  for (int i = 0; i < 20; ++i)
//    if (odds [i])               // getting
//      cout << i << " is odd." << endl;
//    else
//      cout << i << " is even." << endl;
//
//  return 0;
//}


class eBitArray
{
private:
    class Bit
    {
        friend class eBitArray;
    private:
        int i;
        char *data;
        Bit (int i, char *data);
    public:
        operator bool () const;
        Bit operator = (bool val);
    };

private:
    int len_;
    char *array;

public:
    eBitArray (int num_bits);
    ~eBitArray ();
    Bit operator [] (int i);
    int len () const;
};


#endif // __BITARRAY_H__
