#include <windows.h>
#include <list>

class A
{
public:
	int a;
	A(int i)
	{
		a = i;
	}
};

class B
{
public:
	A& a;
	B(A& a)
	{
		this->a = a;
	}
};

std::list<A*> g_l;

void foo(A& a)
{
	g_l.push_back(&a);
}

void main()
{
	A a(5);

	foo(a);

	a.a = 137;
}