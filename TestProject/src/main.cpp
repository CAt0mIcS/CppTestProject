#include "pch.h"


class UserEntry
{
public:
	UserEntry(const std::string& name, unsigned int age) : m_Name(name), m_Age(age), m_CacheEntry(0) {}

	void Load() {}

	const std::string& GetName() const { return m_Name; }
	unsigned int GetAge() const { return m_Age; }

private:
	std::string m_Name;
	unsigned int m_Age;
	size_t m_CacheEntry;
};



namespace std 
{
	template <> struct tuple_size<UserEntry> : integral_constant<size_t, 2> { };
	template <> struct tuple_element<0, UserEntry> { using type = std::string; };
	template <> struct tuple_element<1, UserEntry> { using type = unsigned; };
}

//Couldn't get this to work...
//template<>
//const std::string& get<1>(const UserEntry& u) { return u.GetName(); }
//
//template<>
//unsigned get<0>(const UserEntry& u) { return u.GetAge(); }

//This however works...
template<size_t I>
auto get(const UserEntry& u)
{
	if constexpr (I == 0) return u.GetName();
	else if constexpr (I == 1) return u.GetAge();
}



int main()
{
	UserEntry e("Simon", 15);

	auto [name, age] = e;

	std::cout << "Name: " << name << "\nAge: " << age << '\n';

}
