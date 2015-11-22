#include <cstdio>
#include <string>

template <typename T>
int compare(const T &v1, const T &v2)
{
	if(v1 < v2) return -1;
	if(v2 < v1) return 1;
	return 0;
}

//ok: no distinction between typename and class in template parameter list
template <typename T, class U> T calc(const T&, const U&);

template <class Type> class Queue {
public:
	Queue();
	Type &front();
	const Type &front() const;
private:
	//......
};

//Queue<int> qi;
//Queue<vector<double>> qc;
//Queue<string> qs;

int main()
{
	printf("compare int : %d\n", compare(2, 1));
	std::string s1("hello");
	std::string s2 = "world";
	printf("compare string : %d\n", compare(s1, s2));
	return 0;
}