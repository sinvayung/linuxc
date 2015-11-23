#include <vector>
#include <string>
#include <iostream>

int main()
{
	std::vector<std::string> vec;
	//��˳�����������Ԫ��
	std::string text;
	while(std::cin >> text) {
		if(text == "exit")
			break;
		vec.push_back(text);
	}

	//��ָ��λ�ò���
	std::string abc("abc");
	vec.insert(vec.begin(), abc);
	vec.insert(vec.begin() + 2, "xxx");

	//ɾ��Ԫ��
	vec.erase(vec.end());

	std::cout << "vec.size = " << vec.size() << ", vec empty ? " << vec.empty() << std::endl;
	std::cout << "vec.first = " << *vec.begin() << ", vec.last = " << *(vec.end() - 1) << std::endl;

	//����һ��Ԫ��
	std::vector<std::string> vec2;
	vec2.insert(vec2.begin(), vec.begin(), vec.end());

	//����
	std::vector<std::string>::iterator it;
	for(it = vec2.begin(); it < vec2.end(); it++) {
		std::cout <<  "item " << it - vec2.begin() << " : " << " : " << *it << std::endl;
	}

}

//list�����൱��Java��LinkedList�ࣻvector�����൱��Java��ArrayList��