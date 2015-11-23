#include <vector>
#include <string>
#include <iostream>

int main()
{
	std::vector<std::string> vec;
	//在顺序容器中添加元素
	std::string text;
	while(std::cin >> text) {
		if(text == "exit")
			break;
		vec.push_back(text);
	}

	//在指定位置插入
	std::string abc("abc");
	vec.insert(vec.begin(), abc);
	vec.insert(vec.begin() + 2, "xxx");

	//删除元素
	vec.erase(vec.end());

	std::cout << "vec.size = " << vec.size() << ", vec empty ? " << vec.empty() << std::endl;
	std::cout << "vec.first = " << *vec.begin() << ", vec.last = " << *(vec.end() - 1) << std::endl;

	//插入一段元素
	std::vector<std::string> vec2;
	vec2.insert(vec2.begin(), vec.begin(), vec.end());

	//遍历
	std::vector<std::string>::iterator it;
	for(it = vec2.begin(); it < vec2.end(); it++) {
		std::cout <<  "item " << it - vec2.begin() << " : " << " : " << *it << std::endl;
	}

}

//list容器相当于Java的LinkedList类；vector容器相当于Java的ArrayList类