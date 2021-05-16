#include "pch.h"


#include <mono/jit/jit.h>

#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


void RunStaticMethod(MonoDomain* domain, MonoImage* image, const char* methodDesc)
{
	MonoMethodDesc* TypeMethodDesc;
	TypeMethodDesc = mono_method_desc_new(methodDesc, NULL);
	if (!TypeMethodDesc)
		std::cout << "mono_method_desc_new failed" << std::endl;

	// Search the method in the image
	MonoMethod* method;
	method = mono_method_desc_search_in_image(TypeMethodDesc, image);
	if (!method)
		std::cout << "mono_method_desc_search_in_image failed" << std::endl;

	// run the method
	std::cout << "Running the static method: " << methodDesc << std::endl;
	mono_runtime_invoke(method, nullptr, nullptr, nullptr);
}


int main()
{
#ifdef _WIN32
	std::string command = "\"C:/Program Files/Mono/bin/mcs\" Sources/Test.cs -target:library";
#elif defined(__linux__)
	std::string command = "mcs Sources/Test.cs -target:library";
#endif

	// Compile the script
	system(command.c_str());

#ifdef _WIN32
	mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
#elif defined(__linux__)
	// mono_set_dirs("/usr/lib", "C:\\Program Files\\Mono\\etc");
#endif

	/*
	 * Load the default Mono configuration file, this is needed
	 * if you are planning on using the dllmaps defined on the
	 * system configuration
	 */
	mono_config_parse(NULL);

	MonoDomain* domain = mono_jit_init("Sources/Test.dll");

	MonoAssembly* assembly;

	assembly = mono_domain_assembly_open(domain, "Sources/Test.dll");
	if (!assembly)
		exit(2);

	MonoImage* image;
	image = mono_assembly_get_image(assembly);
	if (!image)
		std::cout << "mono_assembly_get_image failed" << std::endl;

	RunStaticMethod(domain, image, "Dog::Type()");

	mono_jit_cleanup(domain);
}
