//#include <iostream>
//#include "Sequence.h"
//
//using namespace std;
//
//int main()
//{
//	Sequence a;
//	Sequence b;
//	Sequence r;
//	int data = 0;
//	a.insert(0, 30);
//	a.insert(1, 21);
//	a.insert(2, 63);
//	a.insert(3, 42);
//	a.insert(4, 17);
//	a.insert(5, 63);
//
//
//	b.insert(0, 42);
//	b.insert(1, 63);
//	b.insert(2, 84);
//	b.insert(3, 19);
//
//	interleave(a, b, r);
//
//	for (int i = 0; i < r.size(); i++)
//	{
//		r.get(i, data);
//		cout << data << "  ";
//	}
//	cin.get();
//	
//
//     return 0;
//}
//
//

#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s;
    assert(s.insert(0, 10));
    assert(s.insert(0, 20));
    assert(s.size() == 2);
    ItemType x = 999;
    assert(s.get(0, x)  &&  x == 20);
    assert(s.get(1, x)  &&  x == 10);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
