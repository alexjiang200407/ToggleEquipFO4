#pragma once

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <spdlog/sinks/basic_file_sink.h>
#include "SimpleIni.h"
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;

using namespace std::literals;

namespace stl
{
	using namespace F4SE::stl;

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		F4SE::AllocTrampoline(14);

		auto& trampoline = F4SE::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}


	template <class F, size_t index, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[index] };
		T::func = vtbl.write_vfunc(T::size, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		write_vfunc<F, 0, T>();
	}
}


#ifdef NEXT_GEN

#	define OFFSET(og, ng) ng
#	define RELOCATION_ID(og, ng) REL::ID(ng)

#else

#	define OFFSET(og, ng) og
#	define RELOCATION_ID(og, ng) REL::ID(og)

#endif // NEXT_GEN

#include "Version.h"
