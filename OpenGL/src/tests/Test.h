#pragma once

#include <functional>
#include <vector>
#include <string>
#include <iostream>


namespace test {

	enum TestType {
		NONE = 0, ClearColor, SetUniform4f, RenderTexture, RenderCube
	};


	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		//not "= 0" so I don't have to override them
		virtual void OnUpdate(float deltatime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};


	class TestMenu : public Test {
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << '\n';

			m_Tests.emplace_back(std::make_pair(name, []() {return new T; }));
		}

	private:
		Test*& m_CurrentTest;
		//Construct the Test when it is required, not on program startup
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};

}
